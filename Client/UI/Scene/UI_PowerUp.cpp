#include "UI_PowerUp.h"

#include "UI_PowerUp_Icon.h"
#include "UI_Manager.h"
#include "Travis_Manager.h"
#include "UI_CCTV.h"
#include "Travis_World.h"

CUI_PowerUp::CUI_PowerUp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_PowerUp::CUI_PowerUp(const CUI_PowerUp& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_PowerUp::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_PowerUp::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	m_pTransform->Set_Position({ 0.f, 0.f, 0.f });

	Ready_Objects();

	UIMGR->PutScene(GET_CURLEVEL, L"SCENE_PowerUP", this);

	return S_OK;
}

void CUI_PowerUp::Tick(_float fTimeDelta)
{
	if (!m_bInit)
		return;

	CalculateWESN();

	if(m_bLevelUpMotion)
	{
		LevelUpMotion(fTimeDelta);
		return;
	}

	KeyEvent();
	ScreenMoveCheck();
}

void CUI_PowerUp::LateTick(_float fTimeDelta)
{
	if (m_bInit)
		m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);

}

HRESULT CUI_PowerUp::Render()
{
	_vector DefaultColor = { .9f, 0.2f, 0.16f, 1.f };

	m_pContext->GSSetShader(nullptr, nullptr, 0);

	m_pGameInstance->Render_Font(L"FontUI", ConvertNumWstr(m_iSkillLevel[POWERUP_HEALTH]), _float2(100.f, 465.f + m_fTextMoveAmount), 1.3f, DefaultColor, 0.f, CCustomFont::FontAlignCenter);
	m_pGameInstance->Render_Font(L"FontUI", ConvertNumWstr(m_iSkillLevel[POWERUP_ATTACK]), _float2(255.f, 465.f + m_fTextMoveAmount), 1.3f, DefaultColor, 0.f, CCustomFont::FontAlignCenter);
	m_pGameInstance->Render_Font(L"FontUI", ConvertNumWstr(m_iSkillLevel[POWERUP_BATTERY]), _float2(410.f, 465.f + m_fTextMoveAmount), 1.3f, DefaultColor, 0.f, CCustomFont::FontAlignCenter);
	m_pGameInstance->Render_Font(L"FontUI", ConvertNumWstr(m_iSkillLevel[POWERUP_SKILL]), _float2(565.f, 465.f + m_fTextMoveAmount), 1.3f, DefaultColor, 0.f, CCustomFont::FontAlignCenter);

	if (m_state == POWERUP_STAT)
	{
		_int iNeedWESN = m_iWesnCost[m_iSelectIdx] * m_iSkillLevel[m_iSelectIdx];
		wstring CurSkillName = L"";
		wstring CurSkillInfo = L"";
		if (0 == m_iSelectIdx)
		{
			CurSkillName = L"체력";
			CurSkillInfo = L"체력이 증가합니다.";
		}
		else if (1 == m_iSelectIdx)
		{
			CurSkillName = L"공격력";
			CurSkillInfo = L"공격력이 증가합니다.";
		}
		else if (2 == m_iSelectIdx)
		{
			CurSkillName = L"파워";
			CurSkillInfo = L"파워가 증가합니다.";
		}
		else if (3 == m_iSelectIdx)
		{
			CurSkillName = L"데스 스킬";
			CurSkillInfo = L"쿨다운 시간이 감소합니다.";
		}
		else if (4 == m_iSelectIdx)
		{
			CurSkillName = L"특수공격";
			CurSkillInfo = L"특수공격의 데미지가 증가합니다.";
		}

		if (m_iSkillLevel[m_iSelectIdx] == m_iPowerUpMax[m_iSelectIdx])
		{
			m_pGameInstance->Render_Font(L"FontUI", CurSkillName, _float2(1025.f, 307.f), .65f, DefaultColor, 0.f, CCustomFont::FontAlignCenter);
			m_pGameInstance->Render_Font(L"FontUI", L"\" 최고 레벨 \"", _float2(1025.f, 342.f), .65f, DefaultColor, 0.f, CCustomFont::FontAlignCenter);
		}

		else
		{
			m_pGameInstance->Render_Font(L"FontUI", CurSkillName + L" : 레벨 " + to_wstring(m_iSkillLevel[m_iSelectIdx]), _float2(1025.f, 307.f), .65f, DefaultColor, 0.f, CCustomFont::FontAlignCenter);
			m_pGameInstance->Render_Font(L"FontUI", CurSkillInfo, _float2(1025.f, 342.f), .65f, DefaultColor, 0.f, CCustomFont::FontAlignCenter);
			m_pGameInstance->Render_Font(L"FontUI", L"WESN 필요 : " + to_wstring(iNeedWESN), _float2(1025.f, 490.f), .55f, DefaultColor, 0.f, CCustomFont::FontAlignCenter);
		}

	}

	return S_OK;
}

