#include "UI_CCTV.h"

#include "UI_Manager.h"
#include "UI_SushiShop.h"
#include "UI_CCTV_Chating.h"

#include "Travis_Manager.h"
#include "Travis_World.h"
#include "Various_Camera.h"

#include "UI_World_City.h"

CUI_CCTV::CUI_CCTV(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_CCTV::CUI_CCTV(const CUI_CCTV& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_CCTV::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_CCTV::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	m_pTransform->Set_Position({ 0.f, 0.f, 0.f });

	Ready_Object();

	UIMGR->Inactive_UI(L"UI_CCTV");
	UIMGR->PutScene(GET_CURLEVEL, L"SCENE_CCTV", this);

	return S_OK;
}

void CUI_CCTV::Tick(_float fTimeDelta)
{
	SkipCCTVImg(fTimeDelta);

	if (!m_bInit)
		return;

	m_fDelayTime += fTimeDelta;

	ShowContent(fTimeDelta);
	AddChatArea(fTimeDelta);


	if (KEY_DOWN(eKeyCode::Enter))
	{
		_int iEnterCnt = UIMGR->GetCCTVuser(m_eCurUser).iEnterCnt;
		_int iContentTotCnt = (_int)UIMGR->GetChatingContent(UIMGR->GetCCTVuser(m_eCurUser).eUserType).size();

		if ((1 == iEnterCnt && iContentTotCnt - 1 != m_ChatUIvec.size())
			|| (2 <= iEnterCnt && 0 == m_ChatUIvec.size()))
			AddChating(iEnterCnt);
		else
			SceneOut();
	}

}

void CUI_CCTV::LateTick(_float fTimeDelta)
{
	if (m_bLiveOn)
		m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT CUI_CCTV::Render()
{
	_vector GreenColor = { 0.f, 1.f, 0.f, 1.f };
	_vector RedColor = { 1.f, 0.f, 0.f, 1.f };
	_vector PinkColor = { 1.f, 0.f, 1.f, 1.f };
	_vector YellowColor = { 1.f, 1.f, 0.f, 1.f };
	_vector WhiteColor = { 1.f, 1.f, 1.f, 1.f };

	m_pContext->GSSetShader(nullptr, nullptr, 0);

	/* Timer */
	_int iHour = (_int)m_fDelayTime / 720;
	_int iMin = (_int)m_fDelayTime / 60;
	_int iSecond = (_int)m_fDelayTime % 60;
	wstring wstrTime = MakeIntToString(iHour, 2) + L":" + MakeIntToString(iMin, 2) + L":" + MakeIntToString(iSecond, 2);

	m_pGameInstance->Render_Font(L"FontUI", wstrTime, _float2(1095.f, 590.f), .9f, YellowColor, 0.f, CCustomFont::FontAlignLeft);

	/* Live */
	m_pGameInstance->Render_Font(L"FontUI", L"ON AIR", _float2(1140.f, 57.f), .38f, GreenColor, 0.f, CCustomFont::FontAlignLeft);
	m_pGameInstance->Render_Font(L"FontUI", L"REC", _float2(1218.f, 57.f), .38f, RedColor, 0.f, CCustomFont::FontAlignLeft);
	m_pGameInstance->Render_Font(L"FontUI", to_wstring(LiveViewers()), _float2(1140.f, 89.f), .38f, WhiteColor, 0.f, CCustomFont::FontAlignLeft);
	m_pGameInstance->Render_Font(L"FontUI", to_wstring(m_ChatUIvec.size()), _float2(1187.f, 89.f), .38f, WhiteColor, 0.f, CCustomFont::FontAlignLeft);

	_float fAlpha = 1 - m_fAlpha_timer;
	if (0 > fAlpha)
		fAlpha = 0.f;

	PinkColor.m128_f32[3] = fAlpha;
	YellowColor.m128_f32[3] = fAlpha;

	if(0 >= m_fAlpha_timer )
	{
		m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(m_iViewUtopiCoin), _float2(1200.f, 163.f), .9f, YellowColor, 0.f, CCustomFont::FontAlignRight);
		m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(m_iWesn), _float2(1200.f, 222.f), .9f, PinkColor, 0.f, CCustomFont::FontAlignRight);
	}

	return S_OK;
}

