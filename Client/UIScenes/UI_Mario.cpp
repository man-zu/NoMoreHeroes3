#include "UI_Mario.h"

#include "UI_Manager.h"

CUI_Mario::CUI_Mario(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_Mario::CUI_Mario(const CUI_Mario& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_Mario::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_Mario::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	Ready_Object();

	UIMGR->PutScene(GET_CURLEVEL, L"SCENE_Mario", this);

	return S_OK;
}

void CUI_Mario::Tick(_float fTimeDelta)
{
	MoveIn();
	CheckCoinChange(fTimeDelta);
	CoinMotion(fTimeDelta);
	TimerSync(fTimeDelta);


	if (KEY_DOWN(eKeyCode::J))
		UIMGR->GetScene(L"SCENE_Mario")->GetSceneUI(UI_MARIO_STAR0+0)->Set_Texture_Index(1);
	if (KEY_DOWN(eKeyCode::K))
		UIMGR->GetScene(L"SCENE_Mario")->GetSceneUI(UI_MARIO_STAR0+1)->Set_Texture_Index(1);
	if (KEY_DOWN(eKeyCode::L))
		UIMGR->GetScene(L"SCENE_Mario")->GetSceneUI(UI_MARIO_STAR0+2)->Set_Texture_Index(1);
	//if (KEY_DOWN(eKeyCode::K))
	//{
	//	static_cast<CUI_Mario*>(UIMGR->GetScene(L"SCENE_Mario"))->GetCoin(m_iCurCoin);
	//}
	//if (KEY_DOWN(eKeyCode::L))
	//	UIMGR->GetScene(L"SCENE_Mario")->SceneInit();
}

void CUI_Mario::LateTick(_float fTimeDelta)
{

}

HRESULT CUI_Mario::Render()
{
	return S_OK;
}

void CUI_Mario::SceneInit()
{
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_CheckPoint", L"UI_Mario_Num");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_CheckPoint", L"UI_Mario_Timer");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_CheckPoint", L"UI_Mario_Star");

	m_bMoveIn = true;
}

void CUI_Mario::SceneOut()
{
	UIMGR->Set_ReturnToPool(L"UI_Mario_Num", true);
	UIMGR->Set_ReturnToPool(L"UI_Mario_Timer", true);
	UIMGR->Set_ReturnToPool(L"UI_Mario_Star", true);
}

void CUI_Mario::MoveIn()
{
	if (!m_bMoveIn)
		return;

	for (auto pUI : UIMGR->GetUIVec(L"UI_Mario_Num"))
	{
		_vector vCurpos = pUI->Get_Transform()->Get_Position();
		vCurpos.m128_f32[0] = JoMath::Lerp(XMVectorGetX(vCurpos), pUI->GetInitPosX(), 0.05f);
		pUI->Get_Transform()->Set_Position(vCurpos);
	
		if (0.1f >= pUI->GetInitPosX() - vCurpos.m128_f32[0])
			m_bMoveIn = false;
	}

	for (auto pUI : UIMGR->GetUIVec(L"UI_Mario_Timer"))
	{
		_vector vCurpos = pUI->Get_Transform()->Get_Position();
		vCurpos.m128_f32[0] = JoMath::Lerp(XMVectorGetX(vCurpos), pUI->GetInitPosX(), 0.05f);
		pUI->Get_Transform()->Set_Position(vCurpos);
	}

	for (auto pUI : UIMGR->GetUIVec(L"UI_Mario_Star"))
	{
		_vector vCurpos = pUI->Get_Transform()->Get_Position();
		vCurpos.m128_f32[1] = JoMath::Lerp(XMVectorGetY(vCurpos), pUI->GetInitPosY(), 0.05f);
		pUI->Get_Transform()->Set_Position(vCurpos);
	}

	if (!m_bMoveIn)
		m_bLimitTimerOn = true;
}

void CUI_Mario::CheckCoinChange(_float fTimeDelta)
{
	if (m_iCurCoin != m_iViewCoin)
	{
		m_bJump = true;
		m_vYellowColor = { 1.f, .8f, 0.f };

		m_iViewCoin++;
		CoinNumSync();
	}
}

void CUI_Mario::CoinMotion(_float fTimeDelta)
{
	if (!m_bJump && !m_bJFall)
		return;

	for (auto pUI : UIMGR->GetUIVec(L"UI_Mario_Num"))
	{
		_float fOrignPosY = pUI->GetInitPosY();
		_vector vCurpos = pUI->Get_Transform()->Get_Position();

		if(m_bJump)
		{
			vCurpos.m128_f32[1] = JoMath::Lerp(XMVectorGetY(vCurpos), fOrignPosY + 14.f, 0.3f);
			pUI->Get_Transform()->Set_Position(vCurpos);

			if (0.2f >= fOrignPosY + 12.f - vCurpos.m128_f32[1])
			{
				m_bJump = false;
				m_bJFall = true;
			}
		}
		else if(m_bJFall)
		{
			vCurpos.m128_f32[1] = JoMath::Lerp(XMVectorGetY(vCurpos), fOrignPosY, 0.3f);
			pUI->Get_Transform()->Set_Position(vCurpos);

			if (0.2f >= XMVectorGetY(vCurpos) - fOrignPosY)
			{
				m_bJFall = false;
				m_vYellowColor = {1.f, 1.f, 1.f};
			}
		}
	}
}

