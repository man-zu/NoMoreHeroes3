#pragma once

#include "Client_Defines.h"
#include "UI_Define.h"


BEGIN(Client)

class CUI_MoveButton final : public CUIScene
{
private:
	CUI_MoveButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_MoveButton(const CUI_MoveButton& rhs);
	virtual ~CUI_MoveButton() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

public:
	virtual void SceneInit();
	virtual void SceneOut();

private:
	HRESULT Ready_Object();
	void MovePattern(_float fTimeDelta);
	void SetInitPos();

private:
	vector<class CUI*>			m_pUI = {};

	_bool						m_bMoveInOut = false;
	_float						m_fDirR = 0.f;
	_float						m_fDirL = 0.f;
	UI_MOVE_STAT				eMoveStat = { UI_MOVE_END };

public:
	static CUI_MoveButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