void CUI_CCTV::Set_UserType(wstring wstrTriggerName)
{
	CCTV_USER tUserInfo = {};
	if(wstrTriggerName == L"Trigger_Interaction_WUFO_Sushi")
	{
		tUserInfo.eUserType = CCTV_USER_SUSHI;
		tUserInfo.wstrName = L"Bugzaburo";
		tUserInfo.wstrSceneName = L"SCENE_SushiShop";
	}
	else if(wstrTriggerName == L"Trigger_Open_WESN")
	{
		tUserInfo.eUserType = CCTV_USER_TREE;
		tUserInfo.wstrName = L"Naomi";
		tUserInfo.wstrSceneName = L"SCENE_PowerUP";
	}
	else if(wstrTriggerName == L"Trigger_Talk_QuestNPC_Toilet")
	{
		tUserInfo.eUserType = CCTV_USER_CITIZEN_TOILET;
		tUserInfo.wstrName = L"Citizen";
		tUserInfo.wstrSceneName = L"";
	}
	else if(wstrTriggerName == L"Trigger_Talk_QuestNPC_BI")
	{
		tUserInfo.eUserType = CCTV_USER_CITIZEN_ISLAND;
		tUserInfo.wstrName = L"Citizen";
		tUserInfo.wstrSceneName = L"";
	}
	else if(wstrTriggerName == L"Trigger_Talk_QuestNPC_TC")
	{
		tUserInfo.eUserType = CCTV_USER_CITIZEN_TRASH;
		tUserInfo.wstrName = L"Citizen";
		tUserInfo.wstrSceneName = L"";
	}
	m_eCurUser = tUserInfo.eUserType;
	UIMGR->SetCCTVinfo(tUserInfo);
}

void CUI_CCTV::SceneInit()
{
	if (m_bUserContent || m_bInit)
		return;

	/* Player Key 제한 */
	LOCK_PLAYER_KEY_IO;

	/* InitData Setting */
	m_bInit = true;
	m_fAlpha_timer = 1.f;
	m_pUI[UI_CCTV_DULLFILTER]->Set_Active(true);
	
	Ready_PlayerData();
	CheckLevel();

	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_CCTV", L"UI_CCTV");
}

void CUI_CCTV::SceneOut()
{
	m_bInit = false;
	m_bLiveOn = false;

	m_iVeiwer = 0;
	m_fDelayTime = 0.f;

	UIMGR->Set_ReturnToPool(L"UI_CCTV", true);

	/* 채팅 객체 삭제 */
	for (auto pUI : m_ChatUIvec)
	{
		pUI->SceneOut();
		Safe_Release(pUI);
	}
	m_ChatUIvec.resize(0);

	/* userType 별로 init 다르게 해야함 */
	if (L"" != UIMGR->GetCCTVuser(m_eCurUser).wstrSceneName)
	{
		/* 다음 컨텐츠가 있는 경우, UI Scene 에서 Set_CCTVContentClose() 호출 */
		m_bUserContent = true;
		UIMGR->GetScene(UIMGR->GetCCTVuser(m_eCurUser).wstrSceneName)->SceneInit();
	}

	if (CCTV_USER_SUSHI == UIMGR->GetCCTVuser(m_eCurUser).eUserType || CCTV_USER_TREE == UIMGR->GetCCTVuser(m_eCurUser).eUserType)
		return;

	/* SkipCCTV */
	m_bSkipCCTV = true;
	m_pUI[UI_CCTV_LIVEICON_SKIP]->Get_Transform()->Set_Scale({ g_iWinSizeX, g_iWinSizeY, 0.f });
	m_pUI[UI_CCTV_LIVEICON_SKIP]->Get_Transform()->Set_Position(m_pUI[UI_CCTV_LIVEICON_SKIP]->GetInitPos());
	ADD_EVENT(bind(&CUI_Manager::Insert_GameObject, UIMGR, GET_CURLEVEL, L"Layer_CCTV", L"UI_CCTV_Skip"));

	/* Capture CurScene */
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_COPYFINAL, this);

	/* PlayerCam Revert */
	GET_PLAYER_WORLD->Get_PlayerCamera()->Change_State((_uint)VARCAM_STATES::STATE_BACKVIEW);

	/* Player Key 제한풀기 */
	UNLOCK_PLAYER_KEY_IO;
}

