#pragma once
#include "UI_BasicBase.h"

BEGIN(Client)

class CUI_World_City_Minimap final : public CUI_BasicBase
{
private:
	CUI_World_City_Minimap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_World_City_Minimap(const CUI_World_City_Minimap& rhs);
	virtual ~CUI_World_City_Minimap() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void	Tick(_float fTimeDelta)		override;
	HRESULT Render()					override;

private:
	void	SyncMinimapRotate();
	void	SyncMinimapPos();
	void	RotateMap(_fvector vAxis, _float fRadian);

public:
	_float3		GetMovePos() { return m_vMovePos; }
	_float2		GetSavedMovePos() { return m_SavedMovePos; }
	_float		GetRotateAngle() { return m_fRotateAngle; }

private:
	_float4x4	m_PosMatrix = {};
	_float4x4	m_MixWorldMatrix = {};

	/* PLAYER Look, Pos Info */
	_float4		m_vBeforeCamLook = {};
	_float4		m_vCurCamLook = {};
	_float3		m_vBeforeTravisPos = {};
	_float3		m_vCurTravisPos = {};

	/* MAP Pos Info */
	_float3		m_vInitScale = {};
	_float3		m_vMovePos = {};
	_float		m_fRotateAngle = 0.f;
	_float		m_fDelayTime = 0.f;

	UI_VISIBLE		m_bHideYN = UI_VISIBLE_END;

/* static */
private:
	static _float2		m_SavedMovePos;

public:
	static CUI_World_City_Minimap* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END