void CUI_PowerUp::KeyEvent()
{
	if (KEY_DOWN(eKeyCode::ESC))
		SceneOut();

	if (KEY_DOWN(eKeyCode::One))
	{
		PLAY_SOUND("SE_sys_skillTree_ioOut", false, 1.f);

		ResetWESN();
	}

	else if (KEY_DOWN(eKeyCode::RButton) && m_state == POWERUP_STAT)
	{
		PLAY_SOUND("SE_sys_menu_windowMain_close", false, 1.f);

		m_state = POWERUP_MAIN_MOVE;
	}

	if (KEY_DOWN(eKeyCode::LButton) && m_state == POWERUP_MAIN)
	{
		PLAY_SOUND("SE_sys_menu_windowMain_open", false, 1.f);

		m_state = POWERUP_STAT_MOVE;
		m_iSelectIdx = 0;
		ChooseWesn();
	}


	if (m_state != POWERUP_STAT)
		return;

	if (KEY_DOWN(eKeyCode::LButton))
	{
		PLAY_SOUND("JIN_GachaGet", false, 1.f);

		m_pUI[UI_POWERUP_RED_BTN + m_iSelectIdx]->Set_Active(true);
		LevelUP();
	}
	else if (KEY_UP(eKeyCode::LButton))
		m_pUI[UI_POWERUP_RED_BTN + m_iSelectIdx]->Set_Active(false);

	if (KEY_DOWN(eKeyCode::W))
	{
		PLAY_SOUND("SE_sys_skillTree_buttonUpDown", false, 1.f);

		if (0 > --m_iSelectIdx)
			m_iSelectIdx = 4;

		ChooseWesn();
	}

	if (KEY_DOWN(eKeyCode::S))
	{
		PLAY_SOUND("SE_sys_skillTree_buttonUpDown", false, 1.f);

		if (4 < ++m_iSelectIdx)
			m_iSelectIdx = 0;

		ChooseWesn();
	}

}

