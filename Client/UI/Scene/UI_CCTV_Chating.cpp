#include "UI_CCTV_Chating.h"

#include "UI_Manager.h"

CUI_CCTV_Chating::CUI_CCTV_Chating(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_CCTV_Chating::CUI_CCTV_Chating(const CUI_CCTV_Chating& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_CCTV_Chating::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_CCTV_Chating::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	m_pTransform->Set_Position({ 0.f, 0.f, 0.4f });
	
	CCTV_USER* tUser = static_cast<CCTV_USER*>(pArg);
	m_wstrUser = tUser->wstrName;
	m_eUserType = tUser->eUserType;
	m_wstrContent = tUser->wstrContent;
	m_iIdx = tUser->iIdx;
	m_iEnterCnt = tUser->iEnterCnt;
	m_fTextareaSize = tUser->fTextareaSize;

	UIMGR->PutScene(GET_CURLEVEL, L"SCENE_CCTV_Chating", this);

	return S_OK;
}

void CUI_CCTV_Chating::Tick(_float fTimeDelta)
{
	if (!m_bInit)
		return;

	if(m_bChatOn == false && 0 < g_fAlpha_timer)
	{
		g_fAlpha_timer -= fTimeDelta * 2.f;
		if(0 > g_fAlpha_timer)
			m_bChatOn = true;
	}
}

void CUI_CCTV_Chating::LateTick(_float fTimeDelta)
{
	if (0.1f > 1 - g_fAlpha_timer)
		return;

	if(m_bInit)
		m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT CUI_CCTV_Chating::Render()
{
	_float fPosY = XMVectorGetY(m_pTransform->Get_Position());

	_vector PinkColor = { .8f, 0.f, .8f, 1- g_fAlpha_timer };
	_vector DefaultColor = { 0.f, 0.f, 0.f, 1- g_fAlpha_timer };

	m_pContext->GSSetShader(nullptr, nullptr, 0);

	m_pGameInstance->Render_Font(L"FontUIBold", m_wstrUser, _float2(152.f, 581.f - (25.f * (m_iEnterCnt + 1)) + fPosY), 1.f, PinkColor, 0.f, CCustomFont::FontAlignLeft);
	m_pGameInstance->Render_Font(L"FontUIBold", m_wstrContent, _float2(152.f, 610.f - (25.f * (m_iEnterCnt + 1)) + fPosY), 1.f, DefaultColor, 0.f, CCustomFont::FontAlignLeft);

	return S_OK;
}

_bool CUI_CCTV_Chating::MoveUp(_float fY)
{
	_bool bResult = true;

	/* CurScene transform SetY */
	_vector vCurPos = m_pTransform->Get_Position();
	_float fMoveY = JoMath::Lerp(XMVectorGetY(vCurPos), m_vSavePosition.y - fY, 0.1f);
	m_pTransform->Set_PositionY(fMoveY);

	/* UI transform SetY */
	_float fDifferent = fMoveY - XMVectorGetY(vCurPos);
	for (auto pUI : m_pUI)
	{
		_vector vUICurPos = pUI->Get_Pos();
		pUI->Get_Transform()->Set_PositionY(XMVectorGetY(vUICurPos) - fDifferent);
	}


	if(0.5f > abs(m_vSavePosition.y - fY - XMVectorGetY(vCurPos)) )
	{
		XMStoreFloat4(&m_vSavePosition, m_pTransform->Get_Position());
		g_fAlpha_timer += 0.25f;
		bResult = false;
	}

	return bResult;
}

void CUI_CCTV_Chating::SetMoveUp(_float fY)
{
	/* CurScene transform SetY */
	_vector vCurPos = m_pTransform->Get_Position();
	_float fMoveY = m_vSavePosition.y - fY;
	m_pTransform->Set_PositionY(fMoveY);

	/* UI transform SetY */
	_float fDifferent = fMoveY - XMVectorGetY(vCurPos);
	for (auto pUI : m_pUI)
	{
		_vector vUICurPos = pUI->Get_Pos();
		pUI->Get_Transform()->Set_PositionY(XMVectorGetY(vUICurPos) - fDifferent);
	}

	XMStoreFloat4(&m_vSavePosition, m_pTransform->Get_Position());
	g_fAlpha_timer += 0.25f;
}

void CUI_CCTV_Chating::SetContentOn()
{
	SceneInit();

	g_fAlpha_timer = 0.f;
	m_bChatOn = true;
}

void CUI_CCTV_Chating::SceneInit()
{
	if (m_bInit)
		return;

	m_bInit = true;

	Ready_Object();
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_CCTV_Chating", L"UI_CCTV_Chating" + to_wstring(m_iIdx));
}

void CUI_CCTV_Chating::SceneOut()
{
	m_bInit = false;

	UIMGR->Set_ReturnToPool(L"UI_CCTV_Chating" + to_wstring(m_iIdx), true);
	UIMGR->RemoveUI(L"UI_CCTV_Chating" + to_wstring(m_iIdx));
}

HRESULT CUI_CCTV_Chating::Ready_Object()
{
	vector<CUI*> UIvec = {};
	m_pUI.resize(UI_CCTV_CHAT_END);

	m_pUI[UI_CCTV_CHAT_TALKAREA] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_CCTV_TalkArea")));
	m_pUI[UI_CCTV_CHAT_TALKICON] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_CCTV_ChatIcon", UIMGR->GetDesc(L"UI_CCTV_TalkIcon")));


	for(int i = UI_CCTV_CHAT_TALKAREA; i < UI_CCTV_CHAT_END; ++i)
	{
		m_pUI[i]->Set_RawValue("g_fAlpha_Timer", &_float(g_fAlpha_timer), sizeof(_float), true);
		UIvec.emplace_back(m_pUI[i]);
	}
	UIMGR->PutUI(GET_CURLEVEL, L"UI_CCTV_Chating" + to_wstring(m_iIdx), UIvec);
	UIvec.clear();

	/* Set Scale */
	_float3 vScale = m_pUI[UI_CCTV_CHAT_TALKAREA]->Get_Transform()->Get_Scale();
	m_pUI[UI_CCTV_CHAT_TALKAREA]->Get_Transform()->Set_Scale({vScale.x, m_fTextareaSize, vScale.z});

	/* Set Position */
	_vector vPos = m_pUI[UI_CCTV_CHAT_TALKAREA]->Get_Pos();
	m_pUI[UI_CCTV_CHAT_TALKAREA]->Get_Transform()->Set_PositionY(XMVectorGetY(vPos) + (15.f * (m_iEnterCnt + 1)));
	m_pUI[UI_CCTV_CHAT_TALKICON]->Get_Transform()->Set_PositionY(XMVectorGetY(vPos) + (15.f * (m_iEnterCnt + 1)));

	static_cast<CUI_BasicBase*>(m_pUI[UI_CCTV_CHAT_TALKICON])->Set_Texture_Index((_int)m_eUserType);

	return S_OK;
}

CUI_CCTV_Chating* CUI_CCTV_Chating::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_CCTV_Chating* pInstance = new CUI_CCTV_Chating(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_CCTV_Chating"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_CCTV_Chating::Clone(void* pArg)
{
	CUI_CCTV_Chating* pInstance = new CUI_CCTV_Chating(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_CCTV_Chating"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_CCTV_Chating::Free()
{
	__super::Free();

	for (auto pUI : m_pUI)
		Safe_Release(pUI);
}
