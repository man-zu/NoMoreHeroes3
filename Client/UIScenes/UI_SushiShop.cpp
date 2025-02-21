#include "UI_SushiShop.h"

#include "UI_Manager.h"
#include "UI_SushiShop_PopupText.h"
#include "Travis_Manager.h"
#include "UI_CCTV.h"

#include "Travis_World.h"

CUI_SushiShop::CUI_SushiShop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_SushiShop::CUI_SushiShop(const CUI_SushiShop& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_SushiShop::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_SushiShop::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	m_pTransform->Set_Position({ 0.f, 0.f, 0.6f });

	Ready_SushiMenu();
	Ready_Object();

	m_vOriginMenuScale = m_pUI[UI_STATUS_BACK]->Get_Transform()->Get_Scale();
	m_vOriginPopupScale = m_pUI[UI_SUSHISHOP_POPUPBG]->Get_Transform()->Get_Scale();

	UIMGR->PutScene(GET_CURLEVEL, L"SCENE_SushiShop", this);

	return S_OK;
}

void CUI_SushiShop::Tick(_float fTimeDelta)
{
	SushiMenu(fTimeDelta);
	Popup(fTimeDelta);
	KeyEvent(fTimeDelta);

	//if (KEY_DOWN(eKeyCode::Y))
	//	UIMGR->GetScene(L"SCENE_SushiShop")->SceneInit();
}