void CUI_PowerUp::ScreenMoveCheck()
{
	if (m_state == POWERUP_STAT || m_state == POWERUP_MAIN)
		return;

	/* KeyGuide */
	{
		_vector vCurPos = m_pUI[UI_POWERUP_KEYGUIDE]->Get_Pos();
		_float fInitY = m_pUI[UI_POWERUP_KEYGUIDE]->GetInitPosY();
		_int iCurTextureIdx = m_pUI[UI_POWERUP_KEYGUIDE]->Get_Texture_Index();
		/* Down */
		if (iCurTextureIdx != (_int)m_state)
		{
			vCurPos = XMVectorSetY(vCurPos, JoMath::Lerp(XMVectorGetY(vCurPos), fInitY - 95.f, 0.2f));
			m_pUI[UI_POWERUP_KEYGUIDE]->Get_Transform()->Set_Position(vCurPos);

			if (0.1f > XMVectorGetY(vCurPos) - fInitY - 95.f)
				m_pUI[UI_POWERUP_KEYGUIDE]->Set_Texture_Index((_int)m_state);
		}
		/* Up */
		else
		{
			vCurPos = XMVectorSetY(vCurPos, JoMath::Lerp(XMVectorGetY(vCurPos), fInitY, 0.2f));
			m_pUI[UI_POWERUP_KEYGUIDE]->Get_Transform()->Set_Position(vCurPos);

			if (0.1f > XMVectorGetY(vCurPos) - fInitY)
				m_pUI[UI_POWERUP_KEYGUIDE]->Set_Texture_Index((_int)m_state);
		}
	}


	/* Move Font */
	if (m_state == POWERUP_STAT_MOVE)
		m_fTextMoveAmount = JoMath::Lerp(m_fTextMoveAmount, -541.f, 0.2f);
	else if (m_state == POWERUP_MAIN_MOVE)
		m_fTextMoveAmount = JoMath::Lerp(m_fTextMoveAmount, 0.f, 0.2f);


	/* All UIs */
	for (_int i = UI_POWERUP_BG; i <= UI_POWERUP_LINEYELLOW; ++i)
	{
		_vector vCurPos = m_pUI[i]->Get_Pos();
		_float vInitPos = m_pUI[i]->GetInitPosY();

		if (m_state == POWERUP_STAT_MOVE)
		{
			vCurPos = XMVectorSetY(vCurPos, JoMath::Lerp(XMVectorGetY(vCurPos), vInitPos + 541.f, 0.2f));

			if (0.3f > abs(XMVectorGetY(vCurPos) - (vInitPos + 541.f)))
			{
				m_state = POWERUP_STAT;

				m_fTextMoveAmount = -541.f;
				vCurPos = XMVectorSetY(vCurPos, vInitPos + 541.f);
				m_pUI[UI_POWERUP_SKILLBIGICON]->Set_Active(true);
				static_cast<CUI_PowerUp_Icon*>(m_pUI[UI_POWERUP_SKILLBIGICON])->Set_Texture_Index(0);
			}
		}
		else if (m_state == POWERUP_MAIN_MOVE)
		{
			m_pUI[UI_POWERUP_SKILLBIGICON]->Set_Active(false);

			vCurPos = XMVectorSetY(vCurPos, JoMath::Lerp(XMVectorGetY(vCurPos), vInitPos, 0.2f));

			if (0.3f > XMVectorGetY(vCurPos) - vInitPos)
			{
				m_state = POWERUP_MAIN;

 				m_fTextMoveAmount = 0.f;
				vCurPos = XMVectorSetY(vCurPos, vInitPos);
			}
		}
		m_pUI[i]->Get_Transform()->Set_Position(vCurPos);
	}
}

void CUI_PowerUp::PlayerStateCheck()
{
	if (!m_bPlayerMoneyCheck)
	{
		m_bPlayerMoneyCheck = true;

		PLAYER_ABILITY CurABility = CTravis_Manager::Get_Instance()->Get_PlayerAbility();
		m_iSkillLevel[POWERUP_ATTACK] = CurABility.iAttackLevel;
		m_iSkillLevel[POWERUP_BATTERY] = CurABility.iBatteryLevel;
		m_iSkillLevel[POWERUP_HEALTH] = CurABility.iHealthLevel;
		m_iSkillLevel[POWERUP_SKILL] = CurABility.iSkillLevel;
		m_iSkillLevel[POWERUP_SPECIAL] = CurABility.iSpecialAttackLevel;
		m_iViewWesn = CurABility.iWesn;
		m_iCurWesn = CurABility.iWesn;

		/* Set WESN */
		wstring strWesn = to_wstring(m_iViewWesn);
		_int iLen = (_int)strWesn.size();

		for (int i = 0; i < iLen; ++i)
		{
			_int inum = stoi(strWesn.substr(i, 1));
			_int iUIIdx = 6 - iLen + i;
			m_pUI[UI_POWERUP_WESN_NUM0 + iUIIdx]->Set_Texture_Index(inum);
		}
		
		/* Set Skills LEVEL */
		for (int i = 0; i < 5; ++i)
		{
			if (1 == m_iSkillLevel[i])
				continue;

			_int ibeforeidx = GetSelectImgIdxBefore(i);
			_int iIdx = (m_iSkillLevel[i] - 1 + ibeforeidx); /* 현재 idx + 직전 Max들 더한 값 */
			
			for (int j = ibeforeidx; j <= iIdx; ++j)
				m_pUI[UI_POWERUP_REDLEVEL1 + j]->Set_Texture_Index(1);
		}
	}
}

