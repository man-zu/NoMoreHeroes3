#include "UI_Smash.h"

#include "SmashBros_Travis.h"

CUI_Smash::CUI_Smash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_Smash::CUI_Smash(const CUI_Smash& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_Smash::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_Smash::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	Ready_Object();

	UIMGR->PutScene(GET_CURLEVEL, L"SCENE_Smash", this);

	return S_OK;
}

void CUI_Smash::Tick(_float fTimeDelta)
{
	TextMovePattern(fTimeDelta);
	UIKeyEvent();



	/* ****************************************************************** */

	if (KEY_DOWN(eKeyCode::P))
		UIMGR->GetScene(L"SCENE_Smash")->SceneInit();		// 게임 init
	if (KEY_DOWN(eKeyCode::O))
		UIMGR->GetScene(L"SCENE_Smash")->SceneOut();		// 게임 끝났을때
	if (KEY_DOWN(eKeyCode::I))
		UIMGR->FinalSlashSkillInit(FINALSLAH_DIR_L);		// 파이널 슬래쉬 init
	if (KEY_DOWN(eKeyCode::U))
		UIMGR->FinalSlashSkillOut();						// 파이널 슬래쉬 out
	//if (KEY_DOWN(eKeyCode::L))
	//	static_cast<CUI_Smash*>(UIMGR->GetScene(L"SCENE_Smash"))->TravisHPChange(TravisCurHP);
	//if (KEY_DOWN(eKeyCode::K))
	//	static_cast<CUI_Smash*>(UIMGR->GetScene(L"SCENE_Smash"))->DamonHPChange(DamonCurHP);

}

void CUI_Smash::LateTick(_float fTimeDelta)
{
	if (!m_bInit)
		return;

	fTimeDelta /= m_pGameInstance->Get_TimeScale();

	NumCheck(m_iTravisHP, (_int)UI_SMASH_NUMLLL, L"UI_Smash_Travis_State");
	NumCheck(m_iDamonHP, (_int)UI_SMASH_DAMON_NUMLLL, L"UI_Smash_Damon_State");

	NumTravisAttacked(fTimeDelta);
	NumDamonAttacked(fTimeDelta);

	SkillControl(fTimeDelta);
	FinalSlashControl(fTimeDelta);
}

HRESULT CUI_Smash::Render()
{
	return S_OK;
}

