#pragma once

#include "Client_Defines.h"
#include "UI_Define.h"

BEGIN(Client)

class CUI_InBattle_Letter final : public CUIScene
{
private:
	CUI_InBattle_Letter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_InBattle_Letter(const CUI_InBattle_Letter& rhs);
	virtual ~CUI_InBattle_Letter() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

public:
	virtual void SceneInit(INS_LETTER eLetterType);
	virtual void SceneOut(INS_LETTER eLetterType);

private:
	vector<class CUI_InstanceBase*> m_pUI = {};
	wstring		m_sLetter = L"";
	_bool		m_bSpreadOut = false;

private:
	void ReadyLetterKill();
	void ReadyLetterMrBlackHoleDead();
	void ReadyLetterMrBlackHoleDeadQuestion();
	void ReadyLetterSonicJuiceDead();

	HRESULT Ready_Objects();

public:
	static CUI_InBattle_Letter* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

