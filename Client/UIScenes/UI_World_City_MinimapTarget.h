#pragma once
#include "UI_BasicBase.h"

BEGIN(Client)

class CUI_World_City_MinimapTarget final : public CUI_BasicBase
{
public:
	typedef struct tMinimapQuestMark : public tUIDesc
	{
		_float3		vInitPos = {};
	}MINI_QUESTMART;

private:
	CUI_World_City_MinimapTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_World_City_MinimapTarget(const CUI_World_City_MinimapTarget& rhs);
	virtual ~CUI_World_City_MinimapTarget() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void	Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

public:
	_float3			GetLastPos() { return m_vLastMovePos; }

public:
	_float4x4		m_MoveMat = {};
	_float3			m_vLastMovePos = {};

public:
	static CUI_World_City_MinimapTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END