/*
* READY, FIGHT, GAME 이미지 이동패턴
*/
void CUI_Smash::TextMovePattern(_float fTimeDelta)
{
	if (!m_bInit)
		return;

	if (m_eSmashState == SMASH_INGAME)
		return;


	m_fDelayTime += fTimeDelta;

	if (m_eSmashState == SMASH_END)
	{
		if (1.f > m_fDelayTime)
			return;
		
		m_pUI[UI_SMASH_READY]->Set_Active(true);
		m_eSmashState = SMASH_READY;
		m_fDelayTime = 0.f;
	}

	else if (m_eSmashState == SMASH_READY)
	{
		if (m_fSpeed < m_fDelayTime && 4 >= m_iReadyCnt)
		{
			m_fSpeed -= 0.035f;
			m_iReadyCnt++;

			m_pUI[UI_SMASH_READY]->Get_Transform()->Set_Scale({ m_vReadyOriginScale.x * m_iReadyCnt * 0.225f, m_vReadyOriginScale.y * m_iReadyCnt * 0.225f, 0.f });
			m_fDelayTime = 0.f;
		}
		if (5 == m_iReadyCnt)
		{
			if(.5f < m_fDelayTime)
			{
				m_iReadyCnt++;
				m_pUI[UI_SMASH_READY]->Set_Active(false);
				m_fDelayTime = 0.f;
			}
		}
		if (6 == m_iReadyCnt)
		{
			if (.5f <= m_fDelayTime)
			{
				m_eSmashState = SMASH_FIGHT;
				m_fDelayTime = 0.f;
				m_pUI[UI_SMASH_FIGHT]->Set_Active(true);
				m_fSpeed = .3f;
			}
		}
	}
	
	if (m_eSmashState == SMASH_FIGHT)
	{
		if (0 < m_fAlpha && 6 > m_iMoveCnt)
			m_fAlpha -= fTimeDelta * 1.2f;
		if (6 == m_iMoveCnt)
			m_fAlpha += fTimeDelta * 2.f;

		_vector vCurpos = m_pUI[UI_SMASH_FIGHT]->Get_Transform()->Get_Position();
		if (6 > m_iMoveCnt && m_fSpeed <= m_fDelayTime)
		{
			m_fSpeed -= 0.06f;
			m_fDelayTime = 0.f;
			m_iMoveCnt++;

			if (1 == m_iMoveCnt)
				m_pUI[UI_SMASH_FIGHT]->Get_Transform()->Set_Position({ XMVectorGetX(vCurpos) - 30.f, XMVectorGetY(vCurpos), XMVectorGetZ(vCurpos), 1.f });
			if (2 == m_iMoveCnt)
				m_pUI[UI_SMASH_FIGHT]->Get_Transform()->Set_Position({ XMVectorGetX(vCurpos) + 30.f, XMVectorGetY(vCurpos), XMVectorGetZ(vCurpos), 1.f });
			if (3 == m_iMoveCnt)
				m_pUI[UI_SMASH_FIGHT]->Get_Transform()->Set_Position({ XMVectorGetX(vCurpos) - 5.f, XMVectorGetY(vCurpos), XMVectorGetZ(vCurpos), 1.f });
			if (4 == m_iMoveCnt)
				m_fSpeed = .4f;
		}

		if (1.f <= m_fAlpha && 6 == m_iMoveCnt)
		{
			m_eSmashState = SMASH_INGAME;

			m_fDelayTime = 0.f;
			m_iMoveCnt = 0;
			m_fSpeed = .3f;
			m_fAlpha = 1.f;
			m_pUI[UI_SMASH_READY]->Set_ReturnToPool(true);
			m_pUI[UI_SMASH_FIGHT]->Set_ReturnToPool(true);
		}
	}

	else if (!m_bUIout && m_eSmashState == SMASH_GAME)
	{
		if (0 < m_fAlpha && 6 > m_iMoveCnt)
			m_fAlpha -= fTimeDelta * 1.2f;

		_vector vCurpos = m_pUI[UI_SMASH_GAME]->Get_Transform()->Get_Position();
		if (6 > m_iMoveCnt && m_fSpeed <= m_fDelayTime)
		{
			m_fSpeed -= 0.06f;
			m_fDelayTime = 0.f;
			m_iMoveCnt++;

			if (1 == m_iMoveCnt)
				m_pUI[UI_SMASH_GAME]->Get_Transform()->Set_Position({ XMVectorGetX(vCurpos) - 30.f, XMVectorGetY(vCurpos), XMVectorGetZ(vCurpos), 1.f });
			if (2 == m_iMoveCnt)
				m_pUI[UI_SMASH_GAME]->Get_Transform()->Set_Position({ XMVectorGetX(vCurpos) + 30.f, XMVectorGetY(vCurpos), XMVectorGetZ(vCurpos), 1.f });
			if (3 == m_iMoveCnt)
				m_pUI[UI_SMASH_GAME]->Get_Transform()->Set_Position({ XMVectorGetX(vCurpos) - 5.f, XMVectorGetY(vCurpos), XMVectorGetZ(vCurpos), 1.f });
			if (4 == m_iMoveCnt)
				m_fSpeed = .4f;
		}

		if (2.5f <= m_fDelayTime && 6 == m_iMoveCnt)
		{
			m_bUIout = true;

			for(int i = 0; i< UI_SMASH_DAMON_END; ++i)
			{
				m_pUI[i]->Change_ShaderPass(7);
				m_pUI[i]->Set_RawValue("g_fAlpha_Timer", &m_fAlpha, sizeof(_float), true);

				m_pInsUI[INS_SMASH_MINITOP]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 10.f);
				m_pInsUI[INS_SMASH_MINILEFT]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 10.f);
				m_pInsUI[INS_SMASH_MINIRIGHT]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 10.f);
				m_pInsUI[INS_SMASH_MINIBOTTOM]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 10.f);
			}
		}
	}

	else if (m_bUIout && m_eSmashState == SMASH_GAME)
	{
		m_fAlpha += fTimeDelta;

		if (1.f <= m_fAlpha)
		{
			m_fAlpha = 1.f;
			GameOver();
		}
	}
}

