#include "UI_TurnRPG.h"

#include "UI_TurnRPG_AttackOption.h"
#include "Turn_Manager.h"
#include "UI_InBattle_Player.h"
#include "UI_InBattle_Named.h"

CUI_TurnRPG::CUI_TurnRPG(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_TurnRPG::CUI_TurnRPG(const CUI_TurnRPG& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_TurnRPG::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_TurnRPG::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	m_pTransform->Set_Position({ 0.f, 0.f, 0.7f});

	Ready_Object();

	UIMGR->PutScene(GET_CURLEVEL, L"SCENE_TurnRPG", this);
	static_cast<CUI_InBattle_Player*>(UIMGR->GetScene(L"SCENE_INS_PlayerBattle"))->TurnRPGStartYN(true);
	static_cast<CUI_InBattle_Named*>(UIMGR->GetScene(L"SCENE_NamedHP"))->TurnRPGStartYN(true);

	return S_OK;
}

void CUI_TurnRPG::Tick(_float fTimeDelta)
{
	KeyEvent();
	FadeOutTextArea(fTimeDelta);
	MovePattern(fTimeDelta);
	ShowTextArea(fTimeDelta);
	ShowTextAttackArea(fTimeDelta);
}

void CUI_TurnRPG::LateTick(_float fTimeDelta)
{
	if (m_bShowContent)
		m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT CUI_TurnRPG::Render()
{
	m_pContext->GSSetShader(nullptr, nullptr, 0);

	_vector DefaultColor = { 1.f, 1.f, 1.f, 1.f };
	wstring strTravisHP = to_wstring(CTurn_Manager::Get_Instance()->Get_CurHp()) + L" / " + to_wstring(CTurn_Manager::Get_Instance()->Get_MaxHp());
	wstring strTravisChargeAmt = to_wstring(CTurn_Manager::Get_Instance()->Get_CurEnergy()) + L" / " + to_wstring(CTurn_Manager::Get_Instance()->Get_MaxEnergy());

	/* 대화창 공격 받을 때, 텍스트 이동 */
	_float fRotationState = 0.f;
	_float fRotationAttack = 0.f;
	_float fMoveYState = 0.f;
	_float fMoveYAttack = 0.f;

	if (m_bTravisAttackChoose && 0 == m_iTravisSelectIdx)
	{
		if(1 == m_iTravisAttackIdx)
			fMoveYState = m_fTextAreaMoveY;

		if (2 == m_iTravisAttackIdx)
		{
			fRotationAttack = XMConvertToRadians(m_fDelayTime);
			fMoveYAttack = m_fTextAreaMoveY;
		}
	}
	
	/* 최상단 TextArea */
	m_pGameInstance->Render_Font(L"FontUIBold", m_strMainText, _float2(640.f, 52.f), 1.f, DefaultColor, 0.f, CCustomFont::FontAlignCenter);

	if(0 == g_fAlphaTimer[0])
	{
		m_pGameInstance->Render_Font(L"FontUI", L"이름       HP               충전량", _float2(70.f, 471.f + fMoveYState), .42f, DefaultColor, fRotationState);
		m_pGameInstance->Render_Font(L"FontUI", L"Travis", _float2(70.f, 497.f + fMoveYState), .42f, DefaultColor, fRotationState);
		m_pGameInstance->Render_Font(L"FontUI", strTravisHP, _float2(140.f, 497.f + fMoveYState), .42f, DefaultColor, fRotationState);
		m_pGameInstance->Render_Font(L"FontUI", strTravisChargeAmt, _float2(255.f, 497.f + fMoveYState), .42f, DefaultColor, fRotationState);
	}

	if(0 == g_fAlphaTimer[1])
	{
		m_pGameInstance->Render_Font(L"FontUIBold", L"전투", _float2(460.f, 477.f + fMoveYAttack), 1.f, DefaultColor, fRotationAttack);
		m_pGameInstance->Render_Font(L"FontUIBold", L"마법", _float2(460.f, 503.5f + fMoveYAttack), 1.f, DefaultColor, fRotationAttack);
		m_pGameInstance->Render_Font(L"FontUIBold", L"소환", _float2(460.f, 531.f + fMoveYAttack), 1.f, DefaultColor, fRotationAttack);
		m_pGameInstance->Render_Font(L"FontUIBold", L"충전", _float2(460.f, 558.5f + fMoveYAttack), 1.f, DefaultColor, fRotationAttack);
		m_pGameInstance->Render_Font(L"FontUIBold", L"아이템", _float2(460.f, 586.f + fMoveYAttack), 1.f, DefaultColor, fRotationAttack);
		m_pGameInstance->Render_Font(L"FontUIBold", L"도망치기", _float2(460.f, 613.5f + fMoveYAttack), 1.f, DefaultColor, fRotationAttack);
	}

	return S_OK;
}

void CUI_TurnRPG::KeyEvent()
{
	if (!m_bInit || !m_bTravisTurn || (m_bTravisTurn && m_bTravisAttackChoose))
		return;

	if (m_bChanged)
	{
		/* [ Level2 ] Control */
		if (m_bShowPopup)
		{
			if (1 == m_iTravisSelectIdx || 2 == m_iTravisSelectIdx || 4 == m_iTravisSelectIdx)
			{
				m_pUI[UI_TURNRPG_PLAYERATTACKPOP]->Set_Active(true);
				static_cast<CUI_TurnRPG_AttackOption*>(m_pPopupUI)->SetSelectIdx(m_iTravisSelectIdx);
				m_pUI[UI_TURNRPG_SELECTED]->Get_Transform()->Set_Position({ -200.f, -160.f, 0.f, 1.f });
			}
			else if (3 == m_iTravisSelectIdx || 5 == m_iTravisSelectIdx)
				AttackOptionChoose();
		}

		/* [ Level1 ] 마우스 위치 */
		else
			SelectMousePos();

		m_bChanged = false;
	}
}

void CUI_TurnRPG::SelectedMouseControl()
{
	if (!m_bMouseTurn)
		return;

	if (0 == m_iTravisAttackIdx)
		m_pUI[UI_TURNRPG_SELECTED]->Get_Transform()->Set_Position({ 0.f, 50.f, 0.f, 1.f });

	if (1 == m_iTravisAttackIdx)
		m_pUI[UI_TURNRPG_SELECTED]->Get_Transform()->Set_Position({ -410.f, -65.f, 0.f, 1.f });

	if (2 == m_iTravisAttackIdx)
		m_pUI[UI_TURNRPG_SELECTED]->Get_Transform()->Set_Position({ -100.f, -65.f, 0.f, 1.f });

	if (3 == m_iTravisAttackIdx)
		m_pUI[UI_TURNRPG_SELECTED]->Get_Transform()->Set_Position({ 0.f, 360.f, 0.f, 1.f });
}

void CUI_TurnRPG::MovePattern(_float fTimeDelta)
{
	if (m_bTravisAttackChoose && 0 == m_iTravisSelectIdx)
	{
		if (0 == m_iTravisAttackIdx || 3 == m_iTravisAttackIdx)
			return;

		_vector vInitPos = m_pUI[m_iTravisAttackIdx]->GetInitPos();
		_vector vCurPos = m_pUI[m_iTravisAttackIdx]->Get_Pos();

		/* 2nd && 떨어지는 상태일 때, 원래 각도, 위치로 돌아오기 */
		if (2 == m_iTextAreaMoveCnt && m_bTextAreaFall)
		{
			_float fMoveY = JoMath::Lerp(XMVectorGetY(vCurPos), XMVectorGetY(vInitPos), 0.3f);
			m_pUI[m_iTravisAttackIdx]->Get_Transform()->Set_Position({ XMVectorGetX(vCurPos), fMoveY, XMVectorGetZ(vCurPos), 1.f });

			m_fDelayTime = JoMath::Lerp(m_fDelayTime, 0.f, 0.1f);
			m_pUI[m_iTravisAttackIdx]->Get_Transform()->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(m_fDelayTime));

			if (0.2f > m_fDelayTime && 0.5f > fMoveY)
			{
				m_fDelayTime = 0.f;
				m_iTextAreaMoveCnt = 0;

				m_pUI[m_iTravisAttackIdx]->Get_Transform()->Set_Position(vInitPos);
				m_pUI[m_iTravisAttackIdx]->Get_Transform()->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(0));
				m_bColorChangeWhite[m_iTravisAttackIdx-1] = false;

				MonsterTurn();
			}
			m_fTextAreaMoveY = XMVectorGetY(vInitPos) - XMVectorGetY(vCurPos);
			return;
		}

		/* 수직 상하 */
		if (!m_bTextAreaFall && 0.1f < (XMVectorGetY(vInitPos) + 50.f) - XMVectorGetY(vCurPos))
		{
			_float fMoveY = JoMath::Lerp(XMVectorGetY(vCurPos), XMVectorGetY(vInitPos) + 50.f, 0.3f);
			m_pUI[m_iTravisAttackIdx]->Get_Transform()->Set_Position({XMVectorGetX(vCurPos), fMoveY, XMVectorGetZ(vCurPos), 1.f});
		}
		else
		{
			m_bTextAreaFall = true;
			_float fMoveY = JoMath::Lerp(XMVectorGetY(vCurPos), XMVectorGetY(vInitPos), 0.3f);
			m_pUI[m_iTravisAttackIdx]->Get_Transform()->Set_Position({XMVectorGetX(vCurPos), fMoveY, XMVectorGetZ(vCurPos), 1.f});

			if (0.1f > abs(XMVectorGetY(vInitPos) - XMVectorGetY(vCurPos)))
			{
				m_bTextAreaFall = false;
				m_iTextAreaMoveCnt++;
			}
		}

		/* Render() SpriteFont 이동하기 위한 목적 */
		m_fTextAreaMoveY = XMVectorGetY(vInitPos) - XMVectorGetY(vCurPos);

		/* 회전 */
		if (12 < abs(m_fDelayTime))
		{
			m_fDelayTime = m_fTextAreaRotateDir * 12.f;
			m_fTextAreaRotateDir *= -1;
		}
		m_fDelayTime += fTimeDelta * 150.f * m_fTextAreaRotateDir;

		m_pUI[m_iTravisAttackIdx]->Get_Transform()->Rotation({0.f, 0.f, 1.f}, XMConvertToRadians(m_fDelayTime));

		if (5 < abs(m_fDelayTime))
			m_bColorChangeWhite[m_iTravisAttackIdx - 1] = true;
		else
			m_bColorChangeWhite[m_iTravisAttackIdx-1] = false;
	}
}

