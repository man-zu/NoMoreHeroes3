#include "UI_MoveButton.h"

#include "UI_Manager.h"

CUI_MoveButton::CUI_MoveButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_MoveButton::CUI_MoveButton(const CUI_MoveButton& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_MoveButton::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_MoveButton::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	Ready_Object();

	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_Home_ButtonMove", L"UI_Home_ButtonMove");
	UIMGR->Inactive_UI(L"UI_Home_ButtonMove");
	UIMGR->PutScene(GET_CURLEVEL, L"SCENE_HomeButtonMove", this);

	return S_OK;
}

void CUI_MoveButton::Tick(_float fTimeDelta)
{
	MovePattern(fTimeDelta);
}

void CUI_MoveButton::LateTick(_float fTimeDelta)
{

}

HRESULT CUI_MoveButton::Render()
{
	return S_OK;
}

void CUI_MoveButton::SceneInit()
{
	UIMGR->Active_UI(L"UI_Home_ButtonMove");
	SetInitPos();

	m_bMoveInOut = true;
	eMoveStat = UI_MOVE_IN;

}

void CUI_MoveButton::SceneOut()
{
	m_bMoveInOut = true;
	eMoveStat = UI_MOVE_OUT;
}

HRESULT CUI_MoveButton::Ready_Object()
{
	/* 사용할 객체 프로토타입 UIMGR->GetDesc(L"PrototypeName") 가져와서 생성. */

	vector<CUI*> UIvec = {};
	m_pUI.resize(UI_MOVEBUTTON_A_END);

	m_pUI[UI_MOVEBUTTON_A0] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Text0")));
	m_pUI[UI_MOVEBUTTON_A1] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Text1")));
	m_pUI[UI_MOVEBUTTON_A2] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Text2")));
	m_pUI[UI_MOVEBUTTON_A3] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Text3")));
	m_pUI[UI_MOVEBUTTON_A4] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Text4")));
	m_pUI[UI_MOVEBUTTON_A5] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"Text5")));

	SetInitPos();

	for (_int i = 0; i < (_uint)UI_MOVEBUTTON_A_END; ++i)
		UIvec.emplace_back(m_pUI[i]);

	UIMGR->PutUI(GET_CURLEVEL, L"UI_Home_ButtonMove", UIvec);
	UIvec.clear();

	return S_OK;
}

void CUI_MoveButton::MovePattern(_float fTimeDelta)
{
	m_fDirR += fTimeDelta * 0.05f;
	m_fDirL += fTimeDelta * -0.05f;

	if (m_bMoveInOut)
	{
		/* init/out 위치 이동 */
		for (_int i = 0; i < (_uint)UI_MOVEBUTTON_A_END; ++i)
		{
			_float fDir = eMoveStat == UI_MOVE_IN ? -1.f : 1.f;
			_float4 vCurPos;
			XMStoreFloat4(&vCurPos, m_pUI[i]->Get_Pos());

			if (i <= (_uint)UI_MOVEBUTTON_A2)
				m_pUI[i]->Set_Pos({ vCurPos.x, vCurPos.y + 10.f * fDir, vCurPos.z, vCurPos.w});
			else if (i < (_uint)UI_MOVEBUTTON_A_END)
				m_pUI[i]->Set_Pos({ vCurPos.x, vCurPos.y - 10.f * fDir, vCurPos.z, vCurPos.w });

		}

		if ((eMoveStat == UI_MOVE_IN && m_pUI[0]->GetDesc().WorldMat._42 > m_pUI[0]->Get_Transform()->Get_Position().m128_f32[1])
			|| eMoveStat == UI_MOVE_OUT && m_pUI[0]->GetDesc().WorldMat._42 + 250.f < m_pUI[0]->Get_Transform()->Get_Position().m128_f32[1])
			m_bMoveInOut = false;

	}
	else
	{
		/* 마스크 이동 */
		for (_int i = 0; i < (_int)UI_MOVEBUTTON_A_END; ++i)
		{
			if(i % 2 == 0)
				m_pUI[i]->Set_RawValue("g_fTimeDelta", &m_fDirR, sizeof(_float), true);
			else
				m_pUI[i]->Set_RawValue("g_fTimeDelta", &m_fDirL, sizeof(_float), true);

		}
	}
}

void CUI_MoveButton::SetInitPos()
{
	for(_int i = 0; i < (_int)UI_MOVEBUTTON_A_END; ++i)
	{
		m_pUI[i]->Get_Transform()->Set_Position(m_pUI[i]->GetInitPos());

		if (i < 3)
			m_pUI[i]->Get_Transform()->Move_Root({ 0.f, 250.f, 0.f });
		else
			m_pUI[i]->Get_Transform()->Move_Root({ 0.f, -250.f, 0.f });
	}
}

CUI_MoveButton* CUI_MoveButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MoveButton* pInstance = new CUI_MoveButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_MoveButton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_MoveButton::Clone(void* pArg)
{
	CUI_MoveButton* pInstance = new CUI_MoveButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_MoveButton"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MoveButton::Free()
{
	__super::Free();

	for (auto pUI : m_pUI)
		Safe_Release(pUI);
}
