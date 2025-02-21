#pragma once

#include "Tool_Defines.h"
#include "Level.h"

BEGIN(Tool)

class CLevel_UiTool final : public CLevel
{
private:
	CLevel_UiTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_UiTool() = default;

public:
	HRESULT Initialize()			override;
	void Tick(_float fTimeDelta)	override;
	HRESULT Render()				override;

public:
	static CLevel_UiTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Free() override;
};

END




