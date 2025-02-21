#include "UI_CCTV_SkipImg.h"

CUI_CCTV_SkipImg::CUI_CCTV_SkipImg(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI_BasicBase{ pDevice, pContext }
{
}

CUI_CCTV_SkipImg::CUI_CCTV_SkipImg(const CUI_CCTV_SkipImg& rhs)
    : CUI_BasicBase(rhs)
{
}

HRESULT CUI_CCTV_SkipImg::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_CCTV_SkipImg::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

    return S_OK;
}

void CUI_CCTV_SkipImg::Tick(_float fTimeDelta)
{
}

HRESULT CUI_CCTV_SkipImg::Render()
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", m_pTransform->Get_WorldMatrixPtr())))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_SRV(L"Target_CopyFinal", m_pShader, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer_Point->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBuffer_Point->Render()))
		return E_FAIL;

	return S_OK;
}

CUI_CCTV_SkipImg* CUI_CCTV_SkipImg::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_CCTV_SkipImg* pInstance = new CUI_CCTV_SkipImg(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed To Created : CUI_CCTV_SkipImg"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_CCTV_SkipImg::Clone(void* pArg)
{
    CUI_CCTV_SkipImg* pInstance = new CUI_CCTV_SkipImg(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed To Cloned : CUI_CCTV_SkipImg"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_CCTV_SkipImg::Free()
{
    __super::Free();
}
