#pragma once

#include "UI_SushiShop.h"

BEGIN(Client)

class CUI_SushiShop_PopupText final : public CUI_BasicBase
{
private:
	CUI_SushiShop_PopupText(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_SushiShop_PopupText(const CUI_SushiShop_PopupText& rhs);
	virtual ~CUI_SushiShop_PopupText() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

private:
	void CalculateDishes();
	void CalculateUtopiCoin();

public:
	void SetMouseUITF();
	void CurPlayerCoin();
	_int SetUtopiCoinStat();
	void SetInitTF(_bool _b) { m_bInit = _b; }
	void SetPopupInfo(CUI_SushiShop::SUSHIMENU& tSelectedSushi, _int iTabIdx)
	{ 
		m_iAmount = 1;
		m_bHideYN = false;
		m_iTabIdx = iTabIdx;
		m_iPrice = tSelectedSushi.iPrice;
		m_iBoughtTot = tSelectedSushi.iAmount;
		m_strMenuName = tSelectedSushi.sMenuName;

		SetMouseUITF();
	}

private:
	_bool		m_bInit = false;
	_bool		m_bPlayerMoneyCheck = false;
	_bool		m_bHideYN = false;
	wstring		m_strMenuName = L"";

	_int		m_iPrice = 0;
	_int		m_iAmount = 1;
	_int		m_iBoughtTot = 1;
	_int		m_iTabIdx = 0;
	_int		m_iViewUtopiCoin = 0;
	_int		m_iCurUtopiCoin = 0;
	_int		m_iWesn = 0;

public:
	static CUI_SushiShop_PopupText* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

