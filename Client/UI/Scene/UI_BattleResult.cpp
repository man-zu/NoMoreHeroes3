#include "UI_BattleResult.h"

#include "UI_Manager.h"
#include "Travis.h"

CUI_BattleResult::CUI_BattleResult(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_BattleResult::CUI_BattleResult(const CUI_BattleResult& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_BattleResult::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_BattleResult::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	Ready_Object();

	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_Battle_Result", L"UI_Battle_BG");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_Battle_Result", L"UI_Battle_Result");
	UIMGR->Inactive_UI(L"UI_Battle_BG");
	UIMGR->Inactive_UI(L"UI_Battle_Result");
	UIMGR->PutScene(GET_CURLEVEL, L"SCENE_BattleResult", this);

	return S_OK;
}

void CUI_BattleResult::Tick(_float fTimeDelta)
{
	MovePattern(fTimeDelta);
	RankIconApear(fTimeDelta);

	/*
	* 0920 금
	* 폰트, ALPHA 등 디테일 추가 필요
	*/
	/* 임시 코드 */
	if (KEY_DOWN(eKeyCode::F3))
		UIMGR->SceneInit(L"SCENE_BattleResult");
	if (KEY_DOWN(eKeyCode::F4))
		UIMGR->SceneOut(L"SCENE_BattleResult");
}

void CUI_BattleResult::LateTick(_float fTimeDelta)
{
	if(m_bActiveResult)
		m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI_FONT, this);
}

HRESULT CUI_BattleResult::Render()
{
	wstring strText = L"00:00:00";
	_int iLength = (_int)strText.length();

	_int iGetDamaged = 0;
	_int iSkillCnt = 0;

	if(m_pUI[UI_BATTLE_RESULT_CLEARTIME]->Is_Active())
		m_pGameInstance->Render_Font(L"FontUIBold", strText, _float2(1170.f, 166.f), 1.4f, XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, CCustomFont::FontAlignRight);

	if(m_pUI[UI_BATTLE_RESULT_GETDAMAGED]->Is_Active())
		m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(iGetDamaged), _float2(1170.f, 216.f), 1.4f, XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, CCustomFont::FontAlignRight);

	if(m_pUI[UI_BATTLE_RESULT_SKILLCNT]->Is_Active())
		m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(iSkillCnt), _float2(1170.f, 266.f), 1.4f, XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, CCustomFont::FontAlignRight);

	if(m_pUI[UI_BATTLE_RESULT_PLAYERSKILLALL]->Is_Active())
	{
		m_pGameInstance->Render_Font(L"FontUIBold", L"01234567", _float2(690.f, 352.f), 1.4f, XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, CCustomFont::FontAlignRight);
		m_pGameInstance->Render_Font(L"FontUIBold", L"01234567", _float2(690.f, 405.f), 1.4f, XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, CCustomFont::FontAlignRight);
	}

	if(m_pUI[UI_BATTLE_RESULT_REWARD]->Is_Active())
	{
		m_pGameInstance->Render_Font(L"FontUIBold", L"88,888", _float2(1060.f, 430.f), 2.f, XMVectorSet(1.f, 1.f, 0.f, 1.f), 0.f, CCustomFont::FontAlignCenter);
		m_pGameInstance->Render_Font(L"FontUIBold", L"99,999", _float2(1060.f, 512.f), 2.f, XMVectorSet(1.f, 0.f, 1.f, 1.f), 0.f, CCustomFont::FontAlignCenter);
	}

	return S_OK;
}

void CUI_BattleResult::SceneInit()
{
	UIMGR->Active_UI(L"UI_Battle_BG");
	m_bActiveResult = true;
}

void CUI_BattleResult::SceneOut()
{
	UIMGR->Inactive_UI(L"UI_Battle_BG");
	UIMGR->Inactive_UI(L"UI_Battle_Result");

	m_bActiveResult = false;
	m_iActivedIdx = (_int)UI_BATTLE_RESULT_RESULT;
}

HRESULT CUI_BattleResult::Ready_Object()
{
	/* 사용할 객체 프로토타입 UIMGR->GetDesc(L"PrototypeName") 가져와서 생성. */

	vector<CUI*> UIvec = {};
	m_pUI.resize(UI_BATTLE_RESULT_END);

	m_pUI[UI_BATTLE_RESULT_RANKICON] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Result_RankIcon")));
	m_pUI[UI_BATTLE_RESULT_BLACKBG] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Result_BlackBackground")));
	m_pUI[UI_BATTLE_RESULT_BG] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Result_BackGroundImg")));
	m_pUI[UI_BATTLE_RESULT_CLEARTIME] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Result_TxtClearTime")));
	m_pUI[UI_BATTLE_RESULT_GETDAMAGED] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Result_TxtDamaged")));
	m_pUI[UI_BATTLE_RESULT_RESULT] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Result_TxtResult")));
	m_pUI[UI_BATTLE_RESULT_REWARD] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Result_TxtReward")));
	m_pUI[UI_BATTLE_RESULT_SKILLCNT] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Result_TxtSkillCnt")));
	m_pUI[UI_BATTLE_RESULT_PLAYERSKILLALL] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Result_TxtPlayerSkillCnt")));
	m_pUI[UI_BATTLE_RESULT_RANK] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Result_TxtRank")));


	for (_int i = UI_BATTLE_RESULT_BLACKBG; i <= (_uint)UI_BATTLE_RESULT_BG; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_Battle_BG", UIvec);
	UIvec.clear();

	for (_int i = UI_BATTLE_RESULT_RESULT; i <= (_uint)UI_BATTLE_RESULT_REWARD; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_Battle_Result", UIvec);
	UIvec.clear();


	m_vScale = m_pUI[UI_BATTLE_RESULT_RANKICON]->Get_Transform()->Get_Scale();
	m_pUI[UI_BATTLE_RESULT_RANKICON]->Get_Transform()->Set_Scale({ m_vScale.x * 500.f, m_vScale.y * 500.f, 0.f });
	UIvec.clear();

	return S_OK;
}

void CUI_BattleResult::MovePattern(_float fTimeDelta)
{
	if (!m_bActiveResult)
		return;

	m_fDelayTime += fTimeDelta;
	if(m_fDelayTime > .5f)
	{
		if (m_fDelayTime > 1.f)
		{
			m_fDelayTime = 0.f;
			if (m_iActivedIdx <= (_int)UI_BATTLE_RESULT_RESULT)
				m_pUI[m_iActivedIdx++]->Set_Active(true);
			return;
		}

		if (m_iActivedIdx <= (_int)UI_BATTLE_RESULT_REWARD)
			m_pUI[m_iActivedIdx++]->Set_Active(true);

		m_fDelayTime = 0.f;
	}
}

void CUI_BattleResult::RankIconApear(_float fTimeDelta)
{
	if (m_iActivedIdx < UI_BATTLE_RESULT_RANK)
		return;

	_float3 vCurScale = m_pUI[UI_BATTLE_RESULT_RANKICON]->Get_Transform()->Get_Scale();

	if(m_vScale.x < vCurScale.x)
		m_pUI[UI_BATTLE_RESULT_RANKICON]->Get_Transform()->Set_Scale({ vCurScale.x * .935f, vCurScale.y * .935f, 0.f });
}

CUI_BattleResult* CUI_BattleResult::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_BattleResult* pInstance = new CUI_BattleResult(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_BattleResult"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_BattleResult::Clone(void* pArg)
{
	CUI_BattleResult* pInstance = new CUI_BattleResult(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_BattleResult"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BattleResult::Free()
{
	__super::Free();

	for (auto pUI : m_pUI)
		Safe_Release(pUI);
}
