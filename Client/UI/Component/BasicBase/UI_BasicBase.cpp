#include "UI_BasicBase.h"

CUI_BasicBase::CUI_BasicBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CUI_BasicBase::CUI_BasicBase(const CUI_BasicBase& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_BasicBase::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CUI_BasicBase::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Ready_Components();

	m_pTransform->Set_WorldMatrix(XMLoadFloat4x4(&m_tUIDesc.WorldMat));
	m_iPass = m_tUIDesc.iPass;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f));


	return S_OK;
}

void CUI_BasicBase::Tick(_float fTimeDelta)
{
}

HRESULT CUI_BasicBase::Render()
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

HRESULT CUI_BasicBase::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Transform"), (CComponent**)&m_pTransform)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_VIBuffer_Point"), TEXT("VIBuffer"), (CComponent**)&m_pVIBuffer_Point)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_UI"), TEXT("Shader"), (CComponent**)&m_pShader)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, m_tUIDesc.szImgName, TEXT("Texture"), (CComponent**)&m_pTexture)))
		return E_FAIL;

	return S_OK;
}

_vector CUI_BasicBase::Convert_3D()
{
	_vector vUI_3D = m_pTransform->Get_Position();
	_matrix ViewMatrix = m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	_matrix ProjMatrix = m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

	XMStoreFloat4x4(&m_ViewMatrix, ViewMatrix);
	XMStoreFloat4x4(&m_ProjMatrix, ProjMatrix);

	return vUI_3D;
}

void CUI_BasicBase::Set_Billboard()
{
	m_pTransform->LookAt(XMLoadFloat4(&m_pGameInstance->Get_CamPosition()));
	m_pTransform->LookTo(m_pTransform->Get_Look() * -1.f);
}

CUI_BasicBase* CUI_BasicBase::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_BasicBase* pInstance = new CUI_BasicBase(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_BasicBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_BasicBase::Clone(void* pArg)
{
	CUI_BasicBase* pInstance = new CUI_BasicBase(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_BasicBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BasicBase::Free()
{
	__super::Free();

	Safe_Release(m_pVIBuffer_Point);
}
