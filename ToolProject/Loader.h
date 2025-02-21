#pragma once

#include "Tool_Defines.h"
#include "Base.h"


BEGIN(Engine)
class CGameInstance;
END

BEGIN(Tool)
class CLoader : public CBase
{
public:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLoader() = default;

public:
	_bool isFinished() const {
		return m_bIsFinished;
	}

public:
	HRESULT Initialize(LEVEL eNextLevelID);
	unsigned int Loading();

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

private:
	HANDLE				m_hThread = 0;
	CRITICAL_SECTION	m_CriticalSection;
	LEVEL				m_eNextLevelID = LEVEL_END;

private:
	CGameInstance*		m_pGameInstance = { nullptr };

	_tchar				m_szLoadingText[MAX_PATH] = TEXT("");
	_bool				m_bIsFinished = false;

private:
	// UI
	HRESULT Load_UI_Texture(const _tchar* wszPath);
	
	HRESULT Load_Resource(const wstring& strFilePath, const wstring& strNavigationPath = {});

public:
	const _tchar* Get_LoadingText() const {
		return m_szLoadingText;
	}

public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID);
	virtual void Free() override;
};
END