void CUI_SushiShop::LateTick(_float fTimeDelta)
{
	if(m_bShowMenuContent)
		m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT CUI_SushiShop::Render()
{
	_float fAlpha = 1- m_fDelayTime;
	if (0 > fAlpha)
		fAlpha = 0.f;

	_vector SelectedColor = { 1.f, 0.f, 1.f, fAlpha };
	_vector DefaultColor = { 0.f, 0.f, 0.f, fAlpha };
	_vector BlueColor = { 0.f, 0.f, 1.f, fAlpha };
	_vector thisColor = {};

	m_pContext->GSSetShader(nullptr, nullptr, 0);

	m_pGameInstance->Render_Font(L"FontUIBold", L"초 밥", _float2(640.f, 52.f), 1.f, DefaultColor, 0.f, CCustomFont::FontAlignCenter);
	m_pGameInstance->Render_Font(L"FontUI", L"돈마이 스시", _float2(260.f, 78.f), .9f, DefaultColor, 0.f);

	if (0 == m_iTabIdx)
	{
		wstring strAmount = L"Uc            ";
		wstring strTot = L" / 5";

		thisColor = m_iSelectedIdx == 0 ? SelectedColor : DefaultColor;
		m_pGameInstance->Render_Font(L"FontUIBold", m_SushiMenus[m_iTabIdx][0]->sMenuName, _float2(300.f, 300.f), .9f, thisColor, 0.f, CCustomFont::FontAlignCenter);
		m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(m_SushiMenus[m_iTabIdx][0]->iPrice), _float2(235.f, 320.f), .9f, BlueColor, 0.f, CCustomFont::FontAlignLeft);
		m_pGameInstance->Render_Font(L"FontUIBold", strAmount + to_wstring(m_SushiMenus[m_iTabIdx][0]->iAmount) + strTot, _float2(270.f, 320.f), .9f, DefaultColor, 0.f, CCustomFont::FontAlignLeft);

		thisColor = m_iSelectedIdx == 1 ? SelectedColor : DefaultColor;
		m_pGameInstance->Render_Font(L"FontUIBold", m_SushiMenus[m_iTabIdx][1]->sMenuName, _float2(540.f, 300.f), .9f, thisColor, 0.f, CCustomFont::FontAlignCenter);
		m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(m_SushiMenus[m_iTabIdx][1]->iPrice), _float2(475.f, 320.f), .9f, BlueColor, 0.f, CCustomFont::FontAlignLeft);
		m_pGameInstance->Render_Font(L"FontUIBold", strAmount + to_wstring(m_SushiMenus[m_iTabIdx][1]->iAmount) + strTot, _float2(510.f, 320.f), .9f, DefaultColor, 0.f, CCustomFont::FontAlignLeft);

		thisColor = m_iSelectedIdx == 2 ? SelectedColor : DefaultColor;
		m_pGameInstance->Render_Font(L"FontUIBold", m_SushiMenus[m_iTabIdx][2]->sMenuName, _float2(755.f, 300.f), .9f, thisColor, 0.f, CCustomFont::FontAlignCenter);
		m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(m_SushiMenus[m_iTabIdx][2]->iPrice), _float2(695.f, 320.f), .9f, BlueColor, 0.f, CCustomFont::FontAlignLeft);
		m_pGameInstance->Render_Font(L"FontUIBold", strAmount + to_wstring(m_SushiMenus[m_iTabIdx][2]->iAmount) + strTot, _float2(730.f, 320.f), .9f, DefaultColor, 0.f, CCustomFont::FontAlignLeft);

		thisColor = m_iSelectedIdx == 3 ? SelectedColor : DefaultColor;
		m_pGameInstance->Render_Font(L"FontUIBold", m_SushiMenus[m_iTabIdx][3]->sMenuName, _float2(980.f, 300.f), .9f, thisColor, 0.f, CCustomFont::FontAlignCenter);
		m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(m_SushiMenus[m_iTabIdx][3]->iPrice), _float2(920.f, 320.f), .9f, BlueColor, 0.f, CCustomFont::FontAlignLeft);
		m_pGameInstance->Render_Font(L"FontUIBold", strAmount + to_wstring(m_SushiMenus[m_iTabIdx][3]->iAmount) + strTot, _float2(955.f, 320.f), .9f, DefaultColor, 0.f, CCustomFont::FontAlignLeft);

		thisColor = m_iSelectedIdx == 4 ? SelectedColor : DefaultColor;
		m_pGameInstance->Render_Font(L"FontUIBold", m_SushiMenus[m_iTabIdx][4]->sMenuName, _float2(540.f, 475.f), .9f, thisColor, 0.f, CCustomFont::FontAlignCenter);
		m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(m_SushiMenus[m_iTabIdx][4]->iPrice), _float2(475.f, 495.f), .9f, BlueColor, 0.f, CCustomFont::FontAlignLeft);
		m_pGameInstance->Render_Font(L"FontUIBold", strAmount + to_wstring(m_SushiMenus[m_iTabIdx][4]->iAmount) + strTot, _float2(510.f, 495.f), .9f, DefaultColor, 0.f, CCustomFont::FontAlignLeft);

		thisColor = m_iSelectedIdx == 5 ? SelectedColor : DefaultColor;
		m_pGameInstance->Render_Font(L"FontUIBold", m_SushiMenus[m_iTabIdx][5]->sMenuName, _float2(755.f, 475.f), .9f, thisColor, 0.f, CCustomFont::FontAlignCenter);
		m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(m_SushiMenus[m_iTabIdx][5]->iPrice), _float2(695.f, 495.f), .9f, BlueColor, 0.f, CCustomFont::FontAlignLeft);
		m_pGameInstance->Render_Font(L"FontUIBold", strAmount + to_wstring(m_SushiMenus[m_iTabIdx][5]->iAmount) + strTot, _float2(730.f, 495.f), .9f, DefaultColor, 0.f, CCustomFont::FontAlignLeft);
	}

	if (1 == m_iTabIdx)
	{
		for (_int i = 0; i < m_SushiMenus[m_iTabIdx].size(); ++i)
		{
			_int _iX = i % 4;
			_float _iY = 0.f;
			if (i > 3)
				_iY += 171.f;

			thisColor = m_iSelectedIdx == i ? SelectedColor : DefaultColor;
			m_pGameInstance->Render_Font(L"FontUIBold", m_SushiMenus[m_iTabIdx][i]->sMenuName, _float2(300.f + (226.f * _iX), 269.f + _iY), .9f, thisColor, 0.f, CCustomFont::FontAlignCenter);
			m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(m_SushiMenus[m_iTabIdx][i]->iPrice), _float2(300.f + (226.f * _iX), 317.f + _iY), .9f, BlueColor, 0.f, CCustomFont::FontAlignLeft);
			m_pGameInstance->Render_Font(L"FontUIBold", L"Uc", _float2(370.f + (226.f * _iX), 317.f + _iY), .9f, DefaultColor, 0.f, CCustomFont::FontAlignRight);
		}
	}

	m_pGameInstance->Render_Font(L"FontUIBold", m_SushiMenus[m_iTabIdx][m_iSelectedIdx]->sMenuGuide1, _float2(210.f, 538.f), 1.f, DefaultColor, 0.f, CCustomFont::FontAlignLeft);
	m_pGameInstance->Render_Font(L"FontUIBold", m_SushiMenus[m_iTabIdx][m_iSelectedIdx]->sMenuGuide2, _float2(210.f, 563.f), 1.f, DefaultColor, 0.f, CCustomFont::FontAlignLeft);

	return S_OK;
}

