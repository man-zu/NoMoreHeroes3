#include "UI_SushiShop_PopupText.h"

#include "UI_Manager.h"
#include "Travis_Manager.h"

CUI_SushiShop_PopupText::CUI_SushiShop_PopupText(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_BasicBase(pDevice, pContext)
{
}

CUI_SushiShop_PopupText::CUI_SushiShop_PopupText(const CUI_SushiShop_PopupText& rhs)
	: CUI_BasicBase(rhs)
{
}

HRESULT CUI_SushiShop_PopupText::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_SushiShop_PopupText::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransform->Set_Position({ 0.f, 0.f, 0.2f });

	return S_OK;
}

void CUI_SushiShop_PopupText::Tick(_float fTimeDelta)
{
	CalculateDishes();
	CalculateUtopiCoin();
}

void CUI_SushiShop_PopupText::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT CUI_SushiShop_PopupText::Render()
{
	_vector DefaultColor = { 0.f, 0.f, 0.f, 1.f };
	_vector RedColor = { 1.f, 0.f, 0.f, 1.f };
	_vector BlueColor = { 0.f, 0.f, 1.f, 1.f };
	_vector PinkColor = { 1.f, 0.f, 1.f, 1.f };
	_vector WhiteColor = { 1.f, 1.f, 1.f, 1.f };
	_vector YellowColor = { 1.f, 1.f, 0.f, 1.f };

	_int iTotPrice = m_iPrice * m_iAmount * (-1);

	m_pContext->GSSetShader(nullptr, nullptr, 0);

	m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(m_iViewUtopiCoin), _float2(1200.f, 163.f), .9f, YellowColor, 0.f, CCustomFont::FontAlignRight);
	m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(m_iWesn), _float2(1200.f, 222.f), .9f, PinkColor, 0.f, CCustomFont::FontAlignRight);

	if (!m_bInit)
		return S_OK;

	m_pGameInstance->Render_Font(L"FontUI", m_strMenuName, _float2(423.f, 128.f), .9f, DefaultColor, 0.f);
	m_pGameInstance->Render_Font(L"FontUIBold", L"돈마이 스시", _float2(640.f, 103.f), 1.f, DefaultColor, 0.f, CCustomFont::FontAlignCenter);

	if( 0 == m_iTabIdx)
	{
		m_pGameInstance->Render_Font(L"FontUI", L"0", _float2(583.f, 345.f), .8f, WhiteColor, 0.f, CCustomFont::FontAlignCenter);
		m_pGameInstance->Render_Font(L"FontUI", to_wstring(m_iAmount), _float2(608.f, 345.f), .8f, WhiteColor, 0.f, CCustomFont::FontAlignCenter);
		m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(m_iPrice), _float2(670.f, 355.f), 1.f, BlueColor, 0.f, CCustomFont::FontAlignLeft);
		m_pGameInstance->Render_Font(L"FontUIBold", L"Uc", _float2(710.f, 355.f), 1.f, DefaultColor, 0.f, CCustomFont::FontAlignLeft);
	}

	if (0 == m_iTabIdx && 5 == m_iBoughtTot
		|| 1 == m_iTabIdx && 1 == m_iBoughtTot)
	{
		m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(iTotPrice) + L" Uc", _float2(780.f, 390.f), 1.f, RedColor, 0.f, CCustomFont::FontAlignRight);
		m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(m_iViewUtopiCoin) + L" Uc", _float2(780.f, 420.f), 1.f, DefaultColor, 0.f, CCustomFont::FontAlignRight);
		m_pGameInstance->Render_Font(L"FontUIBold", L"품 절", _float2(640.f, 480.f), 1.f, RedColor, 0.f, CCustomFont::FontAlignCenter);
	}
	else if (m_iCurUtopiCoin >= abs(iTotPrice))
	{
		m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(iTotPrice) + L" Uc", _float2(780.f, 390.f), 1.f, BlueColor, 0.f, CCustomFont::FontAlignRight);
		m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(m_iViewUtopiCoin) + L" Uc", _float2(780.f, 420.f), 1.f, DefaultColor, 0.f, CCustomFont::FontAlignRight);

		m_pGameInstance->Render_Font(L"FontUIBold", L"구매하시겠습니까?", _float2(640.f, 460.f), 1.f, DefaultColor, 0.f, CCustomFont::FontAlignCenter);
		if (!m_bHideYN)
		{
			m_pGameInstance->Render_Font(L"FontUIBold", L"예", _float2(550.f, 500.f), 1.f, DefaultColor, 0.f);
			m_pGameInstance->Render_Font(L"FontUIBold", L"아니요", _float2(755.f, 500.f), 1.f, DefaultColor, 0.f);
		}
	}
	else
	{
		m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(iTotPrice) + L" Uc", _float2(780.f, 390.f), 1.f, RedColor, 0.f, CCustomFont::FontAlignRight);
		m_pGameInstance->Render_Font(L"FontUIBold", to_wstring(m_iViewUtopiCoin) + L" Uc", _float2(780.f, 420.f), 1.f, DefaultColor, 0.f, CCustomFont::FontAlignRight);
		m_pGameInstance->Render_Font(L"FontUIBold", L"유토피코인이 부족합니다.", _float2(640.f, 480.f), 1.f, DefaultColor, 0.f, CCustomFont::FontAlignCenter);
	}


	return S_OK;
}