void CUI_Mario::TimerSync(_float fTimeDelta)
{
	if (m_bLimitTimerOn)
		m_fLimitTimer -= fTimeDelta;

	if (0 > m_fLimitTimer)
	{
		m_fLimitTimer = 0.f;
		m_bLimitTimerOn = false;
	}

	_int iMinute = (_int)m_fLimitTimer / 60;
	_int iSecond = (_int)m_fLimitTimer % 60;

	wstring wstrMinute = to_wstring(iMinute);
	wstring wstrSecond = to_wstring(iSecond);

	if (10 > iMinute)
		wstrMinute = L"0" + wstrMinute;
	if (10 > iSecond)
		wstrSecond = L"0" + wstrSecond;

	for (int i = 0; i <= 1; ++i)
	{
		m_pUI[UI_MARIO_TIMEM + i]->Set_Texture_Index(stoi(wstrMinute.substr(i, 1)));
		m_pUI[UI_MARIO_TIMES + i]->Set_Texture_Index(stoi(wstrSecond.substr(i, 1)));
	}
}

void CUI_Mario::CoinNumSync()
{
	string strNum = to_string(m_iViewCoin);

	for (_int i = 0; i < strNum.length(); ++i)
	{
		_int iMyNum = stoi(strNum.substr(i, 1));
		_int idx = UI_MARIO_NUMLLL + 3 - (_int)strNum.length() + i;

		m_pUI[idx]->Set_Texture_Index(iMyNum);
	}
}

HRESULT CUI_Mario::Ready_Object()
{
	vector<CUI*> UIvec = {};
	m_pUI.resize(UI_MARIO_END);

	m_pUI[UI_MARIO_COIN] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Mario_Coin")));
	m_pUI[UI_MARIO_NUMLLL] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Mario_NumLLL")));
	m_pUI[UI_MARIO_NUMLL] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Mario_NumLL")));
	m_pUI[UI_MARIO_NUML] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Mario_NumL")));

	m_pUI[UI_MARIO_TIMEM] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Mario_TimeM")));
	m_pUI[UI_MARIO_TIMEMM] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Mario_TimeMM")));
	m_pUI[UI_MARIO_TIMES] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Mario_TimeS")));
	m_pUI[UI_MARIO_TIMESS] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Mario_TimeSS")));
	m_pUI[UI_MARIO_TIMEDOT0] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Mario_TimeDot0")));
	m_pUI[UI_MARIO_TIMEDOT1] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Mario_TimeDot1")));
	
	m_pUI[UI_MARIO_STAR0] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Mario_Star0")));
	m_pUI[UI_MARIO_STAR1] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Mario_Star1")));
	m_pUI[UI_MARIO_STAR2] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Mario_Star2")));


	for (_int i = UI_MARIO_COIN; i <= (_uint)UI_MARIO_NUML; ++i)
	{
		UIvec.emplace_back(m_pUI[i]);
		m_pUI[i]->Set_RawValue("g_vUIColor", &m_vYellowColor, sizeof(_float3), true);
	}
	UIMGR->PutUI(GET_CURLEVEL, L"UI_Mario_Num", UIvec);
	UIvec.clear();

	for (_int i = UI_MARIO_TIMEM; i <= (_uint)UI_MARIO_TIMEDOT1; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_Mario_Timer", UIvec);
	UIvec.clear();

	for (_int i = UI_MARIO_STAR0; i <= (_uint)UI_MARIO_STAR2; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_Mario_Star", UIvec);
	UIvec.clear();

	for (auto pUI : UIMGR->GetUIVec(L"UI_Mario_Num"))
		pUI->Get_Transform()->Move_Root({ -100.f, 0.f, 0.f });

	for(auto pUI : UIMGR->GetUIVec(L"UI_Mario_Timer"))
		pUI->Get_Transform()->Move_Root({ +100.f, 0.f, 0.f });

	for(auto pUI : UIMGR->GetUIVec(L"UI_Mario_Star"))
		pUI->Get_Transform()->Move_Root({ 0.f , +100.f, 0.f });

	return S_OK;
}

CUI_Mario* CUI_Mario::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Mario* pInstance = new CUI_Mario(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_Mario"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Mario::Clone(void* pArg)
{
	CUI_Mario* pInstance = new CUI_Mario(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_Mario"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Mario::Free()
{
	__super::Free();

	for (auto pUI : m_pUI)
		Safe_Release(pUI);
}
