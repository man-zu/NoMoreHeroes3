#include "UI_InBattle_Letter.h"

#include "UI_Manager.h"

#include "Travis.h"
#include "MrBlackhole.h"

CUI_InBattle_Letter::CUI_InBattle_Letter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_InBattle_Letter::CUI_InBattle_Letter(const CUI_InBattle_Letter& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_InBattle_Letter::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_InBattle_Letter::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	m_pUI.resize(INS_LETTER_END);

	UIMGR->PutScene(GET_CURLEVEL, L"Scene_InBattle_Letter", this);

	return S_OK;
}

void CUI_InBattle_Letter::Tick(_float fTimeDelta)
{
	fTimeDelta /= m_pGameInstance->Get_TimeScale();

	if (m_sLetter != L"")
	{
		vector<class CUI*> UIvec = UIMGR->GetUIVec(m_sLetter);

		for (auto UI : UIvec)
			static_cast<CUI_InstanceBase*>(UI)->Spread(fTimeDelta, 1.5f, 8.f);
	}


	// 0OWN(eKeyCode::K))
	//	UIMGR->CommonBattleEnd(INS_LETTER_QUESTIONMARK);
}

void CUI_InBattle_Letter::LateTick(_float fTimeDelta)
{

}

HRESULT CUI_InBattle_Letter::Render()
{

	return S_OK;
}

void CUI_InBattle_Letter::SceneInit(INS_LETTER eLetterType)
{
	switch (eLetterType)
	{
	case Client::INS_LETTER_KILL:
		ReadyLetterKill();
		break;
	case Client::INS_LETTER_MR:
		ReadyLetterMrBlackHoleDead();
		break;
	case Client::INS_LETTER_QUESTIONMARK:
		ReadyLetterMrBlackHoleDeadQuestion();
		break;
	case Client::INS_LETTER_BLACKHOLE:
		ReadyLetterMrBlackHoleDead();
		break;
	case Client::INS_LETTER_SONIC:
		ReadyLetterSonicJuiceDead();
		break;
	case Client::INS_LETTER_JUICE:
		ReadyLetterSonicJuiceDead();
		break;
	case Client::INS_LETTER_END:
		break;
	}
}

void CUI_InBattle_Letter::SceneOut(INS_LETTER eLetterType)
{
	if(!m_bSpreadOut)
	{
		m_bSpreadOut = true;

		if (m_sLetter != L"")
		{
			vector<class CUI*> UIvec = UIMGR->GetUIVec(m_sLetter);

			for (auto UI : UIvec)
				static_cast<CUI_InstanceBase*>(UI)->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 5.f);
		}
	}
}


void CUI_InBattle_Letter::ReadyLetterKill()
{
	m_pUI.resize(1);
	m_sLetter = L"UI_InBattle_Letter_Kill";

	vector<CUI*> UIvec = {};
	m_pUI[INS_LETTER_KILL] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_Kill")));

	UIvec.emplace_back(m_pUI[INS_LETTER_KILL]);
	UIMGR->PutUI(GET_CURLEVEL, m_sLetter, UIvec);
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_InBattle_End", m_sLetter);
	
	m_pUI[INS_LETTER_KILL]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 20.f);
}

void CUI_InBattle_Letter::ReadyLetterMrBlackHoleDead()
{
	m_sLetter = L"UI_InBattle_Letter_MrBlackHoleDead";

	vector<CUI*> UIvec = {};

	m_pUI[INS_LETTER_MR] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_LetterMr")));
	m_pUI[INS_LETTER_BLACKHOLE] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_LetterBlackHole")));
	m_pUI[INS_LETTER_DEAD] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_LetterDead")));

	UIvec.emplace_back(m_pUI[INS_LETTER_MR]);
	UIvec.emplace_back(m_pUI[INS_LETTER_BLACKHOLE]);
	UIvec.emplace_back(m_pUI[INS_LETTER_DEAD]);
	UIMGR->PutUI(GET_CURLEVEL, m_sLetter, UIvec);
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_InBattle_End", m_sLetter);

	m_pUI[INS_LETTER_DEAD]->Get_Transform()->Set_PositionX(-583.f);

	m_pUI[INS_LETTER_MR]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 20.f);
	m_pUI[INS_LETTER_BLACKHOLE]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 20.f);
	m_pUI[INS_LETTER_DEAD]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 20.f);
}

void CUI_InBattle_Letter::ReadyLetterMrBlackHoleDeadQuestion()
{
	m_sLetter = L"UI_InBattle_Letter_MrBlackHoleDeadQuestion";

	vector<CUI*> UIvec = {};

	m_pUI[INS_LETTER_MR] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_LetterMr")));
	m_pUI[INS_LETTER_BLACKHOLE] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_LetterBlackHole")));
	m_pUI[INS_LETTER_DEAD] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_LetterDead")));
	m_pUI[INS_LETTER_QUESTIONMARK] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_LetterQuestionMark")));

	UIvec.emplace_back(m_pUI[INS_LETTER_MR]);
	UIvec.emplace_back(m_pUI[INS_LETTER_BLACKHOLE]);
	UIvec.emplace_back(m_pUI[INS_LETTER_DEAD]);
	UIvec.emplace_back(m_pUI[INS_LETTER_QUESTIONMARK]);
	UIMGR->PutUI(GET_CURLEVEL, m_sLetter, UIvec);
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_InBattle_End", m_sLetter);

	m_pUI[INS_LETTER_MR]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 20.f);
	m_pUI[INS_LETTER_BLACKHOLE]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 20.f);
	m_pUI[INS_LETTER_DEAD]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 20.f);
	m_pUI[INS_LETTER_QUESTIONMARK]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 20.f);
}

void CUI_InBattle_Letter::ReadyLetterSonicJuiceDead()
{
	m_sLetter = L"UI_InBattle_Letter_MrBlackHoleDead";

	vector<CUI*> UIvec = {};

	m_pUI[INS_LETTER_SONIC] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_SONIC")));
	m_pUI[INS_LETTER_JUICE] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_JUICE")));
	m_pUI[INS_LETTER_DEAD] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_LetterDead")));

	UIvec.emplace_back(m_pUI[INS_LETTER_SONIC]);
	UIvec.emplace_back(m_pUI[INS_LETTER_JUICE]);
	UIvec.emplace_back(m_pUI[INS_LETTER_DEAD]);
	UIMGR->PutUI(GET_CURLEVEL, m_sLetter, UIvec);
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_InBattle_End", m_sLetter);

	m_pUI[INS_LETTER_DEAD]->Get_Transform()->Set_PositionX(-583.f);

	m_pUI[INS_LETTER_SONIC]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 20.f);
	m_pUI[INS_LETTER_JUICE]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 20.f);
	m_pUI[INS_LETTER_DEAD]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 20.f);
}

HRESULT CUI_InBattle_Letter::Ready_Objects()
{
	return S_OK;
}

CUI_InBattle_Letter* CUI_InBattle_Letter::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_InBattle_Letter* pInstance = new CUI_InBattle_Letter(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_InBattle_Letter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_InBattle_Letter::Clone(void* pArg)
{
	CUI_InBattle_Letter* pInstance = new CUI_InBattle_Letter(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_InBattle_Letter"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InBattle_Letter::Free()
{
	__super::Free();

	for(auto pUI : m_pUI)
		Safe_Release(pUI);
}
