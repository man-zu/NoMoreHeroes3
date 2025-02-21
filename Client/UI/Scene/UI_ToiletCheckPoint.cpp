#include "UI_ToiletCheckPoint.h"

#include "UI_Manager.h"
#include "Travis.h"

CUI_ToiletCheckPoint::CUI_ToiletCheckPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_ToiletCheckPoint::CUI_ToiletCheckPoint(const CUI_ToiletCheckPoint& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_ToiletCheckPoint::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_ToiletCheckPoint::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	Ready_Object();

	UIMGR->Insert_GameObject(GET_CURLEVEL, L"Layer_CheckPoint", L"UI_ToiletCheckPoint");
	UIMGR->Inactive_UI(L"UI_ToiletCheckPoint");
	UIMGR->PutScene(GET_CURLEVEL, L"SCENE_ToiletCheckPoint", this);

	return S_OK;
}

void CUI_ToiletCheckPoint::Tick(_float fTimeDelta)
{
	MovePattern(fTimeDelta);
	//MovePlayerCenter();

	//if (KEY_DOWN(eKeyCode::F3))
	//	UIMGR->SceneInit(L"SCENE_ToiletCheckPoint");
	//if (KEY_DOWN(eKeyCode::F4))
	//	UIMGR->SceneOut(L"SCENE_ToiletCheckPoint");
}

void CUI_ToiletCheckPoint::LateTick(_float fTimeDelta)
{

}

HRESULT CUI_ToiletCheckPoint::Render()
{
	return S_OK;
}

void CUI_ToiletCheckPoint::SceneInit()
{
	UIMGR->Active_UI(L"UI_ToiletCheckPoint");
	SetInitPos();

	eMoveStat = UI_MOVE_IN;
}

void CUI_ToiletCheckPoint::SceneOut()
{
	eMoveStat = UI_MOVE_OUT;
}

HRESULT CUI_ToiletCheckPoint::Ready_Object()
{
	/* 사용할 객체 프로토타입 UIMGR->GetDesc(L"PrototypeName") 가져와서 생성. */
	vector<CUI*> UIvec = {};
	m_pUI.resize(UI_TOILET_END);

	m_pUI[UI_TOILET_TISSUE_LEFT] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Toilet_Tissue0")));
	m_pUI[UI_TOILET_TISSUE_RIGHT] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Toilet_Tissue1")));
	m_pUI[UI_TOILET_CHECKPOINT] = static_cast<CUI_BasicBase*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_BasicBase", UIMGR->GetDesc(L"UI_Toilet_CheckPoint")));

	m_pUI[UI_TOILET_CHECKPOINT]->Set_RawValue("g_fAlpha_Timer", &m_fTextAlpha, sizeof(_float), true);

	for (_int i = 0; i < (_uint)UI_TOILET_END; ++i)
		UIvec.emplace_back(m_pUI[i]);

	UIMGR->PutUI(GET_CURLEVEL, L"UI_ToiletCheckPoint", UIvec);
	UIvec.clear();

	return S_OK;
}