void CUI_TurnRPG::ShowTextArea(_float fTimeDelta)
{
	if (!m_bInit || m_bShowContent)
		return;

	_bool bResult = true;
	for(int i= UI_TURNRPG_TOPTEXTAREA; i<= UI_TURNRPG_PLAYERATTACKAREA; ++i)
	{
		_float3 curScale = m_pUI[i]->Get_Transform()->Get_Scale();
		_float fSetX = JoMath::Lerp(curScale.x, vTextAreaScale[i].x, 0.2f);
		_float fSetY = JoMath::Lerp(curScale.y, vTextAreaScale[i].y, 0.2f);

		m_pUI[i]->Get_Transform()->Set_Scale({ fSetX, fSetY, 1.f });

		_vector vecLen = XMVector3Length(XMLoadFloat3(&vTextAreaScale[i]) - XMLoadFloat3(&curScale));
		if (XMVectorGetX(vecLen) >= 3.f)
			bResult = false;
	}

	m_bShowContent = bResult;
	if(m_bShowContent)
		m_pUI[UI_TURNRPG_SELECTED]->Set_Active(true);
}

void CUI_TurnRPG::ShowTextAttackArea(_float fTimeDelta)
{
	if (false == m_pUI[UI_TURNRPG_PLAYERATTACKPOP]->Is_Active())
		return;

	if (m_bShowPopup)
	{
		_bool bResult = true;

		_float3 curScale = m_pUI[UI_TURNRPG_PLAYERATTACKPOP]->Get_Transform()->Get_Scale();
		_float fSetX = JoMath::Lerp(curScale.x, vTextAreaScale[UI_TURNRPG_PLAYERATTACKPOP].x, 0.2f);
		_float fSetY = JoMath::Lerp(curScale.y, vTextAreaScale[UI_TURNRPG_PLAYERATTACKPOP].y, 0.2f);

		m_pUI[UI_TURNRPG_PLAYERATTACKPOP]->Get_Transform()->Set_Scale({ fSetX, fSetY, 1.f });

		_vector vecLen = XMVector3Length(XMLoadFloat3(&vTextAreaScale[UI_TURNRPG_PLAYERATTACKPOP]) - XMLoadFloat3(&curScale));
		if (XMVectorGetX(vecLen) >= 3.f)
			bResult = false;

		/* PopUP Text Show */
		if (bResult)
			m_pPopupUI->Set_Active(true);
	}
	else
	{
		_bool bResult = true;

		_float3 curScale = m_pUI[UI_TURNRPG_PLAYERATTACKPOP]->Get_Transform()->Get_Scale();
		_float fSetX = JoMath::Lerp(curScale.x, 1.f, 0.1f);
		_float fSetY = JoMath::Lerp(curScale.y, 1.f, 0.1f);

		m_pUI[UI_TURNRPG_PLAYERATTACKPOP]->Get_Transform()->Set_Scale({ fSetX, fSetY, 1.f });

		_vector vecLen = XMVector3Length(XMLoadFloat3(&vTextAreaScale[UI_TURNRPG_PLAYERATTACKPOP]) - XMLoadFloat3(&curScale));
		if (XMVectorGetX(vecLen) <= 1.f)
			bResult = false;

		if (bResult)
			m_pUI[UI_TURNRPG_PLAYERATTACKPOP]->Set_Active(false);
	}
}