void CUI_SushiShop_PopupText::CalculateDishes()
{
	if (!m_bInit || m_bHideYN)
		return;

	if (KEY_DOWN(eKeyCode::W))
	{
		if (5 - m_iBoughtTot == m_iAmount)
			return;

		if(m_iAmount < 5)
			++m_iAmount;

		SetMouseUITF();
	}
	if (KEY_DOWN(eKeyCode::S))
	{
		if (m_iAmount > 1)
			--m_iAmount;

		SetMouseUITF();
	}
}

void CUI_SushiShop_PopupText::CalculateUtopiCoin()
{
	if (m_iViewUtopiCoin != m_iCurUtopiCoin)
	{
		_int iChange = m_iViewUtopiCoin - m_iCurUtopiCoin;
		wstring strUtopiCoin = to_wstring(iChange);
		_int iLen = (_int)strUtopiCoin.size();

		for (int i = 0; i < iLen; ++i)
		{
			_int iDigit = (_int)pow(10, i);
			if (iChange < iDigit)
				continue;

			iChange -= iDigit;
			m_iViewUtopiCoin -= iDigit;
		}
	}
}

void CUI_SushiShop_PopupText::SetMouseUITF()
{
	if (0 == m_iTabIdx && 5 == m_iBoughtTot || 1 == m_iTabIdx && 1 == m_iBoughtTot
		|| m_iCurUtopiCoin < m_iPrice * m_iAmount)
	{
		UIMGR->GetUI(L"UI_SushiPopup", 3)->Set_Active(false);
		UIMGR->GetUI(L"UI_SushiPopup", 4)->Set_Active(false);
	}
	else
	{
		UIMGR->GetUI(L"UI_SushiPopup", 3)->Set_Active(true);
		UIMGR->GetUI(L"UI_SushiPopup", 4)->Set_Active(true);
	}
}

void CUI_SushiShop_PopupText::CurPlayerCoin()
{
	if (!m_bPlayerMoneyCheck)
	{
		m_bPlayerMoneyCheck = true;
		PLAYER_ABILITY CurABility = CTravis_Manager::Get_Instance()->Get_PlayerAbility();
		m_iViewUtopiCoin = CurABility.iUtopiCoin;
		m_iCurUtopiCoin = CurABility.iUtopiCoin;
		m_iWesn = CurABility.iWesn;
	}
}

_int CUI_SushiShop_PopupText::SetUtopiCoinStat()
{
	if (5 < m_iBoughtTot + m_iAmount)
		return 0;

	if (m_iCurUtopiCoin < m_iPrice * m_iAmount)
		return 0;

	CTravis_Manager::Get_Instance()->Decrease_UtopiCoin(m_iPrice * m_iAmount);
	m_iCurUtopiCoin = CTravis_Manager::Get_Instance()->Get_PlayerAbility().iUtopiCoin;
	m_iBoughtTot += m_iAmount;

	/* 예 아니오 Hide */
	m_bHideYN = true;
	UIMGR->GetUI(L"UI_SushiPopup", 3)->Set_Active(false);
	UIMGR->GetUI(L"UI_SushiPopup", 4)->Set_Active(false);

	return m_iAmount;
}

CUI_SushiShop_PopupText* CUI_SushiShop_PopupText::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_SushiShop_PopupText* pInstance = new CUI_SushiShop_PopupText(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_SushiShop_PopupText"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_SushiShop_PopupText::Clone(void* pArg)
{
	CUI_SushiShop_PopupText* pInstance = new CUI_SushiShop_PopupText(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_SushiShop_PopupText"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SushiShop_PopupText::Free()
{
	__super::Free();
}
