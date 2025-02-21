#pragma once

#include "UI_Manager.h"

BEGIN(Client)

class CUI_InBattle_Monster final : public CUIScene
{
private:
private:
	CUI_InBattle_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_InBattle_Monster(const CUI_InBattle_Monster& rhs);
	virtual ~CUI_InBattle_Monster() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

private:
	void MonsterHPControl(_float fTimeDelta);

public:
	virtual void SceneInit();
	virtual void SceneOut();

	void AppearHP(MON_TYPE tType, _int iIdx, CUI_InstanceBase::INSUI_STAT eParam, _int iCur, _int iMax);

	HRESULT InsertMonsterHP(MON_TYPE tType, _int iIdx);

	void	UpdateUIPos(MON_TYPE tType, _int iIdx, _vector vPos);
	void	MonsterHPUIDestroy(MON_TYPE tType, _int iIdx);
	void	MonsterHPUIsync(MON_TYPE tType, _int iIdx, CUI_InstanceBase::INSUI_STAT eParam, _int iCur, _int iMax);

private:
	vector<vector<class CUI_Instance_MonsterHP*>>		m_HPs = {};


private:
	class CUI_Instance_MonsterHP* CheckObj(MON_TYPE tType, _int iIdx);

public:
	static CUI_InBattle_Monster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

