#pragma once

#include "Client_Defines.h"
#include "UI_Define.h"


BEGIN(Client)

class CUI_PowerUp final : public CUIScene
{
public:
	enum POWERUP_STATE { POWERUP_MAIN_MOVE, POWERUP_STAT_MOVE, POWERUP_MAIN, POWERUP_STAT};
	enum POWERUP_SKILL { POWERUP_HEALTH, POWERUP_ATTACK, POWERUP_BATTERY, POWERUP_SKILL, POWERUP_SPECIAL};

private:
	CUI_PowerUp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_PowerUp(const CUI_PowerUp& rhs);
	virtual ~CUI_PowerUp() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

private:
	void	KeyEvent();
	void	ScreenMoveCheck();
	void	PlayerStateCheck();
	void	CalculateWESN();
	void	ChooseWesn();
	void	ResetWESN();
	void	LevelUP();
	void	LevelUpMotion(_float fTimeDelta);
	void	UIPosReset();

	_int	GetSelectImgIdx(_int curIdx);
	_int	GetSelectImgLastIdx(_int curIdx);
	_int	GetSelectImgIdxBefore(_int curIdx);
	wstring	ConvertNumWstr(_int iNum);

private:
	HRESULT Ready_Objects();

public:
	virtual void SceneInit();
	virtual void SceneOut();

private:
	POWERUP_STATE	m_state = POWERUP_MAIN;

	_bool			m_bPlayerMoneyCheck = false;
	_bool			m_bLevelUpMotion = false;

	/* PlayerState */
	_int			m_iSkillLevel[5] = {};
	_int			m_iViewWesn = 0;
	_int			m_iCurWesn = 0;

	/* PowerUP */
	_int			m_iSelectIdx = 0;
	_int			m_iPowerUpMax[5] = {10, 10, 6, 3, 6};
	_int			m_iWesnCost[5] = {500, 800, 500, 4000, 3000};	// { 5, 8, 5, 4, 3 };//
	_int			m_iLevelUpMotionCnt = 0;
	_float			m_fDelayTime = 0.f;

	_float			m_fTextMoveAmount = 0.f;

public:
	static CUI_PowerUp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

