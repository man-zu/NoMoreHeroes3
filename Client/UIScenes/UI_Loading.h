#pragma once

#include "Client_Defines.h"
#include "UI_Define.h"

BEGIN(Client)

class CUI_Loading final : public CUIScene
{
private:
	CUI_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Loading(const CUI_Loading& rhs);
	virtual ~CUI_Loading() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

public:
	// Get Set 들..


private:
	HRESULT Ready_Objects();

	void Loading_Star(_float fTimeDelta);

private:
	vector<class CUI*>	m_pUI = {};

private:
	// Common Loading (이후 패스 바꿔서 색 바꾸기)
	_float				m_fStar_Loading_Timer = _float{};
	_int				m_fStar_Texture_Index = _int{};

public:
	static CUI_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

