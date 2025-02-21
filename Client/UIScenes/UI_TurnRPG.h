#pragma once

#include "UI_Manager.h"

BEGIN(Client)

class CUI_TurnRPG final : public CUIScene
{
private:
	CUI_TurnRPG(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_TurnRPG(const CUI_TurnRPG& rhs);
	virtual ~CUI_TurnRPG() = default;

private:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

private:
	void KeyEvent();
	void SelectedMouseControl();

	/* TextArea */
	void ShowTextArea(_float fTimeDelta);
	void ShowTextAttackArea(_float fTimeDelta);
	void FadeOutTextArea(_float fTimeDelta);
	void MovePattern(_float fTimeDelta);

	void SelectMousePos();


public:
	void SetTravisIdx(_int _iParam, _bool _bParam = false);					/* 선택지1 공격 세팅 */
	void SetTravisAttackIdx(_int _iParam = 0);								/* 선택지2 공격 세팅 */
	void SetTravisDoAttack();												/* 플레이어 공격 */
	void SetTextArea(wstring strText) { m_strMainText = strText; }			/* 최상단 대화창 문구 설정 */
	void ReturnPopup();														/* 팝업 닫기 */
	void DeleteTextArea(_int iIdx);		/* TextArea 공격 */

	void ResetPlayerRPGInfo();												/* 선택지1 -> 0번 옵션 화살표모양 되돌리기 */

	void PlayerTurn();
	void MonsterTurn();
	void AttackOptionChoose(_int iAttackIdx = 0);							/* 선택 확정 */

public:
	virtual void SceneInit();
	virtual void SceneOut();

private:
	HRESULT Ready_Object();

private:
	class CUI_TurnRPG_AttackOption*		m_pPopupUI = nullptr;

	_bool		m_bChanged = false;

	/* Travis State */
	//_int		m_iTravisMaxHP = 0;
	//_int		m_iTravisCurHP = 0;
	//_int		m_iTravisMaxEnergy = 0;
	//_int		m_iTravisCurEnergy = 0;


	_float3		vTextAreaScale[4] = {};
	_float		m_fDelayTime = 0.f;

	_int		m_iTravisSelectIdx = 0;
	_int		m_iTravisAttackIdx = 0;

	_bool		m_bShowContent = false;
	_bool		m_bShowPopup = false;
	_bool		m_bTravisAttackChoose = false;

	/* TextArea 베기 모션 */
	_bool		m_bMouseTurn = false;	// 0번 회전여부
	_bool		m_bTextAreaFall = false;
	_bool		m_bColorChangeWhite[2] = {false, false};
	_float		g_fAlphaTimer[2] = {0.f, 0.f};
	_float		m_fTextAreaRotateDir = 1.f;
	_float		m_fTextAreaMoveY = 0.f;
	_int		m_iTextAreaMoveCnt = 0;

	/* Who's Turn */
	_bool		m_bMonsterTurn = false;
	_bool		m_bTravisTurn = false;

	/* GiveMe TurnManger  */
	_bool		m_bDeleteTextArea[2] = {false, false};

	wstring		m_strMainText = L"";

public:
	static CUI_TurnRPG* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg = nullptr)	override;
	void Free()									override;
};

END

