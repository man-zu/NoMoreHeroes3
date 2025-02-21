#include "UI_Instance_MonsterHP.h"

CUI_Instance_MonsterHP::CUI_Instance_MonsterHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_InstanceBase(pDevice, pContext)
{
}

CUI_Instance_MonsterHP::CUI_Instance_MonsterHP(const CUI_Instance_MonsterHP& rhs)
	: CUI_InstanceBase(rhs)
{
}

HRESULT CUI_Instance_MonsterHP::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Instance_MonsterHP::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_iIdx = static_cast<UI_MONSTERHP*>(pArg)->iMonsterIdx;

	return S_OK;
}

void CUI_Instance_MonsterHP::Tick(_float fTimeDelta)
{
	
}

HRESULT CUI_Instance_MonsterHP::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

CUI_Instance_MonsterHP* CUI_Instance_MonsterHP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Instance_MonsterHP* pInstance = new CUI_Instance_MonsterHP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_Instance_MonsterHP"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_Instance_MonsterHP::Clone(void* pArg)
{
	CUI_Instance_MonsterHP* pInstance = new CUI_Instance_MonsterHP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_Instance_MonsterHP"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Instance_MonsterHP::Free()
{
	__super::Free();
}