void CUI_SushiShop::SceneInit()
{
	if(m_bInit)
		return;

	m_bInit = true;
	m_bShowMainPopup = true;
	m_iTabIdx = 0;
	m_iButtonCnt = 0;
	m_iSelectedIdx = 0;

	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_SushiShopUI", L"UI_SushiShop_BG");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_SushiShopUI", L"UI_SushiTab1");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_SushiShopUI", L"UI_SushiTab2");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_SushiShopUI", L"UI_SushiPopup");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_SushiShopUI", L"UI_SushiPopupTab1");
	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_SushiShopUI", L"UI_SushiPopupTab2");

	static_cast<CUI_SushiShop_PopupText*>(m_pUI[UI_SUSHISHOP_POPUPTEXT])->CurPlayerCoin();

	UpdateSushiImg();

	/* Set Scale */
	m_pUI[UI_SUSHISHOP_BG]->Get_Transform()->Set_Scale({ 1.f, 1.f, 1.f });
	m_pUI[UI_SUSHISHOP_POPUPBG]->Get_Transform()->Set_Scale({ 1.f, 1.f, 1.f });

	/* Active */
	m_pUI[UI_SUSHISHOP_BG]->Set_Active(true);

	/* InActive */
	UIMGR->Inactive_UI(L"UI_SushiTab1");
	UIMGR->Inactive_UI(L"UI_SushiTab2");
	m_pUI[UI_SUSHISHOP_ICON]->Set_Active(false);
}

void CUI_SushiShop::SceneOut()
{
	m_bShowMainPopup = false;

	if (NULL != UIMGR->GetScene(L"SCENE_CCTV"))
		static_cast<CUI_CCTV*>(UIMGR->GetScene(L"SCENE_CCTV"))->Set_CCTVContentClose();
}

void CUI_SushiShop::SceneReset()
{
	m_bInit = false;

	UIMGR->Set_ReturnToPool(L"UI_SushiShop_BG", true);
	UIMGR->Set_ReturnToPool(L"UI_SushiTab1", true);
	UIMGR->Set_ReturnToPool(L"UI_SushiTab2", true);
	UIMGR->Set_ReturnToPool(L"UI_SushiPopup", true);
	UIMGR->Set_ReturnToPool(L"UI_SushiPopupTab1", true);
	UIMGR->Set_ReturnToPool(L"UI_SushiPopupTab2", true);

	/* Player Key 제한 풀기 */
	UNLOCK_PLAYER_KEY_IO;
}

void CUI_SushiShop::SushiMenu(_float fTimeDelta)
{
	if (!m_bInit)
		return;

	if(m_bShowMainPopup)
	{
		/* showSushiMenu */
		/* 뒷배경 ScaleUp */
		if (!m_bShowMenuContent)
		{
			_float3 curScale = m_pUI[UI_SUSHISHOP_BG]->Get_Transform()->Get_Scale();
			_float fSetX = JoMath::Lerp(curScale.x, m_vOriginMenuScale.x, 0.4f);
			_float fSetY = JoMath::Lerp(curScale.y, m_vOriginMenuScale.y, 0.4f);

			m_pUI[UI_SUSHISHOP_BG]->Get_Transform()->Set_Scale({ fSetX, fSetY, 1.f });

			if (fSetX == m_vOriginMenuScale.x && fSetY == m_vOriginMenuScale.y)
			{
				m_bShowMenuContent = true;

				/* Show MenuUIs */
				UIMGR->Active_UI(L"UI_SushiTab1");
				m_pUI[UI_SUSHISHOP_ICON]->Set_Active(true);
			}

			return;
		}

		/* MenuUIs FadeIn */
		if(0 < m_fDelayTime)
		{
			m_fDelayTime -= fTimeDelta * 5.f;
			for (auto UIs : UIMGR->GetUIVec(L"UI_SushiTab1"))
				UIs->Set_RawValue("g_fAlpha_Timer", &m_fDelayTime, sizeof(_float), true);
			m_pUI[UI_SUSHISHOP_ICON]->Set_RawValue("g_fAlpha_Timer", &m_fDelayTime, sizeof(_float), true);
		}
	}
	else
	{
		/* HideSushiMenu */
		_float3 curScale = m_pUI[UI_SUSHISHOP_BG]->Get_Transform()->Get_Scale();
		if (curScale.x == 1.f && curScale.y == 1.f)
			return;

		/* 글씨 Hide */
		if (m_bShowMenuContent)
		{
			UIMGR->Inactive_UI(L"UI_SushiTab1");
			UIMGR->Inactive_UI(L"UI_SushiTab2");
			m_pUI[UI_SUSHISHOP_ICON]->Set_Active(false);

			/* 글씨 FadeOut */
			if(1 >= m_fDelayTime)
			{
				m_fDelayTime += fTimeDelta * 5.f;
				for (auto UIs : UIMGR->GetUIVec(L"UI_SushiTab1"))
					UIs->Set_RawValue("g_fAlpha_Timer", &m_fDelayTime, sizeof(_float), true);
				for (auto UIs : UIMGR->GetUIVec(L"UI_SushiTab2"))
					UIs->Set_RawValue("g_fAlpha_Timer", &m_fDelayTime, sizeof(_float), true);
				m_pUI[UI_SUSHISHOP_ICON]->Set_RawValue("g_fAlpha_Timer", &m_fDelayTime, sizeof(_float), true);
			}
			else
				m_bShowMenuContent = false;

			return;
		}

		/* Popup 창 작아지기 */
		_float fSetX = JoMath::Lerp(curScale.x, 1.f, 0.2f);
		_float fSetY = JoMath::Lerp(curScale.y, 1.f, 0.2f);

		m_pUI[UI_SUSHISHOP_BG]->Get_Transform()->Set_Scale({ fSetX, fSetY, 1.f });


		if (5.f >= curScale.x)
			SceneReset();

	}
}