void CUI_Smash::NumTravisAttacked(_float fTimeDelta)
{
	if (!m_bTravisAttacked)
		return;

	/* 원래 자리로 돌아옴 */
	if (m_bTravisNumReturnInitPos)
	{
		_bool bResult = true;

		for (int i = UI_SMASH_NUMLLL; i <= UI_SMASH_PERSENT; ++i)
		{
			_vector vCurpos = m_pUI[i]->Get_Pos();
			_vector vLen = XMVector3Length(vCurpos - m_pUI[i]->GetInitPos());

			_float fSetX = JoMath::Lerp(XMVectorGetX(vCurpos), m_pUI[i]->GetInitPosX(), 0.4f);
			_float fSetY = JoMath::Lerp(XMVectorGetY(vCurpos), m_pUI[i]->GetInitPosY(), 0.4f);

			m_pUI[i]->Get_Transform()->Set_Position({ fSetX, fSetY, XMVectorGetZ(vCurpos) , 1.f });

			if (0.1 > XMVectorGetX(vLen))
				bResult = false;
		}

		m_bTravisAttacked = bResult;
		m_bTravisNumReturnInitPos = bResult;

		return;
	}

	/* 원 운동 */
	/* 홀 */
	m_fTravisDelayTime += fTimeDelta * 0.1f;
	m_pUI[UI_SMASH_NUMLLL]->Set_Pos({ m_pUI[UI_SMASH_NUMLLL]->GetInitPosX() + 5 * cos(360 * m_fTravisDelayTime), m_pUI[UI_SMASH_NUMLLL]->GetInitPosY() + 5 * sin(360 * m_fTravisDelayTime), m_pUI[UI_SMASH_NUMLLL]->GetInitPosZ(), 1.f });
	m_pUI[UI_SMASH_NUML]->Set_Pos({ m_pUI[UI_SMASH_NUML]->GetInitPosX() + 5 * cos(360 * m_fTravisDelayTime), m_pUI[UI_SMASH_NUML]->GetInitPosY() + 5 * sin(360 * m_fTravisDelayTime), m_pUI[UI_SMASH_NUML]->GetInitPosZ(), 1.f });
	m_pUI[UI_SMASH_NUMS]->Set_Pos({ m_pUI[UI_SMASH_NUMS]->GetInitPosX() + 5 * cos(360 * m_fTravisDelayTime), m_pUI[UI_SMASH_NUMS]->GetInitPosY() + 5 * sin(360 * m_fTravisDelayTime), m_pUI[UI_SMASH_NUMS]->GetInitPosZ(), 1.f });

	/* 짝 */
	_float fEvenDelayTime = m_fTravisDelayTime - 0.00835f;
	m_pUI[UI_SMASH_DOT]->Set_Pos({ m_pUI[UI_SMASH_DOT]->GetInitPosX() + 5 * cos(360 * fEvenDelayTime), m_pUI[UI_SMASH_DOT]->GetInitPosY() + 5 * sin(360 * fEvenDelayTime), m_pUI[UI_SMASH_DOT]->GetInitPosZ(), 1.f });
	m_pUI[UI_SMASH_NUMLL]->Set_Pos({ m_pUI[UI_SMASH_NUMLL]->GetInitPosX() + 5 * cos(360 * fEvenDelayTime), m_pUI[UI_SMASH_NUMLL]->GetInitPosY() + 5 * sin(360 * fEvenDelayTime), m_pUI[UI_SMASH_NUMLL]->GetInitPosZ(), 1.f });
	m_pUI[UI_SMASH_PERSENT]->Set_Pos({ m_pUI[UI_SMASH_PERSENT]->GetInitPosX() + 5 * cos(360 * fEvenDelayTime), m_pUI[UI_SMASH_PERSENT]->GetInitPosY() + 5 * sin(360 * fEvenDelayTime), m_pUI[UI_SMASH_PERSENT]->GetInitPosZ(), 1.f });

	if(0 >= cos(360 * m_fTravisDelayTime))
		m_iTravisHPColor = { 1.f, 1.f, 1.f, 1.f };

	if (m_fTravisDelayTime > 0.05f)
	{
		m_bTravisNumReturnInitPos = true;
		m_fTravisDelayTime = 0.f;

		/* Set TextColor */
		_int iDiv = (_int)m_iTravisHP / 10;
		m_iTravisHPColor.x = .9f - (0.01f * iDiv);
		m_iTravisHPColor.y = .8f - (0.1f * iDiv);
		m_iTravisHPColor.z = 0.f;
	}

}

