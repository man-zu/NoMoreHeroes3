#include "UI_World_City_Minimap.h"

#include "Various_Camera.h"
#include "Travis.h"
#include "UI_World_City.h"
#include "UI_Manager.h"

/* static */
_float2		CUI_World_City_Minimap::m_SavedMovePos = {};

CUI_World_City_Minimap::CUI_World_City_Minimap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI_BasicBase{ pDevice, pContext }
{
}

CUI_World_City_Minimap::CUI_World_City_Minimap(const CUI_World_City_Minimap& rhs)
    : CUI_BasicBase(rhs)
{
}

HRESULT CUI_World_City_Minimap::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_World_City_Minimap::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	XMStoreFloat4x4(&m_PosMatrix, XMMatrixIdentity());
	XMStoreFloat3(&m_vBeforeTravisPos, GET_PLAYER->Get_Pos());
	XMStoreFloat4(&m_vBeforeCamLook, m_pGameInstance->Get_MainCamera()->Get_Transform()->Get_GroundLook());

	m_PosMatrix.m[3][0] -= m_SavedMovePos.x;
	m_PosMatrix.m[3][1] += m_SavedMovePos.y;

	m_vInitScale = Get_Scale();

	m_bHideYN = static_cast<CUI_World_City*>(UIMGR->GetScene(L"SCENE_World_City"))->GetVisibleYN();

    return S_OK;
}

void CUI_World_City_Minimap::Tick(_float fTimeDelta)
{
	if(UI_VISIBLE_FALSE == m_bHideYN)
	{
		m_fDelayTime += fTimeDelta;
		if (1.5f > m_fDelayTime)
			return;
	}

	SyncMinimapRotate();
	SyncMinimapPos();

	XMStoreFloat4x4(&m_MixWorldMatrix, XMLoadFloat4x4(&m_PosMatrix) * m_pTransform->Get_WorldMatrix());
}

HRESULT CUI_World_City_Minimap::Render()
{
	if (UI_VISIBLE_FALSE == m_bHideYN && 2.f > m_fDelayTime)
		return S_OK;

	m_SavedMovePos = { m_SavedMovePos.x + m_vMovePos.z * 1.f / 525.f, m_SavedMovePos.y + m_vMovePos.x * 1.f / 525.f };

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_MixWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_MovePos", &m_SavedMovePos, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pTexture->Set_SRV(m_pShader, "g_DiffuseTexture", m_iTexIndex)))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(m_iPass)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer_Point->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBuffer_Point->Render()))
		return E_FAIL;

	return S_OK;
}

void CUI_World_City_Minimap::SyncMinimapRotate()
{
	XMStoreFloat4(&m_vCurCamLook, m_pGameInstance->Get_MainCamera()->Get_Transform()->Get_GroundLook());

	_vector vTargetLook = XMVector3Normalize(XMVectorSetY(XMLoadFloat4(&m_vBeforeCamLook), 0.f));
	_vector vNormalizedSrcLook = XMVector3Normalize(XMVectorSetY(XMLoadFloat4(&m_vCurCamLook), 0.f));

	_float fAngle = acosf(XMVector3Dot(vNormalizedSrcLook, vTargetLook).m128_f32[0]);
	_vector vCross = XMVector3Cross(vNormalizedSrcLook, vTargetLook);

	if (vCross.m128_f32[1] < 0.f)
		fAngle = -fAngle;

	_float angle = XMConvertToRadians(180) + fAngle * -1.f;

	if (0.001f >= abs(XMVector3Length(vTargetLook + vNormalizedSrcLook).m128_f32[0]))
		return;
	if (0.001f >= abs(XMVector3Length(vTargetLook - vNormalizedSrcLook).m128_f32[0]))
		return;

	RotateMap({0.f, 0.f, 1.f}, angle);
	m_fRotateAngle = fAngle * -1.f;
}

void CUI_World_City_Minimap::SyncMinimapPos()
{
	XMStoreFloat3(&m_vCurTravisPos, GET_PLAYER->Get_Pos());

	XMStoreFloat3(&m_vMovePos, XMLoadFloat3(&m_vCurTravisPos) - XMLoadFloat3(&m_vBeforeTravisPos));
	_float3 vCurMapPos = (_float3&)m_PosMatrix.m[3];

	if (m_vMovePos.x == 0 && m_vMovePos.y == 0)
		return;

	m_PosMatrix.m[3][0] = vCurMapPos.x - m_vMovePos.z * 1.f / 525.f;	/* ratio 1/525 */
	m_PosMatrix.m[3][1] = vCurMapPos.y + m_vMovePos.x * 1.f / 525.f;	/* ratio 1/525 */

	XMStoreFloat3(&m_vBeforeTravisPos, GET_PLAYER->Get_Pos());
}

void CUI_World_City_Minimap::RotateMap(_fvector vAxis, _float fRadian)
{
	/* m_vInitScale 저장해서 써야 nan 일때도 돌아올 수 있음. 그래서 함수 따로 팜 */
	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * m_vInitScale.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * m_vInitScale.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * m_vInitScale.z;

	_matrix	RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	m_pTransform->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransform->Set_State(CTransform::STATE_UP, vUp);
	m_pTransform->Set_State(CTransform::STATE_LOOK, vLook);
}

CUI_World_City_Minimap* CUI_World_City_Minimap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_World_City_Minimap* pInstance = new CUI_World_City_Minimap(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed To Created : CUI_World_City_Minimap"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_World_City_Minimap::Clone(void* pArg)
{
    CUI_World_City_Minimap* pInstance = new CUI_World_City_Minimap(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed To Cloned : CUI_World_City_Minimap"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_World_City_Minimap::Free()
{
    __super::Free();
}
