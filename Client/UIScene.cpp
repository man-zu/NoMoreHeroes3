#include "UIScene.h"

CUIScene::CUIScene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CUIScene::CUIScene(const CUIScene& rhs)
	: CGameObject(rhs)
{
}


HRESULT CUIScene::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIScene::Initialize(void* pArg)
{
	return S_OK;
}

void CUIScene::Tick(_float fTimeDelta)
{
}

void CUIScene::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT CUIScene::Render()
{
	return S_OK;
}

HRESULT CUIScene::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Transform"), (CComponent**)&m_pTransform)))
		return E_FAIL;

	m_pTransform->Set_WorldMatrix(XMMatrixIdentity());

	return S_OK;
}

CUI_BasicBase* CUIScene::GetSceneUI(_int idx)
{
	if (idx < m_pUI.size())
		return m_pUI[idx];
	else
		return nullptr;
}


void CUIScene::Free()
{
	__super::Free();
}