HRESULT CUI_CCTV::Ready_Object()
{
	vector<CUI*> UIvec = {};
	m_pUI.resize(UI_CCTV_END);

	m_pUI[UI_CCTV_DULLFILTER] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_CCTV_DullFilter")));
	m_pUI[UI_CCTV_PLAYERSTATUS] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_CCTV_PlayerStatus")));

	m_pUI[UI_CCTV_TIMER] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_CCTV_Timer")));
	m_pUI[UI_CCTV_LIVEBG] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_CCTV_LiveBG")));
	m_pUI[UI_CCTV_LIVEICON_WIFI] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_CCTV_LiveIcon_Wifi")));
	m_pUI[UI_CCTV_LIVEICON_REC] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_CCTV_LiveIcon_REC")));
	m_pUI[UI_CCTV_LIVEICON_USER] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_CCTV_LiveIcon_User")));
	m_pUI[UI_CCTV_LIVEICON_CHAT] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_CCTV_LiveIcon_Chat")));
	m_pUI[UI_CCTV_LIVEICON_SKIP] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_CCTV_SkipImg", UIMGR->GetDesc(L"UI_CCTV_Skip")));


	for(int i = UI_CCTV_DULLFILTER; i <= UI_CCTV_LIVEICON_CHAT; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_CCTV", UIvec);
	UIvec.clear();

	UIvec.emplace_back(m_pUI[UI_CCTV_LIVEICON_SKIP]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_CCTV_Skip", UIvec);
	UIvec.clear();

	m_pUI[UI_CCTV_PLAYERSTATUS]->Set_RawValue("g_fAlpha_Timer", &_float(m_fAlpha_timer), sizeof(_float), true);

	return S_OK;
}

void CUI_CCTV::AddChating(_int iStartIdx)
{
	if (UIMGR->GetChatingContent(UIMGR->GetCCTVuser(m_eCurUser).eUserType).size() == m_ChatUIvec.size())
		return;

	AllChatingActive();

	m_bAddChat = true;


	/*
	* 첫 번째 진입시 m_tCCTVchat[1] ~
	* 두 번째 이후: m_tCCTVchat[0].
	*/
	iStartIdx = iStartIdx >= 2 ? 0 : 1;

	CCTV_USER tDesc;
	tDesc.eUserType = UIMGR->GetCCTVuser(m_eCurUser).eUserType;
	tDesc.wstrName = UIMGR->GetCCTVuser(m_eCurUser).wstrName;
	tDesc.wstrContent = UIMGR->GetChatingContent(UIMGR->GetCCTVuser(m_eCurUser).eUserType)[(_int)m_ChatUIvec.size() + iStartIdx];
	tDesc.iIdx = (_int)m_ChatUIvec.size();
	tDesc.iEnterCnt = FindWordCount(tDesc.wstrContent, L"\n");
	tDesc.fTextareaSize = 25.f + 35.f * (tDesc.iEnterCnt + 1);

	CUI_CCTV_Chating* pObj = static_cast<CUI_CCTV_Chating*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_CCTV_Chating", &tDesc));

	m_pGameInstance->Insert_GameObject(GET_CURLEVEL, L"Layer_CCTV_Chating", pObj);
	m_ChatUIvec.emplace_back(pObj);
	Safe_AddRef(pObj);
}

void CUI_CCTV::AllChatingActive()
{
	if (!m_bAddChat)
		return;

	if (m_ChatUIvec.size() == 0)
		return;

	_float fTextareaY = m_ChatUIvec[m_ChatUIvec.size() - 1]->GetTextAreaY();
	for (int i = 0; i <= m_ChatUIvec.size() - 1; ++i)
	{
		m_ChatUIvec[i]->SetContentOn();
		if(i != m_ChatUIvec.size() - 1)
			m_ChatUIvec[i]->SetMoveUp(fTextareaY + 10.f);
	}
}

void CUI_CCTV::ShowContent(_float fTimeDelta)
{
	if (!m_bLiveOn && .5f <= m_fDelayTime)
	{
		m_bLiveOn = true;

		for (int i = UI_CCTV_TIMER; i <= UI_CCTV_LIVEICON_CHAT; ++i)
			m_pUI[i]->Set_Active(true);
	}

	if (0 < m_fAlpha_timer)
	{
		if (m_bLiveOn && 1.f <= m_fDelayTime)
		{
			m_pUI[UI_CCTV_PLAYERSTATUS]->Set_Active(true);
			m_fAlpha_timer -= fTimeDelta * 1.5f;
		}
	}
}

void CUI_CCTV::AddChatArea(_float fTimeDelta)
{
	if (!m_bAddChat)
		return;

	if (m_ChatUIvec.size() == 0)
		return;

	for (int i = 0; i < m_ChatUIvec.size()-1; ++i)
	{
		_float fTextareaY = m_ChatUIvec[m_ChatUIvec.size() - 1]->GetTextAreaY();
		m_bAddChat = m_ChatUIvec[i]->MoveUp(fTextareaY + 10.f);
	}


	if (m_ChatUIvec.size() == 1 || !m_bAddChat)
	{
		m_bAddChat = false;
		m_ChatUIvec[m_ChatUIvec.size() - 1]->SceneInit();
	}
}

/* Skip CCTV Motion */
void CUI_CCTV::SkipCCTVImg(_float fTimeDelta)
{
	if (!m_bSkipCCTV)
		return;

	_float3 vScale = m_pUI[UI_CCTV_LIVEICON_SKIP]->Get_Transform()->Get_Scale();
	m_pUI[UI_CCTV_LIVEICON_SKIP]->Get_Transform()->Set_Scale({ JoMath::Lerp(vScale.x, g_iWinSizeX >> 1, 0.1f) , JoMath::Lerp(vScale.y, g_iWinSizeY >> 1, 0.1f), vScale.z});

	if ((_int)vScale.x == g_iWinSizeX >> 1)
	{
		_vector vCurPos = m_pUI[UI_CCTV_LIVEICON_SKIP]->Get_Transform()->Get_Position();
		vCurPos.m128_f32[0] -= 30.f;
		m_pUI[UI_CCTV_LIVEICON_SKIP]->Get_Transform()->Set_Position(vCurPos);

		if((-1.f * g_iWinSizeX) > vCurPos.m128_f32[0])
		{
			m_bSkipCCTV = false;
			UIMGR->Set_ReturnToPool(L"UI_CCTV_Skip", true);

			/* Level 별로 show hide 조건 처리 */
			CheckLevel();
		}
	}
}

_int CUI_CCTV::LiveViewers()
{
	if((_int)m_fDelayTime % 3 == 0 && 0.05 > m_fDelayTime - (_int)m_fDelayTime)
	{
		_int iSign = rand() % 2 == 0 ? -1 : 1;
		m_iVeiwer += iSign;

		if (0 > m_iVeiwer)
			m_iVeiwer = 0;
	}
	return m_iVeiwer;
}

_int CUI_CCTV::FindWordCount(wstring wstrContent, wstring Findword)
{
	_int iCnt = 0;
	while (true)
	{
		size_t sfind = wstrContent.find(Findword);
		if (sfind == -1)
			return iCnt;

		++iCnt;
		wstrContent = wstrContent.substr(sfind + 1, wstrContent.size());
	}
	return iCnt;
}

HRESULT CUI_CCTV::Ready_PlayerData()
{
	PLAYER_ABILITY CurABility = CTravis_Manager::Get_Instance()->Get_PlayerAbility();
	m_iViewUtopiCoin = CurABility.iUtopiCoin;
	m_iCurUtopiCoin = CurABility.iUtopiCoin;
	m_iWesn = CurABility.iWesn;

	return S_OK;
}

wstring CUI_CCTV::MakeIntToString(_int _iNum, _int iDigit)
{
	wstring wstrNum = to_wstring(_iNum);
	
	if(wstrNum.length() < iDigit)
		for (int i = 0; i < iDigit - wstrNum.length(); ++i)
			wstrNum = L"0" + wstrNum;

	return wstrNum;
}

void CUI_CCTV::CheckLevel()
{
	if(m_bInit == true)
	{
		if (GET_CURLEVEL == LEVEL_WORLD_CITY)
			static_cast<CUI_World_City*>(UIMGR->GetScene(L"SCENE_World_City"))->SetVisible(UI_VISIBLE_FALSE);
	}
	else
	{
		if (GET_CURLEVEL == LEVEL_WORLD_CITY)
			static_cast<CUI_World_City*>(UIMGR->GetScene(L"SCENE_World_City"))->SetVisible(UI_VISIBLE_TRUE);
	}
}

CUI_CCTV* CUI_CCTV::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_CCTV* pInstance = new CUI_CCTV(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_CCTV"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_CCTV::Clone(void* pArg)
{
	CUI_CCTV* pInstance = new CUI_CCTV(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_CCTV"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_CCTV::Free()
{
	__super::Free();

	for (auto pUI : m_pUI)
		Safe_Release(pUI);

	for (auto pUI : m_ChatUIvec)
		Safe_Release(pUI);
}