void CUI_PowerUp::CalculateWESN()
{
	if (m_iViewWesn != m_iCurWesn)
	{
		_int iChange = m_iViewWesn - m_iCurWesn;
		_int iLen = 0;
		wstring strWesn = L"";

		/* 감소 */
		if(0 < iChange)
		{
			strWesn = to_wstring(iChange);
			iLen = (_int)strWesn.size();

			for (int i = 0; i < iLen; ++i)
			{
				_int iDigit = (_int)pow(10, i);
				if (iChange < iDigit)
					continue;

				iChange -= iDigit;
				m_iViewWesn -= iDigit;
			}
		}
		/* 증가 */
		else if (0 > iChange)
		{
			strWesn = to_wstring(abs(iChange));
			iLen = (_int)strWesn.size();

			for (int i = 0; i < iLen; ++i)
			{
				_int iDigit = (_int)pow(10, i);
				if (abs(iChange) < iDigit)
					continue;

				iChange += iDigit;
				m_iViewWesn += iDigit;
			}
		}

		/* Texture Setting */
		strWesn = to_wstring(m_iViewWesn);
		iLen = (_int)strWesn.size();

		for (int i = 0; i < 6; ++i)
		{
			_int iExceptIdx = 6 - iLen;
			if(i < iExceptIdx)
			{
				m_pUI[UI_POWERUP_WESN_NUM0 + i]->Set_Texture_Index(10);
				continue;
			}

			_int inum = stoi(strWesn.substr(i- iExceptIdx, 1));
			m_pUI[UI_POWERUP_WESN_NUM0 + i]->Set_Texture_Index(inum);
		}

	}
}

void CUI_PowerUp::ChooseWesn()
{
	/* 아이콘 뒤 빨간 배경 */
	for (_int i = UI_POWERUP_REDMOVE; i <= UI_POWERUP_YELLOWMOVE; ++i)
		m_pUI[i]->Set_Active(false);

	_int idx = UI_POWERUP_REDMOVE + m_iSelectIdx;
	m_pUI[idx]->Set_Active(true);


	/* 아이콘 */
	idx = UI_POWERUP_REDICON + m_iSelectIdx;
	for (_int i = UI_POWERUP_REDICON; i <= UI_POWERUP_YELLOWICON; ++i)
		static_cast<CUI_PowerUp_Icon*>(m_pUI[i])->SetIconInActive();
	static_cast<CUI_PowerUp_Icon*>(m_pUI[idx])->SetIconActive();

	static_cast<CUI_PowerUp_Icon*>(m_pUI[UI_POWERUP_SKILLBIGICON])->Set_Texture_Index(m_iSelectIdx);


	/* WESN Level */
	for (int i = POWERUP_HEALTH; i <= POWERUP_SPECIAL; ++i)
	{
		if (m_iSkillLevel[i] == m_iPowerUpMax[i])
		{
			m_pUI[GetSelectImgIdx(i)]->Set_Texture_Index(1);
			continue;
		}

		_int iIdx = GetSelectImgIdx(i);
		m_pUI[iIdx]->Set_Active(false);
	}

	/* IF MAX LEVEL */
	if (m_iSkillLevel[m_iSelectIdx] == m_iPowerUpMax[m_iSelectIdx])
		m_pUI[GetSelectImgIdx(m_iSelectIdx)]->Set_Texture_Index(2);
	else
		m_pUI[GetSelectImgIdx(m_iSelectIdx)]->Set_Active(true);
}

void CUI_PowerUp::ResetWESN()
{
	_int fResultWESN = 0;

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < m_iSkillLevel[i]; ++j)
			fResultWESN += m_iWesnCost[i] * j;

		m_iSkillLevel[i] = 1;
	}
	m_iCurWesn += fResultWESN;

	for(int i = UI_POWERUP_REDLEVEL1; i<= UI_POWERUP_YELLOWLEVEL5; ++i)
	{
		m_pUI[i]->Set_Texture_Index(0);
		m_pUI[i]->Set_Active(false);
	}
	ChooseWesn();
	

	CTravis_Manager::Get_Instance()->Increase_Wesn(fResultWESN);
	CTravis_Manager::Get_Instance()->ResetAll();
}

