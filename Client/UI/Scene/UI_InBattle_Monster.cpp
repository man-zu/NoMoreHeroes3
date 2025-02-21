#include "UI_InBattle_Monster.h"

#include "UI_Instance_MonsterHP.h"

CUI_InBattle_Monster::CUI_InBattle_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIScene(pDevice, pContext)
{
}

CUI_InBattle_Monster::CUI_InBattle_Monster(const CUI_InBattle_Monster& rhs)
	: CUIScene(rhs)
{
}

HRESULT CUI_InBattle_Monster::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CUI_InBattle_Monster::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	UIMGR->PutScene(GET_CURLEVEL, L"SCENE_MonsterHP", this);

	return S_OK;
}

void CUI_InBattle_Monster::Tick(_float fTimeDelta)
{
	fTimeDelta /= m_pGameInstance->Get_TimeScale();

	MonsterHPControl(fTimeDelta);
}

void CUI_InBattle_Monster::LateTick(_float fTimeDelta)
{

}
 
HRESULT CUI_InBattle_Monster::Render()
{
	return S_OK;
}

void CUI_InBattle_Monster::MonsterHPControl(_float fTimeDelta)
{
	for(vector<class CUI_Instance_MonsterHP*> mons : m_HPs)
	{
		for (auto HP : mons)
		{
			//HP->Beat(fTimeDelta, .5f);
			HP->ColorCheck(fTimeDelta);
			HP->Spread(fTimeDelta, .8f, 8.f);
		}
	}
}

void CUI_InBattle_Monster::SceneInit()
{
}

void CUI_InBattle_Monster::SceneOut()
{
}

void CUI_InBattle_Monster::AppearHP(MON_TYPE tType, _int iIdx, CUI_InstanceBase::INSUI_STAT eParam, _int iCur, _int iMax)
{
	if (m_bInit)
		return;

	m_bInit = true;
	CUI_Instance_MonsterHP* pTarget = CheckObj(tType, iIdx);
	pTarget->SetColorStat(eParam, iCur, iMax);
	pTarget->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADIN, 2.f);
}

HRESULT CUI_InBattle_Monster::InsertMonsterHP(MON_TYPE tType, _int iIdx)
{
	m_HPs.resize(MON_END);

	CUI_Instance_MonsterHP::UI_MONSTERHP tMonDesc = {};
	tMonDesc.iMonsterIdx = iIdx;
	tMonDesc.eUIType = UIMGR->GetDesc(L"IN_MonsterHP")->eUIType;
	tMonDesc.iPass = UIMGR->GetDesc(L"IN_MonsterHP")->iPass;
	tMonDesc.WorldMat = UIMGR->GetDesc(L"IN_MonsterHP")->WorldMat;
	lstrcpy(tMonDesc.szImgName, UIMGR->GetDesc(L"IN_MonsterHP")->szImgName);
	lstrcpy(tMonDesc.szLayerName, UIMGR->GetDesc(L"IN_MonsterHP")->szLayerName);
	lstrcpy(tMonDesc.szPrototpyeName, UIMGR->GetDesc(L"IN_MonsterHP")->szPrototpyeName);

	CUI_Instance_MonsterHP* obj = static_cast<CUI_Instance_MonsterHP*>(m_pGameInstance->Clone_GameObject(L"Prototype_UI_Instance_MonsterHP", &tMonDesc));
	m_HPs[tType].emplace_back(obj);

	m_pGameInstance->Insert_GameObject(GET_CURLEVEL, L"Layer_InBattle_Monster", obj);
	Safe_AddRef(obj);

	return S_OK;
}

void CUI_InBattle_Monster::UpdateUIPos(MON_TYPE tType, _int iIdx, _vector vPos)
{
	if (this == NULL)
		return;

	for (int i = 0; i < m_HPs[tType].size(); ++i)
		if (m_HPs[tType][i]->GetIdx() == iIdx)
			m_HPs[tType][i]->SetViewPos(vPos);
}

void CUI_InBattle_Monster::MonsterHPUIDestroy(MON_TYPE tType, _int iIdx)
{

	for (int i = 0; i < m_HPs[tType].size(); ++i)
	{
		if (m_HPs[tType][i]->GetIdx() == iIdx)
		{
			m_HPs[tType][i]->Set_ReturnToPool(true);

			Safe_Release(m_HPs[tType][i]);
			m_HPs[tType].erase(m_HPs[tType].begin() + i);
		}
	}
}

void CUI_InBattle_Monster::MonsterHPUIsync(MON_TYPE tType, _int iIdx, CUI_InstanceBase::INSUI_STAT eParam, _int iCur, _int iMax)
{
	if (0 >= iCur)
		iCur = 0;
	
	m_bInit = false;

	CUI_Instance_MonsterHP* pTarget = CheckObj(tType, iIdx);
	pTarget->SetColorStat(eParam, iCur, iMax); 

	if(0 == iCur)
	{
		pTarget->SetInsUIStat(CUI_InstanceBase::INSUI_INOUT_SPREADOUT, 2.f);
	}
}

CUI_Instance_MonsterHP* CUI_InBattle_Monster::CheckObj(MON_TYPE tType, _int iIdx)
{
	CUI_Instance_MonsterHP* pTarget = nullptr;

	for (auto obj : m_HPs[tType])
		if (obj->GetIdx() == iIdx)
			pTarget = obj;

	return pTarget;
}

CUI_InBattle_Monster* CUI_InBattle_Monster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_InBattle_Monster* pInstance = new CUI_InBattle_Monster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_InBattle_Monster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_InBattle_Monster::Clone(void* pArg)
{
	CUI_InBattle_Monster* pInstance = new CUI_InBattle_Monster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_InBattle_Monster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InBattle_Monster::Free()
{
	__super::Free();
	
	for(vector<class CUI_Instance_MonsterHP*> mons : m_HPs)
	{
		for (auto pUI : mons)
			Safe_Release(pUI);
	}
}
