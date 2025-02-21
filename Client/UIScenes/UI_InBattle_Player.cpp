#include "UI_InBattle_Player.h"

#include "UI_Manager.h"
#include "Travis.h"


CUI_InBattle_Player::CUI_InBattle_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_InBattle_Player::CUI_InBattle_Player(const CUI_InBattle_Player& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_InBattle_Player::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_InBattle_Player::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	Ready_Objects();

	UIMGR->PutScene(GET_CURLEVEL, L"SCENE_INS_PlayerBattle", this);

	return S_OK;
}

void CUI_InBattle_Player::Tick(_float fTimeDelta)
{
	fTimeDelta /= m_pGameInstance->Get_TimeScale();
	SceneCheck(fTimeDelta);
	UIKeyEvent();
}

void CUI_InBattle_Player::LateTick(_float fTimeDelta)
{	
	fTimeDelta /= m_pGameInstance->Get_TimeScale();
	ClickButtonControl(fTimeDelta);

	if (m_bUIOff || m_bInTurnRPG)
		return;

	LockOnCheck();
	BatteryControl(fTimeDelta);
	HPControl(fTimeDelta);
	SkillControl(fTimeDelta);
	FinalSlashControl(fTimeDelta);

}

HRESULT CUI_InBattle_Player::Render()
{
	return S_OK;
}

void CUI_InBattle_Player::InBattleStart()
{
	m_bInBattle = true;
	m_bUIOff = false;
	m_bCallUIOff = false;

	/* 얘네 그룹화 시키고 싶은디 */
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_InBattle", L"UI_InBattle_Player_Hit");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_InBattle", L"UI_InBattle_Player");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_InBattle", L"UI_InBattle_PlayerStatic");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_InBattle", L"UI_InBattle_PlayerLockOn");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_InBattle", L"UI_InBattle_PlayerNonBattery");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_InBattle", L"UI_InBattle_PlayerSkill");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_InBattle", L"UI_InBattle_PlayerSkillMini");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_InBattle", L"UI_InBattle_PlayerSkillDir");

	UIMGR->Inactive_UI(L"UI_InBattle_StartBtn");

	m_pInsUI[INS_PLAYER_HP]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 10.f);
	m_pInsUI[INS_PLAYER_BATTERY]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 10.f);
	m_pInsUI[INS_PLAYER_BATTERYCASE]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 10.f);
	m_pPlayerSkillMiniTop->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 10.f);
	m_pPlayerSkillMiniLeft->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 10.f);
	m_pPlayerSkillMiniRight->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 10.f);
	m_pPlayerSkillMiniBottom->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 10.f);

	m_pInsUI[INS_PLAYER_BATTERY]->SetColorStat(CUI_InstanceBase::INSUI_STAT::INSUI_STAT_INC, GET_PLAYER->Get_CurBattery(), GET_PLAYER->Get_MaxBattery());
}

void CUI_InBattle_Player::BatteryControl(_float fTimeDelta)
{
	if (0 == GET_PLAYER->Get_CurBattery() && GET_PLAYER->Get_IsBattleStarted())
	{
		if (!m_pPlayerNonBattery->Is_Active())
		{
			UIMGR->Active_UI(L"UI_InBattle_PlayerNonBattery");
			m_pInsUI[INS_PLAYER_BATTERY]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 10.f);
			m_pInsUI[INS_PLAYER_BATTERYCASE]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 10.f);
		}

		m_pPlayerNonBattery->SetViewPos(GET_PLAYER->Get_Center());
	}
	else
	{
		if (m_pPlayerNonBattery->Is_Active())
		{
			m_pInsUI[INS_PLAYER_BATTERY]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 10.f);
			m_pInsUI[INS_PLAYER_BATTERYCASE]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 10.f);
			UIMGR->Inactive_UI(L"UI_InBattle_PlayerNonBattery");
		}
	}

	m_pInsUI[INS_PLAYER_BATTERY]->ColorCheck(fTimeDelta);
	m_pInsUI[INS_PLAYER_BATTERY]->Spread(fTimeDelta, .8f, 15.f);
	m_pInsUI[INS_PLAYER_BATTERYCASE]->Spread(fTimeDelta, .8f, 15.f);
}

