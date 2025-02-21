#pragma once

#include "Client_Defines.h"
#include "UI_Define.h"

BEGIN(Client)

class CUI_CCTV_Chating final : public CUIScene
{

private:
	CUI_CCTV_Chating(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_CCTV_Chating(const CUI_CCTV_Chating& rhs);
	virtual ~CUI_CCTV_Chating() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

public:
	_bool MoveUp(_float fY);
	void SetMoveUp(_float fY);
	void SetContentOn();

public:
	virtual void SceneInit();
	virtual void SceneOut();

	_float GetTextAreaY() { return m_fTextareaSize; }
	_float4 GetSavePosition() { return m_vSavePosition; }
	void SetSavePosition(_vector _vPos) { XMStoreFloat4(&m_vSavePosition, _vPos); }

private:
	HRESULT Ready_Object();

private:
	CCTV_USERTYPE	m_eUserType = CCTV_USER_END;
	wstring			m_wstrUser = L"";
	wstring			m_wstrContent = L"";
	_float			m_fTextareaSize = 0;
	_int			m_iIdx = 0;
	_int			m_iEnterCnt = 0;

	_float			g_fAlpha_timer = 1.f;
	_float4			m_vSavePosition = {};

	_bool			m_bChatOn = false;

public:
	static CUI_CCTV_Chating* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