void CUI_TurnRPG::FadeOutTextArea(_float fTimeDelta)
{
	for (int i = 0; i < 2; ++i)
	{
		if (!m_bDeleteTextArea[i])
			continue;

		if (1 >= g_fAlphaTimer[i])
		{
			g_fAlphaTimer[i] += fTimeDelta;
			static_cast<CUI_BasicBase*>(m_pUI[UI_TURNRPG_PLAYERSATEAREA + i])->Set_RawValue("g_fAlpha_Timer", &_float(g_fAlphaTimer[i]), sizeof(_float), true);
		}
		else
			m_bDeleteTextArea[i] = false;
	}
}

void CUI_TurnRPG::SelectMousePos()
{
	_vector vCurPos = m_pUI[UI_TURNRPG_SELECTED]->GetInitPos();
	vCurPos.m128_f32[1] -= 27.5f * m_iTravisSelectIdx;
	m_pUI[UI_TURNRPG_SELECTED]->Get_Transform()->Set_Position(vCurPos);
}

void CUI_TurnRPG::SetTravisIdx(_int _iParam, _bool _bParam)
{
	m_iTravisSelectIdx = _iParam;
	m_bShowPopup = _bParam;
	m_bChanged = true;
}

void CUI_TurnRPG::SetTravisAttackIdx(_int _iParam)
{
	m_iTravisAttackIdx = _iParam;

	if (m_iTravisSelectIdx == 0)
	{
		m_bMouseTurn = true;
		SelectedMouseControl();
		m_pUI[UI_TURNRPG_SELECTED]->Get_Transform()->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(-90.f));
	}
	else if (m_iTravisSelectIdx == 1)
		static_cast<CUI_TurnRPG_AttackOption*>(m_pUI[UI_TURNRPG_ATTACKTEXT])->SetAttackIdx(m_iTravisAttackIdx);
}