void CUI_InBattle_Player::HPControl(_float fTimeDelta)
{
	m_pInsUI[INS_PLAYER_HP]->Beat(fTimeDelta, .5f);
	m_pInsUI[INS_PLAYER_HP]->ColorCheck(fTimeDelta);
	m_pInsUI[INS_PLAYER_HP]->Spread(fTimeDelta, .8f, 15.f);
}

void CUI_InBattle_Player::SkillControl(_float fTimeDelta)
{
	vector<class CUI*> UIvec = {};

	/* Skill Alt UI */
	UIvec = UIMGR->GetUIVec(L"UI_InBattle_PlayerSkill");
	for (auto UI : UIvec)
	{
		if(!m_bSkillActive && CUI_InstanceBase::INSUI_INOUT_IDLE == static_cast<CUI_InstanceBase*>(UI)->GetInsUIStat())
			UIMGR->Inactive_UI(L"UI_InBattle_PlayerSkill");

		static_cast<CUI_InstanceBase*>(UI)->SetViewPos(GET_PLAYER->Get_Center());
		static_cast<CUI_InstanceBase*>(UI)->ColorCheck(fTimeDelta);
		static_cast<CUI_InstanceBase*>(UI)->Spread(fTimeDelta, .2f, 15.f);
	}

	/* Skill Mini UI */
	UIvec = UIMGR->GetUIVec(L"UI_InBattle_PlayerSkillMini");
	for (auto UI : UIvec)
	{
		static_cast<CUI_InstanceBase*>(UI)->ColorCheck(fTimeDelta);
		static_cast<CUI_InstanceBase*>(UI)->Spread(fTimeDelta, .8f, 15.f);
	}

	m_pPlayerSkillMiniTop->DirMove(fTimeDelta, .5f, CVIBuffer_UI_Rect_Instance::INSUI_MOVE_TOP);
	m_pPlayerSkillMiniLeft->DirMove(fTimeDelta, .5f, CVIBuffer_UI_Rect_Instance::INSUI_MOVE_LEFT);
	m_pPlayerSkillMiniRight->DirMove(fTimeDelta, .5f, CVIBuffer_UI_Rect_Instance::INSUI_MOVE_RIGHT);
	m_pPlayerSkillMiniBottom->DirMove(fTimeDelta, .5f, CVIBuffer_UI_Rect_Instance::INSUI_MOVE_BOTTOM);

	m_pPlayerSkillMiniTop->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER->Get_SkillInfo(0).fCurCoolDown, (_int)GET_PLAYER->Get_SkillInfo(0).fMaxCoolDown);
	m_pPlayerSkillTop->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER->Get_SkillInfo(0).fCurCoolDown, (_int)GET_PLAYER->Get_SkillInfo(0).fMaxCoolDown);
	m_pPlayerSkillTopIcon->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER->Get_SkillInfo(0).fCurCoolDown, (_int)GET_PLAYER->Get_SkillInfo(0).fMaxCoolDown);
	
	m_pPlayerSkillMiniLeft->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER->Get_SkillInfo(1).fCurCoolDown, (_int)GET_PLAYER->Get_SkillInfo(1).fMaxCoolDown);
	m_pPlayerSkillLeft->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER->Get_SkillInfo(1).fCurCoolDown, (_int)GET_PLAYER->Get_SkillInfo(1).fMaxCoolDown);
	m_pPlayerSkillLeftIcon->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER->Get_SkillInfo(1).fCurCoolDown, (_int)GET_PLAYER->Get_SkillInfo(1).fMaxCoolDown);
	
	m_pPlayerSkillMiniBottom->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER->Get_SkillInfo(2).fCurCoolDown, (_int)GET_PLAYER->Get_SkillInfo(2).fMaxCoolDown);
	m_pPlayerSkillBottom->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER->Get_SkillInfo(2).fCurCoolDown, (_int)GET_PLAYER->Get_SkillInfo(2).fMaxCoolDown);
	m_pPlayerSkillBottomIcon->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER->Get_SkillInfo(2).fCurCoolDown, (_int)GET_PLAYER->Get_SkillInfo(2).fMaxCoolDown);
	
	m_pPlayerSkillMiniRight->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER->Get_SkillInfo(3).fCurCoolDown, (_int)GET_PLAYER->Get_SkillInfo(3).fMaxCoolDown);
	m_pPlayerSkillRight->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER->Get_SkillInfo(3).fCurCoolDown, (_int)GET_PLAYER->Get_SkillInfo(3).fMaxCoolDown);
	m_pPlayerSkillRightIcon->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER->Get_SkillInfo(3).fCurCoolDown, (_int)GET_PLAYER->Get_SkillInfo(3).fMaxCoolDown);


}

