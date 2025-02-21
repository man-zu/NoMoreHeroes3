#include "UI_Loading.h"

#include "UI_Manager.h"
#include "Texture.h"

CUI_Loading::CUI_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_Loading::CUI_Loading(const CUI_Loading& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_Loading::Initialize_Prototype()
{
	UIMGR->LoadUIData(m_pDevice, m_pContext, L"../../Resources/UI/Data/UI_Loading.dat");

	// UI_Load_Back
	if (FAILED(m_pGameInstance->Add_Prototype((LEVEL)LEVEL_STATIC, L"UI_Loading_Back", CTexture::Create(m_pDevice, m_pContext,
		L"../../Resources/UI/UI_Loading/UI_Loading_Star/UI_Loading_Back.dds"))))
		return E_FAIL;

	/*
	// UI_Load_Start
	if (FAILED(m_pGameInstance->Add_Prototype((LEVEL)LEVEL_STATIC, L"(텍스쳐 이름)", CTexture::Create(m_pDevice, m_pContext,
		L"../../Resources/UI/UI_Loading/UI_Loading_Start/UI_Intro_Start.dds"))))
		return E_FAIL;
	*/

	// UI_Load_Star
	if (FAILED(m_pGameInstance->Add_Prototype((LEVEL)LEVEL_STATIC, L"TX_UI_HUD_AnimeStar_00", CTexture::Create(m_pDevice, m_pContext,
		L"../../Resources/UI/UI_Loading/UI_Loading_Star/TX_UI_HUD_AnimeStar_%02d.dds", 16))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading::Initialize(void* pArg)
{
	m_pUI.resize(UI_LOADING_END);

	// 기능 UI 제외 Static UI만 Active 
	if (FAILED(Ready_Objects()))
		return E_FAIL;

	UIMGR->Insert_GameObject(LEVEL_LOADING, L"Layer_UI_Loading", L"UI_Loading");

	UIMGR->PutScene(LEVEL_LOADING, L"SCENE_Loading", this);
	/*
	*/

	return S_OK;
}

void CUI_Loading::Tick(_float fTimeDelta)
{ 
	if (GET_CURLEVEL == LEVEL_STATIC)
	{
		m_pUI[UI_LOAD_BACK]->Set_Active(true);
		//m_pUI[UI_LOAD_START]->Set_Active(true);
	}
	else
	{
		// 여기서 레벨 특정 조건별로 로딩 이미지 바꾸기
		m_pUI[UI_LOAD_STAR]->Set_Active(true);

		Loading_Star(fTimeDelta);
	}
}

void CUI_Loading::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_Loading::Render()
{
	return S_OK;
}

HRESULT CUI_Loading::Ready_Objects()
{
	wstring UI_ProtoTag = L"Prototype_UI_BasicBase";

	// Clone_GameObject(만드는 UI 종류(Basic / Instance)의 원형(PrototypeTag) , 만든 UI의 원형(PrototypeTag))
	m_pUI[UI_LOAD_BACK] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(UI_ProtoTag, UIMGR->GetDesc(L"UI_Loading_Back")));
	//m_pUI[UI_LOAD_START] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(UI_ProtoTag, UIMGR->GetDesc(L"")));
	m_pUI[UI_LOAD_STAR] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(UI_ProtoTag, UIMGR->GetDesc(L"UI_Load_Star_Logo")));

	vector<CUI*> UIvec = {};

	for (_int i = 0; i < UI_LOADING_END; i++)
		UIvec.emplace_back(m_pUI[i]);

	UIMGR->PutUI(LEVEL_LOADING, L"UI_Loading", UIvec);
	UIvec.clear();

	// 각 로딩 UI 조건별로 켜주기위해 일단 꺼줌
	for (_int i = 0; i < UI_LOADING_END; i++)
		m_pUI[i]->Set_Active(false);

	return S_OK;
}

void CUI_Loading::Loading_Star(_float fTimeDelta)
{
	m_fStar_Loading_Timer += fTimeDelta;
	
	if (m_fStar_Loading_Timer > 0.3f)
	{
		if (m_fStar_Texture_Index == 15)
			m_fStar_Texture_Index = 0;

		m_fStar_Texture_Index++;
		m_fStar_Loading_Timer = 0.f;
	}

	static_cast<CUI_BasicBase*>(m_pUI[UI_LOAD_STAR])->Set_Texture_Index(m_fStar_Texture_Index);
}

CUI_Loading* CUI_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Loading* pInstance = new CUI_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_Loading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Loading::Clone(void* pArg)
{
	CUI_Loading* pInstance = new CUI_Loading(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_Loading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Loading::Free()
{
	__super::Free();

	for (auto pUI : m_pUI)
		Safe_Release(pUI);
}
