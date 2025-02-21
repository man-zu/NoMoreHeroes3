#include "UI_MainMenu.h"

#include "Travis.h"

CUI_MainMenu::CUI_MainMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_MainMenu::CUI_MainMenu(const CUI_MainMenu& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_MainMenu::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_MainMenu::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	Ready_Object();

	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_MainMenu", L"UI_MainMenu");
	UIMGR->Inactive_UI(L"UI_MainMenu");
	UIMGR->PutScene(GET_CURLEVEL, L"SCENE_MainMenu", this);

	return S_OK;
}

void CUI_MainMenu::Tick(_float fTimeDelta)
{
	UIKeyEvent();
	MovePattern(fTimeDelta);
}

void CUI_MainMenu::LateTick(_float fTimeDelta)
{

}

HRESULT CUI_MainMenu::Render()
{
	return S_OK;
}

void CUI_MainMenu::SceneInit()
{
	UIMGR->Active_UI(L"UI_MainMenu");

	m_pUI[INS_MAIN_GAMESTART]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 20.f);
	m_pUI[INS_MAIN_EXIT]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 20.f);
	m_pUI[INS_MAIN_PARARIGHT]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 20.f);
	m_pUI[INS_MAIN_PARALEFT]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 20.f);

	m_pUI[INS_MAIN_PARARIGHT]->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4), true);
	m_pUI[INS_MAIN_PARALEFT]->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4), true);
}

void CUI_MainMenu::SceneOut()
{
	m_pUI[INS_MAIN_GAMESTART]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 15.f);
	m_pUI[INS_MAIN_EXIT]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 15.f);
	m_pUI[INS_MAIN_PARARIGHT]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 15.f);
	m_pUI[INS_MAIN_PARALEFT]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 15.f);
}

void CUI_MainMenu::UIKeyEvent()
{
	if (KEY_DOWN(eKeyCode::T))
	{
		PLAY_SOUND("MainMenu_T", false, 1.f);
		SceneInit();
	}

	if (KEY_DOWN(eKeyCode::W))
	{
		PLAY_SOUND("MainMenu_SelectKey", false, 1.f);
		if (m_iCurIdx > (_int)INS_MAIN_GAMESTART)
		{
			m_iCurIdx--;
			MoveParaY(-270.f, 270.f, { 0.f, 1.f, 0.f, 1.f });
		}
	}
	if (KEY_DOWN(eKeyCode::S))
	{
		PLAY_SOUND("MainMenu_SelectKey", false, 1.f);
		if (m_iCurIdx < (_int)INS_MAIN_EXIT)
		{
			m_iCurIdx++;
			MoveParaY(270.f, -270.f, { 1.f, 0.f, 0.f, 1.f });
		}
	}
}

void CUI_MainMenu::MovePattern(_float fTimeDelta)
{
	m_pUI[INS_MAIN_GAMESTART]->Spread(fTimeDelta, .5f, 10.f);
	m_pUI[INS_MAIN_EXIT]->Spread(fTimeDelta, .5f, 10.f);
	m_pUI[INS_MAIN_PARARIGHT]->Spread(fTimeDelta, .5f, 10.f);
	m_pUI[INS_MAIN_PARALEFT]->Spread(fTimeDelta, .5f, 10.f);
}

void CUI_MainMenu::MoveParaY(_float fX, _float fY, _float4 vColor)
{
	_vector vParaR = m_pUI[INS_MAIN_PARARIGHT]->Get_Pos();
	_vector vParaL = m_pUI[INS_MAIN_PARALEFT]->Get_Pos();
	vParaR = XMVectorSet(vParaR.m128_f32[0] - fX, vParaR.m128_f32[1] + fY, vParaR.m128_f32[2], vParaR.m128_f32[3]);
	vParaL = XMVectorSet(vParaL.m128_f32[0] + fX, vParaL.m128_f32[1] + fY, vParaL.m128_f32[2], vParaL.m128_f32[3]);

	m_pUI[INS_MAIN_PARARIGHT]->Get_Transform()->Set_Position(vParaR);
	m_pUI[INS_MAIN_PARALEFT]->Get_Transform()->Set_Position(vParaL);

	m_pUI[INS_MAIN_PARARIGHT]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 1.f);
	m_pUI[INS_MAIN_PARALEFT]->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 1.f);

	m_vColor = vColor;
	m_pUI[INS_MAIN_PARARIGHT]->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4), true);
	m_pUI[INS_MAIN_PARALEFT]->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4), true);
}

HRESULT CUI_MainMenu::Ready_Object()
{
	vector<CUI*> UIvec = {};
	m_pUI.resize(INS_MAIN_END);

	m_pUI[INS_MAIN_GAMESTART] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_NewGame")));
	m_pUI[INS_MAIN_EXIT] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_EXIT")));
	m_pUI[INS_MAIN_PARARIGHT] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_ParaRight")));
	m_pUI[INS_MAIN_PARALEFT] = static_cast<CUI_InstanceBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_InstanceBase", UIMGR->GetDesc(L"IN_ParaLeft")));

	for (_int i = 0; i < (_uint)INS_MAIN_END; ++i)
		UIvec.emplace_back(m_pUI[i]);

	UIMGR->PutUI(GET_CURLEVEL, L"UI_MainMenu", UIvec);
	UIvec.clear();

	return S_OK;
}

CUI_MainMenu* CUI_MainMenu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MainMenu* pInstance = new CUI_MainMenu(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_MainMenu"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_MainMenu::Clone(void* pArg)
{
	CUI_MainMenu* pInstance = new CUI_MainMenu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_MainMenu"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MainMenu::Free()
{
	__super::Free();

	for (auto pUI : m_pUI)
		Safe_Release(pUI);
}