void CUI_Smash::NumDamonAttacked(_float fTimeDelta)
{
	if (!m_bDamonAttacked)
		return;
	
	/* 원래 자리로 돌아옴 */
	if (m_bDamonNumReturnInitPos)
	{
		_bool bResult = true;

		for (int i = UI_SMASH_DAMON_NUMLLL; i <= UI_SMASH_DAMON_PERSENT; ++i)
		{
			_vector vCurpos = m_pUI[i]->Get_Pos();
			_vector vLen = XMVector3Length(vCurpos - m_pUI[i]->GetInitPos());

			_float fSetX = JoMath::Lerp(XMVectorGetX(vCurpos), m_pUI[i]->GetInitPosX(), 0.4f);
			_float fSetY = JoMath::Lerp(XMVectorGetY(vCurpos), m_pUI[i]->GetInitPosY(), 0.4f);

			m_pUI[i]->Get_Transform()->Set_Position({ fSetX, fSetY, XMVectorGetZ(vCurpos) , 1.f });

			if (0.1 > XMVectorGetX(vLen))
				bResult = false;
		}

		m_bDamonAttacked = bResult;
		m_bDamonNumReturnInitPos = bResult;

		return;
	}

	/* 원 운동 */
	/* 홀 */
	m_fDemonDelayTime += fTimeDelta * 0.1f;
	m_pUI[UI_SMASH_DAMON_NUMLLL]->Set_Pos({ m_pUI[UI_SMASH_DAMON_NUMLLL]->GetInitPosX() + 5 * cos(360 * m_fDemonDelayTime), m_pUI[UI_SMASH_DAMON_NUMLLL]->GetInitPosY() + 5 * sin(360 * m_fDemonDelayTime), m_pUI[UI_SMASH_DAMON_NUMLLL]->GetInitPosZ(), 1.f});
	m_pUI[UI_SMASH_DAMON_NUML]->Set_Pos({ m_pUI[UI_SMASH_DAMON_NUML]->GetInitPosX() + 5 * cos(360 * m_fDemonDelayTime), m_pUI[UI_SMASH_DAMON_NUML]->GetInitPosY() + 5 * sin(360 * m_fDemonDelayTime), m_pUI[UI_SMASH_DAMON_NUML]->GetInitPosZ(), 1.f});
	m_pUI[UI_SMASH_DAMON_NUMS]->Set_Pos({ m_pUI[UI_SMASH_DAMON_NUMS]->GetInitPosX() + 5 * cos(360 * m_fDemonDelayTime), m_pUI[UI_SMASH_DAMON_NUMS]->GetInitPosY() + 5 * sin(360 * m_fDemonDelayTime), m_pUI[UI_SMASH_DAMON_NUMS]->GetInitPosZ(), 1.f});

	/* 짝 */
	_float fEvenDelayTime = m_fDemonDelayTime - 0.00835f;
	m_pUI[UI_SMASH_DAMON_DOT]->Set_Pos({ m_pUI[UI_SMASH_DAMON_DOT]->GetInitPosX() + 5 * cos(360 * fEvenDelayTime), m_pUI[UI_SMASH_DAMON_DOT]->GetInitPosY() + 5 * sin(360 * fEvenDelayTime), m_pUI[UI_SMASH_DAMON_DOT]->GetInitPosZ(), 1.f });
	m_pUI[UI_SMASH_DAMON_NUMLL]->Set_Pos({ m_pUI[UI_SMASH_DAMON_NUMLL]->GetInitPosX() + 5 * cos(360 * fEvenDelayTime), m_pUI[UI_SMASH_DAMON_NUMLL]->GetInitPosY() + 5 * sin(360 * fEvenDelayTime), m_pUI[UI_SMASH_DAMON_NUMLL]->GetInitPosZ(), 1.f });
	m_pUI[UI_SMASH_DAMON_PERSENT]->Set_Pos({ m_pUI[UI_SMASH_DAMON_PERSENT]->GetInitPosX() + 5 * cos(360 * fEvenDelayTime), m_pUI[UI_SMASH_DAMON_PERSENT]->GetInitPosY() + 5 * sin(360 * fEvenDelayTime), m_pUI[UI_SMASH_DAMON_PERSENT]->GetInitPosZ(), 1.f });

	if (0 >= cos(360 * m_fDemonDelayTime))
		m_iDamonHPColor = { 1.f, 1.f, 1.f, 1.f };

	if (m_fDemonDelayTime > 0.05f)
	{
		m_bDamonNumReturnInitPos = true;
		m_fDemonDelayTime = 0.f;

		/* Set TextColor */
		_int iDiv = (_int)m_iDamonHP / 10;
		m_iDamonHPColor.x = .9f - (0.01f * iDiv);
		m_iDamonHPColor.y = .8f - (0.1f * iDiv);
		m_iDamonHPColor.z = 0.f;
	}

}

void CUI_Smash::NumCheck(_float fNum, _int idx, wstring strUIName)
{
	string strNum = to_string(fNum);
	_int iDir = (_int)strNum.find(".");
	string strLNum = strNum.substr(0, iDir);
	_int iDiffirent = 3 - (_int)strLNum.length();

	for (auto UIitem : UIMGR->GetUIVec(strUIName))
		UIitem->Set_Active(true);

	for (int i = 0; i < iDiffirent; ++i)
		m_pUI[idx]->Set_Active(false);

	for (_int i = 0; i < strLNum.length(); ++i)
	{
		_int iMyNum = stoi(strLNum.substr(i, 1));
		static_cast<CUI_BasicBase*>(m_pUI[idx + i + iDiffirent])->Set_Texture_Index(iMyNum);
	}

	string strSNum = strNum.substr(iDir+1, 1);
	static_cast<CUI_BasicBase*>(m_pUI[idx+3])->Set_Texture_Index(stoi(strSNum));
}