void CUI_SushiShop::Popup(_float fTimeDelta)
{
	if (m_bBuySushi)
	{
		m_fPopupDelayTime += fTimeDelta;
		if(1.f <= m_fPopupDelayTime)
		{
			m_bPopup = false;
			m_bBuySushi = false;
			m_iButtonCnt = 0;
			m_fPopupDelayTime = 0.f;
		}
	}

	if(m_bPopup)
	{
		if (!m_bShowPopupContent)
		{
			_float3 curScale = m_pUI[UI_SUSHISHOP_POPUPBG]->Get_Transform()->Get_Scale();
			_float fSetX = JoMath::Lerp(curScale.x, m_vOriginPopupScale.x, 0.2f);
			_float fSetY = JoMath::Lerp(curScale.y, m_vOriginPopupScale.y, 0.2f);

			m_pUI[UI_SUSHISHOP_BLACKBGPOP]->Set_Active(true);
			m_pUI[UI_SUSHISHOP_POPUPBG]->Set_Active(true);

			m_pUI[UI_SUSHISHOP_POPUPBG]->Get_Transform()->Set_Scale({ fSetX, fSetY, 1.f });

			if (m_vOriginPopupScale.x - fSetX < 0.5f)
			{
				m_pUI[UI_SUSHISHOP_POPUPBG]->Get_Transform()->Set_Scale({ m_vOriginPopupScale.x, m_vOriginPopupScale.y, 1.f });
				ShowPopupImg();
				UIMGR->Active_UI(L"UI_SushiPopup");

				static_cast<CUI_SushiShop_PopupText*>(m_pUI[UI_SUSHISHOP_POPUPTEXT])->SetInitTF(true);
				static_cast<CUI_SushiShop_PopupText*>(m_pUI[UI_SUSHISHOP_POPUPTEXT])->
					SetPopupInfo(*m_SushiMenus[m_iTabIdx][m_iSelectedIdx], m_iTabIdx);

				m_bShowPopupContent = true;
			}
		}
	}
	else
	{
		_float3 curScale = m_pUI[UI_SUSHISHOP_POPUPBG]->Get_Transform()->Get_Scale();
		if (curScale.x == 1.f && curScale.y == 1.f)
			return;

		/* 글씨 FadeOut */
		if (m_bShowPopupContent)
		{
			HidePopupImg();
			UIMGR->Inactive_UI(L"UI_SushiPopup");
			m_pUI[UI_SUSHISHOP_POPUPBG]->Set_Active(true);
			static_cast<CUI_SushiShop_PopupText*>(m_pUI[UI_SUSHISHOP_POPUPTEXT])->SetInitTF(false);

			m_bShowPopupContent = false;
		}

		/* Popup 창 작아지기 */
		_float fSetX = JoMath::Lerp(curScale.x, 1.f, 0.2f);
		_float fSetY = JoMath::Lerp(curScale.y, 1.f, 0.2f);

		m_pUI[UI_SUSHISHOP_POPUPBG]->Get_Transform()->Set_Scale({ fSetX, fSetY, 1.f });

		if(5.f >= curScale.x)
		{
			m_pUI[UI_SUSHISHOP_POPUPBG]->Set_Active(false);
		}
	}
}

