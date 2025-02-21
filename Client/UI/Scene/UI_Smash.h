#pragma once

#include "UI_Manager.h"

BEGIN(Client)

class CUI_Smash final : public CUIScene
{
private:
	enum SMASH_STATE { SMASH_READY, SMASH_FIGHT, SMASH_INGAME, SMASH_GAME, SMASH_END };
private:
	CUI_Smash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Smash(const CUI_Smash& rhs);
	virtual ~CUI_Smash() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

private:
	void TextMovePattern(_float fTimeDelta);
	void NumTravisAttacked(_float fTimeDelta);
	void NumDamonAttacked(_float fTimeDelta);
	void NumCheck(_float fNum, _int idx, wstring strUIName);
	void SkillControl(_float fTimeDelta);
	void FinalSlashControl(_float fTimeDelta);
	void UIKeyEvent();

public:
	virtual void SceneInit();
	virtual void SceneOut();
	void GameOver();

	void FinalSlashSkillInit(FINALSLAH_DIR eDir);
	void FinalSlashSkillOut();

	void TravisHPChange(_float fTravisHP);
	void DamonHPChange(_float fDamonHP);

private:
	HRESULT Ready_Object();

private:
	vector<class CUI_InstanceBase*>		m_pInsUI = {};

	/* Text Move */
	SMASH_STATE		m_eSmashState = SMASH_END;
	_float3			m_vReadyOriginScale = {};
	_float			m_fDelayTime = 0.f;
	_float			m_fTravisDelayTime = 0.f;
	_float			m_fDemonDelayTime = 0.f;
	_float			m_fAlpha = 1.f;
	_float			m_fSpeed = .3f;
	_int			m_iReadyCnt = 0;
	_int			m_iMoveCnt = 0;
	_bool			m_bUIout = false;

	/* Travis HP */
	_float4			m_iTravisHPColor = {1.f, 1.f, 1.f, 1.f};
	_float			m_iTravisHP = 0.f;
	_float			m_iTravisHPDiv100 = 0.f;		/* 셰이더에 넘겨줄 체력바 */
	_bool			m_bTravisAttacked = false;
	_bool			m_bTravisNumReturnInitPos = false;
	/* Damon HP */
	_float4			m_iDamonHPColor = { 1.f, 1.f, 1.f, 1.f };
	_float			m_iDamonHP = 0.f;
	_bool			m_bDamonAttacked = false;
	_bool			m_bDamonNumReturnInitPos = false;

	_bool			m_bSkillActive = false;
	_bool			m_bFinalSlashInit = false;


public:
	static CUI_Smash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