void CUI_PowerUp::LevelUP()
{
	if (m_iSkillLevel[m_iSelectIdx] == m_iPowerUpMax[m_iSelectIdx])
		return;

	_int iWESNcost = m_iWesnCost[m_iSelectIdx] * m_iSkillLevel[m_iSelectIdx];
	if (m_iCurWesn < iWESNcost)
		return;

	/* WESN 비용*/
	m_iCurWesn -= iWESNcost;
	CTravis_Manager::Get_Instance()->Decrease_Wesn(iWESNcost);


	m_bLevelUpMotion = true;
	_int iSelectedInitIdx = GetSelectImgIdx(m_iSelectIdx) - m_iSkillLevel[m_iSelectIdx] + 1;		// 선택한 스킬의 시작Idx
	for (int i = iSelectedInitIdx; i < iSelectedInitIdx + m_iPowerUpMax[m_iSelectIdx] - 1; ++i)
		m_pUI[i]->Set_Active(true);

	/* 뒷 배경 이미지 밝게 */
	m_pUI[UI_POWERUP_BG]->Set_Texture_Index(1);

	/* 선택한 Level highlight 이미지 ON */
	_int iIdx = UI_POWERUP_LINERED + m_iSelectIdx;
	m_pUI[iIdx]->Set_Active(true);

	/* 플레이어 Level Update */
	if (0 == m_iSelectIdx)
		CTravis_Manager::Get_Instance()->Increase_Health(1);
	if (1 == m_iSelectIdx)
		CTravis_Manager::Get_Instance()->Increase_Attack(1);
	if (2 == m_iSelectIdx)
		CTravis_Manager::Get_Instance()->Increase_Battery(1);
	if (3 == m_iSelectIdx)
		CTravis_Manager::Get_Instance()->Increase_Skill(1);
	if (4 == m_iSelectIdx)
		CTravis_Manager::Get_Instance()->Increase_SpecialAttack(1);

	return;
}

void CUI_PowerUp::LevelUpMotion(_float fTimeDelta)
{
	m_fDelayTime += fTimeDelta * 3;

	_float fLimitTime = (_float)1 / (m_iPowerUpMax[m_iSelectIdx]-1) * 10;
	fLimitTime = round(fLimitTime) / 10;

	_int iSelectedInitIdx = GetSelectImgIdx(m_iSelectIdx) - m_iSkillLevel[m_iSelectIdx] + 1;		// 선택한 스킬의 시작Idx

	if (fLimitTime < m_fDelayTime)
	{
		m_fDelayTime = 0.f;

		for(int i = 0; i< m_iPowerUpMax[m_iSelectIdx] - 1; ++i)
		{
			/* 마지막 idx 3 or 2 : 앞자리부터 꺼지기 */
			_int iLastIdxTexture = m_pUI[GetSelectImgLastIdx(m_iSelectIdx)]->Get_Texture_Index();

			if (1 == iLastIdxTexture || 2 == iLastIdxTexture)
			{
				_int iTextureIdx = m_pUI[iSelectedInitIdx + i]->Get_Texture_Index();
				_int iNextTextureIdx = m_pUI[iSelectedInitIdx + i + 1]->Get_Texture_Index();
				_bool bReturnYN = false;
				if (1 == iTextureIdx || 2 == iTextureIdx)
					bReturnYN = iTextureIdx == iNextTextureIdx ? true : false;

				m_pUI[iSelectedInitIdx + i]->Set_Texture_Index(3);
				if ((m_iSkillLevel[m_iSelectIdx] - 1) == i)
					m_pUI[iSelectedInitIdx + i]->Set_Texture_Index(0);

				iLastIdxTexture = m_pUI[GetSelectImgLastIdx(m_iSelectIdx)]->Get_Texture_Index();

				if(0 == iLastIdxTexture || 3 == iLastIdxTexture)
					++m_iLevelUpMotionCnt;

				if (bReturnYN)
					return;
			}

			/* 나도 0, 뒷 IDX 0 이면 RETURN */
			else if (i != m_iPowerUpMax[m_iSelectIdx] - 1)
			{
				_int iTextureIdx = m_pUI[iSelectedInitIdx + i]->Get_Texture_Index();
				_int iNextTextureIdx = m_pUI[iSelectedInitIdx + i + 1]->Get_Texture_Index();
				_bool bReturnYN = false;
				if(0 == iTextureIdx || 3 == iTextureIdx)
					bReturnYN = iTextureIdx == iNextTextureIdx ? true : false;

				m_pUI[iSelectedInitIdx + i]->Set_Texture_Index(1);
				if ((m_iSkillLevel[m_iSelectIdx] - 1) == i)
					m_pUI[iSelectedInitIdx + i]->Set_Texture_Index(2);

				if (bReturnYN)
					return;
			}
		}

	}

	if (3 > m_iLevelUpMotionCnt)
		return;

	/*****************************************************************************************************/

	m_fDelayTime = 0.f;
	m_bLevelUpMotion = false;
	m_iLevelUpMotionCnt = 0;

	/* 이미지 원복 */
	m_pUI[UI_POWERUP_BG]->Set_Texture_Index(0);
	m_pUI[UI_POWERUP_LINERED + m_iSelectIdx]->Set_Active(false);

	/* 아이콘 변경 */
	_int idx = GetSelectImgIdx(m_iSelectIdx);
	_int curIdxMax = UI_POWERUP_REDLEVEL1 + GetSelectImgIdxBefore(m_iSelectIdx) + m_iPowerUpMax[m_iSelectIdx] - 1;

	for(int i = iSelectedInitIdx; i<= idx; ++i)
		m_pUI[i]->Set_Texture_Index(1);

	if (curIdxMax > idx + 1)
		m_pUI[idx + 1]->Set_Texture_Index(0);

	/* 마지막 Idx 일 경우 선택된 이미지로 변경 */
	m_iSkillLevel[m_iSelectIdx]++;
	if (m_iSkillLevel[m_iSelectIdx] == m_iPowerUpMax[m_iSelectIdx])
		m_pUI[GetSelectImgIdx(m_iSelectIdx)]->Set_Texture_Index(2);
}

