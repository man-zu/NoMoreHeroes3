#include "UI_MiniGame_Toilet.h"

#include "UI_Manager.h"

CUI_MiniGame_Toilet::CUI_MiniGame_Toilet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_MiniGame_Toilet::CUI_MiniGame_Toilet(const CUI_MiniGame_Toilet& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_MiniGame_Toilet::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_MiniGame_Toilet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	m_pTransform->Set_Position({ 0.f, 0.f, 0.2f });

	Ready_Object();

	UIMGR->PutScene(GET_CURLEVEL, L"SCENE_Minigame_Toilet", this);

	return S_OK;
}

void CUI_MiniGame_Toilet::Tick(_float fTimeDelta)
{
	StartIn(fTimeDelta);
	MoveIn(fTimeDelta);
	ShowKeep(fTimeDelta);
}

void CUI_MiniGame_Toilet::LateTick(_float fTimeDelta)
{
	DirCheck();
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT CUI_MiniGame_Toilet::Render()
{
	_vector DefaultColor = { 1.f, 1.f, 1.f, 1.f };

	m_pContext->GSSetShader(nullptr, nullptr, 0);

	//m_pGameInstance->Render_Font(L"FontUI", L"막힌 화장실을 뚫자 !", _float2(640.f, 65.f - 400.f + m_MovePos), .6f, DefaultColor, 0.f, CCustomFont::FontAlignCenter);
	m_pGameInstance->Render_Font(L"FontUI", L"조사", _float2(285.f - 400.f + m_MovePos, 445.f), .6f, DefaultColor, 0.f);
	m_pGameInstance->Render_Font(L"FontUI", L"수리", _float2(165.f - 400.f + m_MovePos, 491.f), .6f, DefaultColor, 0.f);

	return S_OK;
}

void CUI_MiniGame_Toilet::SceneInit()
{
	if (m_bInit)
		return;

	m_bStart = true;
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_Minigame_Toilet", L"UI_Minigame_Toilet");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_Minigame_Toilet", L"UI_Minigame_Toilet_Init");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_Minigame_Toilet", L"UI_Minigame_Toilet_Keep");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_Minigame_Toilet", L"UI_Minigame_Toilet_Mouse");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_Minigame_Toilet", L"UI_Minigame_Toilet_Guide");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_Minigame_Toilet", L"UI_Minigame_Toilet_InGame");

}

void CUI_MiniGame_Toilet::SceneOut()
{
	m_bInit = false;

	UIMGR->Set_ReturnToPool(L"UI_Minigame_Toilet", true);
	UIMGR->Set_ReturnToPool(L"UI_Minigame_Toilet_Init", true);
	UIMGR->Set_ReturnToPool(L"UI_Minigame_Toilet_Keep", true);
	UIMGR->Set_ReturnToPool(L"UI_Minigame_Toilet_Mouse", true);
	UIMGR->Set_ReturnToPool(L"UI_Minigame_Toilet_Guide", true);
	UIMGR->Set_ReturnToPool(L"UI_Minigame_Toilet_InGame", true);
}

void CUI_MiniGame_Toilet::SetFindDdong(_bool bFindYN)
{
	m_bFindDdong = bFindYN;

	if(m_bFindDdong)
	{
		UIMGR->Active_UI(L"UI_Minigame_Toilet_Keep");

		_vector vInitPos = m_pUI[UI_TMINI_KEEP]->GetInitPos();

		m_pUI[UI_TMINI_KEEP]->Get_Transform()->Set_PositionX(XMVectorGetX(vInitPos) + 95.f * sin(XMConvertToRadians(45.f * m_iCurStateDir)));
		m_pUI[UI_TMINI_KEEP]->Get_Transform()->Set_PositionY(XMVectorGetY(vInitPos) + 95.f * cos(XMConvertToRadians(45.f * m_iCurStateDir)));
	}
	else
		UIMGR->Inactive_UI(L"UI_Minigame_Toilet_Keep");
}

void CUI_MiniGame_Toilet::SetMoveMouse(_bool bFindYN)
{
	if (bFindYN)
		UIMGR->Active_UI(L"UI_Minigame_Toilet_Mouse");
	else
		UIMGR->Inactive_UI(L"UI_Minigame_Toilet_Mouse");
}