void CUI_SushiShop::KeyEvent(_float fTimeDelta)
{
	if (!m_bInit || !m_bShowMenuContent)
		return;

	// m_iButtonCnt -> enum 으로 고치기
	// 0 : 메뉴화면
	// 1 : 구매팝업
	if (KEY_DOWN(eKeyCode::LButton))
	{
		++m_iButtonCnt;

		if (1 == m_iButtonCnt)
		{
			if (0 == m_iTabIdx && 5 <= m_SushiMenus[0][m_iSelectedIdx]->iAmount
				|| 1 == m_iTabIdx && 1 <= m_SushiMenus[1][m_iSelectedIdx]->iAmount)
			{
				m_iButtonCnt = 0;
			}
			else
				m_bPopup = true;

		}
		else if (2 == m_iButtonCnt && !m_bBuySushi)
		{
			m_bBuySushi = true;

			/* Buy Sushi */
			_int iBuyAmount = static_cast<CUI_SushiShop_PopupText*>(m_pUI[UI_SUSHISHOP_POPUPTEXT])->SetUtopiCoinStat();
			m_SushiMenus[m_iTabIdx][m_iSelectedIdx]->iAmount += iBuyAmount;

			if (0 == m_iTabIdx && 5 <= m_SushiMenus[m_iTabIdx][m_iSelectedIdx]->iAmount)
			{
				_int idx = _int(UI_SUSHISHOP_TAB1_SUSHI1) + m_iSelectedIdx;
				static_cast<CUI_BasicBase*>(m_pUI[idx])->Set_Texture_Index(2);
			}
			if (1 == m_iTabIdx && 1 == iBuyAmount)
			{
				GET_PLAYER_WORLD->Set_EatSushi(true);

				for(int i= UI_SUSHISHOP_TAB2_SUSHI1; i <= UI_SUSHISHOP_TAB2_SUSHI7; ++i)
				{
					_int idx = i - UI_SUSHISHOP_TAB2_SUSHI1;
					static_cast<CUI_BasicBase*>(m_pUI[i])->Set_Texture_Index(1);	// Tab2 : All Dishes Update
					m_SushiMenus[m_iTabIdx][idx]->iAmount += iBuyAmount;
				}
			}

			m_pUI[UI_SUSHISHOP_POPUPLBTN]->Set_Active(false);
			m_pUI[UI_SUSHISHOP_POPUPRBTN]->Set_Active(false);
		}
	}
	if (KEY_DOWN(eKeyCode::RButton))
	{
		if (2 == m_iButtonCnt)
			m_iButtonCnt = 0;
		else
			--m_iButtonCnt;

		if (-1 == m_iButtonCnt)
			SceneOut();

		m_bPopup = false;
	}


	if (m_bPopup)
		return;

	/* Tab 1, 2 */
	if (KEY_DOWN(eKeyCode::One))
	{
		m_iTabIdx = 0;
		m_iSelectedIdx = 0;

		UIMGR->Active_UI(L"UI_SushiTab1");
		UIMGR->Inactive_UI(L"UI_SushiTab2");

		UpdateSushiImg();
	}
	if (KEY_DOWN(eKeyCode::Two))
	{
		m_iTabIdx = 1;
		m_iSelectedIdx = 0;

		UIMGR->Inactive_UI(L"UI_SushiTab1");
		UIMGR->Active_UI(L"UI_SushiTab2");

		UpdateSushiImg();
	}

	/* Sushi Item : WASD */
	if (KEY_DOWN(eKeyCode::D))
	{
		if(0 == m_iTabIdx)
		{
			if (++m_iSelectedIdx >= 5)
				m_iSelectedIdx = 5;

		}
		else if (1 == m_iTabIdx)
		{
			if (++m_iSelectedIdx >= 6)
				m_iSelectedIdx = 6;
		}
		UpdateSushiImg();
	}
	if (KEY_DOWN(eKeyCode::A))
	{
		if (--m_iSelectedIdx <= 0)
			m_iSelectedIdx = 0;

		UpdateSushiImg();
	}
	if (KEY_DOWN(eKeyCode::W))
	{
		if (0 == m_iTabIdx)
		{
			if (m_iSelectedIdx > 3)
				m_iSelectedIdx -= 3;
		}
		else if (1 == m_iTabIdx)
		{
			if (m_iSelectedIdx > 3)
				m_iSelectedIdx -= 4;
		}

		UpdateSushiImg();
	}
	if (KEY_DOWN(eKeyCode::S))
	{
		if (0 == m_iTabIdx)
		{
			m_iSelectedIdx += 2;
			m_iSelectedIdx = m_iSelectedIdx < 4 ? 4 : 5;
		}
		else if (1 == m_iTabIdx)
		{
			if (m_iSelectedIdx < 4)
				m_iSelectedIdx += 4;
			m_iSelectedIdx = m_iSelectedIdx > 6 ? 6 : m_iSelectedIdx;
		}
		UpdateSushiImg();
	}


}

void CUI_SushiShop::ShowPopupImg()
{
	if (0 == m_iTabIdx)
	{
		UIMGR->Active_UI(L"UI_SushiPopupTab1");
		static_cast<CUI_BasicBase*>(m_pUI[UI_SUSHISHOP_POPUP_TAB1IMG])->Set_Texture_Index(m_iSelectedIdx);
	}
	else if (1 == m_iTabIdx)
	{
		UIMGR->Active_UI(L"UI_SushiPopupTab2");
		static_cast<CUI_BasicBase*>(m_pUI[UI_SUSHISHOP_POPUP_TAB2IMG])->Set_Texture_Index(m_iSelectedIdx);
	}
}

