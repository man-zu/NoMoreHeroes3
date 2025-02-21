#pragma once

#include "Client_Defines.h"
#include "UI_Define.h"

BEGIN(Client)

class CUI_InBattle_Boss final : public CUIScene
{
private:
	CUI_InBattle_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_InBattle_Boss(const CUI_InBattle_Boss& rhs);
	virtual ~CUI_InBattle_Boss() = default;

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
	void SceneCheck();

private:
	vector<class CUI_InstanceBase*> m_pBossUI = {};

	_bool						m_bInBattle = false;
	_bool						m_bUIOff = false;

private:
	HRESULT Ready_Objects();

public:
	static CUI_InBattle_Boss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

