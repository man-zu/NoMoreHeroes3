#include "UI_SubTitle.h"

#include "UI_Manager.h"
#include "UI_World_City.h"

CUI_SubTitle::CUI_SubTitle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_SubTitle::CUI_SubTitle(const CUI_SubTitle& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_SubTitle::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_SubTitle::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	m_pTransform->Set_Position({ 0.f, 0.f, 0.f });

	Ready_Object();

	UIMGR->PutScene(GET_CURLEVEL, L"SCENE_SubTitle", this);

	return S_OK;
}

void CUI_SubTitle::Tick(_float fTimeDelta)
{
	MovePattern(fTimeDelta * 2.f);

	if (KEY_DOWN(eKeyCode::Enter))
		NextSubTitle();

	TimerSubTitle(fTimeDelta);
}

void CUI_SubTitle::LateTick(_float fTimeDelta)
{
	if(m_bInit)
	{
		if (m_iIdx >= m_vecContent.size())
			return;

		m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_SubTitle::Render()
{
	_vector WhiteColor = { 1.f, 1.f, 1.f, 1.f };

	m_pContext->GSSetShader(nullptr, nullptr, 0);

	_int iCnt = (_int)m_vecContent[m_iIdx].find(L"\n");
	if(-1 == iCnt)
		m_pGameInstance->Render_Font(L"FontUI", m_vecContent[m_iIdx], _float2(640.f, 640.f), .8f, WhiteColor, 0.f, CCustomFont::FontAlignCenter);
	else
	{
		wstring wstrContent1 = m_vecContent[m_iIdx].substr(0, iCnt);
		wstring wstrContent2 = m_vecContent[m_iIdx].substr(iCnt + 1, m_vecContent[m_iIdx].size());

		m_pGameInstance->Render_Font(L"FontUI", wstrContent1, _float2(640.f, 628.f), .8f, WhiteColor, 0.f, CCustomFont::FontAlignCenter);
		m_pGameInstance->Render_Font(L"FontUI", wstrContent2, _float2(640.f, 661.f), .8f, WhiteColor, 0.f, CCustomFont::FontAlignCenter);
	}

	return S_OK;
}

void CUI_SubTitle::MovePattern(_float fTimeDelta)
{
	if (m_bBackgroundOn && 0 < m_fAlpha_timer)
	{
		m_fAlpha_timer -= fTimeDelta;
		if (0 > m_fAlpha_timer)
			m_bInit = true;
	}

	if(!m_bBackgroundOn && 1 > m_fAlpha_timer)
	{
		m_fAlpha_timer += fTimeDelta;

		if (1 <= m_fAlpha_timer)
			SceneOut();
	}
}

void CUI_SubTitle::ShowSubTitle(wstring wstrName)
{
	if (!m_bInit)
		SceneInit();

	if (NULL != UIMGR->GetSubTitle(wstrName))
		m_vecContent = *(UIMGR->GetSubTitle(wstrName));
	else
		m_vecContent = {};

	m_bTimerMode = false;
}

void CUI_SubTitle::ShowSubTitle_Timer(wstring wstrName, _float _fTotalTime, _float _fInterval)
{
	if (!m_bInit)
		SceneInit();

	if (NULL != UIMGR->GetSubTitle(wstrName)) {
		m_vecContent = *(UIMGR->GetSubTitle(wstrName));
		m_fTotalTime = _fTotalTime;
		m_fInterval = _fInterval;
	}
	else
		m_vecContent = {};

	m_bTimerMode = true;
}

void CUI_SubTitle::HideSubTitle()
{
	if (nullptr != m_FuncOnExit) {
		m_FuncOnExit();
		m_FuncOnExit = nullptr;
	}

	m_bBackgroundOn = false;
}

void CUI_SubTitle::NextSubTitle()
{
	if (!m_bInit)
		return;

	m_iIdx++;

	if (m_iIdx >= m_vecContent.size())
		HideSubTitle();
}

void CUI_SubTitle::TimerSubTitle(_float _fTimeDelta)
{
	if (!m_bInit||!m_bTimerMode)
		return;

	m_fTimeAcc += _fTimeDelta;

	if (m_fTimeAcc > m_fInterval) {
		m_iIdx++;

		if (m_iIdx >= m_vecContent.size())
			HideSubTitle();

		m_fTimeAcc = 0.f;
	}
}

void CUI_SubTitle::SceneInit()
{
	m_bBackgroundOn = true;

	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_SubTitle", L"UI_SubTitle");

	/* [Scene] UI Show/Hide 해야할 경우 추가 */
	if (NULL != UIMGR->GetScene(L"SCENE_World_City"))
		static_cast<CUI_World_City*>(UIMGR->GetScene(L"SCENE_World_City"))->SetVisible(UI_VISIBLE_FALSE);
}

void CUI_SubTitle::SceneOut()
{
	m_iIdx = 0;
	m_bInit = false;
	m_bBackgroundOn = false;

	UIMGR->Set_ReturnToPool(L"UI_SubTitle", true);

	/* [Scene] UI Show/Hide 해야할 경우 추가 */
	if (NULL != UIMGR->GetScene(L"SCENE_World_City"))
		static_cast<CUI_World_City*>(UIMGR->GetScene(L"SCENE_World_City"))->SetVisible(UI_VISIBLE_TRUE);
}

HRESULT CUI_SubTitle::Ready_Object()
{
	vector<CUI*> UIvec = {};
	m_pUI.resize(UI_SUBTITLE_END);
	
	m_pUI[UI_SUBTITLE_BACKGROUND] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SubTitleBG")));
	
	for (_int i = 0; i < (_uint)UI_SUBTITLE_END; ++i)
		UIvec.emplace_back(m_pUI[i]);
	
	UIMGR->PutUI(GET_CURLEVEL, L"UI_SubTitle", UIvec);
	UIvec.clear();

	m_pUI[UI_SUBTITLE_BACKGROUND]->Set_RawValue("g_fAlpha_Timer", &_float(m_fAlpha_timer), sizeof(_float), true);

	return S_OK;
}

CUI_SubTitle* CUI_SubTitle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_SubTitle* pInstance = new CUI_SubTitle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_SubTitle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_SubTitle::Clone(void* pArg)
{
	CUI_SubTitle* pInstance = new CUI_SubTitle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_SubTitle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SubTitle::Free()
{
	__super::Free();

	for (auto pUI : m_pUI)
		Safe_Release(pUI);
}
