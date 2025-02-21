#pragma once

#include "UI_Manager.h"

BEGIN(Client)

class CUI_MiniGame_Toilet final : public CUIScene
{
private:
	CUI_MiniGame_Toilet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_MiniGame_Toilet(const CUI_MiniGame_Toilet& rhs);
	virtual ~CUI_MiniGame_Toilet() = default;

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
	void SetCurStateDir(_int iCurStatePos) { m_iCurStateDir = iCurStatePos;	}
	void SetInitFalse() { m_bInit = false; }
	void SetFindDdong(_bool bFindYN);
	void SetMoveMouse(_bool bFindYN);
	void HideGameUI();

private:
	void StartIn(_float fTimeDelta);
	void MoveIn(_float fTimeDelta);
	void ShowKeep(_float fTimeDelta);
	void DirCheck();

private:
	HRESULT Ready_Object();

private:
	_float3		m_GreenColor = {0.f, 1.f, 0.f};
	_float		m_MovePos = 0.f;
	_float		m_fDelayTime = 6.f;
	_float		m_fKeepDelayTime = 0.f;

	_bool		m_bStart = false;
	_bool		m_bMoveIn = false;

	/* Find Ddong */
	//_bool		m_bKeep = false;
	_bool		m_bFindDdong = false;
	_int		m_iCurStateDir = 0;
	_int		m_iMouseMoveDir = -1;

public:
	static CUI_MiniGame_Toilet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

