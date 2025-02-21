#pragma once

#include "Client_Defines.h"
#include "UI_Define.h"


BEGIN(Client)

class CUI_SubTitle final : public CUIScene
{
private:
	CUI_SubTitle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_SubTitle(const CUI_SubTitle& rhs);
	virtual ~CUI_SubTitle() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

private:
	void MovePattern(_float fTimeDelta);

public:
	void ShowSubTitle(wstring wstrName = L"");
	void ShowSubTitle_Timer(wstring wstrName = L"", _float _fTotalTime = 0.f, _float _fInterval = 1.f);
	void HideSubTitle();
	void NextSubTitle();
	void TimerSubTitle(_float _fTimeDelta);

	void Insert_ExitFunc(function<void()> _Func) {
		m_FuncOnExit = _Func;
	}

public:
	virtual void SceneInit();
	virtual void SceneOut();

private:
	HRESULT Ready_Object();

private:
	wstring				m_wstrName = L"";
	vector<wstring>		m_vecContent = {};
	_int				m_iIdx = 0;
	_float				m_fAlpha_timer = 1.f;
	_bool				m_bBackgroundOn = false;

	_float m_fTimeAcc = { 0.f };
	_float m_fTotalTime = { 0.f };
	_float m_fInterval = { 0.f };
	_bool m_bTimerMode = { false };
	function<void()> m_FuncOnExit = {nullptr};

public:
	static CUI_SubTitle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

