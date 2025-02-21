#include "UI_InstanceBase.h"

#include "Travis.h"

CUI_InstanceBase::CUI_InstanceBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
}

CUI_InstanceBase::CUI_InstanceBase(const CUI_InstanceBase& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_InstanceBase::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CUI_InstanceBase::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Ready_Components();

	_float4 vPos = (_float4)m_tUIDesc.WorldMat.m[3];

	m_pTransform->Set_WorldMatrix(XMMatrixIdentity());
	m_pTransform->Set_Position(XMLoadFloat4(&vPos));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH((_float)g_iWinSizeX, (_float)g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_InstanceBase::Tick(_float fTimeDelta)
{
	
}

HRESULT CUI_InstanceBase::Render()
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", m_pTransform->Get_WorldMatrixPtr())))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTexture->Set_SRV(m_pShader, "g_DiffuseTexture", 0)))
		return E_FAIL;

	if (m_bBind_RawValue)
	{
		if (FAILED(m_pShader->Bind_RawValue(m_pConstantName, m_pData, m_iSize)))
			return E_FAIL;
	}

	if (FAILED(m_pShader->Begin(m_tUIDesc.iPass)))
		return E_FAIL;

	if (FAILED(m_pVIBufferInstance->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferInstance->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_InstanceBase::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Transform"), TEXT("Transform"), (CComponent**)&m_pTransform)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, m_tUIDesc.szPrototpyeName, TEXT("VIBuffer"), (CComponent**)&m_pVIBufferInstance)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Shader_UI_Instance"), TEXT("Shader"), (CComponent**)&m_pShader)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Texture_InstanceImg"), TEXT("Texture"), (CComponent**)&m_pTexture)))
		return E_FAIL;

	return S_OK;
}

void CUI_InstanceBase::SetColorStat(INSUI_STAT eParam, _int iCurData, _int iMaxData)
{
	if (this == NULL)
	{
		cout << "[UI] Target Obj is NULL" << endl;
		return;
	}

	if (m_eInsUIStat == INSUI_STAT_DEC)
		if(eParam != INSUI_STAT_INC)
			return;
	
	m_eInsUIStat = eParam;

	if (iCurData > iMaxData)
		iCurData = iMaxData;
	if (0 >= iCurData)
		iCurData = 0;

	m_iMaxData = iMaxData;
	m_iCurData = iCurData;
}

void CUI_InstanceBase::Beat(_float fTimeDelta, _float fDuringTime)
{
	fTimeDelta = min(0.0167f, fTimeDelta);

	if (m_eInsUIInOut != INSUI_INOUT_IDLE)
		return;

	m_pVIBufferInstance->Beat(fTimeDelta, fDuringTime);
}

void CUI_InstanceBase::DirMove(_float fTimeDelta, _float fDuringTime, CVIBuffer_UI_Rect_Instance::INSUI_MOVETYPE eType)
{
	/*
	 * N방향 객체들(Player_MiniSkill)의 움직임을 동일하게 하기 위해서
	 * (다른 함수와 다르게) m_fDelayTime 을 인자로 사용함.
	 */

	fTimeDelta = min(0.0167f, fTimeDelta);

	if (m_eInsUIInOut != INSUI_INOUT_IDLE)
		return;


	m_fDelayTime += fTimeDelta;
	if (m_fDelayTime >= fDuringTime * 2.5f)
	{
		m_fDelayTime = 0.f;
		m_bInit = false;
	}

	if(m_fDelayTime == 0.f)
		m_bInit = true;

	if(m_bInit && m_iCurData >= m_iMaxData)
		m_pVIBufferInstance->DirMove(m_fDelayTime, fDuringTime, eType);
}

void CUI_InstanceBase::Wave(_float fTimeDelta)
{
	// X
	//if (fTimeDelta >= 0.0167f)
	//	fTimeDelta = 0.0167f;

	m_pVIBufferInstance->Wave(fTimeDelta);
}

void CUI_InstanceBase::Spread(_float fTimeDelta, _float fDuringTime, _float fSpeed)
{
	fTimeDelta = min(0.0167f, fTimeDelta);

	if(m_eInsUIInOut == INSUI_INOUT_SPREADIN )
		m_pVIBufferInstance->SpreadIn(fTimeDelta, fDuringTime, m_iInsUIInOutSize, fSpeed, (_uint*)&m_eInsUIInOut);

	if(m_eInsUIInOut == INSUI_INOUT_SPREADOUT)
		m_pVIBufferInstance->SpreadOut(fTimeDelta, m_iInsUIInOutSize, fSpeed, (_uint*)&m_eInsUIInOut);
}

void CUI_InstanceBase::Move(_float fTimeDelta)
{
	// X
	//if (fTimeDelta >= 0.0167f)
	//	fTimeDelta = 0.0167f;

	if(m_eInsUIInOut == INSUI_INOUT_IN)
		m_pVIBufferInstance->MoveIn(fTimeDelta, (_uint*)&m_eInsUIInOut);

	else if(m_eInsUIInOut == INSUI_INOUT_OUT)
		m_pVIBufferInstance->MoveOut(fTimeDelta);
}

void CUI_InstanceBase::ColorCheck(_float fTimeDelta)
{
	fTimeDelta = min(0.0167f, fTimeDelta);

	if(m_eInsUIStat == INSUI_STAT_DEC)
		m_pVIBufferInstance->ColorDecrease(fTimeDelta, m_iMaxData, m_iCurData, (_uint*)&m_eInsUIStat);

	if (m_eInsUIStat == INSUI_STAT_INC)
		m_pVIBufferInstance->ColorIncrease(fTimeDelta, m_iMaxData, m_iCurData, (_uint*)&m_eInsUIStat);
	
	if (m_eInsUIStat == INSUI_STAT_SKILL || m_eInsUIStat == INSUI_STAT_SKILLICON)
		m_pVIBufferInstance->ColorRowSkillState(fTimeDelta, m_iMaxData, m_iCurData, (_uint)m_eInsUIStat);

}

CUI_InstanceBase* CUI_InstanceBase::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_InstanceBase* pInstance = new CUI_InstanceBase(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed To Created : CUI_InstanceBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CUI_InstanceBase::Clone(void* pArg)
{
	CUI_InstanceBase* pInstance = new CUI_InstanceBase(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Cloned : CUI_InstanceBase"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InstanceBase::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferInstance);
}
