#pragma once


#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_UI_Rect_Instance;
END

BEGIN(Tool)
class CToolUIObj final : public CGameObject
{
public:
	enum UITYPE { UI_BASIC, UI_INSTANCE, UI_TYPE_END };
	enum UIALIGN { UI_LT, UI_RT, UI_ALIGN_END };

public:
	typedef struct tUIDesc
	{
		UITYPE		eUIType = { UI_TYPE_END };
		UIALIGN		eUIAlignType = { UI_LT };

		_tchar		szLayer[MAX_PATH] = L"";
		_tchar		szProCom[MAX_PATH] = L"";
		_tchar		szTexture[MAX_PATH] = L"";

		_float4x4	WorldMatrix = _float4x4{};

		_int		iPass = {};


	}UI_DESC;

	typedef struct tInstanceUIDesc : public UI_DESC
	{
		_int		iRow = { 0 };
		_int		iCol = { 0 };
		_uint		iAlpha = { 0 };				// TOOL 에서 보여주기 위한 용도. CLIENT 파싱X
		_uint*		pAlpha = { nullptr };
		_float		fScale = { 0.f };
		_float		fMargin = { 0.f };
		_float		fSpeed = { 0.f };
		_float4		vColor = {};
	}UI_INSTANCE_DESC;

private:
	CToolUIObj(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CToolUIObj(const CToolUIObj& rhs);
	virtual ~CToolUIObj() = default;

public:
	HRESULT Initialize_Prototype()		override;
	HRESULT Initialize(void* pArg)		override;
	void Tick(_float fTimeDelta)		override;
	void LateTick(_float fTimeDelta)	override;
	HRESULT Render()					override;

	UI_DESC Get_Desc() { return m_tUIDesc; }
	void Set_Pass(_int PassNum) { m_tUIDesc.iPass = PassNum; }
	void Set_Scale(_float3 vScale) {
		m_pTransform->Set_Scale(vScale);
	}
	void Set_Pos(_float3 vPos) {
		m_pTransform->Set_State(CTransform::STATE_POSITION, { vPos.x, vPos.y, vPos.z, 1.f });
	}

private:
	CShader*						m_pShader = { nullptr };
	CTexture*						m_pUITexture = { nullptr };
	CVIBuffer*						m_pVIBuffer = { nullptr };
	CVIBuffer_UI_Rect_Instance*		m_pVIBufferInstance = { nullptr };

	_float4x4		m_ViewMatrix = {};
	_float4x4		m_ProjMatrix = {};

	UI_DESC			m_tUIDesc = {};

	_bool			m_bAlpha = true;
	_bool			m_bColorUse = false;
	_float4			m_vColor = {0.f, 0.f, 0.f, 0.f};

private:
	HRESULT Ready_Components();

public:
	static CToolUIObj* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg)	override;
	void Free() override;
};

END

