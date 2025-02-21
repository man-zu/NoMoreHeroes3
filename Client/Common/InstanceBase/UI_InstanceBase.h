#pragma once

#include "UI.h"

BEGIN(Client)

class CUI_InstanceBase : public CUI
{
public:
	enum INSUI_STAT { INSUI_STAT_IDLE, INSUI_STAT_DEC, INSUI_STAT_INC, INSUI_STAT_SKILL, INSUI_STAT_SKILLICON, INSUI_STAT_END };
	enum INSUI_INOUT { INSUI_INOUT_IDLE, INSUI_INOUT_SPREADIN, INSUI_INOUT_SPREADOUT, INSUI_INOUT_IN, INSUI_INOUT_OUT, INSUI_INOUT_END};

protected:
	CUI_InstanceBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_InstanceBase(const CUI_InstanceBase& rhs);
	virtual ~CUI_InstanceBase() = default;

protected:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	//void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

private:
	HRESULT Ready_Components();

private:
	CVIBuffer_UI_Rect_Instance* m_pVIBufferInstance = { nullptr };

private:
	_float			m_fAlpha = { 0.f };
	_float			m_iInsUIInOutSize = 0.f;
	_float			m_fDelayTime = 0.f;

	INSUI_STAT		m_eInsUIStat = { INSUI_STAT_IDLE };
	INSUI_INOUT		m_eInsUIInOut = { INSUI_INOUT_IDLE };

	_uint			m_iMaxData = 0;
	_uint			m_iCurData = 0;

	_bool			m_bInit = false;

public:
	/* GET SET */
	void SetColorStat(INSUI_STAT eParam, _int iCur, _int iMax);
	INSUI_INOUT GetInsUIStat() { return m_eInsUIInOut; }
	void SetInsUIStat(INSUI_INOUT eParam, _float fSize = 0.f) { 
		m_eInsUIInOut = eParam; 
		m_iInsUIInOutSize = fSize; 
		m_pVIBufferInstance->ResetData();
	}

	/* buffer MovePattern*/
	void Beat(_float fTimeDelta, _float fDuringTime);
	void DirMove(_float fTimeDelta, _float fDuringTime, CVIBuffer_UI_Rect_Instance::INSUI_MOVETYPE eType);
	void Wave(_float fTimeDelta);
	void Spread(_float fTimeDelta, _float fDuringTime, _float fSpeed);
	void Move(_float fTimeDelta);

	/* buffer ColorPattern*/
	void ColorCheck(_float fTimeDelta);


public:
	static CUI_InstanceBase* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