void CUI_MiniGame_Toilet::StartIn(_float fTimeDelta)
{
	if (!m_bStart)
		return;

	m_fDelayTime -= fTimeDelta;
	if (1.f > m_fDelayTime)
	{
		m_fDelayTime = 0.f;
		m_bStart = false;
		m_bMoveIn = true;

		UIMGR->Active_UI(L"UI_Minigame_Toilet");
		UIMGR->Active_UI(L"UI_Minigame_Toilet_Guide");
		UIMGR->Active_UI(L"UI_Minigame_Toilet_InGame");
		UIMGR->Inactive_UI(L"UI_Minigame_Toilet_Init");
		return;
	}

	if (4.f > m_fDelayTime)
	{
		UIMGR->Active_UI(L"UI_Minigame_Toilet_Init");
		m_pUI[UI_TMINI_INIT]->Set_Texture_Index((_int)m_fDelayTime);
	}
}

void CUI_MiniGame_Toilet::MoveIn(_float fTimeDelta)
{
	if (m_bInit)
		return;

	_bool bResult = true;

	if(m_bMoveIn)
	{
		_vector vCurpos = m_pUI[UI_TMINI_TITLE]->Get_Transform()->Get_Position();
		_float MoveY = JoMath::Lerp(XMVectorGetY(vCurpos), m_pUI[UI_TMINI_TITLE]->GetInitPosY(), 0.05f);
		m_MovePos += XMVectorGetY(vCurpos) - MoveY;
		m_pUI[UI_TMINI_TITLE]->Get_Transform()->Set_PositionY(MoveY);

		if (0.1f < m_pUI[UI_TMINI_TITLE]->GetInitPosY() - vCurpos.m128_f32[1])
			bResult = false;

		for (auto& pUI : UIMGR->GetUIVec(L"UI_Minigame_Toilet_Guide"))
		{
			vCurpos = pUI->Get_Transform()->Get_Position();
			pUI->Get_Transform()->Set_PositionX(JoMath::Lerp(XMVectorGetX(vCurpos), pUI->GetInitPosX(), 0.05f));

			if (0.1f < pUI->GetInitPosX() - vCurpos.m128_f32[0])
				bResult = false;
		}

		m_bInit = bResult;
		m_bMoveIn = !bResult;
	}
	else if (!m_bMoveIn)
	{
		_vector vCurpos = m_pUI[UI_TMINI_TITLE]->Get_Transform()->Get_Position();
		_float MoveY = JoMath::Lerp(XMVectorGetY(vCurpos), m_pUI[UI_TMINI_TITLE]->GetInitPosY() + 400.f, 0.05f);
		m_MovePos += XMVectorGetY(vCurpos) - MoveY;
		m_pUI[UI_TMINI_TITLE]->Get_Transform()->Set_PositionY(MoveY);

		for (auto& pUI : UIMGR->GetUIVec(L"UI_Minigame_Toilet_Guide"))
		{
			vCurpos = pUI->Get_Transform()->Get_Position();
			pUI->Get_Transform()->Set_PositionX(JoMath::Lerp(XMVectorGetX(vCurpos), pUI->GetInitPosX() - 400.f, 0.05f));
		}
	}
}

void CUI_MiniGame_Toilet::ShowKeep(_float fTimeDelta)
{
	if (!m_bFindDdong)
		return;

	/* 빨간 위치 Texture 변경 */
	m_fKeepDelayTime += fTimeDelta * 5.f;
	if (3.f < m_fKeepDelayTime)
		m_fKeepDelayTime = 0.f;

	m_pUI[UI_TMINI_KEEP]->Set_Texture_Index((_int)m_fKeepDelayTime);

	/* 초록색 마우스 위 아래 이동 */
	_vector vCurPos = m_pUI[UI_TMINI_MOUSE]->Get_Pos();
	_vector vInitPos = m_pUI[UI_TMINI_MOUSE]->GetInitPos();
	m_pUI[UI_TMINI_MOUSE]->Get_Transform()->Set_PositionY(XMVectorGetY(vCurPos) + 8.f * m_iMouseMoveDir);

	if(50.f < abs(abs(XMVectorGetY(vInitPos)) - abs(XMVectorGetY(vCurPos))))
	{
		m_pUI[UI_TMINI_MOUSE]->Get_Transform()->Set_PositionY(XMVectorGetY(vInitPos) + 50.f * m_iMouseMoveDir);
		m_iMouseMoveDir *= -1;
	}
}

void CUI_MiniGame_Toilet::DirCheck()
{
	if (!m_bInit)
		return;

	_vector vInitPos = m_pUI[UI_TMINI_POS]->GetInitPos();

	if (8 <= m_iCurStateDir)
	{
		m_pUI[UI_TMINI_POS]->Get_Transform()->Set_Position(m_pUI[UI_TMINI_POS]->GetInitPos());
		return;
	}

	m_pUI[UI_TMINI_POS]->Get_Transform()->Set_PositionX(XMVectorGetX(vInitPos) + 95.f * sin(XMConvertToRadians(45.f * m_iCurStateDir)));
	m_pUI[UI_TMINI_POS]->Get_Transform()->Set_PositionY(XMVectorGetY(vInitPos) + 95.f * cos(XMConvertToRadians(45.f * m_iCurStateDir)));
}