void CUI_ToiletCheckPoint::MovePattern(_float fTimeDelta)
{
	/* MoveIn */
	if(eMoveStat == UI_MOVE_IN)
	{
		_vector vCurRightPos = m_pUI[UI_TOILET_TISSUE_RIGHT]->Get_Pos();
		_float vTargetRightX = m_pUI[UI_TOILET_TISSUE_RIGHT]->GetDesc().WorldMat._41;
		vCurRightPos = XMVectorSetX(vCurRightPos, JoMath::Lerp(XMVectorGetX(vCurRightPos), vTargetRightX, 0.2f));
		m_pUI[UI_TOILET_TISSUE_RIGHT]->Get_Transform()->Set_Position(vCurRightPos);

		if (10.f > abs(vTargetRightX - XMVectorGetX(vCurRightPos)))
		{
			_vector vCurLeftPos = m_pUI[UI_TOILET_TISSUE_LEFT]->Get_Pos();
			_float vTargetLeftX = m_pUI[UI_TOILET_TISSUE_LEFT]->GetDesc().WorldMat._41;
			vCurLeftPos = XMVectorSetX(vCurLeftPos, JoMath::Lerp(XMVectorGetX(vCurLeftPos), vTargetLeftX, 0.2f));
			m_pUI[UI_TOILET_TISSUE_LEFT]->Get_Transform()->Set_Position(vCurLeftPos);

			if (10.f > abs(abs(vTargetLeftX) - abs(XMVectorGetX(vCurLeftPos))))
				eMoveStat = UI_MOVE_TEXTIO;
		}
	}

	/* MoveOut */
	if (eMoveStat == UI_MOVE_OUT && 0.f == m_fDelayTime)
	{
		_vector vCurLeftPos = m_pUI[UI_TOILET_TISSUE_LEFT]->Get_Pos();
		vCurLeftPos = XMVectorSetX(vCurLeftPos, JoMath::Lerp(XMVectorGetX(vCurLeftPos), (-1.f * g_iWinSizeX) - 50.f, 0.2f));
		m_pUI[UI_TOILET_TISSUE_LEFT]->Get_Transform()->Set_Position(vCurLeftPos);

		if (10.f > abs(abs(XMVectorGetX(vCurLeftPos)) - (1.f * g_iWinSizeX + 50.f)))
		{
			_vector vCurRightPos = m_pUI[UI_TOILET_TISSUE_RIGHT]->Get_Pos();
			vCurRightPos = XMVectorSetX(vCurRightPos, JoMath::Lerp(XMVectorGetX(vCurRightPos), g_iWinSizeX + 50.f, 0.2f));
			m_pUI[UI_TOILET_TISSUE_RIGHT]->Get_Transform()->Set_Position(vCurRightPos);
		}
	}

	/* CheckPoint Text */
	if (UI_MOVE_TEXTIO == eMoveStat)
	{
		/* In */
		m_fDelayTime += fTimeDelta;
		if (0.f <= m_fTextAlpha && 1.5f >= m_fDelayTime)
			m_fTextAlpha -= fTimeDelta;
	}
	/* Out */
	if (eMoveStat == UI_MOVE_OUT)
	{
		if (1.f >= m_fTextAlpha)
			m_fTextAlpha += fTimeDelta;
		else
			m_fDelayTime = 0.f;
	}
	m_pUI[UI_TOILET_CHECKPOINT]->Set_RawValue("g_fAlpha_Timer", &m_fTextAlpha, sizeof(_float), true);

}

void CUI_ToiletCheckPoint::MovePlayerCenter()
{
	_float fCurPlayerPosY = GET_PLAYER->Get_Center().m128_f32[1];
	_float fMoveY = m_fBeforePlayerPosY - fCurPlayerPosY;

	if (0.f == fMoveY)
		return;

	m_fBeforePlayerPosY = fCurPlayerPosY;

	vector<class CUI*> UIvec = UIMGR->GetUIVec(L"UI_ToiletCheckPoint");
	for (auto item : UIvec)
	{
		_vector curPosR = item->Get_Pos();
		curPosR = XMVectorSetY(curPosR, XMVectorGetY(curPosR) + fMoveY);
		item->Get_Transform()->Set_Position(curPosR);
	}
}

void CUI_ToiletCheckPoint::SetInitPos()
{
	_vector InitPosL = m_pUI[UI_TOILET_TISSUE_LEFT]->GetInitPos();
	m_pUI[UI_TOILET_TISSUE_LEFT]->Set_Pos({ InitPosL.m128_f32[0] + -1.f * g_iWinSizeX, InitPosL.m128_f32[1], InitPosL.m128_f32[2], InitPosL.m128_f32[3]});

	_vector InitPosR = m_pUI[UI_TOILET_TISSUE_RIGHT]->GetInitPos();
	m_pUI[UI_TOILET_TISSUE_RIGHT]->Set_Pos({ InitPosR.m128_f32[0] + g_iWinSizeX, InitPosR.m128_f32[1], InitPosR.m128_f32[2], InitPosR.m128_f32[3]});
}

CUI_ToiletCheckPoint* CUI_ToiletCheckPoint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ToiletCheckPoint* pInstance = new CUI_ToiletCheckPoint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_ToiletCheckPoint"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_ToiletCheckPoint::Clone(void* pArg)
{
	CUI_ToiletCheckPoint* pInstance = new CUI_ToiletCheckPoint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_ToiletCheckPoint"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ToiletCheckPoint::Free()
{
	__super::Free();

	for (auto pUI : m_pUI)
		Safe_Release(pUI);
}