void CUI_SushiShop::HidePopupImg()
{
	UIMGR->Inactive_UI(L"UI_SushiPopupTab1");
	UIMGR->Inactive_UI(L"UI_SushiPopupTab2");
}

void CUI_SushiShop::UpdateSushiImg()
{
	if(0 == m_iTabIdx)
	{
		for (int i = UI_SUSHISHOP_TAB1_SUSHI1; i <= UI_SUSHISHOP_TAB1_SUSHI6; ++i)
		{
			_int getAmount = i - (_int)UI_SUSHISHOP_TAB1_SUSHI1;
			if (5 <= m_SushiMenus[m_iTabIdx][getAmount]->iAmount)
			{
				static_cast<CUI_BasicBase*>(m_pUI[i])->Set_Texture_Index(2);
				continue;
			}
			static_cast<CUI_BasicBase*>(m_pUI[i])->Set_Texture_Index(0);
		}

		_int idx = _int(UI_SUSHISHOP_TAB1_SUSHI1) + m_iSelectedIdx;
		if(5 < static_cast<CUI_BasicBase*>(m_pUI[idx])->Get_Texture_Index())
			static_cast<CUI_BasicBase*>(m_pUI[idx])->Set_Texture_Index(1);
	}
	else if (1 == m_iTabIdx)
	{
		for (int i = UI_SUSHISHOP_TAB2_DISH1; i <= UI_SUSHISHOP_TAB2_DISH7; ++i)
			static_cast<CUI_BasicBase*>(m_pUI[i])->Set_Texture_Index(0);
	
		_int idx = _int(UI_SUSHISHOP_TAB2_DISH1) + m_iSelectedIdx;
		static_cast<CUI_BasicBase*>(m_pUI[idx])->Set_Texture_Index(1);
	}
}