void CUI_InBattle_Player::FinalSlashControl(_float fTimeDelta)
{
	if (!m_pInsUI[INS_PALEYR_SKILLDIR]->Is_Active())
		return;

	/* IN -> WAVE -> OUT */
	if (CUI_InstanceBase::INSUI_INOUT_OUT == m_pInsUI[INS_PALEYR_SKILLDIR]->GetInsUIStat())
	{
		m_fDelayTime += fTimeDelta;
		if (m_fDelayTime >= 1.5f)
		{
			m_pInsUI[INS_PALEYR_SKILLDIR]->Set_Active(false);
			m_fDelayTime = 0.f;
		}
	}

	if (CUI_InstanceBase::INSUI_INOUT_IDLE == m_pInsUI[INS_PALEYR_SKILLDIR]->GetInsUIStat())
		m_pInsUI[INS_PALEYR_SKILLDIR]->Wave(fTimeDelta * 100.f);
	else
		m_pInsUI[INS_PALEYR_SKILLDIR]->Move(fTimeDelta);
}

void CUI_InBattle_Player::ClickButtonControl(_float fTimeDelta)
{
	if (!m_bMouseInit)
		return;

	m_pBattleStartMouseWhite->Spread(fTimeDelta, 1.f, 15.f);
	m_pBattleStartMouseRed->Spread(fTimeDelta, 1.f, 15.f);
}

//void CUI_InBattle_Player::UIHitControl()
//{
//	if (0 == m_iHit)
//		return;
//
//
//}

void CUI_InBattle_Player::SceneInit()
{
	m_bMouseInit = true;

	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_InBattle", L"UI_InBattle_StartBtn");
	BattleButtonIn();
}

void CUI_InBattle_Player::SceneOut()
{
	m_bCallUIOff = true;
	m_fDelayTime = 0.f;

	m_pInsUI[INS_PLAYER_HP]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 10.f);
	m_pInsUI[INS_PLAYER_BATTERY]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 10.f);
	m_pInsUI[INS_PLAYER_BATTERYCASE]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 10.f);
	m_pPlayerSkillMiniTop->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 10.f);
	m_pPlayerSkillMiniLeft->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 10.f);
	m_pPlayerSkillMiniRight->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 10.f);
	m_pPlayerSkillMiniBottom->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 10.f);

	UIMGR->Set_ReturnToPool(L"UI_InBattle_Player_Hit", true);
}

