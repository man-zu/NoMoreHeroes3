#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "UI_Define.h"


BEGIN(Client)

class CUIScene abstract : public CGameObject
{
protected:
	CUIScene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIScene(const CUIScene& rhs);
	virtual ~CUIScene() = default;

public:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

protected:
	HRESULT Ready_Components();

public:
	virtual void SceneInit() {};
	virtual void SceneOut() {};
	class CUI_BasicBase* GetSceneUI(_int idx);

protected:
	vector<class CUI_BasicBase*>	m_pUI = {};
	_bool							m_bInit = false;

public:
	void Free() override;
};

END