HRESULT CUI_SushiShop::Ready_Object()
{
	vector<CUI*> UIvec = {};
	m_pUI.resize(UI_SUSHISHOP_END);

	m_pUI[UI_SUSHISHOP_BG] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_BG")));
	m_pUI[UI_SUSHISHOP_BLACKBG] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_BlackBack")));
	m_pUI[UI_SUSHISHOP_ICON] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Icon")));
	m_pUI[UI_SUSHISHOP_KEYGUIDE] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_KeyGuide")));
	m_pUI[UI_SUSHISHOP_PLAYERSTATUS] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_PlayerStatus")));
	m_pUI[UI_SUSHISHOP_POPUPTEXT] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_SushiShop_PopupText"));

	m_pUI[UI_SUSHISHOP_TAB1] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab1")));
	m_pUI[UI_SUSHISHOP_TAB1_MENU0] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab1Menu0")));
	m_pUI[UI_SUSHISHOP_TAB1_MENU1] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab1Menu1")));
	m_pUI[UI_SUSHISHOP_TAB2_MENU] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab2Menu0")));
	m_pUI[UI_SUSHISHOP_TAB2] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab2")));
	
	m_pUI[UI_SUSHISHOP_TAB1_SUSHI1] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab1Sushi1")));
	m_pUI[UI_SUSHISHOP_TAB1_SUSHI2] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab1Sushi2")));
	m_pUI[UI_SUSHISHOP_TAB1_SUSHI3] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab1Sushi3")));
	m_pUI[UI_SUSHISHOP_TAB1_SUSHI4] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab1Sushi4")));
	m_pUI[UI_SUSHISHOP_TAB1_SUSHI5] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab1Sushi5")));
	m_pUI[UI_SUSHISHOP_TAB1_SUSHI6] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab1Sushi6")));

	m_pUI[UI_SUSHISHOP_TAB2_DISH1] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab2Dish1")));
	m_pUI[UI_SUSHISHOP_TAB2_DISH2] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab2Dish2")));
	m_pUI[UI_SUSHISHOP_TAB2_DISH3] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab2Dish3")));
	m_pUI[UI_SUSHISHOP_TAB2_DISH4] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab2Dish4")));
	m_pUI[UI_SUSHISHOP_TAB2_DISH5] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab2Dish5")));
	m_pUI[UI_SUSHISHOP_TAB2_DISH6] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab2Dish6")));
	m_pUI[UI_SUSHISHOP_TAB2_DISH7] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab2Dish7")));
	m_pUI[UI_SUSHISHOP_TAB2_SUSHI1] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab2Sushi1")));
	m_pUI[UI_SUSHISHOP_TAB2_SUSHI2] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab2Sushi2")));
	m_pUI[UI_SUSHISHOP_TAB2_SUSHI3] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab2Sushi3")));
	m_pUI[UI_SUSHISHOP_TAB2_SUSHI4] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab2Sushi4")));
	m_pUI[UI_SUSHISHOP_TAB2_SUSHI5] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab2Sushi5")));
	m_pUI[UI_SUSHISHOP_TAB2_SUSHI6] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab2Sushi6")));
	m_pUI[UI_SUSHISHOP_TAB2_SUSHI7] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_Tab2Sushi7")));

	m_pUI[UI_SUSHISHOP_BLACKBGPOP] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_BlackBackPop")));
	m_pUI[UI_SUSHISHOP_POPUPBG] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_PopupBG")));
	m_pUI[UI_SUSHISHOP_POPUPICON] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_PopIcon")));
	m_pUI[UI_SUSHISHOP_POPUPLBTN] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_PopLbtn")));
	m_pUI[UI_SUSHISHOP_POPUPRBTN] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_PopRbtn")));
	m_pUI[UI_SUSHISHOP_POPUPLINE] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_PopLine")));

	m_pUI[UI_SUSHISHOP_POPUP_NUMBACK1] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_PopNumBack1")));
	m_pUI[UI_SUSHISHOP_POPUP_NUMBACK2] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_PopNumBack2")));
	m_pUI[UI_SUSHISHOP_POPUP_TAB1IMG] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_PopTab1Img")));
	m_pUI[UI_SUSHISHOP_POPUP_DISH] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_PopTab2Dish")));
	m_pUI[UI_SUSHISHOP_POPUP_TAB2IMG] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_SushiShop_PopTab2Img")));


	for (_int i = (_int)UI_SUSHISHOP_BG; i <= (_int)UI_SUSHISHOP_POPUPTEXT; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_SushiShop_BG", UIvec);
	UIvec.clear();

	for (_int i = (_int)UI_SUSHISHOP_TAB1_MENU0; i < (_int)UI_SUSHISHOP_TAB2_MENU; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_SushiTab1", UIvec);
	UIvec.clear();

	for (_int i = (_int)UI_SUSHISHOP_TAB2_MENU; i < (_int)UI_SUSHISHOP_BLACKBGPOP; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_SushiTab2", UIvec);
	UIvec.clear();

	for (_int i = (_int)UI_SUSHISHOP_BLACKBGPOP; i <= (_int)UI_SUSHISHOP_POPUPLINE; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_SushiPopup", UIvec);
	UIvec.clear();

	for (_int i = (_int)UI_SUSHISHOP_POPUP_NUMBACK1; i <= (_int)UI_SUSHISHOP_POPUP_TAB1IMG; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_SushiPopupTab1", UIvec);
	UIvec.clear();

	for (_int i = (_int)UI_SUSHISHOP_POPUP_DISH; i <= (_int)UI_SUSHISHOP_POPUP_TAB2IMG; ++i)
		UIvec.emplace_back(m_pUI[i]);
	UIMGR->PutUI(GET_CURLEVEL, L"UI_SushiPopupTab2", UIvec);
	UIvec.clear();

	UIMGR->Active_UI(L"UI_SushiTab1");
	UIMGR->Inactive_UI(L"UI_SushiTab2");
	UIMGR->Inactive_UI(L"UI_SushiPopup");
	UIMGR->Inactive_UI(L"UI_SushiPopupTab1");
	UIMGR->Inactive_UI(L"UI_SushiPopupTab2");

	return S_OK;
}

HRESULT CUI_SushiShop::Ready_SushiMenu()
{
	/* tab1 SushiMenu */
	SUSHIMENU* tab1Sushi1 = new SUSHIMENU;
	SUSHIMENU* tab1Sushi2 = new SUSHIMENU;
	SUSHIMENU* tab1Sushi3 = new SUSHIMENU;
	SUSHIMENU* tab1Sushi4 = new SUSHIMENU;
	SUSHIMENU* tab1Sushi5 = new SUSHIMENU;
	SUSHIMENU* tab1Sushi6 = new SUSHIMENU;

	*tab1Sushi1 = {L"참치", L"초밥의 왕. 다랑어의 살코기 부분입니다.", L"전투 중에 먹으면 체력이 회복됩니다.", 50, 0};
	*tab1Sushi2 = {L"갓파", L"역사 깊은 오이 롤 초밥입니다. 아보카도가 아니라 오이입니다.", L"전투 중에 먹으면 전력이 회복됩니다.", 10, 0};
	*tab1Sushi3 = {L"계란", L"약간 단맛이 납니다. 샛노란 노른자로 만들었습니다.", L"전투 중에 먹으면 차지가 증가합니다.", 280, 0};
	*tab1Sushi4 = {L"연어알", L"작고 붉은 보석 같은 연어알이 위에 가득 채워져 있습니다. 김으로 감싼 군함말이입니다.", L"전투 중에 먹으면 공격력이 일시적으로 증가합니다.", 350, 0};
	*tab1Sushi5 = {L"새우", L"당신은 갑각류 알레르기가 있습니다. 새우 초밥을 먹으면 두드러기가 납니다.", L"전투 중에 먹으면 으악", 280, 0};
	*tab1Sushi6 = {L"후토마키", L"말이 형태의 초밥으로, 형형색색의 재료를 김이 역동적으로 감싼 음식입니다.", L"전투 중에 먹으면 슬래시 릴의 발동 확률이 증가합니다.", 210, 0};

	vector<SUSHIMENU*> m_tab1SushiMenus;
	m_tab1SushiMenus.push_back(tab1Sushi1);
	m_tab1SushiMenus.push_back(tab1Sushi2);
	m_tab1SushiMenus.push_back(tab1Sushi3);
	m_tab1SushiMenus.push_back(tab1Sushi4);
	m_tab1SushiMenus.push_back(tab1Sushi5);
	m_tab1SushiMenus.push_back(tab1Sushi6);

	m_SushiMenus.push_back(m_tab1SushiMenus);

	/* tab2 SushiMenu */
	SUSHIMENU* tab2Sushi1 = new SUSHIMENU;
	SUSHIMENU* tab2Sushi2 = new SUSHIMENU;
	SUSHIMENU* tab2Sushi3 = new SUSHIMENU;
	SUSHIMENU* tab2Sushi4 = new SUSHIMENU;
	SUSHIMENU* tab2Sushi5 = new SUSHIMENU;
	SUSHIMENU* tab2Sushi6 = new SUSHIMENU;
	SUSHIMENU* tab2Sushi7 = new SUSHIMENU;

	*tab2Sushi1 = { L"특상 초밥", L"버그자부로 셰프의 걸작이자 일품요리입니다. 오직 최고급 생선만 사용합니다.", L"사망 시 체력 최대 상태로 부활합니다.", 500, 0 };
	*tab2Sushi2 = { L"초밥: \"소나무\"", L"최고급 초밥의 호화스러운 향연이 펼쳐집니다.", L"체력, 공격력, 차지가 매우 크게 증가한 상태로 전투를 시작합니다.", 1820, 0 };
	*tab2Sushi3 = { L"초밥: \"대나무\"", L"돈마이 스시의 인기 세트 메뉴 중 하나입니다. 그만한 값어치를 하는 메뉴입니다. ", L"체력, 공격력, 차지가 크게 증가한 상태로 전투를 시작합니다.", 1540, 0 };
	*tab2Sushi4 = { L"초밥: \"매화나무\"", L"좀 더 합리적인 가격으로 만나볼 수 있는 푸짐한 초밥 모음입니다.", L"체력, 공격력, 차지가 증가한 상태로 전투를 시작합니다.", 1260, 0 };
	*tab2Sushi5 = { L"유부초밥", L"생강을 달콤하고 매콤한 유부로 감싸 간단하지만 깊은 풍미를 지닌 요리입니다.", L"다음 전투에서 데스 스킬 게이지 회복 속도가 증가합니다.", 840, 0 };
	*tab2Sushi6 = { L"치라시 스시", L"해물 치라시 스시는 양과 미적인 부분에서 모두 월등합니다.", L"다음 전투에서 적들이 더 쉽게 기절합니다.", 2940, 0 };
	*tab2Sushi7 = { L"셰프의 선택", L"버그자부로 셰프는 오늘의 메뉴를 만들기 위해 매일 시장에서 신선한 생선을 사 옵니다.", L"어떤 효과가 있을지는 먹어봐야 합니다.", 2450, 0 };

	vector<SUSHIMENU*> m_tab2SushiMenus;

	m_tab2SushiMenus.push_back(tab2Sushi1);
	m_tab2SushiMenus.push_back(tab2Sushi2);
	m_tab2SushiMenus.push_back(tab2Sushi3);
	m_tab2SushiMenus.push_back(tab2Sushi4);
	m_tab2SushiMenus.push_back(tab2Sushi5);
	m_tab2SushiMenus.push_back(tab2Sushi6);
	m_tab2SushiMenus.push_back(tab2Sushi7);

	m_SushiMenus.push_back(m_tab2SushiMenus);

	return S_OK;
}


CUI_SushiShop* CUI_SushiShop::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_SushiShop* pInstance = new CUI_SushiShop(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_SushiShop"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_SushiShop::Clone(void* pArg)
{
	CUI_SushiShop* pInstance = new CUI_SushiShop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_SushiShop"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SushiShop::Free()
{
	__super::Free();

	for (auto pUI : m_pUI)
		Safe_Release(pUI);

	for (vector<SUSHIMENU*> TabVec : m_SushiMenus)
	{
		for (auto SushiItem : TabVec)
			Safe_Delete(SushiItem);
	}
}