void CUI_InBattle_Player::UpdateHitUI(_int iHit)
{
	m_iHit = iHit;

	if (0 > iHit)
		m_iHit = 0;

	if (0 == m_iHit)
	{
		UIMGR->Inactive_UI(L"UI_InBattle_Player_Hit");
		m_pUI[UI_INBATTLE_PLAYERHITNUM_0]->Set_Texture_Index(0);
		m_pUI[UI_INBATTLE_PLAYERHITNUM_1]->Set_Texture_Index(0);
	}
	else
		UIMGR->Active_UI(L"UI_InBattle_Player_Hit");

	if (20 > iHit)
		m_vHitColor = {1.f, 1.f, 0.f};
	else if (40 > iHit)
		m_vHitColor = {0.f, 1.f, 0.f};
	else if (60 > iHit)
		m_vHitColor = {1.f, 1.f, 0.f};
	else if (80 > iHit)
		m_vHitColor = {1.f, 0.f, 0.f};

	string strNum = to_string(m_iHit);

	for (_int i = 0; i < strNum.length(); ++i)
	{
		_int iMyNum = stoi(strNum.substr(i, 1));
		_int idx = UI_INBATTLE_PLAYERHITNUM_0 + 2 - (_int)strNum.length() + i;

		m_pUI[idx]->Set_Texture_Index(iMyNum);
	}
}

void CUI_InBattle_Player::BatteryCharge()
{
	m_pInsUI[INS_PLAYER_BATTERY]->SetColorStat(CUI_InstanceBase::INSUI_STAT::INSUI_STAT_INC, GET_PLAYER->Get_CurBattery(), GET_PLAYER->Get_MaxBattery());
}

void CUI_InBattle_Player::BatteryMinus()
{
	m_pInsUI[INS_PLAYER_BATTERY]->SetColorStat(CUI_InstanceBase::INSUI_STAT::INSUI_STAT_DEC, GET_PLAYER->Get_CurBattery(), GET_PLAYER->Get_MaxBattery());
}

void CUI_InBattle_Player::BattleButtonIn()
{
	if (m_bInBattleButtonInit)
		return;

	m_bInBattleButtonInit = true;
	UIMGR->Active_UI(L"UI_InBattle_StartBtn");
	m_pBattleStartMouseWhite->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 3.f);
	m_pBattleStartMouseRed->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 3.f);
}

void CUI_InBattle_Player::BattleButtonOut()
{
	if (!m_bInBattleButtonInit)
		return;

	m_bInBattleButtonInit = false;
	m_pBattleStartMouseWhite->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 3.f);
	m_pBattleStartMouseRed->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 3.f);
}

void CUI_InBattle_Player::FinalSlashSkillInit(FINALSLAH_DIR eDir)
{
	if (m_bFinalSlashInit)
		return;

	m_bFinalSlashInit = true;
	UIMGR->Active_UI(L"UI_InBattle_PlayerSkillDir");
	m_pInsUI[INS_PALEYR_SKILLDIR]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_IN);

	/* 방향에 따라 transform 회전 */
	_vector vOrginPos = XMLoadFloat4((_float4*)&m_pInsUI[INS_PALEYR_SKILLDIR]->GetDesc().WorldMat.m[3]);

	switch (eDir)
	{
	case Client::FINALSLAH_DIR_D:
		m_pInsUI[INS_PALEYR_SKILLDIR]->Get_Transform()->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(0));
		break;
	case Client::FINALSLAH_DIR_L:
		vOrginPos.m128_f32[0] += 310.f;
		vOrginPos.m128_f32[1] -= 50.f;
		m_pInsUI[INS_PALEYR_SKILLDIR]->Get_Transform()->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(-90));
		break;
	case Client::FINALSLAH_DIR_R:
		vOrginPos.m128_f32[1] -= 230.f;
		m_pInsUI[INS_PALEYR_SKILLDIR]->Get_Transform()->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(90));
		break;
	case Client::FINALSLAH_DIR_SD:
		vOrginPos.m128_f32[0] += 100.f;
		vOrginPos.m128_f32[1] -= 30.f;
		m_pInsUI[INS_PALEYR_SKILLDIR]->Get_Transform()->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(-45));
		break;
	case Client::FINALSLAH_DIR_SV:
		vOrginPos.m128_f32[0] += 120.f;
		vOrginPos.m128_f32[1] -= 300.f;
		m_pInsUI[INS_PALEYR_SKILLDIR]->Get_Transform()->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(135));
		break;
	case Client::FINALSLAH_DIR_V:
		vOrginPos.m128_f32[0] += 190.f;
		vOrginPos.m128_f32[1] -= 280.f;
		m_pInsUI[INS_PALEYR_SKILLDIR]->Get_Transform()->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(180));
		break;
	default:
		break;
	}

	m_pInsUI[INS_PALEYR_SKILLDIR]->Get_Transform()->Set_Position({ vOrginPos });

}
void CUI_InBattle_Player::FinalSlashSkillOut()
{
	if (!m_bFinalSlashInit)
		return;

	m_bFinalSlashInit = false;
	m_pInsUI[INS_PALEYR_SKILLDIR]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_OUT);
}