void CUI_TurnRPG::SetTravisDoAttack()
{
	m_bTravisAttackChoose = true;
	m_pUI[UI_TURNRPG_SELECTED]->Set_Active(false);

	if (m_bShowPopup)
	{
		m_bShowPopup = false;
		m_bMouseTurn = false;
		m_pPopupUI->Set_Active(false);
		m_pPopupUI->ClosePopup();
	}
}

void CUI_TurnRPG::ReturnPopup()
{
	m_bShowPopup = false;
	m_bMouseTurn = false;
	m_pPopupUI->Set_Active(false);
	m_pPopupUI->ClosePopup();
	SelectMousePos();
}

void CUI_TurnRPG::DeleteTextArea(_int iIdx)
{
	m_bDeleteTextArea[iIdx] = true;
	static_cast<CUI_BasicBase*>(m_pUI[UI_TURNRPG_PLAYERSATEAREA + iIdx])->Change_ShaderPass(7);
}

void CUI_TurnRPG::ResetPlayerRPGInfo()
{
	m_bMouseTurn = false;
	m_bTravisAttackChoose = false;

	m_iTravisSelectIdx = 0;
	m_iTravisAttackIdx = 0;
	m_strMainText = L"";

	m_pUI[UI_TURNRPG_SELECTED]->Set_Active(true);
	m_pUI[UI_TURNRPG_SELECTED]->Get_Transform()->Set_WorldMatrix(XMLoadFloat4x4(&m_pUI[UI_TURNRPG_SELECTED]->GetDesc().WorldMat));
}

void CUI_TurnRPG::PlayerTurn()
{
	PLAY_SOUND("SE_en_SJ_p1_YubiIcon", false, 0.5f);

	m_bTravisTurn = true;
	m_bMonsterTurn = false;
	m_strMainText = L"";

	ResetPlayerRPGInfo();
}

