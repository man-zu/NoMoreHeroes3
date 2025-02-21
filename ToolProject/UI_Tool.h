#pragma once

#include "Tool_Super.h"
#include "ToolUIObj.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Tool)

class CUI_Tool : public CTool_Super
{
	/*
	 * Basic UI & COMMON : 최윤영
	 * Instance UI & Data Parsing / Save / Load : 박경현
	 */

public:
	using GUIZMODESC = struct tagGuizmoDesc
	{
		ImGuizmo::MODE CurrentGuizmoMode = ImGuizmo::WORLD;
		ImGuizmo::OPERATION CurrentGuizmoOperation = ImGuizmo::TRANSLATE;
		bool  bUseSnap = false;
		bool  boundSizing = false;
		bool  boundSizingSnap = false;
		float snap[3] = { 0.1f, 0.1f, 0.1f };
		float bounds[6] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
		float boundsSnap[3] = { 0.1f, 0.1f, 0.1f };
	};

protected:
	CUI_Tool();
	CUI_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CUI_Tool() = default;

public:
	HRESULT Initialize()			override;
	void Start_Tool()				override;
	void Tick(_float fTimeDelta)	override;
	void LateTick(_float fTimeDelta) override;

private:
	void Main_Window();
	void Tab_Bar();

	/* Data Parsing, Save, Load */
	void OpenFile();
	void SaveFile();
	void LoadFile(char* szPreview);

	/* Tool Layout UI Active or InActive */
	void Active_UI(const wstring& strLayerTag);
	void Inactive_UI(const wstring& strLayerTag);

	HRESULT Ready_TexVec(const _tchar* wszPath);

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

private:
	GUIZMODESC			m_tGuizmoDesc = {};

	/* Instance UI */
	_bool PutInstanceData(_int idx, _bool bModify = false);
	void UpdateInstanceData(const wstring& strLayerTag, _int iSelectedIdx, _float fRectSize, _float fSpeed, _float4 color_hsv, _int iAlign);
	_int SaveInstanceData(_float fRectSize, _float fSpeed, _float4 color_hsv, _int iAlign);
	void RemoveInstanceData();
	void RemoveInstanceAllData();
	void MakeRect(_int i);

	CGameObject* GetSelectedObj(_bool bInstance = false);
		
private:
	vector<CToolUIObj::UI_DESC*>			m_pUIvec = {};
	vector<CToolUIObj::UI_INSTANCE_DESC*>	m_pUIInstanceVec = {};
	_int									m_iUIIndex = {};
	vector<CToolUIObj*>						m_vecObjects = {};

	/* InstanceUI Data */
	_int		m_iRow = 0;
	_int		m_iCol = 0;
	_float		m_fMargin = 0;
	_uint*		m_pAlpha = { nullptr };
	_bool		m_bToolAlphaUse = false;
	_bool		m_bActiveArea = false;

	/* InstanceUI For Preview */
	vector<CTexture*>						m_vecTexture = {};
	vector<string>							m_vecFileName = {};
	vector<ID3D11ShaderResourceView*>		m_vecSRVs = {};


	/****************** COMMON && Basic UI  ******************/
	_int									m_iFileIndex = {};
	/* For Check Data (Layer/Component) */
	_int									m_iLayerIndex = {};
	_int									m_iComIndex = {};

	_char									m_szSelectedLayer[MAX_PATH] = "";
	_char									m_szSelectedComTag[MAX_PATH] = "";
	_int									m_iSelectedUiType = {};

	/* Real Data */
	/* UIDesc */
	_char									m_LayerTag[MAX_PATH] = "";
	_char									m_ComTag[MAX_PATH] = "";

	/* Input Map */
	_tchar									m_szLyerTag[MAX_PATH] = L"";
	_tchar									m_szcomTag[MAX_PATH] = L"";

	_float3									m_vPos = { 0.f, 0.f, 0.f };
	_float3									m_vScale = { 0.f, 0.f, 0.f };
	_float3									m_vRot = { 0.f, 0.f, 0.f };

	_int									m_iPassNum = {};

	// 정보 저장용
	map<string,vector<string>>				m_ComTagMap = {};
	CToolUIObj::UI_DESC*					m_pSelectedDesc = { nullptr };

	_float4x4								m_WorldMat = {};



public:
	static CUI_Tool* Create();
	void Free() override;
};

END

