#include "UI_InBattle_Named.h"


CUI_InBattle_Named::CUI_InBattle_Named(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_InBattle_Named::CUI_InBattle_Named(const CUI_InBattle_Named& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_InBattle_Named::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_InBattle_Named::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	Ready_Objects();

	UIMGR->PutScene(GET_CURLEVEL, L"SCENE_NamedHP", this);

	return S_OK;
}

void CUI_InBattle_Named::Tick(_float fTimeDelta)
{
	if (m_bInTurnRPG)
		return;

	fTimeDelta /= m_pGameInstance->Get_TimeScale();

	MonsterHPControl(fTimeDelta);
}

void CUI_InBattle_Named::LateTick(_float fTimeDelta)
{

}
 
HRESULT CUI_InBattle_Named::Render()
{
	return S_OK;
}

void CUI_InBattle_Named::SceneInit()
{
	if (!m_bInTurnRPG)
		InsertNamedHP();
}

void CUI_InBattle_Named::SceneOut()
{
	m_pNamedUI[INS_NAMED_PHASELEFT]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 3.f);
	m_pNamedUI[INS_NAMED_PHASERIGHT]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 3.f);
	m_pNamedUI[INS_NAMED_HP]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 3.f);
}

HRESULT CUI_InBattle_Named::InsertNamedHP()
{
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_InBattle", L"UI_InBattle_Named");

	vector<class CUI*> UIvec = UIMGR->GetUIVec(L"UI_InBattle_Named");
	for (auto UI : UIvec)
		static_cast<CUI_InstanceBase*>(UI)->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 3.f);

	return S_OK;
}

void CUI_InBattle_Named::NamedMonsterHPUIsync(CUI_InstanceBase::INSUI_STAT eParam, _int iCur, _int iMax)
{
	m_pNamedUI[INS_NAMED_HP]->SetColorStat(eParam, iCur, iMax);

	if (0 == iCur)
	{
		m_pNamedUI[INS_NAMED_HP]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 3.f);
	}
}

void CUI_InBattle_Named::MonsterHPControl(_float fTimeDelta)
{
	m_pNamedUI[INS_NAMED_HP]->ColorCheck(fTimeDelta);
	m_pNamedUI[INS_NAMED_HP]->Spread(fTimeDelta, .7f, 17.f);
	m_pNamedUI[INS_NAMED_PHASELEFT]->Spread(fTimeDelta, .7f, 17.f);
	m_pNamedUI[INS_NAMED_PHASERIGHT]->Spread(fTimeDelta, .7f, 17.f);
}

HRESULT CUI_InBattle_Named::Ready_Objects()
{
	m_pNamedUI.resize(INS_NAMED_END);

	vector<CUI*> UIvec = {};

	m_pNamedUI[INS_NAMED_PHASELEFT] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_BossPhaseLeft")));
	m_pNamedUI[INS_NAMED_PHASERIGHT] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_BossPhaseRight")));
	m_pNamedUI[INS_NAMED_HP] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_BossHP")));

	UIvec.emplace_back(m_pNamedUI[INS_NAMED_PHASELEFT]);
	UIvec.emplace_back(m_pNamedUI[INS_NAMED_PHASERIGHT]);
	UIvec.emplace_back(m_pNamedUI[INS_NAMED_HP]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_InBattle_Named", UIvec);
	UIvec.clear();

	return S_OK;
}

CUI_InBattle_Named* CUI_InBattle_Named::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_InBattle_Named* pInstance = new CUI_InBattle_Named(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_InBattle_Named"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_InBattle_Named::Clone(void* pArg)
{
	CUI_InBattle_Named* pInstance = new CUI_InBattle_Named(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_InBattle_Named"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InBattle_Named::Free()
{
	__super::Free();

	for (auto pUI : m_pNamedUI)
		Safe_Release(pUI);

}
