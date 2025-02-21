#include "Loader.h"

#include "GameInstance.h"

// Manager
#include "UI_Manager.h"

/* UI */
#include "UI_Instance_MonsterHP.h"
#include "UI_SushiShop_PopupText.h"
#include "UI_InBattle_Boss.h"
#include "UI_InBattle_Player.h"
#include "UI_InBattle_Monster.h"
#include "UI_InBattle_Named.h"
#include "UI_InBattle_Letter.h"
#include "UI_Loading.h"
#include "UI_Cosmic_Battle.h"
#include "UI_PowerUp.h"
#include "UI_PowerUp_Icon.h"
#include "UI_PlayerStatus.h"
#include "UI_MoveButton.h"
#include "UI_BattleResult.h"
#include "UI_ToiletCheckPoint.h"
#include "UI_MainMenu.h"
#include "UI_Home.h"
#include "UI_Intro_Logo.h"
#include "UI_SushiShop.h"
#include "UI_TurnRPG.h"
#include "UI_InBattle_Mechanic.h"
#include "UI_Smash.h"
#include "UI_Mario.h"
#include "UI_TurnRPG_AttackOption.h"
#include "UI_PlayerStatus_Obj.h"
#include "UI_World_City.h"
#include "UI_World_City_Minimap.h"
#include "UI_World_City_MinimapTarget.h"
#include "UI_CCTV.h"
#include "UI_CCTV_Chating.h"
#include "UI_CCTV_ChatIcon.h"
#include "UI_CCTV_SkipImg.h"
#include "UI_SubTitle.h"
#include "UI_MiniGame_Toilet.h"
#include "UI_MiniGame_Trash.h"



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

	const char* strSoundPath[] = {
		"Sound/BGM",
		"Sound/Static"
	};

	_int iPathCount = sizeof(strSoundPath) / sizeof(strSoundPath[0]);

	if (FAILED(Load_Sound(strSoundPath, iPathCount)))
	{
		LeaveCriticalSection(&m_CriticalSection);
		return 1;
	}

	switch (m_eNextLevelID)
	{
	case LEVEL_MAINMENU:
		hr = Load_Level_MainMenu();
		break;
	case LEVEL_WORLD_HOME:
		hr = Load_Level_World_Home();
		break;
	case LEVEL_WORLD_HOME_LAB:
		hr = Load_Level_World_Home_Lab();
		break;
	case LEVEL_BATTLE_MOTEL_FRONT:
		hr = Load_Level_Battle_Motel_Front();
		break;
	case LEVEL_WORLD_CITY:
		hr = Load_Level_World_City();
		break;
	case LEVEL_WORLD_CITY_TRASHCOLLECTOR:
		hr = Load_Level_World_City_TrashCollector();
		break;
	case LEVEL_WORLD_CITY_TOILET:
		hr = Load_Level_World_City_Toilet();
		break;
	case LEVEL_BATTLE_UFO_OUTSIDE:
		hr = Load_Level_Battle_UFO_Outside();
		break;
	case LEVEL_WORLD_UFO:
		hr = Load_Level_World_UFO();
		break;
	case LEVEL_WORLD_UFO_TOILET:
		hr = Load_Level_World_UFO_Toilet();
		break;
	case LEVEL_BATTLE_UFO_INSIDE:
		hr = Load_Level_Battle_UFO_Inside();
		break;
	case LEVEL_BOSS_BLACKHOLE_GROUND:
		hr = Load_Level_Boss_BlackHole_Ground();
		break;
	case LEVEL_BOSS_BLACKHOLE_SPACE:
		hr = Load_Level_Boss_BlackHole_Space();
		break;
	case LEVEL_BOSS_SONICJUICE:
		hr = Load_Level_Boss_SonicJuice();
		break;
	case LEVEL_SMASHBROS_BOSS_DAMON:
		hr = Load_Level_SmashBros_Boss_Damon();
		break;
	case LEVEL_ARROW_A_ROW:
		hr = Load_Level_Arrow_A_Row();
		break;
	case LEVEL_MARIO:
		hr = Load_Level_Mario();
		break;
	case LEVEL_BATTLE_ISLAND:
		hr = Load_Level_Battle_Island();
		break;
	}

	LeaveCriticalSection(&m_CriticalSection);

	if (FAILED(hr))
		return 1;

	return 0;

}

HRESULT CLoader::Load_Level_MainMenu()
{
	if (FAILED(Load_Resource(L"../../Resources/DataFiles/AddressData/Level_MainMenu.dat")))
		return E_FAIL;

	if (FAILED(Ready_Prototypes_GameObject()))
		return E_FAIL;

	const char* strSoundPath[] =
	{
		"Sound/MainMenu",
	};

	_int iPathCount = sizeof(strSoundPath) / sizeof(strSoundPath[0]);

	if (FAILED(Load_Sound(strSoundPath, iPathCount)))
		return E_FAIL;

	if (FAILED(Load_Effects_Prototype(LEVEL_STATIC, TEXT("../../Resources/Effect/SaveData/Cinematic"))))
		return E_FAIL;

	m_bIsFinished = true;

	return S_OK;
}

