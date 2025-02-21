#include "UI_InBattle_Boss.h"

#include "UI_Manager.h"

#include "Travis.h"
#include "MrBlackhole.h"

CUI_InBattle_Boss::CUI_InBattle_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_InBattle_Boss::CUI_InBattle_Boss(const CUI_InBattle_Boss& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_InBattle_Boss::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_InBattle_Boss::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	Ready_Objects();

	UIMGR->PutScene(GET_CURLEVEL, L"SCENE_INS_BossHP", this);

	return S_OK;
}

void CUI_InBattle_Boss::Tick(_float fTimeDelta)
{
	SceneCheck();

}

void CUI_InBattle_Boss::LateTick(_float fTimeDelta)
{
	fTimeDelta /= m_pGameInstance->Get_TimeScale();

	m_pBossUI[INS_BOSS_PHASE1HP]->ColorCheck(fTimeDelta);
	m_pBossUI[INS_BOSS_PHASE2HP]->ColorCheck(fTimeDelta);

	vector<class CUI*> UIvec = UIMGR->GetUIVec(L"UI_InBattle_Boss");
	for (auto UI : UIvec)
	{
		static_cast<CUI_InstanceBase*>(UI)->Spread(fTimeDelta, .8f, 15.f);
	}
}

HRESULT CUI_InBattle_Boss::Render()
{

	return S_OK;
}

void CUI_InBattle_Boss::SceneInit()
{
	m_bInBattle = true;
	m_bUIOff = false;

	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_InBattle", L"UI_InBattle_Boss");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_InBattle_End", L"UI_InBattle_End");

	vector<class CUI*> UIvec = UIMGR->GetUIVec(L"UI_InBattle_Boss");
	for (auto UI : UIvec)
		static_cast<CUI_InstanceBase*>(UI)->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 10.f);
}

void CUI_InBattle_Boss::SceneOut()
{
	m_bUIOff = true;

	vector<class CUI*> UIvec = UIMGR->GetUIVec(L"UI_InBattle_Boss");
	for (auto UI : UIvec)
		static_cast<CUI_InstanceBase*>(UI)->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 10.f);
}

void CUI_InBattle_Boss::SceneCheck()
{
	if (!m_bInBattle && GET_PLAYER->Get_IsBattleStarted())
		SceneInit();
}



HRESULT CUI_InBattle_Boss::Ready_Objects()
{
	/* 사용할 객체 프로토타입 UIMGR->GetDesc(L"PrototypeName") 가져와서 생성. */
	m_pBossUI.resize(INS_BOSS_END);

	vector<CUI*> UIvec = {};

	m_pBossUI[INS_BOSS_PHASELEFT] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_BossPhaseLeft")));
	m_pBossUI[INS_BOSS_PHASERIGHT] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_BossPhaseRight")));
	m_pBossUI[INS_BOSS_PHASE1HP] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_BossHP_1Phaset")));
	m_pBossUI[INS_BOSS_PHASE2HP] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_BossHP_2Phaset")));

	UIvec.emplace_back(m_pBossUI[INS_BOSS_PHASELEFT]);
	UIvec.emplace_back(m_pBossUI[INS_BOSS_PHASERIGHT]);
	UIvec.emplace_back(m_pBossUI[INS_BOSS_PHASE1HP]);
	UIvec.emplace_back(m_pBossUI[INS_BOSS_PHASE2HP]);

	UIMGR->PutUI(GET_CURLEVEL, L"UI_InBattle_Boss", UIvec);
	UIvec.clear();

	return S_OK;
}

CUI_InBattle_Boss* CUI_InBattle_Boss::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_InBattle_Boss* pInstance = new CUI_InBattle_Boss(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_InBattle_Boss"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_InBattle_Boss::Clone(void* pArg)
{
	CUI_InBattle_Boss* pInstance = new CUI_InBattle_Boss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_InBattle_Boss"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InBattle_Boss::Free()
{
	__super::Free();

	for(auto pUI : m_pBossUI)
		Safe_Release(pUI);
}
