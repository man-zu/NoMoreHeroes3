#pragma once

#include "UI_InstanceBase.h"

BEGIN(Engine)
class CVIBuffer_UI_Rect_Instance;
END

BEGIN(Client)

class CUI_Instance_MonsterHP final : public CUI_InstanceBase
{
public:
	typedef struct tUIInsMonsterHP : public UI_DESC
	{
		_int	iMonsterIdx = 0;
	}UI_MONSTERHP;

private:
	CUI_Instance_MonsterHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Instance_MonsterHP(const CUI_Instance_MonsterHP& rhs);
	virtual ~CUI_Instance_MonsterHP() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	//void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

private:
	_int	m_iIdx = 0;

public:
	_int GetIdx() { return m_iIdx; }

public:
	static CUI_Instance_MonsterHP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