HRESULT CLoader::Ready_Prototypes_GameObject()
{
#pragma region
	if (FAILED(Load_UI()))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CLoader::Load_UI()
{
	// ==========================================Texture================================================================================

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"Prototype_Texture_InstanceImg", CTexture::Create(m_pDevice, m_pContext,
		L"../../Resources/UI/InstanceRect.png"))))
		return E_FAIL;

	wstring wstrUIPath(TEXT("../../Resources/UI/UI/"));

	if (FAILED(Load_UI_Texture(wstrUIPath.c_str())))
		return E_FAIL;

	// ==========================================Prototype================================================================================
	/* Base */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_Instance_MonsterHP", CUI_Instance_MonsterHP::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_SushiShop_PopupText", CUI_SushiShop_PopupText::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 00. Intro_Logo */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_Intro_Logo", CUI_Intro_Logo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 00. BattleResult */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_Battle_Result", CUI_BattleResult::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 00. MainMenu */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_MainMenu", CUI_MainMenu::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 01.InBattle */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_InBattle_Player", CUI_InBattle_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_InBattle_Boss", CUI_InBattle_Boss::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_InBattle_Monster", CUI_InBattle_Monster::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_InBattle_Named", CUI_InBattle_Named::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_InBattle_Letter", CUI_InBattle_Letter::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_InBattle_Mechanic", CUI_InBattle_Mechanic::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 02. CosmicBattle */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_Cosmic", CUI_Cosmic_Battle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 03. PlayerStatus */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_PlayerStatus", CUI_PlayerStatus::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_PlayerStatus_Obj", CUI_PlayerStatus_Obj::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 04. PowerUP */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_PowerUp", CUI_PowerUp::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_PowerUp_Icon", CUI_PowerUp_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 05. Home */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_Home", CUI_Home::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 06. SushiShop */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_SushiShop", CUI_SushiShop::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 08. TurnRPG */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_Smash", CUI_Smash::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 13. Mario */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_Mario", CUI_Mario::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 10. ButtonA */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_MoveButton", CUI_MoveButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 11. ToiletCheckPoint */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_ToiletCheckPoint", CUI_ToiletCheckPoint::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 12. TurnRPG */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_TurnRPG", CUI_TurnRPG::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_TurnRPG_AttackOption", CUI_TurnRPG_AttackOption::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 14. World City */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_World_City", CUI_World_City::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_World_City_Minimap", CUI_World_City_Minimap::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_World_City_MinimapTarget", CUI_World_City_MinimapTarget::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 14. CCTV */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_CCTV", CUI_CCTV::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_CCTV_Chating", CUI_CCTV_Chating::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_CCTV_ChatIcon", CUI_CCTV_ChatIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_CCTV_SkipImg", CUI_CCTV_SkipImg::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 15. SubTitle */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_SubTitle", CUI_SubTitle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 16. MiniGame_Trash */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_MiniGame_Trash", CUI_MiniGame_Trash::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 17. MiniGame_Toilet */
	if (FAILED(m_pGameInstance->Add_Prototype(L"Prototype_UI_MiniGame_Toilet", CUI_MiniGame_Toilet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	// ==========================================LoadData================================================================================
	if (FAILED(UIMGR->LoadUISavedData(m_pDevice, m_pContext)))
		return E_FAIL;


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

			/* N장의 이미지 넣을 경우 폴더명 "MultiImg" 으로 구분 */
			if (-1 != wstrTexDir.find(L"MultiImg"))
			{
				if (0 == m_iMultiImageCnt)
				{
					wstring sFindImgCnt = fileTitle.c_str();
					sFindImgCnt = sFindImgCnt.substr(0, 2);
					m_iMultiImageCnt = stoi(sFindImgCnt);

					wstring strMultiFileName = fileTitle.c_str();
					strMultiFileName = strMultiFileName.substr(0, strMultiFileName.length() - 1);
					strMultiFileName += L"%d";

					wstring MultiFilePath = wstrTexDir.substr(0, wstrTexDir.length() - strFileName.length());
					MultiFilePath += strMultiFileName + ext.c_str();

					if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, fileTitle,
						CTexture::Create(m_pDevice, m_pContext, MultiFilePath.c_str(), m_iMultiImageCnt))))
						return E_FAIL;
				}

				--m_iMultiImageCnt;
			}
			else
			{
				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, fileTitle,
					CTexture::Create(m_pDevice, m_pContext, wstrTexDir.c_str()))))
					return E_FAIL;
			}
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