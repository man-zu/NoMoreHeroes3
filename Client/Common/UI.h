#pragma once

#include "Client_Defines.h"
#include "GameObject.h"



BEGIN(Client)

class CUI abstract : public CGameObject
{
public:
	enum UITYPE { UI_BASIC, UI_INSTANCE, UI_END };
	
public:
	typedef struct tUIDesc
	{
		UITYPE		eUIType = { UI_END };
		_tchar		szPrototpyeName[MAX_PATH] = L"";
		_tchar		szLayerName[MAX_PATH] = L"";
		_tchar		szImgName[MAX_PATH] = L"";
		_float4x4	WorldMat = {};
		_int		iPass = {};
	}UI_DESC;

protected:
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI(const CUI& rhs);
	virtual ~CUI() = default;

public:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

public:
	_vector Convert_WorldToScreen(_fvector vUIWorldPos);
	_vector Convert_ScreenToRenderPos(_fvector vUIScreenPos);
	_float  Convert_ScreenToRenderPosY(_float fY);

public:
	_vector		Get_Pos();
	_float3		Get_Scale();
	_float4x4	Get_ViewMatrix() { return m_ViewMatrix; }
	_float4x4	Get_ProjMatrix() { return m_ProjMatrix; }
	_bool		Get_Run_UI() { return m_bStart; }
	UI_DESC		GetDesc() { return m_tUIDesc; }
	_vector		GetInitPos() { return { m_tUIDesc.WorldMat._41, m_tUIDesc.WorldMat._42, m_tUIDesc.WorldMat._43, 1.f}; }
	_float		GetInitPosX() { return m_tUIDesc.WorldMat._41; }
	_float		GetInitPosY() { return m_tUIDesc.WorldMat._42; }
	_float		GetInitPosZ() { return m_tUIDesc.WorldMat._43; }

	void SetViewPos(_vector vTargetPos);		/* 현재 3D 좌표공간에 띄울때 사용중 */
	void Set_Pos(const _float4& vPosition);

	void Set_ViewMatrix(_float4x4 View) { m_ViewMatrix = View; }
	void Set_ProjMatrix(_float4x4 Proj) { m_ProjMatrix = Proj; }

	void Change_InitPos(const _float3& vPos) { m_tUIDesc.WorldMat._41 = vPos.x; m_tUIDesc.WorldMat._42 = vPos.y; m_tUIDesc.WorldMat._43 = vPos.z; };

	void Set_Scale(const _float3& vScale);
	void Set_Run_UI(_bool isOn) { m_bStart = isOn; }
	void Turn(const _fvector& _vAxis, const _float& _fTimeDelta);

	// 일반 UI - Normal / 3D좌표 UI - view
	void Set_Normal_State(const _float3& _vScale, const _float3& _vPos, const _float& _vFar, const _float2& _vWSize);
	void Set_View_State(const _float3& _vScale, const _float3& _vPos, const _float& _vFar, const _float2& _vWSize);

	void Set_BindRawValueYN(_bool b) { m_bBind_RawValue = b; };
	void Set_RawValue(const _char* pConstantName, const void* pData, _uint iSize, _bool b) {
		m_pConstantName = pConstantName;
		m_pData = pData;
		m_iSize = iSize;
		m_bBind_RawValue = b;
	};

	// 3D UI 전용 함수 (충돌 박스, 뷰포트 전환)
	void Set_ColBox(HWND hWnd, const _float3& _vScale, _float2& _vWSize);
	void Set_ViewPort(const _float3& _vPlacePos, _float3& _vPlaceDir, const _float& fPlaceDistance, _float2& _vWSize);

	/* INSTANCE UI */


protected:
	_float4x4					m_ViewportMatrix = {};

protected:
	CShader*					m_pShader = { nullptr };
	CTexture*					m_pTexture = {nullptr};

	UI_DESC						m_tUIDesc = { };

	_float4x4					m_ViewMatrix = {};
	_float4x4					m_ProjMatrix = {};

	_bool						m_bCol = { false };		// 충돌박스
	_bool						m_bStart = { false };	// 이후 각 UI 효과 발동

	// 셰이더 입력값
	_bool						m_bBind_RawValue = false;
	const _char*				m_pConstantName = {};
	const void*					m_pData = {};
	_uint						m_iSize = {};

public:
	void Free() override;
};

END

