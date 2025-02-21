#include "UI_PowerUp_Icon.h"

CUI_PowerUp_Icon::CUI_PowerUp_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_BasicBase(pDevice, pContext)
{
}

CUI_PowerUp_Icon::CUI_PowerUp_Icon(const CUI_PowerUp_Icon& rhs)
	: CUI_BasicBase(rhs)
{
}

HRESULT CUI_PowerUp_Icon::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CUI_PowerUp_Icon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI_PowerUp_Icon::Tick(_float fTimeDelta)
{
	if (m_bIconActive)
	{
		m_fDelayTime += fTimeDelta * 5;
		m_iTexIndex = (_int)m_fDelayTime % 4 + 1;
	}
}

CUI_PowerUp_Icon* CUI_PowerUp_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_PowerUp_Icon* pInstance = new CUI_PowerUp_Icon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_PowerUp_Icon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_PowerUp_Icon::Clone(void* pArg)
{
	CUI_PowerUp_Icon* pInstance = new CUI_PowerUp_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_PowerUp_Icon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PowerUp_Icon::Free()
{
	__super::Free();

	Safe_Release(m_pVIBuffer_Point);
}
