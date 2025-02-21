#pragma once

#include "Client_Defines.h"
#include "UI_Define.h"

BEGIN(Client)

class CUI_ToiletCheckPoint final : public CUIScene
{
private:
	CUI_ToiletCheckPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_ToiletCheckPoint(const CUI_ToiletCheckPoint& rhs);
	virtual ~CUI_ToiletCheckPoint() = default;

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
	void MovePlayerCenter();
	void SetInitPos();

private:
	UI_MOVE_STAT			eMoveStat = { UI_MOVE_END };
	_float					m_fDirR = 0.f;
	_float					m_fDirL = 0.f;
	_float					m_fTextAlpha = 1.f;
	_float					m_fBeforePlayerPosY = 0.f;
	_float					m_fDelayTime = 0.f;


public:
	static CUI_ToiletCheckPoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

