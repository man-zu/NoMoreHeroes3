#include "Loader.h"

#include "GameInstance.h"

#include "ToolAnimObj.h"
#include "ToolMapObj.h"
#include "ToolTriggerObj.h"
#include "ToolColliderObj.h"
#include "Tool_Weapon.h"
#include "TestGround.h"
#include "Terrain.h"
#include "Grid.h"
#include "Water.h"
#include "Effect_Particle_Tool.h"
#include "Effect_Mesh_Tool.h"
#include "Effect_Trail_Tool.h"
#include "Effect_Group_Tool.h"
#include "ToolUIObj.h"
#include "Free_Camera.h"
#include "Grass_Cube.h"

#include "CinemaObject.h"
#include "Cinema_Camera.h"




CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

unsigned int APIENTRY Loading_Main(void* pArg)
{
	CLoader* pLoader = (CLoader*)pArg;

	return pLoader->Loading();
}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Loading_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

unsigned int CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	CoInitializeEx(nullptr, 0);

	HRESULT hr = S_OK;

	switch (m_eNextLevelID)
	{
	case LEVEL_ANIMTOOL:
		hr = Loading_For_AnimTool();
		break;
	case LEVEL_MAPTOOL:
		hr = Loading_For_MapTool();
		break;
	case LEVEL_UITOOL:
		hr = Loading_For_UiTool();
		break;
	case LEVEL_EFFECTTOOL:
		hr = Loading_For_EffectTool();
		break;
	case LEVEL_CINEMATOOL:
		hr = Loading_For_CinemaTool();
		break;
	}

	LeaveCriticalSection(&m_CriticalSection);

	if (FAILED(hr))
		return 1;

	return 0;

}


HRESULT CLoader::Loading_For_UiTool()
{
	// UI 관련 텍스쳐 한번에 가져오기
	wstring wstrUIPath(TEXT("../../Resources/UI/"));

	if (FAILED(Load_UI_Texture(wstrUIPath.c_str())))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(m_eNextLevelID, L"Prototype_Texture_UI_TestImg", CTexture::Create(m_pDevice, m_pContext,
		L"../../Resources/UI/InstanceRect.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_ToolUIObj", CToolUIObj::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	m_bIsFinished = true;

	return S_OK;
}

HRESULT CLoader::Load_UI_Texture(const _tchar* wszPath)
{
	fs::path UIPath(wszPath);
	for (const fs::directory_entry& entry : fs::directory_iterator(UIPath))
	{
		if (entry.is_directory())
		{
			wstring wstrPath(entry.path().c_str());
			wstrPath.append(TEXT("/"));
			if (FAILED(Load_UI_Texture(wstrPath.c_str())))
				return E_FAIL;
		}
		else
		{
			fs::path fileName = entry.path().filename();
			fs::path fileTitle = fileName.stem();
			fs::path fullPath = entry.path();
			fs::path ext = fileName.extension();
			if (fileName.extension().compare(".dds"))
				continue;

			string strFileName = fileName.string();
			wstring wstrTexDir(fullPath);

			if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_UITOOL, fileTitle,
				CTexture::Create(m_pDevice, m_pContext, wstrTexDir.c_str()))))
				return E_FAIL;
		}
	}
	return S_OK;
}

CLoader* CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLoader* pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CriticalSection);

	CloseHandle(m_hThread);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}