void CUI_InBattle_Player::SceneCheck(_float fTimeDelta)
{
	if (!m_bInBattle && GET_PLAYER->Get_IsBattleStarted() && !m_bInTurnRPG)
		InBattleStart();

	if (!m_bUIOff && m_bCallUIOff)
	{
		m_fDelayTime += fTimeDelta;
		if (m_fDelayTime < 1.5f)
			return;

		m_fDelayTime = 0.f;
		m_bUIOff = true;

		UIMGR->Set_ReturnToPool(L"UI_InBattle_Player", true);
		UIMGR->Set_ReturnToPool(L"UI_InBattle_PlayerStatic", true);
		UIMGR->Set_ReturnToPool(L"UI_InBattle_PlayerLockOn", true);
		UIMGR->Set_ReturnToPool(L"UI_InBattle_PlayerNonBattery", true);
		UIMGR->Set_ReturnToPool(L"UI_InBattle_PlayerSkill", true);
		UIMGR->Set_ReturnToPool(L"UI_InBattle_PlayerSkillMini", true);
		UIMGR->Set_ReturnToPool(L"UI_InBattle_PlayerSkillDir", true);
	}
}

void CUI_InBattle_Player::UIKeyEvent()
{
	if (!GET_PLAYER->Get_IsBattleStarted() || m_bCallUIOff)
		return;

	/********************************************* BATTERY *********************************************/
	//if (KEY_DOWN(eKeyCode::LButton))
	//	cout << GET_PLAYER->Get_CurBattery() << endl;
	if (KEY_DOWN(eKeyCode::LButton) || KEY_DOWN(eKeyCode::RButton))
		m_pInsUI[INS_PLAYER_BATTERY]->SetColorStat(CUI_InstanceBase::INSUI_STAT::INSUI_STAT_DEC, GET_PLAYER->Get_CurBattery(), GET_PLAYER->Get_MaxBattery());

	/********************************************* SKILL *********************************************/
	if (m_bSkillActive)
	{
		if (KEY_DOWN(eKeyCode::One) || KEY_DOWN(eKeyCode::Two) || KEY_DOWN(eKeyCode::Three) || KEY_DOWN(eKeyCode::Four) || KEY_UP(eKeyCode::LAlt))
		{
			m_bSkillActive = false;

			vector<class CUI*> UIvec = {};

			/* INSUI_INOUT_SPREADOUT */
			UIvec = UIMGR->GetUIVec(L"UI_InBattle_PlayerSkill");
			for (auto UI : UIvec)
				static_cast<CUI_InstanceBase*>(UI)->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 3.f);


			/* INSUI_INOUT_SPREADIN */
			UIvec = UIMGR->GetUIVec(L"UI_InBattle_PlayerSkillMini");
			for (auto UI : UIvec)
				static_cast<CUI_InstanceBase*>(UI)->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 3.f);
		}

	}
	if (KEY_DOWN(eKeyCode::LAlt))
	{
		if (!m_bSkillActive)
		{
			UIMGR->Active_UI(L"UI_InBattle_PlayerSkill");

			vector<class CUI*> UIvec = {};

			/* INSUI_INOUT_SPREADIN */
			UIvec = UIMGR->GetUIVec(L"UI_InBattle_PlayerSkill");
			for (auto UI : UIvec)
				static_cast<CUI_InstanceBase*>(UI)->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 3.f);


			/* INSUI_INOUT_SPREADOUT */
			UIvec = UIMGR->GetUIVec(L"UI_InBattle_PlayerSkillMini");
			for (auto UI : UIvec)
				static_cast<CUI_InstanceBase*>(UI)->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 3.f);

		}
		m_bSkillActive = true;
	}

}

