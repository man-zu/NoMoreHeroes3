#pragma once

#include "UI_Define.h"
#include "UI_Manager.h"

BEGIN(Client)

class CUI_MainMenu final : public CUIScene
{
private:
	CUI_MainMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_MainMenu(const CUI_MainMenu& rhs);
	virtual ~CUI_MainMenu() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

public:
	virtual void SceneInit();
	virtual void SceneOut();

	INS_MAIN GetCurIdx() { return (INS_MAIN)m_iCurIdx; }

private:
	vector<class CUI_InstanceBase*>		m_pUI = {};
	_int					m_iCurIdx = 0;
	_float4					m_vColor = { 0.f, 1.f, 0.f, 1.f };

private:
	void UIKeyEvent();
	void MovePattern(_float fTimeDelta);
	void MoveParaY(_float fX, _float fY, _float4 vColor);
	HRESULT Ready_Object();

public:
	static CUI_MainMenu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

