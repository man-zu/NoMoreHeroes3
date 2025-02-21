#include "UI_TurnRPG_AttackOption.h"

#include "UI_Manager.h"
#include "UI_TurnRPG.h"

CUI_TurnRPG_AttackOption::CUI_TurnRPG_AttackOption(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_BasicBase(pDevice, pContext)
{
}

CUI_TurnRPG_AttackOption::CUI_TurnRPG_AttackOption(const CUI_TurnRPG_AttackOption& rhs)
	: CUI_BasicBase(rhs)
{
}

HRESULT CUI_TurnRPG_AttackOption::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_TurnRPG_AttackOption::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	m_pTransform->Set_Position({ 0.f, 0.f, 0.2f });

	return S_OK;
}

void CUI_TurnRPG_AttackOption::Tick(_float fTimeDelta)
{
}

void CUI_TurnRPG_AttackOption::LateTick(_float fTimeDelta)
{
	KeyEvent();

	if(m_bInit)
		m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT CUI_TurnRPG_AttackOption::Render()
{
	_vector DefaultColor = { 1.f, 1.f, 1.f, 1.f };

	m_pContext->GSSetShader(nullptr, nullptr, 0);

	if(1 == m_iSelectIdx)
	{
		m_pGameInstance->Render_Font(L"FontUIBold", L"파이어", _float2(484.f, 507.f), 1.f, DefaultColor, 0.f, CCustomFont::FontAlignLeft);
		m_pGameInstance->Render_Font(L"FontUIBold", L"블리자드", _float2(484.f, 533.5f), 1.f, DefaultColor, 0.f, CCustomFont::FontAlignLeft);
		m_pGameInstance->Render_Font(L"FontUIBold", L"선더", _float2(484.f, 560.f), 1.f, DefaultColor, 0.f, CCustomFont::FontAlignLeft);
	}

	else if(2 == m_iSelectIdx)
		m_pGameInstance->Render_Font(L"FontUIBold", L"????", _float2(484.f, 507.f), 1.f, DefaultColor, 0.f, CCustomFont::FontAlignLeft);

	else if(4 == m_iSelectIdx)
		m_pGameInstance->Render_Font(L"FontUIBold", L"아이템 - 약초", _float2(484.f, 507.f), 1.f, DefaultColor, 0.f, CCustomFont::FontAlignLeft);


	return S_OK;
}

void CUI_TurnRPG_AttackOption::KeyEvent()
{
	if (!m_bInit)
		return;

	//if (KEY_DOWN(eKeyCode::Up))
	//{
	//	if (1 != m_iSelectIdx)
	//		return;
	//
	//	if (0 == m_iAttackIdx)
	//		return;
	//
	//	--m_iAttackIdx;
	//
	//}
	//if (KEY_DOWN(eKeyCode::Down))
	//{
	//	if (1 != m_iSelectIdx)
	//		return;
	//
	//	if (2 == m_iAttackIdx)
	//		return;
	//
	//	++m_iAttackIdx;
	//}

	/* Set Mouse Pos */
	CUI* pUI = UIMGR->GetScene(L"SCENE_TurnRPG")->GetSceneUI((_int)UI_TURNRPG_SELECTED);
	_vector vCurPos = pUI->Get_Pos();
	vCurPos.m128_f32[1] = -160.f + (-27.5f * m_iAttackIdx);
	pUI->Get_Transform()->Set_Position(vCurPos);
}

void CUI_TurnRPG_AttackOption::ClosePopup()
{
	m_iAttackIdx = 0;
	m_iSelectIdx = 0;
	m_bInit = false;
}

CUI_TurnRPG_AttackOption* CUI_TurnRPG_AttackOption::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_TurnRPG_AttackOption* pInstance = new CUI_TurnRPG_AttackOption(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_TurnRPG_AttackOption"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_TurnRPG_AttackOption::Clone(void* pArg)
{
	CUI_TurnRPG_AttackOption* pInstance = new CUI_TurnRPG_AttackOption(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_TurnRPG_AttackOption"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_TurnRPG_AttackOption::Free()
{
	__super::Free();
}
