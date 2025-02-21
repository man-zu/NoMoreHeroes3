#pragma once

#include "Client_Defines.h"
#include "UI_Define.h"

BEGIN(Client)

class CUI_CCTV final : public CUIScene
{
public:
	typedef struct tCCTVchat
	{
		wstring		wstrChat = L"";
	}CCTV_CHAT;

private:
	CUI_CCTV(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_CCTV(const CUI_CCTV& rhs);
	virtual ~CUI_CCTV() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

public:
	void Set_CCTVContentClose() { m_bUserContent = false; }
	void Set_UserType(wstring wstrTriggerName);

public:
	virtual void SceneInit();
	virtual void SceneOut();

private:
	void AddChating(_int iStartIdx);
	void AllChatingActive();
	void ShowContent(_float fTimeDelta);
	void AddChatArea(_float fTimeDelta);
	void SkipCCTVImg(_float fTimeDelta);
	_int LiveViewers();
	_int FindWordCount(wstring wstrContent, wstring Findword);
	wstring MakeIntToString(_int _iNum, _int iDigit);

	void CheckLevel();

private:
	HRESULT Ready_Object();
	HRESULT Ready_PlayerData();

private:
	vector<class CUI_CCTV_Chating*>		m_ChatUIvec = {};
	//CCTV_USER							m_tCCTVuser = {};
	CCTV_USERTYPE	m_eCurUser = { CCTV_USER_END };

	_float			m_fDelayTime = 0.f;
	_float			m_fAlpha_timer = 1.f;

	_int			m_iViewUtopiCoin = 0;
	_int			m_iCurUtopiCoin = 0;
	_int			m_iWesn = 0;
	_int			m_iVeiwer = 0;

	_bool			m_bLiveOn = false;
	_bool			m_bUserContent = false;		/* CCTV 통한 컨텐츠가 띄워졌는지 확인 */
	_bool			m_bAddChat = false;			/* ChatAdd */
	_bool			m_bSkipCCTV = false;


public:
	static CUI_CCTV* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