void CUI_Smash::SkillControl(_float fTimeDelta)
{
	vector<class CUI*> UIvec = {};

	/* Skill Alt UI */
	UIvec = UIMGR->GetUIVec(L"UI_Smash_PlayerSkill");
	for (auto UI : UIvec)
	{
		if (!m_bSkillActive && CUI_InstanceBase::INSUI_INOUT_IDLE == static_cast<CUI_InstanceBase*>(UI)->GetInsUIStat())
			UIMGR->Inactive_UI(L"UI_Smash_PlayerSkill");

		static_cast<CUI_InstanceBase*>(UI)->SetViewPos(GET_PLAYER_SMASH->Get_Center());
		static_cast<CUI_InstanceBase*>(UI)->ColorCheck(fTimeDelta);
		static_cast<CUI_InstanceBase*>(UI)->Spread(fTimeDelta, .2f, 15.f);
	}

	/* Skill Mini UI */
	UIvec = UIMGR->GetUIVec(L"UI_Smash_PlayerSkillMini");
	for (auto UI : UIvec)
	{
		static_cast<CUI_InstanceBase*>(UI)->ColorCheck(fTimeDelta);
		static_cast<CUI_InstanceBase*>(UI)->Spread(fTimeDelta, .8f, 15.f);
	}

	m_pInsUI[INS_SMASH_MINITOP]->DirMove(fTimeDelta, .5f, CVIBuffer_UI_Rect_Instance::INSUI_MOVE_TOP);
	m_pInsUI[INS_SMASH_MINILEFT]->DirMove(fTimeDelta, .5f, CVIBuffer_UI_Rect_Instance::INSUI_MOVE_LEFT);
	m_pInsUI[INS_SMASH_MINIRIGHT]->DirMove(fTimeDelta, .5f, CVIBuffer_UI_Rect_Instance::INSUI_MOVE_RIGHT);
	m_pInsUI[INS_SMASH_MINIBOTTOM]->DirMove(fTimeDelta, .5f, CVIBuffer_UI_Rect_Instance::INSUI_MOVE_BOTTOM);

	m_pInsUI[INS_SMASH_MINITOP]->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER_SMASH->Get_SkillInfo(0).fCurCoolDown, (_int)GET_PLAYER_SMASH->Get_SkillInfo(0).fMaxCoolDown);
	m_pInsUI[INS_SMASH_TOP]->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER_SMASH->Get_SkillInfo(0).fCurCoolDown, (_int)GET_PLAYER_SMASH->Get_SkillInfo(0).fMaxCoolDown);
	m_pInsUI[INS_SMASH_TOPICON]->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER_SMASH->Get_SkillInfo(0).fCurCoolDown, (_int)GET_PLAYER_SMASH->Get_SkillInfo(0).fMaxCoolDown);
	m_pInsUI[INS_SMASH_MINILEFT]->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER_SMASH->Get_SkillInfo(1).fCurCoolDown, (_int)GET_PLAYER_SMASH->Get_SkillInfo(1).fMaxCoolDown);
	m_pInsUI[INS_SMASH_LEFT]->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER_SMASH->Get_SkillInfo(1).fCurCoolDown, (_int)GET_PLAYER_SMASH->Get_SkillInfo(1).fMaxCoolDown);
	m_pInsUI[INS_SMASH_LEFTICON]->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER_SMASH->Get_SkillInfo(1).fCurCoolDown, (_int)GET_PLAYER_SMASH->Get_SkillInfo(1).fMaxCoolDown);
	m_pInsUI[INS_SMASH_MINIBOTTOM]->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER_SMASH->Get_SkillInfo(2).fCurCoolDown, (_int)GET_PLAYER_SMASH->Get_SkillInfo(2).fMaxCoolDown);
	m_pInsUI[INS_SMASH_BOTTOM]->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER_SMASH->Get_SkillInfo(2).fCurCoolDown, (_int)GET_PLAYER_SMASH->Get_SkillInfo(2).fMaxCoolDown);
	m_pInsUI[INS_SMASH_BOTTOMICON]->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER_SMASH->Get_SkillInfo(2).fCurCoolDown, (_int)GET_PLAYER_SMASH->Get_SkillInfo(2).fMaxCoolDown);
	m_pInsUI[INS_SMASH_MINIRIGHT]->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER_SMASH->Get_SkillInfo(3).fCurCoolDown, (_int)GET_PLAYER_SMASH->Get_SkillInfo(3).fMaxCoolDown);
	m_pInsUI[INS_SMASH_RIGHT]->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER_SMASH->Get_SkillInfo(3).fCurCoolDown, (_int)GET_PLAYER_SMASH->Get_SkillInfo(3).fMaxCoolDown);
	m_pInsUI[INS_SMASH_RIGHTICON]->SetColorStat(CUI_InstanceBase::INSUI_STAT_SKILL, (_int)GET_PLAYER_SMASH->Get_SkillInfo(3).fCurCoolDown, (_int)GET_PLAYER_SMASH->Get_SkillInfo(3).fMaxCoolDown);

}