void CUI_PowerUp::UIPosReset()
{
	for (auto pUI : UIMGR->GetUIVec(L"UI_PowerUP"))
		pUI->Get_Transform()->Set_Position(pUI->GetInitPos());

	for (int i = UI_POWERUP_REDICON; i <= UI_POWERUP_LINEYELLOW; ++i)
		m_pUI[i]->Get_Transform()->Set_Position(m_pUI[i]->GetInitPos());

	m_pUI[UI_POWERUP_KEYGUIDE]->Set_Texture_Index(0);
	m_pUI[UI_POWERUP_SKILLBIGICON]->Set_Active(false);
	m_fTextMoveAmount = 0.f;
}

_int CUI_PowerUp::GetSelectImgIdx(_int curIdx)
{
	if (m_iSkillLevel[curIdx] == m_iPowerUpMax[curIdx])
		return UI_POWERUP_REDLEVEL1 + m_iPowerUpMax[curIdx] - 2 + GetSelectImgIdxBefore(curIdx);
	return UI_POWERUP_REDLEVEL1 + m_iSkillLevel[curIdx] - 1 + GetSelectImgIdxBefore(curIdx);
}

_int CUI_PowerUp::GetSelectImgLastIdx(_int curIdx)
{
	return UI_POWERUP_REDLEVEL1 + GetSelectImgIdxBefore(curIdx + 1) - 1;
}

_int CUI_PowerUp::GetSelectImgIdxBefore(_int curIdx)
{
	_int iBeforeIdxs = 0;
	if (curIdx != 0)
	{
		for (int j = 0; j <= curIdx - 1; ++j)
			iBeforeIdxs += m_iPowerUpMax[j]-1;
	}
	return iBeforeIdxs;
}

wstring CUI_PowerUp::ConvertNumWstr(_int iNum)
{
	wstring wstrNum = to_wstring(iNum);
	if (10 > iNum)
		wstrNum = L"0" + wstrNum;
	return wstrNum;
}

void CUI_PowerUp::SceneInit()
{
	if (m_bInit)
		return;

	m_pGameInstance->SetVolume("BGM - Naomis Lab", 0.f);
	PLAY_SOUND("BGM - PowerUp Machine", true, 0.5f);

	m_bInit = true;
	m_iSelectIdx = 0;

	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_PowerUp", L"UI_PowerUP");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_PowerUp", L"UI_PowerUP_Option");

	UIPosReset();
	PlayerStateCheck();
	ChooseWesn();
}