void CUI_TurnRPG::MonsterTurn()
{
	m_bTravisAttackChoose = false;
	m_bTravisTurn = false;
	m_bMonsterTurn = true;
	m_strMainText = L"";
}

void CUI_TurnRPG::AttackOptionChoose(_int iAttackIdx)
{
	m_iTravisAttackIdx = iAttackIdx;

	m_bMouseTurn = false;
	m_bShowPopup = false;
	//m_bTravisAttackChoose = true;

	m_pPopupUI->Set_Active(false);
	m_pUI[UI_TURNRPG_SELECTED]->Set_Active(false);
}

void CUI_TurnRPG::SceneInit()
{
	m_bInit = true;
	m_bTravisTurn = true;

	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_TurnRPG", L"UI_TurnRPG");

	m_pUI[UI_TURNRPG_TOPTEXTAREA]->Set_Active(true);
	m_pUI[UI_TURNRPG_PLAYERSATEAREA]->Set_Active(true);
	m_pUI[UI_TURNRPG_PLAYERATTACKAREA]->Set_Active(true);
}

void CUI_TurnRPG::SceneOut()
{
	m_bInit = false;
	m_bTravisTurn = false;
	m_bMonsterTurn = false;
	m_bShowContent = false;

	UIMGR->Set_ReturnToPool(L"UI_TurnRPG", true);
	//static_cast<CUI_InBattle_Player*>(UIMGR->GetScene(L"SCENE_INS_PlayerBattle"))->TurnRPGStartYN(false);
	static_cast<CUI_InBattle_Named*>(UIMGR->GetScene(L"SCENE_NamedHP"))->TurnRPGStartYN(false);
	static_cast<CUI_InBattle_Named*>(UIMGR->GetScene(L"SCENE_NamedHP"))->SceneInit();
}

HRESULT CUI_TurnRPG::Ready_Object()
{
	vector<CUI*> UIvec = {};
	m_pUI.resize(UI_TURNRPG_END);

	m_pUI[UI_TURNRPG_TOPTEXTAREA] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TurnRPG_TopTextArea")));
	m_pUI[UI_TURNRPG_PLAYERSATEAREA] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TurnRPG_PlayerStateArea")));
	m_pUI[UI_TURNRPG_PLAYERATTACKAREA] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TurnRPG_PlayerAttackArea")));
	m_pUI[UI_TURNRPG_PLAYERATTACKPOP] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TurnRPG_PlayerAttackArea2")));
	m_pUI[UI_TURNRPG_ATTACKTEXT] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_TurnRPG_AttackOption", UIMGR->GetDesc(L"UI_TurnRPG_Selected")));
	m_pUI[UI_TURNRPG_SELECTED] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_TurnRPG_Selected")));

	m_pPopupUI = static_cast<CUI_TurnRPG_AttackOption*>(m_pUI[UI_TURNRPG_ATTACKTEXT]);
	Safe_AddRef(m_pPopupUI);

	for (_int i = 0; i < (_uint)UI_TURNRPG_END; ++i)
		UIvec.emplace_back(m_pUI[i]);

	UIMGR->PutUI(GET_CURLEVEL, L"UI_TurnRPG", UIvec);
	UIvec.clear();

	UIMGR->Inactive_UI(L"UI_TurnRPG");

	for(int i= UI_TURNRPG_TOPTEXTAREA; i<= UI_TURNRPG_PLAYERATTACKPOP; ++i)
	{
		vTextAreaScale[i] = m_pUI[i]->Get_Transform()->Get_Scale();
		m_pUI[i]->Set_Scale({ 1.f, 1.f, 1.f });
	}

	m_pUI[UI_TURNRPG_PLAYERSATEAREA]->Set_RawValue("m_bColorChangeWhite", &m_bColorChangeWhite[0], sizeof(_bool), true);
	m_pUI[UI_TURNRPG_PLAYERATTACKAREA]->Set_RawValue("m_bColorChangeWhite", &m_bColorChangeWhite[1], sizeof(_bool), true);

	return S_OK;
}

CUI_TurnRPG* CUI_TurnRPG::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_TurnRPG* pInstance = new CUI_TurnRPG(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_TurnRPG"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_TurnRPG::Clone(void* pArg)
{
	CUI_TurnRPG* pInstance = new CUI_TurnRPG(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_TurnRPG"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_TurnRPG::Free()
{
	__super::Free();

	for (auto pUI : m_pUI)
		Safe_Release(pUI);

	Safe_Release(m_pPopupUI);
}
