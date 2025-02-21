#pragma once

#include "UI_PowerUp.h"

BEGIN(Engine)
class CVIBuffer_Point;

END

BEGIN(Client)

class CUI_PowerUp_Icon : public CUI_BasicBase
{
protected:
	CUI_PowerUp_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_PowerUp_Icon(const CUI_PowerUp_Icon& rhs);
	virtual ~CUI_PowerUp_Icon() = default;

protected:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;

public:
	void SetIconActive() {
		m_bIconActive = true;
		m_fDelayTime = 0.f;
		m_iTexIndex = 1;
	}
	void SetIconInActive() {
		m_bIconActive = false;
		m_iTexIndex = 0;
	}

private:
	_bool	m_bIconActive = false;
	_float	m_fDelayTime = 0.f;
	
public:
	static CUI_PowerUp_Icon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

