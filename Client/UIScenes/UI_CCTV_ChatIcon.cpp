#include "UI_CCTV_ChatIcon.h"

CUI_CCTV_ChatIcon::CUI_CCTV_ChatIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_BasicBase(pDevice, pContext)
{
}

CUI_CCTV_ChatIcon::CUI_CCTV_ChatIcon(const CUI_CCTV_ChatIcon& rhs)
	: CUI_BasicBase(rhs)
{
}

HRESULT CUI_CCTV_ChatIcon::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CUI_CCTV_ChatIcon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI_CCTV_ChatIcon::Tick(_float fTimeDelta)
{
}

HRESULT CUI_CCTV_ChatIcon::Render()
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", m_pTransform->Get_WorldMatrixPtr())))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (m_bBind_RawValue)
	{
		if (FAILED(m_pShader->Bind_RawValue(m_pConstantName, m_pData, m_iSize)))
			return E_FAIL;
	}

	if (FAILED(m_pTexture->Set_SRV(m_pShader, "g_CutTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTexture->Set_SRV(m_pShader, "g_DiffuseTexture", m_iTexIndex)))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(m_iPass)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer_Point->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBuffer_Point->Render()))
		return E_FAIL;

	return S_OK;
}

CUI_CCTV_ChatIcon* CUI_CCTV_ChatIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_CCTV_ChatIcon* pInstance = new CUI_CCTV_ChatIcon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_CCTV_ChatIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_CCTV_ChatIcon::Clone(void* pArg)
{
	CUI_CCTV_ChatIcon* pInstance = new CUI_CCTV_ChatIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_CCTV_ChatIcon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_CCTV_ChatIcon::Free()
{
	__super::Free();

	Safe_Release(m_pVIBuffer_Point);
}