void CUI_InBattle_Player::LockOnCheck()
{
	if (GET_PLAYER->Get_IsLockOn()) {
		UIMGR->Active_UI(L"UI_InBattle_PlayerLockOn");
		static_cast<CUI_InstanceBase*>(UIMGR->GetUI(L"UI_InBattle_PlayerLockOn", 0))->SetViewPos(GET_PLAYER->Get_LockOnTargetCenterPos());
	}
	else {
		if (UIMGR->GetUI(L"UI_InBattle_PlayerLockOn", 0)->Is_Active())
			UIMGR->Inactive_UI(L"UI_InBattle_PlayerLockOn");
	}
}

HRESULT CUI_InBattle_Player::Ready_Objects()
{
	vector<CUI*> UIvec = {};

	m_pUI.resize(UI_INBATTLE_PLAYER_END);

	m_pUI[UI_INBATTLE_PLAYERHITNUM_0] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_PlayerHitNum_0")));
	m_pUI[UI_INBATTLE_PLAYERHITNUM_1] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_PlayerHitNum_1")));
	m_pUI[UI_INBATTLE_PLAYERHIT] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_PlayerHit")));

	for(int i = UI_INBATTLE_PLAYERHITNUM_0;i <= UI_INBATTLE_PLAYERHIT; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_InBattle_Player_Hit", UIvec);
	UIvec.clear();

	UIMGR->Inactive_UI(L"UI_InBattle_Player_Hit");
	m_pUI[UI_INBATTLE_PLAYERHITNUM_0]->Set_RawValue("g_vUIColor", &m_vHitColor, sizeof(_float3), true);
	m_pUI[UI_INBATTLE_PLAYERHITNUM_1]->Set_RawValue("g_vUIColor", &m_vHitColor, sizeof(_float3), true);

	/**************************************************************************************************/

	m_pInsUI.resize(INS_PLAYER_END);

	m_pInsUI[INS_PLAYER_HP] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerHP")));
	m_pInsUI[INS_PLAYER_BATTERY] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerBattery")));
	m_pInsUI[INS_PLAYER_BATTERYCASE] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerBatteryCase")));
	m_pInsUI[INS_PALEYR_SKILLDIR] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_Player_Dir")));
	m_pPlayerNonBattery = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerNONBattery")));
	m_pPlayerLockOn = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_MonsterLockOn")));

	m_pPlayerSkillMiniTop = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillMiniTop")));
	m_pPlayerSkillMiniLeft = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillMiniLeft")));
	m_pPlayerSkillMiniRight = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillMiniRight")));
	m_pPlayerSkillMiniBottom = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillMiniBottom")));

	m_pPlayerSkillTop = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillTop")));
	m_pPlayerSkillLeft = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillLeft")));
	m_pPlayerSkillRight = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillRight")));
	m_pPlayerSkillBottom = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillBottom")));
	m_pPlayerSkillTopIcon = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillTopIcon")));
	m_pPlayerSkillLeftIcon = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillLeftIcon")));
	m_pPlayerSkillRightIcon = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillRightIcon")));
	m_pPlayerSkillBottomIcon = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillBottomIcon")));

	m_pBattleStartMouseWhite = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_BattleStart_Mouse_White")));
	m_pBattleStartMouseRed = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_BattleStart_Mouse_Red")));

	UIvec.emplace_back(m_pInsUI[INS_PLAYER_HP]);
	UIvec.emplace_back(m_pInsUI[INS_PLAYER_BATTERY]);
	UIvec.emplace_back(m_pInsUI[INS_PLAYER_BATTERYCASE]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_InBattle_Player", UIvec);
	UIvec.clear();

	UIvec.emplace_back(m_pPlayerNonBattery);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_InBattle_PlayerNonBattery", UIvec);
	UIvec.clear();

	UIvec.emplace_back(m_pPlayerLockOn);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_InBattle_PlayerLockOn", UIvec);
	UIvec.clear();

	UIvec.emplace_back(m_pPlayerSkillMiniTop);
	UIvec.emplace_back(m_pPlayerSkillMiniLeft);
	UIvec.emplace_back(m_pPlayerSkillMiniBottom);
	UIvec.emplace_back(m_pPlayerSkillMiniRight);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_InBattle_PlayerSkillMini", UIvec);
	UIvec.clear();

	/* Player Skill */
	UIvec.emplace_back(m_pPlayerSkillTop);
	UIvec.emplace_back(m_pPlayerSkillLeft);
	UIvec.emplace_back(m_pPlayerSkillBottom);
	UIvec.emplace_back(m_pPlayerSkillRight);
	UIvec.emplace_back(m_pPlayerSkillTopIcon);
	UIvec.emplace_back(m_pPlayerSkillLeftIcon);
	UIvec.emplace_back(m_pPlayerSkillBottomIcon);
	UIvec.emplace_back(m_pPlayerSkillRightIcon);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_InBattle_PlayerSkill", UIvec);
	UIvec.clear();

	UIvec.emplace_back(m_pBattleStartMouseWhite);
	UIvec.emplace_back(m_pBattleStartMouseRed);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_InBattle_StartBtn", UIvec);
	UIvec.clear();

	UIvec.emplace_back(m_pInsUI[INS_PALEYR_SKILLDIR]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_InBattle_PlayerSkillDir", UIvec);
	UIvec.clear();


	/* Mouse UI Insert */
	m_pBattleStartMouseWhite->Get_Transform()->Move_Root({-400.f, 50.f, 0.f});
	m_pBattleStartMouseRed->Get_Transform()->Move_Root({-400.f, 50.f, 0.f});

	UIMGR->Inactive_UI(L"UI_InBattle_PlayerLockOn");
	UIMGR->Inactive_UI(L"UI_InBattle_PlayerNonBattery");
	UIMGR->Inactive_UI(L"UI_InBattle_PlayerSkill");
	UIMGR->Inactive_UI(L"UI_InBattle_PlayerSkillDir");


	return S_OK;
}

CUI_InBattle_Player* CUI_InBattle_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_InBattle_Player* pInstance = new CUI_InBattle_Player(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_InBattle_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_InBattle_Player::Clone(void* pArg)
{
	CUI_InBattle_Player* pInstance = new CUI_InBattle_Player(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_InBattle_Player"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InBattle_Player::Free()
{
	__super::Free();

	for (auto pUI : m_pInsUI)
		Safe_Release(pUI);

	for (auto pUI : m_pUI)
		Safe_Release(pUI);

	Safe_Release(m_pPlayerNonBattery);
	Safe_Release(m_pPlayerLockOn);
	Safe_Release(m_pPlayerSkillMiniTop);
	Safe_Release(m_pPlayerSkillMiniLeft);
	Safe_Release(m_pPlayerSkillMiniRight);
	Safe_Release(m_pPlayerSkillMiniBottom);
	
	Safe_Release(m_pPlayerSkillTop);
	Safe_Release(m_pPlayerSkillLeft);
	Safe_Release(m_pPlayerSkillRight);
	Safe_Release(m_pPlayerSkillBottom);
	Safe_Release(m_pPlayerSkillTopIcon);
	Safe_Release(m_pPlayerSkillLeftIcon);
	Safe_Release(m_pPlayerSkillRightIcon);
	Safe_Release(m_pPlayerSkillBottomIcon);

	Safe_Release(m_pBattleStartMouseRed);
	Safe_Release(m_pBattleStartMouseWhite);
}
