#pragma once

#include "UI_Manager.h"

BEGIN(Client)

class CUI_InBattle_Named final : public CUIScene
{
private:
private:
	CUI_InBattle_Named(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_InBattle_Named(const CUI_InBattle_Named& rhs);
	virtual ~CUI_InBattle_Named() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

public:
	virtual void SceneInit();
	virtual void SceneOut();

	HRESULT InsertNamedHP();
	void	NamedMonsterHPUIsync(CUI_InstanceBase::INSUI_STAT eParam, _int iCur, _int iMax);

private:
	void MonsterHPControl(_float fTimeDelta);

public:
	void TurnRPGStartYN(_bool _b) { m_bInTurnRPG = _b; }

private:
	vector<class CUI_InstanceBase*>				m_pNamedUI = {};

	_bool			m_bInTurnRPG = false;		/* Level SonicJuice. 추후 확장성 필요하면 분리 */


private:
	HRESULT Ready_Objects();

public:
	static CUI_InBattle_Named* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

