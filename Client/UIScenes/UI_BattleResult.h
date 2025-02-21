#pragma once

#include "Client_Defines.h"
#include "UI_Define.h"
#include "CustomFont.h"

BEGIN(Client)

class CUI_BattleResult final : public CUIScene
{
private:
	CUI_BattleResult(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_BattleResult(const CUI_BattleResult& rhs);
	virtual ~CUI_BattleResult() = default;

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
	void RankIconApear(_float fTimeDelta);

private:
	vector<class CUI*>			m_pUI = {};
	_bool						m_bActiveResult = false;
	_int						m_iActivedIdx = (_int)UI_BATTLE_RESULT_RESULT;
	_float						m_fDelayTime = 0.f;
	_float3						m_vScale = {};

	/* 현재 플레이어한테 데이터 없어서 일단 하드코딩 */
	_int		iDamaged = 0;
	_int		iSkill = 0;
	_int		iThrow = 0;
	_int		iDeathKick = 0;
	_int		iRank = (_int)UI_BATTLE_RANK_SS;
	_int		iCoin = 0;
	_int		iWESN = 0;


public:
	static CUI_BattleResult* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