void CUI_MiniGame_Toilet::HideGameUI()
{
	UIMGR->Inactive_UI(L"UI_Minigame_Toilet_Keep");
	UIMGR->Inactive_UI(L"UI_Minigame_Toilet_Mouse");
	UIMGR->Inactive_UI(L"UI_Minigame_Toilet_InGame");
}

HRESULT CUI_MiniGame_Toilet::Ready_Object()
{
	vector<CUI*> UIvec = {};
	m_pUI.resize(UI_TMINI_END);

	/* UI_Minigame_Toilet_Init */
	m_pUI[UI_TMINI_INIT] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TMini_Init")));
	
	UIvec.emplace_back(m_pUI[UI_TMINI_INIT]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_Minigame_Toilet_Init", UIvec);
	UIvec.clear();


	/* UI_Minigame_Toilet_Guide */
	m_pUI[UI_TMINI_GUIDE] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TMini_Guide")));
	m_pUI[UI_TMINI_MOUSEGUIDE] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TMini_MouseGuide")));
	m_pUI[UI_TMINI_W] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TMini_W")));
	m_pUI[UI_TMINI_A] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TMini_A")));
	m_pUI[UI_TMINI_S] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TMini_S")));
	m_pUI[UI_TMINI_D] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TMini_D")));
	
	for (_int i = UI_TMINI_GUIDE; i <= (_uint)UI_TMINI_D; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_Minigame_Toilet_Guide", UIvec);
	UIvec.clear();

	/* UI_Minigame_Toilet_Keep */
	m_pUI[UI_TMINI_KEEP] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TMini_Keep")));
	m_pUI[UI_TMINI_KEEPWORLD] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TMini_KeepWorld")));
	for (_int i = UI_TMINI_KEEP; i <= (_uint)UI_TMINI_KEEPWORLD; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_Minigame_Toilet_Keep", UIvec);
	UIvec.clear();

	/* UI_Minigame_Toilet_Mouse */
	m_pUI[UI_TMINI_MOUSE] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TMini_Mouse")));
	UIvec.emplace_back(m_pUI[UI_TMINI_MOUSE]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_Minigame_Toilet_Mouse", UIvec);
	UIvec.clear();


	/* UI_Minigame_Toilet */
	m_pUI[UI_TMINI_TITLE] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TMini_Title")));

	UIvec.emplace_back(m_pUI[UI_TMINI_TITLE]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_Minigame_Toilet", UIvec);
	UIvec.clear();


	/* UI_Minigame_Toilet_InGame */
	m_pUI[UI_TMINI_CIRCLE] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TMini_Circle")));
	m_pUI[UI_TMINI_POS] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TMini_Pos")));

	for (_int i = UI_TMINI_CIRCLE; i <= (_uint)UI_TMINI_POS; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_Minigame_Toilet_InGame", UIvec);
	UIvec.clear();



	/******************************************* Init Setting *******************************************/

	m_pUI[UI_TMINI_TITLE]->Get_Transform()->Move_Root({ 0.f, 400.f, 0.f });
	for(auto& pUI : UIMGR->GetUIVec(L"UI_Minigame_Toilet_Guide"))
		pUI->Get_Transform()->Move_Root({ -400.f, 0.f, 0.f });

	UIMGR->Inactive_UI(L"UI_Minigame_Toilet");
	UIMGR->Inactive_UI(L"UI_Minigame_Toilet_Keep");
	UIMGR->Inactive_UI(L"UI_Minigame_Toilet_Mouse");
	UIMGR->Inactive_UI(L"UI_Minigame_Toilet_Init");
	UIMGR->Inactive_UI(L"UI_Minigame_Toilet_Guide");
	UIMGR->Inactive_UI(L"UI_Minigame_Toilet_InGame");
	m_pUI[UI_TMINI_MOUSE]->Set_Active(false);

	m_pUI[UI_TMINI_INIT]->Set_RawValue("g_vUIColor", &m_GreenColor, sizeof(_float3), true);

	return S_OK;
}

CUI_MiniGame_Toilet* CUI_MiniGame_Toilet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MiniGame_Toilet* pInstance = new CUI_MiniGame_Toilet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_MiniGame_Toilet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_MiniGame_Toilet::Clone(void* pArg)
{
	CUI_MiniGame_Toilet* pInstance = new CUI_MiniGame_Toilet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_MiniGame_Toilet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MiniGame_Toilet::Free()
{
	__super::Free();

	for (auto pUI : m_pUI)
		Safe_Release(pUI);
}
