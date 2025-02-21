#include "UI.h"

CUI::CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CUI::CUI(const CUI& rhs)
	: CGameObject(rhs)
{
}


HRESULT CUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI::Initialize(void* pArg)
{
	_matrix ViewportMatrix = XMMatrixSet(
		g_iWinSizeX * 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -(g_iWinSizeY * 0.5f), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f, 0.f, 1.0f
	);

	XMStoreFloat4x4(&m_ViewportMatrix, ViewportMatrix);

	if (nullptr != pArg)
		memcpy(&m_tUIDesc, pArg, sizeof(CUI::UI_DESC));

	return S_OK;
}

void CUI::Tick(_float fTimeDelta)
{
}

void CUI::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT CUI::Render()
{
	return S_OK;
}

_vector CUI::Convert_WorldToScreen(_fvector vUIWorldPos)
{
	_vector vUIScreenPos = vUIWorldPos;

	_matrix ViewMatrix = m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	_matrix ProjMatrix = m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

	vUIScreenPos = XMVector3TransformCoord(vUIScreenPos, ViewMatrix);
	vUIScreenPos = XMVector3TransformCoord(vUIScreenPos, ProjMatrix);
	vUIScreenPos = XMVector3TransformCoord(vUIScreenPos, XMLoadFloat4x4(&m_ViewportMatrix));

	return vUIScreenPos;
}

_vector CUI::Convert_ScreenToRenderPos(_fvector vUIScreenPos)
{
	return XMVectorSet(vUIScreenPos.m128_f32[0] - _float(g_iWinSizeX) * 0.5f
		, _float(g_iWinSizeY) * 0.5f - vUIScreenPos.m128_f32[1]
		, 1.f, 1.f);
}

_float CUI::Convert_ScreenToRenderPosY(_float fY)
{
	return  _float(g_iWinSizeY) * 0.5f - fY;
}

_vector CUI::Get_Pos()
{
	return m_pTransform->Get_State(CTransform::STATE_POSITION);
}

_float3 CUI::Get_Scale()
{
	return m_pTransform->Get_Scale();
}

void CUI::SetViewPos(_vector vTargetPos)
{
	_vector vConvertTargetPos = Convert_WorldToScreen({ vTargetPos });
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(
		XMVectorGetX(vConvertTargetPos) + m_tUIDesc.WorldMat._41 - g_iWinSizeX * 0.5f,
		-XMVectorGetY(vConvertTargetPos) + m_tUIDesc.WorldMat._42 + g_iWinSizeY * 0.5f,
		XMVectorGetZ(vConvertTargetPos), 1.f));
}

void CUI::Set_Pos(const _float4& vPosition)
{
	m_pTransform->Set_Position(XMLoadFloat4(&vPosition));
}

void CUI::Set_Scale(const _float3& vScale)
{
	m_pTransform->Set_Scale(vScale);
}

void CUI::Turn(const _fvector& _vAxis, const _float& _fTimeDelta)
{
	m_pTransform->Turn(_vAxis, _fTimeDelta);
}

void CUI::Set_Normal_State(const _float3& _vScale, const _float3& _vPos, const _float& _vFar, const _float2& _vWSize)
{
	m_pTransform->Set_Scale(_float3(_vScale.x, _vScale.y, _vScale.z));
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(_vPos.x - _vWSize.x * 0.5f, -_vPos.y + _vWSize.y * 0.5f, _vPos.z, 1.f));

	//XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	//XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	//XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH((_float)_vWSize.x, (_float)_vWSize.y, 0.f, _vFar));
}

void CUI::Set_View_State(const _float3& _vScale, const _float3& _vPos, const _float& _vFar, const _float2& _vWSize)
{
	m_pTransform->Set_Scale(_float3(_vScale.x, _vScale.y, 1.f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH((_float)_vWSize.x, (_float)_vWSize.y, 0.f, _vFar));
}

void CUI::Set_ColBox(HWND hWnd, const _float3& _vScale, _float2& _vWSize)
{
	if (m_bCol/* && KEY_DOWN(eKeyCode::Enter)*/)		// KEY_DOWN 각 UI 객체에서 지정. 요 코드 나중에 다시 볼게
		m_bStart = true;

	POINT			ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	// DX 좌표계 <> WIN 좌표계 (V) => Y 축 '-'
	RECT			Battle_Icon = {};
	_vector			ColPos = m_pTransform->Get_Position();

	Battle_Icon.left = _long(XMVectorGetX(ColPos) - _vScale.x * 0.5f + _vWSize.x * 0.5f);
	Battle_Icon.top = _long(-XMVectorGetY(ColPos) - _vScale.y * 0.5f + _vWSize.y * 0.5f);
	Battle_Icon.right = _long(XMVectorGetX(ColPos) + _vScale.x * 0.5f + _vWSize.x * 0.5f);
	Battle_Icon.bottom = _long(-XMVectorGetY(ColPos) + _vScale.y * 0.5f + _vWSize.y * 0.5f);

	SetRect(&Battle_Icon, Battle_Icon.left, Battle_Icon.top, Battle_Icon.right, Battle_Icon.bottom);

	if ((BOOL)true == PtInRect(&Battle_Icon, ptMouse))
	{
		//MSG_BOX(TEXT("col!"));
		m_bCol = true;
	}
	else
		m_bCol = false;
}

void CUI::Set_ViewPort(const _float3& _vPlacePos, _float3& _vPlaceDir, const _float& fPlaceDistance, _float2& _vWSize)
{
	// 뷰포트 변환
	_float3 IconPos = _vPlacePos;

	_float4x4 ViewMatrix = m_pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW);
	_float4x4 ProjMatrix = m_pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ);

	_vector vIconPos = XMVector3TransformCoord(XMLoadFloat3(&IconPos), XMLoadFloat4x4(&ViewMatrix));
	vIconPos = XMVector3TransformCoord(vIconPos, XMLoadFloat4x4(&ProjMatrix));

	_float fX = XMVectorGetX(vIconPos) * _vWSize.x * 0.5f;
	_float fY = XMVectorGetY(vIconPos) * _vWSize.y * 0.5f;

	_float3 ViewPos = { fX, fY, 0.3f };
	XMVector3Normalize(XMLoadFloat3(&_vPlaceDir));

	ViewPos.x += _vPlaceDir.x * fPlaceDistance;
	ViewPos.y += _vPlaceDir.y * fPlaceDistance;
	ViewPos.z += _vPlaceDir.z * fPlaceDistance;

	m_pTransform->Set_Position(XMLoadFloat3(&ViewPos));
}



void CUI::Free()
{
	__super::Free();

	Safe_Release(m_pShader);
	Safe_Release(m_pTexture);
}