void CUI_Smash::FinalSlashControl(_float fTimeDelta)
{
	if (!m_pInsUI[INS_SMASH_SKILLDIR]->Is_Active())
		return;

	/* IN -> WAVE -> OUT */
	if (CUI_InstanceBase::INSUI_INOUT_OUT == m_pInsUI[INS_SMASH_SKILLDIR]->GetInsUIStat())
	{
		m_fDelayTime += fTimeDelta;
		if (m_fDelayTime >= 1.5f)
		{
			m_pInsUI[INS_SMASH_SKILLDIR]->Set_Active(false);
			m_fDelayTime = 0.f;
		}
	}

	if (CUI_InstanceBase::INSUI_INOUT_IDLE == m_pInsUI[INS_SMASH_SKILLDIR]->GetInsUIStat())
		m_pInsUI[INS_SMASH_SKILLDIR]->Wave(fTimeDelta * 100.f);
	else
		m_pInsUI[INS_SMASH_SKILLDIR]->Move(fTimeDelta);
}

void CUI_Smash::UIKeyEvent()
{
	if (/*!GET_PLAYER_SMASH->Get_IsBattleStarted() || */!m_bInit)
		return;

	/********************************************* SKILL *********************************************/
	if (KEY_DOWN(eKeyCode::LAlt))
	{
		if (!m_bSkillActive)
		{
			UIMGR->Active_UI(L"UI_Smash_PlayerSkill");

			vector<class CUI*> UIvec = {};

			/* INSUI_INOUT_SPREADIN */
			UIvec = UIMGR->GetUIVec(L"UI_Smash_PlayerSkill");
			for (auto UI : UIvec)
				static_cast<CUI_InstanceBase*>(UI)->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 3.f);


			/* INSUI_INOUT_SPREADOUT */
			UIvec = UIMGR->GetUIVec(L"UI_Smash_PlayerSkillMini");
			for (auto UI : UIvec)
				static_cast<CUI_InstanceBase*>(UI)->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 3.f);

		}
		m_bSkillActive = true;
	}
	if (KEY_UP(eKeyCode::LAlt))
	{
		m_bSkillActive = false;

		vector<class CUI*> UIvec = {};

		/* INSUI_INOUT_SPREADOUT */
		UIvec = UIMGR->GetUIVec(L"UI_Smash_PlayerSkill");
		for (auto UI : UIvec)
			static_cast<CUI_InstanceBase*>(UI)->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 3.f);


		/* INSUI_INOUT_SPREADIN */
		UIvec = UIMGR->GetUIVec(L"UI_Smash_PlayerSkillMini");
		for (auto UI : UIvec)
			static_cast<CUI_InstanceBase*>(UI)->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 3.f);
	}
}

void CUI_Smash::SceneInit()
{
	m_bInit = true;

	UIMGR->Active_UI(L"UI_Smash");

	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_SushiShopUI", L"UI_Smash");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_SushiShopUI", L"UI_Smash_Travis_State");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_SushiShopUI", L"UI_Smash_Damon_State");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_SushiShopUI", L"UI_Smash_PlayerSkillMini");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_SushiShopUI", L"UI_Smash_PlayerSkill");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_SushiShopUI", L"UI_Smash_PlayerSkillDir");

	m_pUI[UI_SMASH_READY]->Set_Active(false);
	m_pUI[UI_SMASH_FIGHT]->Set_Active(false);
	m_pUI[UI_SMASH_GAME]->Set_Active(false);
}

void CUI_Smash::SceneOut()
{
	m_eSmashState = SMASH_GAME;
	m_pUI[UI_SMASH_GAME]->Set_Active(true);
}

void CUI_Smash::GameOver()
{
	m_bInit = false;

	UIMGR->Set_ReturnToPool(L"UI_Smash", true);
	UIMGR->Set_ReturnToPool(L"UI_Smash_Travis_State", true);
	UIMGR->Set_ReturnToPool(L"UI_Smash_Damon_State", true);
	UIMGR->Set_ReturnToPool(L"UI_Smash_PlayerSkill", true);
	UIMGR->Set_ReturnToPool(L"UI_Smash_PlayerSkillMini", true);
	UIMGR->Set_ReturnToPool(L"UI_Smash_PlayerSkillDir", true);
}