void CUI_PowerUp::SceneOut()
{
	STOP_SOUND("BGM - PowerUp Machine");
	m_pGameInstance->SetVolume("BGM - Naomis Lab", 0.5f);

	m_bInit = false;
	m_state = POWERUP_MAIN;

	UIMGR->Set_ReturnToPool(L"UI_PowerUP", true);
	UIMGR->Set_ReturnToPool(L"UI_PowerUP_Option", true);

	if (NULL != UIMGR->GetScene(L"SCENE_CCTV"))
		static_cast<CUI_CCTV*>(UIMGR->GetScene(L"SCENE_CCTV"))->Set_CCTVContentClose();

	/* Player KEY IO */
	UNLOCK_PLAYER_KEY_IO;
}

HRESULT CUI_PowerUp::Ready_Objects()
{
	m_pUI.resize(UI_POWERUP_END);

	vector<CUI*> UIvec = {};

	m_pUI[UI_POWERUP_BG] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_BackGround")));

	m_pUI[UI_POWERUP_WESN_NUM0] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"WESN_Num0")));
	m_pUI[UI_POWERUP_WESN_NUM1] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"WESN_Num1")));
	m_pUI[UI_POWERUP_WESN_NUM2] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"WESN_Num2")));
	m_pUI[UI_POWERUP_WESN_NUM3] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"WESN_Num3")));
	m_pUI[UI_POWERUP_WESN_NUM4] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"WESN_Num4")));
	m_pUI[UI_POWERUP_WESN_NUM5] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"WESN_Num5")));

	m_pUI[UI_POWERUP_KEYGUIDE] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_KeyGuide")));

	m_pUI[UI_POWERUP_REDICON] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_PowerUp_Icon", UIMGR->GetDesc(L"PowerUP_IconRed")));
	m_pUI[UI_POWERUP_BLUEICON] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_PowerUp_Icon", UIMGR->GetDesc(L"PowerUP_IconSky")));
	m_pUI[UI_POWERUP_GREENICON] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_PowerUp_Icon", UIMGR->GetDesc(L"PowerUP_IconGreen")));
	m_pUI[UI_POWERUP_PINKICON] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_PowerUp_Icon", UIMGR->GetDesc(L"PowerUP_IconPink")));
	m_pUI[UI_POWERUP_YELLOWICON] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_PowerUp_Icon", UIMGR->GetDesc(L"PowerUP_IconYellow")));

	m_pUI[UI_POWERUP_REDMOVE] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Red_MoveOn")));
	m_pUI[UI_POWERUP_BLUEMOVE] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Blue_MoveOn")));
	m_pUI[UI_POWERUP_GREENMOVE] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Green_MoveOn")));
	m_pUI[UI_POWERUP_PINKMOVE] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Pink_MoveOn")));
	m_pUI[UI_POWERUP_YELLOWMOVE] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Orange_MoveOn")));

	m_pUI[UI_POWERUP_RED_BTN] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Red_BtnSelected")));
	m_pUI[UI_POWERUP_GREEN_BTN] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Green_BtnSelected")));
	m_pUI[UI_POWERUP_PINK_BTN] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Pink_BtnSelected")));
	m_pUI[UI_POWERUP_YELLOW_BTN] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Orange_BtnSelected")));
	m_pUI[UI_POWERUP_BLUE_BTN] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Blue_BtnSelected")));

	m_pUI[UI_POWERUP_REDLEVEL1] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_RedLevel1")));
	m_pUI[UI_POWERUP_REDLEVEL2] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_RedLevel2")));
	m_pUI[UI_POWERUP_REDLEVEL3] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_RedLevel3")));
	m_pUI[UI_POWERUP_REDLEVEL4] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_RedLevel4")));
	m_pUI[UI_POWERUP_REDLEVEL5] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_RedLevel5")));
	m_pUI[UI_POWERUP_REDLEVEL6] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_RedLevel6")));
	m_pUI[UI_POWERUP_REDLEVEL7] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_RedLevel7")));
	m_pUI[UI_POWERUP_REDLEVEL8] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_RedLevel8")));
	m_pUI[UI_POWERUP_REDLEVEL9] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_RedLevel9")));
	m_pUI[UI_POWERUP_BLUELEVEL1] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_SkyLevel1")));
	m_pUI[UI_POWERUP_BLUELEVEL2] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_SkyLevel2")));
	m_pUI[UI_POWERUP_BLUELEVEL3] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_SkyLevel3")));
	m_pUI[UI_POWERUP_BLUELEVEL4] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_SkyLevel4")));
	m_pUI[UI_POWERUP_BLUELEVEL5] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_SkyLevel5")));
	m_pUI[UI_POWERUP_BLUELEVEL6] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_SkyLevel6")));
	m_pUI[UI_POWERUP_BLUELEVEL7] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_SkyLevel7")));
	m_pUI[UI_POWERUP_BLUELEVEL8] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_SkyLevel8")));
	m_pUI[UI_POWERUP_BLUELEVEL9] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_SkyLevel9")));
	m_pUI[UI_POWERUP_GREENLEVEL1] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_GreenLevel1")));
	m_pUI[UI_POWERUP_GREENLEVEL2] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_GreenLevel2")));
	m_pUI[UI_POWERUP_GREENLEVEL3] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_GreenLevel3")));
	m_pUI[UI_POWERUP_GREENLEVEL4] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_GreenLevel4")));
	m_pUI[UI_POWERUP_GREENLEVEL5] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_GreenLevel5")));
	m_pUI[UI_POWERUP_PINKLEVEL1] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_PinkLevel1")));
	m_pUI[UI_POWERUP_PINKLEVEL2] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_PinkLevel2")));
	m_pUI[UI_POWERUP_YELLOWLEVEL1] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_YellowLevel1")));
	m_pUI[UI_POWERUP_YELLOWLEVEL2] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_YellowLevel2")));
	m_pUI[UI_POWERUP_YELLOWLEVEL3] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_YellowLevel3")));
	m_pUI[UI_POWERUP_YELLOWLEVEL4] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_YellowLevel4")));
	m_pUI[UI_POWERUP_YELLOWLEVEL5] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_YellowLevel5")));

	m_pUI[UI_POWERUP_SKILLBIGICON] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_SkillBigIcon")));

	m_pUI[UI_POWERUP_LINERED] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_LvLineRed")));
	m_pUI[UI_POWERUP_LINEBLUE] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_LvLineBlue")));
	m_pUI[UI_POWERUP_LINEGREEN] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_LvLineGreen")));
	m_pUI[UI_POWERUP_LINEPINK] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_LvLinePink")));
	m_pUI[UI_POWERUP_LINEYELLOW] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"PowerUP_LvLineYellow")));


	UIvec.emplace_back(m_pUI[UI_POWERUP_KEYGUIDE]);
	UIvec.emplace_back(m_pUI[UI_POWERUP_SKILLBIGICON]);
	for(int i= UI_POWERUP_BG; i<= UI_POWERUP_WESN_NUM5; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_PowerUP", UIvec);
	UIvec.clear();

	for(int i= UI_POWERUP_REDICON; i<= UI_POWERUP_LINEYELLOW; ++i)
	{
		UIvec.emplace_back(m_pUI[i]);

		_vector vInitPos = m_pUI[i]->GetInitPos();
		m_pUI[i]->Change_InitPos({ XMVectorGetX(vInitPos), XMVectorGetY(vInitPos) -541.f, XMVectorGetZ(vInitPos)});
		m_pUI[i]->Get_Transform()->Move_Root({ 0.f, -541.f, 0.f });
		if (i > (_int)UI_POWERUP_RED_BTN)
			m_pUI[i]->Set_Active(false);
	}
	UIMGR->PutUI(GET_CURLEVEL, L"UI_PowerUP_Option", UIvec);
	UIvec.clear();

	/* Default Setting */
	for (int i = UI_POWERUP_WESN_NUM0; i <= UI_POWERUP_WESN_NUM5; ++i)
		m_pUI[i]->Set_Texture_Index(10);

	m_pUI[UI_POWERUP_SKILLBIGICON]->Set_Active(false);

	return S_OK;
}

CUI_PowerUp* CUI_PowerUp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_PowerUp* pInstance = new CUI_PowerUp(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_PowerUp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_PowerUp::Clone(void* pArg)
{
	CUI_PowerUp* pInstance = new CUI_PowerUp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_PowerUp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PowerUp::Free()
{
	__super::Free();

	for (auto pUI : m_pUI)
		Safe_Release(pUI);
}
