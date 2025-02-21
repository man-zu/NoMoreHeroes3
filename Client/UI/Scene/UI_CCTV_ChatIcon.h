#pragma once

#include "UI_CCTV.h"

BEGIN(Engine)
class CVIBuffer_Point;

END

BEGIN(Client)

class CUI_CCTV_ChatIcon : public CUI_BasicBase
{
protected:
	CUI_CCTV_ChatIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_CCTV_ChatIcon(const CUI_CCTV_ChatIcon& rhs);
	virtual ~CUI_CCTV_ChatIcon() = default;

protected:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	HRESULT Render()					override;

public:
	static CUI_CCTV_ChatIcon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