void CUI_Smash::FinalSlashSkillInit(FINALSLAH_DIR eDir)
{
	if (m_bFinalSlashInit)
		return;

	m_bFinalSlashInit = true;
	UIMGR->Active_UI(L"UI_Smash_PlayerSkillDir");
	m_pInsUI[INS_SMASH_SKILLDIR]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_IN);

	/* 방향에 따라 transform 회전 */
	_vector vOrginPos = XMLoadFloat4((_float4*)&m_pInsUI[INS_SMASH_SKILLDIR]->GetDesc().WorldMat.m[3]);

	switch (eDir)
	{
	case Client::FINALSLAH_DIR_D:
		m_pInsUI[INS_SMASH_SKILLDIR]->Get_Transform()->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(0));
		break;
	case Client::FINALSLAH_DIR_L:
		vOrginPos.m128_f32[0] += 310.f;
		vOrginPos.m128_f32[1] -= 50.f;
		m_pInsUI[INS_SMASH_SKILLDIR]->Get_Transform()->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(-90));
		break;
	case Client::FINALSLAH_DIR_R:
		vOrginPos.m128_f32[1] -= 230.f;
		m_pInsUI[INS_SMASH_SKILLDIR]->Get_Transform()->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(90));
		break;
	case Client::FINALSLAH_DIR_SD:
		vOrginPos.m128_f32[0] += 100.f;
		vOrginPos.m128_f32[1] -= 30.f;
		m_pInsUI[INS_SMASH_SKILLDIR]->Get_Transform()->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(-45));
		break;
	case Client::FINALSLAH_DIR_SV:
		vOrginPos.m128_f32[0] += 120.f;
		vOrginPos.m128_f32[1] -= 300.f;
		m_pInsUI[INS_SMASH_SKILLDIR]->Get_Transform()->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(135));
		break;
	case Client::FINALSLAH_DIR_V:
		vOrginPos.m128_f32[0] += 190.f;
		vOrginPos.m128_f32[1] -= 280.f;
		m_pInsUI[INS_SMASH_SKILLDIR]->Get_Transform()->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(180));
		break;
	default:
		break;
	}

	m_pInsUI[INS_SMASH_SKILLDIR]->Get_Transform()->Set_Position({ vOrginPos });
}

void CUI_Smash::FinalSlashSkillOut()
{
	if (!m_bFinalSlashInit)
		return;

	m_bFinalSlashInit = false;
	m_pInsUI[INS_SMASH_SKILLDIR]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_OUT);
}

void CUI_Smash::TravisHPChange(_float fTravisHP)
{
	if (fTravisHP >= 100.f)
		fTravisHP = 100.f;

	m_iTravisHP = fTravisHP;
	m_iTravisHPDiv100 = m_iTravisHP * 0.01f;
	m_bTravisAttacked = true;
	m_fTravisDelayTime = 0.f;


	m_iTravisHPColor = { 1.f, 0.f, 0.f, 1.f };;
}

void CUI_Smash::DamonHPChange(_float fDamonHP)
{
	if (fDamonHP >= 100.f)
		fDamonHP = 100.f;

	m_iDamonHP = fDamonHP;
	m_bDamonAttacked = true;
	m_fDemonDelayTime = 0.f;

	m_iDamonHPColor = { 1.f, 0.f, 0.f, 1.f };;
}

