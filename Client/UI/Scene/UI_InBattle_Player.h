#pragma once

#include "Client_Defines.h"
#include "UI_Define.h"

BEGIN(Client)

class CUI_InBattle_Player final : public CUIScene
{
private:
	CUI_InBattle_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_InBattle_Player(const CUI_InBattle_Player& rhs);
	virtual ~CUI_InBattle_Player() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

	void InBattleStart();

	void BatteryControl(_float fTimeDelta);
	void HPControl(_float fTimeDelta);
	void SkillControl(_float fTimeDelta);
	void FinalSlashControl(_float fTimeDelta);
	void ClickButtonControl(_float fTimeDelta);

public:
	virtual void SceneInit();
	virtual void SceneOut();

public:
	void TurnRPGStartYN(_bool _b) { m_bInTurnRPG = _b; }
	void UpdateHitUI(_int iHit);

public:
	void BatteryCharge();
	void BatteryMinus();
	void BattleButtonIn();
	void BattleButtonOut();
	void FinalSlashSkillInit(FINALSLAH_DIR eDir);
	void FinalSlashSkillOut();
	
private:
	void SceneCheck(_float fTimeDelta);
	void UIKeyEvent();
	void LockOnCheck();

private:
	vector<class CUI_InstanceBase*>			m_pInsUI = {};

	class CUI_InstanceBase*		m_pPlayerNonBattery = nullptr;
	class CUI_InstanceBase*		m_pPlayerLockOn = nullptr;
	class CUI_InstanceBase*		m_pPlayerSkillMiniTop = nullptr;
	class CUI_InstanceBase*		m_pPlayerSkillMiniLeft = nullptr;
	class CUI_InstanceBase*		m_pPlayerSkillMiniRight = nullptr;
	class CUI_InstanceBase*		m_pPlayerSkillMiniBottom = nullptr;

	class CUI_InstanceBase*		m_pPlayerSkillTop = nullptr;
	class CUI_InstanceBase*		m_pPlayerSkillLeft = nullptr;
	class CUI_InstanceBase*		m_pPlayerSkillRight = nullptr;
	class CUI_InstanceBase*		m_pPlayerSkillBottom = nullptr;
	class CUI_InstanceBase*		m_pPlayerSkillTopIcon = nullptr;
	class CUI_InstanceBase*		m_pPlayerSkillLeftIcon = nullptr;
	class CUI_InstanceBase*		m_pPlayerSkillRightIcon = nullptr;
	class CUI_InstanceBase*		m_pPlayerSkillBottomIcon = nullptr;

	class CUI_InstanceBase*		m_pBattleStartMouseWhite = nullptr;
	class CUI_InstanceBase*		m_pBattleStartMouseRed = nullptr;

	_float3						m_vHitColor = {0.f, 1.f, 0.f};
	_float						m_fDelayTime = 0.f;
	_int						m_iHit = 0;

	_bool						m_bMouseInit = false;
	_bool						m_bInBattle = false;
	_bool						m_bInTurnRPG = false;		/* Level SonicJuice. 추후 확장성 필요하면 분리하기 */
	_bool						m_bCallUIOff = true;		// SceneOut() 호출한 Bool
	_bool						m_bUIOff = true;			// RealUIOff
	_bool						m_bSkillActive = false;
	_bool						m_bInBattleButtonInit = false;
	_bool						m_bFinalSlashInit = false;


private:
	HRESULT Ready_Objects();

public:
	static CUI_InBattle_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

