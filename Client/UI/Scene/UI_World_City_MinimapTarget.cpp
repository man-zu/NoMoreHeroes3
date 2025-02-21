#include "UI_World_City_MinimapTarget.h"

#include "Various_Camera.h"
#include "Travis.h"
#include "UI_World_City.h"
#include "UI_Manager.h"

#include "UI_World_City_Minimap.h"

CUI_World_City_MinimapTarget::CUI_World_City_MinimapTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI_BasicBase{ pDevice, pContext }
{
}

CUI_World_City_MinimapTarget::CUI_World_City_MinimapTarget(const CUI_World_City_MinimapTarget& rhs)
    : CUI_BasicBase(rhs)
{
}

HRESULT CUI_World_City_MinimapTarget::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_World_City_MinimapTarget::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	XMStoreFloat4x4(&m_MoveMat, XMMatrixIdentity());

	if(m_vLastMovePos.x == 0.f && m_vLastMovePos.y == 0.f)
	{
		m_MoveMat.m[3][0] = static_cast<MINI_QUESTMART*>(pArg)->vInitPos.x;
		m_MoveMat.m[3][1] = static_cast<MINI_QUESTMART*>(pArg)->vInitPos.y;
		m_MoveMat.m[3][2] = static_cast<MINI_QUESTMART*>(pArg)->vInitPos.z;
	}

    return S_OK;
}

void CUI_World_City_MinimapTarget::Tick(_float fTimeDelta)
{
}

void CUI_World_City_MinimapTarget::LateTick(_float fTimeDelta)
{
	if (nullptr == UIMGR->GetScene(L"SCENE_World_City"))
		return;


	CUI_World_City_Minimap* pUI = static_cast<CUI_World_City_Minimap*>(UIMGR->GetScene(L"SCENE_World_City")->GetSceneUI(UI_WORLD_MINIMAP_MAP));

	/* m_pTransform 회전 */
	m_pTransform->Rotation({ 0.f, 0.f, 1.f }, pUI->GetRotateAngle());

	/* 이동량 저장 */
	_float3 fMovePos = pUI->GetMovePos();

	m_MoveMat.m[3][0] = m_MoveMat.m[3][0] + fMovePos.z * 1.f / 18.f;	/* ratio 1/18.f */
	m_MoveMat.m[3][1] = m_MoveMat.m[3][1] - fMovePos.x * 1.f / 18.f;	/* ratio 1/18.f */

	m_vLastMovePos.x = m_MoveMat.m[3][0];
	m_vLastMovePos.y = m_MoveMat.m[3][1];

	_vector pos = XMLoadFloat4((_float4*)&m_MoveMat.m[3]);
	_vector vInit = { 0.f, 0.f, 0.f, 1.f };
	_vector vLength = XMVector3Length(pos - vInit);

	if(4.f > XMVectorGetX(vLength))
		m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);

}

HRESULT CUI_World_City_MinimapTarget::Render()
{
	_float4x4 m_WorldMatrix;
	XMStoreFloat4x4(&m_WorldMatrix, XMLoadFloat4x4(&m_MoveMat) * m_pTransform->Get_WorldMatrix());

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
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

CUI_World_City_MinimapTarget* CUI_World_City_MinimapTarget::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_World_City_MinimapTarget* pInstance = new CUI_World_City_MinimapTarget(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed To Created : CUI_World_City_MinimapTarget"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_World_City_MinimapTarget::Clone(void* pArg)
{
    CUI_World_City_MinimapTarget* pInstance = new CUI_World_City_MinimapTarget(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed To Cloned : CUI_World_City_MinimapTarget"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_World_City_MinimapTarget::Free()
{
    __super::Free();
}