HRESULT CUI_Smash::Ready_Object()
{
	vector<CUI*> UIvec = {};
	m_pUI.resize(UI_SMASH_DAMON_END);
	m_pInsUI.resize(INS_SMASH_DAMON_END);
	
	m_pUI[UI_SMASH_READY] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_Ready")));
	m_pUI[UI_SMASH_FIGHT] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_Fight")));
	m_pUI[UI_SMASH_GAME] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_Game")));
	
	m_pUI[UI_SMASH_TRAVIS] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_Travis")));
	m_pUI[UI_SMASH_TRAVISHP] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_TravisHP")));
	m_pUI[UI_SMASH_DAMONIOCN] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_DamonIocn")));

	m_pUI[UI_SMASH_NUMLLL] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_NumLLL")));
	m_pUI[UI_SMASH_NUMLL] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_NumLL")));
	m_pUI[UI_SMASH_NUML] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_NumL")));
	m_pUI[UI_SMASH_NUMS] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_NumS")));
	m_pUI[UI_SMASH_DOT] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_Dot")));
	m_pUI[UI_SMASH_PERSENT] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_Persent")));

	m_pUI[UI_SMASH_DAMON_NUMLLL] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_Damon_NumLLL")));
	m_pUI[UI_SMASH_DAMON_NUMLL] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_Damon_NumLL")));
	m_pUI[UI_SMASH_DAMON_NUML] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_Damon_NumL")));
	m_pUI[UI_SMASH_DAMON_NUMS] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_Damon_NumS")));
	m_pUI[UI_SMASH_DAMON_DOT] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_Damon_Dot")));
	m_pUI[UI_SMASH_DAMON_PERSENT] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Smash_Damon_Persent")));

	m_pInsUI[INS_SMASH_MINITOP]= static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillMiniTop")));
	m_pInsUI[INS_SMASH_MINILEFT]= static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillMiniLeft")));
	m_pInsUI[INS_SMASH_MINIRIGHT]= static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillMiniRight")));
	m_pInsUI[INS_SMASH_MINIBOTTOM]= static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillMiniBottom")));

	m_pInsUI[INS_SMASH_TOP] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillTop")));
	m_pInsUI[INS_SMASH_LEFT] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillLeft")));
	m_pInsUI[INS_SMASH_RIGHT] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillRight")));
	m_pInsUI[INS_SMASH_BOTTOM] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillBottom")));
	m_pInsUI[INS_SMASH_TOPICON] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillTopIcon")));
	m_pInsUI[INS_SMASH_LEFTICON] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillLeftIcon")));
	m_pInsUI[INS_SMASH_RIGHTICON] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillRightIcon")));
	m_pInsUI[INS_SMASH_BOTTOMICON] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_PlayerSkillBottomIcon")));

	m_pInsUI[INS_SMASH_SKILLDIR] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_Player_Dir")));

	for (_int i = 0; i <= (_uint)UI_SMASH_DAMONIOCN; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_Smash", UIvec);
	UIvec.clear();

	for (_int i = UI_SMASH_NUMLLL; i <= (_uint)UI_SMASH_PERSENT; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_Smash_Travis_State", UIvec);
	UIvec.clear();

	for (_int i = UI_SMASH_DAMON_NUMLLL; i <= (_uint)UI_SMASH_DAMON_PERSENT; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_Smash_Damon_State", UIvec);
	UIvec.clear();

	for (_int i = INS_SMASH_MINITOP; i <= (_uint)INS_SMASH_MINIBOTTOM; ++i)
		UIvec.emplace_back(m_pInsUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_Smash_PlayerSkillMini", UIvec);
	UIvec.clear();

	for (_int i = INS_SMASH_TOP; i <= (_uint)INS_SMASH_BOTTOMICON; ++i)
		UIvec.emplace_back(m_pInsUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_Smash_PlayerSkill", UIvec);
	UIvec.clear();

	UIvec.emplace_back(m_pInsUI[INS_SMASH_SKILLDIR]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_Smash_PlayerSkillDir", UIvec);
	UIvec.clear();

	/* InActive */
	UIMGR->Inactive_UI(L"UI_Smash_PlayerSkillDir");

	/* Save Init Scale */
	m_vReadyOriginScale = m_pUI[UI_SMASH_READY]->Get_Transform()->Get_Scale();
	m_pUI[UI_SMASH_READY]->Get_Transform()->Set_Scale({ .3f, .3f, 0.f });

	/* SetRawValue */
	m_pUI[SMASH_FIGHT]->Set_RawValue("g_fAlpha_Timer", &m_fAlpha, sizeof(_float), true);
	m_pUI[UI_SMASH_GAME]->Set_RawValue("g_fAlpha_Timer", &m_fAlpha, sizeof(_float), true);
	m_pUI[UI_SMASH_TRAVISHP]->Set_RawValue("g_fGauge", &m_iTravisHPDiv100, sizeof(_float), true);

	for (auto pUI : UIMGR->GetUIVec(L"UI_Smash_Travis_State"))
		pUI->Set_RawValue("g_vUIColor", &m_iTravisHPColor, sizeof(_float3), true);

	for (auto pUI : UIMGR->GetUIVec(L"UI_Smash_Damon_State"))
		pUI->Set_RawValue("g_vUIColor", &m_iDamonHPColor, sizeof(_float3), true);

	return S_OK;
}

CUI_Smash* CUI_Smash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Smash* pInstance = new CUI_Smash(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_Smash"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Smash::Clone(void* pArg)
{
	CUI_Smash* pInstance = new CUI_Smash(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_Smash"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Smash::Free()
{
	__super::Free();

	for (auto pUI : m_pUI)
		Safe_Release(pUI);

	for (auto pUI : m_pInsUI)
		Safe_Release(pUI);
}
