#pragma once

#include "Client_Defines.h"
#include "UI_Define.h"

BEGIN(Client)

class CUI_Mario final : public CUIScene
{
private:
	CUI_Mario(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Mario(const CUI_Mario& rhs);
	virtual ~CUI_Mario() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

public:
	virtual void SceneInit();
	virtual void SceneOut();

public:
	void GetCoin(_int iCurCoin) { m_iCurCoin = iCurCoin; };

private:
	void MoveIn();

	void CheckCoinChange(_float fTimeDelta);
	void CoinMotion(_float fTimeDelta);
	void TimerSync(_float fTimeDelta);
	void CoinNumSync();

private:
	HRESULT Ready_Object();

private:
	_float					m_fDelayTime = 0.f;
	_float					m_fLimitTimer = 300.f;
	_float3					m_vYellowColor = {1.f, 1.f, 1.f};

	_int					m_iCurCoin = 0;
	_int					m_iViewCoin = 0;

	_bool					m_bJump = false;
	_bool					m_bJFall = false;
	_bool					m_bMoveIn = false;
	_bool					m_bLimitTimerOn = false;


public:
	static CUI_Mario* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

