#pragma once
#include "UI_BasicBase.h"

BEGIN(Client)

class CUI_CCTV_SkipImg final : public CUI_BasicBase
{
private:
	CUI_CCTV_SkipImg(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_CCTV_SkipImg(const CUI_CCTV_SkipImg& rhs);
	virtual ~CUI_CCTV_SkipImg() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void	Tick(_float fTimeDelta)		override;
	HRESULT Render()					override;

public:
	static CUI_CCTV_SkipImg* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END
