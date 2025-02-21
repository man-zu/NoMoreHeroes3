#pragma once

#include "UI_TurnRPG.h"


BEGIN(Client)

class CUI_TurnRPG_AttackOption final : public CUI_BasicBase
{
private:
	CUI_TurnRPG_AttackOption(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_TurnRPG_AttackOption(const CUI_TurnRPG_AttackOption& rhs);
	virtual ~CUI_TurnRPG_AttackOption() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

private:
	void KeyEvent();

private:
	_int			m_iAttackIdx = 0;
	_int			m_iSelectIdx = 0;

	_bool			m_bInit = false;

public:
	void ClosePopup();
	void SetAttackIdx(_int iAttackIdx) { m_iAttackIdx = iAttackIdx; };
	void SetSelectIdx(_int iAttackIdx) { m_bInit = true; m_iSelectIdx = iAttackIdx; };

public:
	static CUI_TurnRPG_AttackOption* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

