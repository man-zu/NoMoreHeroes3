#pragma once

#include "Client_Defines.h"
#include "UI_Define.h"


BEGIN(Client)

class CUI_SushiShop final : public CUIScene
{
private:
	enum SUSHISHOP_STATE {SUSHISHOP_CLOSE, SUSHISHOP_MENU, SUSHISHOP_POPUP, SUSHISHOP_END};

public:
	typedef struct tSushiMenu 
	{
		wstring		sMenuName = L"";
		wstring		sMenuGuide1 = L"";
		wstring		sMenuGuide2 = L"";
		_int		iPrice = 0;
		_int		iAmount = 0;
	}SUSHIMENU;

private:
	CUI_SushiShop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_SushiShop(const CUI_SushiShop& rhs);
	virtual ~CUI_SushiShop() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

public:
	virtual void SceneInit();
	virtual void SceneOut();
	virtual void SceneReset();

private:
	void SushiMenu(_float fTimeDelta);
	void Popup(_float fTimeDelta);
	void ShowPopupImg();
	void HidePopupImg();
	void UpdateSushiImg();

private:
	void KeyEvent(_float fTimeDelta);
	HRESULT Ready_Object();
	HRESULT Ready_SushiMenu();

private:
	vector<class CUI*>			m_pUI = {};
	vector<vector<SUSHIMENU*>>	m_SushiMenus = {};

	_float3					m_vOriginMenuScale = {};
	_float3					m_vOriginPopupScale = {};

	_bool					m_bPopup = false;
	_bool					m_bShowMainPopup = false;
	_bool					m_bShowMenuContent = false;
	_bool					m_bShowPopupContent = false;
	_bool					m_bBuySushi = false;

	_float					m_fDelayTime = 1.f;
	_float					m_fPopupDelayTime = 0.f;

	_int					m_iButtonCnt = 0;
	_int					m_iSelectedIdx = 0;
	_int					m_iTabIdx = 0;

public:
	static CUI_SushiShop* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

