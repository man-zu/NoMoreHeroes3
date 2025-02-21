#include "UI_Manager.h"

#include "GameInstance.h"
#include "Layer.h"

#include "UI_InBattle_Letter.h"
#include "UI_InBattle_Player.h"
#include "UI_SubTitle.h"
#include "UI_Smash.h"

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
	m_UIs = new map<const wstring, vector<class CUI*>>[LEVEL_END];
	m_Scenes = new map<const wstring, class CUIScene*>[LEVEL_END];
	Safe_AddRef(m_pGameInstance);

	m_pGameInstance->Add_Font(L"FontUI", L"../../Resources/UI/Font_UI_Tool/Gothic.spritefont");
	m_pGameInstance->Add_Font(L"FontUIBold", L"../../Resources/UI/Font_UI_Tool/BoldGothic.spritefont");
}

CUI_Manager::~CUI_Manager()
{
	Release();
}

HRESULT CUI_Manager::Initialize()
{
	return S_OK;
}

void CUI_Manager::Active_UI(const wstring& strLayerTag)
{
	vector<CUI*> UIs = UIMGR->GetUIVec(strLayerTag);

	for (auto UIitem : UIs)
		static_cast<CUI*>(UIitem)->Set_Active(true);
}

void CUI_Manager::Inactive_UI(const wstring& strLayerTag)
{
	vector<CUI*> UIs = UIMGR->GetUIVec(strLayerTag);

	for (auto UIitem : UIs)
		static_cast<CUI*>(UIitem)->Set_Active(false);
}

void CUI_Manager::Insert_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strUIName)
{
	vector<CUI*> UIs = m_UIs[iLevelIndex][strUIName];

	for (auto UIitem : UIs)
	{
		m_pGameInstance->Insert_GameObject(iLevelIndex, strLayerTag, UIitem);
		Safe_AddRef(UIitem);
		UIitem->Set_ReturnToPool(false);
	}
}

void CUI_Manager::Set_ReturnToPool(const wstring& strLayerTag, _bool b)
{
	vector<CUI*> UIs = UIMGR->GetUIVec(strLayerTag);

	for (auto item : UIs)
		item->Set_ReturnToPool(b);
}

void CUI_Manager::PutScene(_int eLevel, wstring SceneName, class CUIScene* pScene)
{
	if (m_Scenes[eLevel].find(SceneName) == m_Scenes[eLevel].end())
	{
		m_Scenes[eLevel].insert({ SceneName, pScene });
		Safe_AddRef(pScene);
	}

#ifdef _DEBUG
	//else
	//	cout << "SCENE EXIST " << endl;
#endif
}

void CUI_Manager::RemoveScene(wstring SceneName)
{
	if (m_Scenes[GET_CURLEVEL].find(SceneName) != m_Scenes[GET_CURLEVEL].end())
	{
		m_Scenes[GET_CURLEVEL].erase(SceneName);
	}
}

CUIScene* CUI_Manager::GetScene(wstring SceneName)
{
	if (m_Scenes[GET_CURLEVEL].find(SceneName) != m_Scenes[GET_CURLEVEL].end())
		return m_Scenes[GET_CURLEVEL][SceneName];

#ifdef _DEBUG
	else
		cout << "SCENE NOT EXIST" << endl;
#endif
	return NULL;
}

vector<wstring>* CUI_Manager::GetSubTitle(wstring TitleName)
{
	if (m_UISubTitle.find(TitleName) != m_UISubTitle.end())
		return m_UISubTitle[TitleName];
	
	MSG_BOX(L"TITLE NAME ERROR!");
	return NULL;
}

void CUI_Manager::SceneInit(wstring SceneName)
{
	if (m_Scenes[GET_CURLEVEL].find(SceneName) != m_Scenes[GET_CURLEVEL].end())
		m_Scenes[GET_CURLEVEL][SceneName]->SceneInit();

#ifdef _DEBUG
	else
		cout << "SCENE NOT EXIST" << endl;
#endif
}

void CUI_Manager::SceneOut(wstring SceneName)
{
	if (m_Scenes[GET_CURLEVEL].find(SceneName) != m_Scenes[GET_CURLEVEL].end())
		m_Scenes[GET_CURLEVEL][SceneName]->SceneOut();

#ifdef _DEBUG
	else
		cout << "SCENE NOT EXIST" << endl;
#endif
}

void CUI_Manager::ChangeLevel()
{
	for (auto& itemList : m_UIs[GET_CURLEVEL])
	{
		for (auto item : itemList.second)
			Safe_Release(item);
	}
	for (auto item : m_Scenes[GET_CURLEVEL])
	{
		Safe_Release(item.second);
	}
	m_Scenes[GET_CURLEVEL].clear();
	m_UIs[GET_CURLEVEL].clear();

}

void CUI_Manager::BatteryCharge()
{
	if (NULL != GetScene(L"SCENE_INS_PlayerBattle"))
		static_cast<CUI_InBattle_Player*>(UIMGR->GetScene(L"SCENE_INS_PlayerBattle"))->BatteryCharge();
}

void CUI_Manager::BatteryMinus()
{
	if (NULL != GetScene(L"SCENE_INS_PlayerBattle"))
		static_cast<CUI_InBattle_Player*>(UIMGR->GetScene(L"SCENE_INS_PlayerBattle"))->BatteryMinus();
}

void CUI_Manager::BattleButtonIn()
{
	if (NULL != GetScene(L"SCENE_INS_PlayerBattle"))
		static_cast<CUI_InBattle_Player*>(GetScene(L"SCENE_INS_PlayerBattle"))->BattleButtonIn();
}

void CUI_Manager::BattleButtonOut()
{
	if (NULL != GetScene(L"SCENE_INS_PlayerBattle"))
		static_cast<CUI_InBattle_Player*>(GetScene(L"SCENE_INS_PlayerBattle"))->BattleButtonOut();
}

void CUI_Manager::FinalSlashSkillInit(_uint _eDir)
{
	if (NULL != GetScene(L"SCENE_INS_PlayerBattle"))
		static_cast<CUI_InBattle_Player*>(GetScene(L"SCENE_INS_PlayerBattle"))->FinalSlashSkillInit(FINALSLAH_DIR(_eDir));
	else if (NULL != GetScene(L"SCENE_Smash"))
		static_cast<CUI_Smash*>(GetScene(L"SCENE_Smash"))->FinalSlashSkillInit(FINALSLAH_DIR(_eDir));
}

void CUI_Manager::FinalSlashSkillOut()
{
	if (NULL != GetScene(L"SCENE_INS_PlayerBattle"))
		static_cast<CUI_InBattle_Player*>(GetScene(L"SCENE_INS_PlayerBattle"))->FinalSlashSkillOut();
	else if (NULL != GetScene(L"SCENE_Smash"))
		static_cast<CUI_Smash*>(GetScene(L"SCENE_Smash"))->FinalSlashSkillOut();
}

/* 인스턴스 관련 UI 사라지게 && KILL 문구 뜨도록 */
void CUI_Manager::CommonBattleEnd(INS_LETTER eLetterType)
{
	if (NULL != GetScene(L"Scene_InBattle_Letter"))
		static_cast<CUI_InBattle_Letter*>(GetScene(L"Scene_InBattle_Letter"))->SceneInit(eLetterType);

	if(NULL != GetScene(L"SCENE_INS_PlayerBattle")) GetScene(L"SCENE_INS_PlayerBattle")->SceneOut();
	if(NULL != GetScene(L"SCENE_MonsterHP")) GetScene(L"SCENE_MonsterHP")->SceneOut();
	if(NULL != GetScene(L"SCENE_NamedHP")) GetScene(L"SCENE_NamedHP")->SceneOut();
	if(NULL != GetScene(L"SCENE_INS_BossHP")) GetScene(L"SCENE_INS_BossHP")->SceneOut();
	if(NULL != GetScene(L"SCENE_Mechanic_Battle")) GetScene(L"SCENE_Mechanic_Battle")->SceneOut();
}

void CUI_Manager::CommonBattleEndLetterDisappear(INS_LETTER eLetterType)
{
	if (NULL != GetScene(L"Scene_InBattle_Letter"))
		static_cast<CUI_InBattle_Letter*>(GetScene(L"Scene_InBattle_Letter"))->SceneOut(eLetterType);
}

void CUI_Manager::SetCCTVinfo(CCTV_USER tUserInfo)
{
	m_tCCTVuser[tUserInfo.eUserType].eUserType = tUserInfo.eUserType;
	m_tCCTVuser[tUserInfo.eUserType].wstrName = tUserInfo.wstrName;
	m_tCCTVuser[tUserInfo.eUserType].wstrSceneName = tUserInfo.wstrSceneName;
	m_tCCTVuser[tUserInfo.eUserType].iEnterCnt += 1;
}

void CUI_Manager::ShowSubTitle(wstring wstrContent)
{
	if (NULL != GetScene(L"SCENE_SubTitle"))
		static_cast<CUI_SubTitle*>(GetScene(L"SCENE_SubTitle"))->ShowSubTitle(wstrContent);
}

void CUI_Manager::ShowSubTitle_Timer(wstring wstrContent, _float _fTotalTime, _float _fInterval)
{
	if (NULL != GetScene(L"SCENE_SubTitle"))
		static_cast<CUI_SubTitle*>(GetScene(L"SCENE_SubTitle"))->ShowSubTitle_Timer(wstrContent, _fTotalTime, _fInterval);
}

void CUI_Manager::HideSubTitle()
{
	if (NULL != GetScene(L"SCENE_SubTitle"))
		static_cast<CUI_SubTitle*>(GetScene(L"SCENE_SubTitle"))->HideSubTitle();
}

void CUI_Manager::NextSubTitle()
{
	if (NULL != GetScene(L"SCENE_SubTitle"))
		static_cast<CUI_SubTitle*>(GetScene(L"SCENE_SubTitle"))->NextSubTitle();
}

void CUI_Manager::Insert_ExitFunc(function<void()> _Func)
{
	if (NULL != GetScene(L"SCENE_SubTitle"))
		static_cast<CUI_SubTitle*>(GetScene(L"SCENE_SubTitle"))->Insert_ExitFunc(_Func);
}

_bool CUI_Manager::PutUI(_int eLevel, wstring UIName, vector<class CUI*>& pObjvec)
{
	if (m_UIs[eLevel].find(UIName) == m_UIs[eLevel].end())
	{
		m_UIs[eLevel].insert({ UIName, pObjvec });
		for (auto pUI : pObjvec)
			Safe_AddRef(pUI);

		return true;
	}
	else
		return false;
}

void CUI_Manager::RemoveUI(wstring UIName)
{
	if (m_UIs[GET_CURLEVEL].find(UIName) != m_UIs[GET_CURLEVEL].end())
	{
		for (auto pUI : GetUIVec(UIName))
			Safe_Release(pUI);
		m_UIs[GET_CURLEVEL].erase(UIName);
	}
}

HRESULT CUI_Manager::LoadUISavedData(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	fs::path UIPath(L"../../Resources/UI/Data/");
	for (const fs::directory_entry& entry : fs::directory_iterator(UIPath))
	{
		fs::path fileName = entry.path().filename();
		fs::path fullPath = entry.path();
		fs::path ext = fileName.extension();

		if (!entry.is_directory() && ext == ".dat")
		{
			if (fileName == L"UI_Loading.dat")
				continue;

			/* Saved Tool DATA */
			if (FAILED(UIMGR->LoadUIData(pDevice, pContext, fullPath)))
				return E_FAIL;
		}
		if (!entry.is_directory() && ext == ".json")
		{
			/* SubTitle JSON DATA */
			if (FAILED(UIMGR->LoadSubTitleData(fullPath)))
				return E_FAIL;
		}
	}

	if (FAILED(UIMGR->ReadyCCTVChatData()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Manager::ReadyCCTVChatData()
{
	m_tCCTVuser.resize((_int)CCTV_USER_END);
	m_tCCTVchat.resize((_int)CCTV_USER_END);

	/*
	* [UI_CCTV.cpp - AddChating(_int iStartIdx)]
	* 첫 번째 진입시 m_tCCTVchat[1] ~
	* 두 번째 이후: m_tCCTVchat[0].
	*/

	m_tCCTVchat[CCTV_USER_SUSHI].push_back(L"오! 어서옵쇼!\n오늘은 무얼 먹을겐가?");	
	m_tCCTVchat[CCTV_USER_SUSHI].push_back(L"오! 어서옵쇼! \n내 이름은 버그자부로.\n펄떡이는 생선을 위장으로도 느껴보자!");	
	m_tCCTVchat[CCTV_USER_SUSHI].push_back(L"자네, 지금 날 보며 UFO에 무슨 초밥\n가게가 다 있느냐고 생각하는 거겠지 ?\n하지만 잘 듣게, 친구!");
	m_tCCTVchat[CCTV_USER_SUSHI].push_back(L"구로디지털단지에 가게를 하나 차리고 있었지.\n그런데 갑자기 하늘에서 내려온 빛에 빨려 들어가는 바람에\n영문도 모른채 이곳까지 오게 된거야.");
	m_tCCTVchat[CCTV_USER_SUSHI].push_back(L"뭐, 이정도면 내 사정은 충분히 알아들었겠지!\n어때, 초밥 좀 먹지 않겠나 친구 ?!");
	m_tCCTVchat[CCTV_USER_SUSHI].push_back(L"1, 2 버튼으로\n\"포장\"이나 \"식사\" 중 하나를 고를 수 있어!");
	m_tCCTVchat[CCTV_USER_SUSHI].push_back(L"\"포장\"은 전투 중 배가 고프면\n바로 한 입 먹을 수 있도록 잘 포장한 녀석이지!");
	m_tCCTVchat[CCTV_USER_SUSHI].push_back(L"\"식사\"는 여기 앉아서 먹고 가는 거야.\n다음 전투를 위해 잠깐 쉬었다 갈 수 있어!");
	m_tCCTVchat[CCTV_USER_SUSHI].push_back(L"뭘 선택하든 최고의 메뉴를 만날 수 있지!\n좋아, 어떻게 할 건가 ? ");

	m_tCCTVchat[CCTV_USER_TREE].push_back(L"수중에 있는 WESN을 모조리 쏟아부으라고 !");	
	m_tCCTVchat[CCTV_USER_TREE].push_back(L"이봐. 못생긴 오타쿠.");	
	m_tCCTVchat[CCTV_USER_TREE].push_back(L"내 파워 업 머신으로\n스킬을 강화해 주지.");
	m_tCCTVchat[CCTV_USER_TREE].push_back(L"수중에 있는 WESN을 모조리 쏟아부으라고 !");

	m_tCCTVchat[CCTV_USER_CITIZEN_TOILET].push_back(L"덕분에 공중화장실을 쓸 수 있게 되었어요.\n다음에 또 막히면 부탁할게요!");	
	m_tCCTVchat[CCTV_USER_CITIZEN_TOILET].push_back(L"정말 끔찍하지 않나요?\n도시에 있는 공중화장실은 항상 막혀있다니까요.");	
	m_tCCTVchat[CCTV_USER_CITIZEN_TOILET].push_back(L"당신이 정말 용감한지 보고 싶군요.\n온 세상이 배설물로 뒤덮이기 전에\n화장실을 고쳐주시겠어요?");
	m_tCCTVchat[CCTV_USER_CITIZEN_TOILET].push_back(L"당신의 그 찰나의 선한 마음에 대한 \n존경의 표시로, 후한 보상을 드리겠습니다.");

	m_tCCTVchat[CCTV_USER_CITIZEN_TRASH].push_back(L"아무데나 쓰레기 버리는 사람들이 참 많아...\n걸리기만 해봐...");	
	m_tCCTVchat[CCTV_USER_CITIZEN_TRASH].push_back(L"정말 끔찍하지 않나요?\n빈 깡통이 여기저기 널브러져 있다니요.");	
	m_tCCTVchat[CCTV_USER_CITIZEN_TRASH].push_back(L"당신이 정말 용감한지 보고 싶군요.\n가서 쓰레기를 전부 처리해 주시겠어요?");
	m_tCCTVchat[CCTV_USER_CITIZEN_TRASH].push_back(L"당신의 그 찰나의 선한 마음에 대한\n존경의 표시로, 후한 보상을 드리겠습니다.");

	m_tCCTVchat[CCTV_USER_CITIZEN_ISLAND].push_back(L"흠.. 외계인이 계속 나타나네..\n이거 섬을 팔아야하나?");	
	m_tCCTVchat[CCTV_USER_CITIZEN_ISLAND].push_back(L"정말 끔찍하지 않나요?\n도시에 외계인들이 출몰했다니");	
	m_tCCTVchat[CCTV_USER_CITIZEN_ISLAND].push_back(L"제 개인 소유의 섬에도 외계인이 득실득실해요\n제 땅인데 무서워서 갈 수가 없다니깐요");
	m_tCCTVchat[CCTV_USER_CITIZEN_ISLAND].push_back(L"당신이 정말 용감한지 보고 싶군요\n가서 외계인들 좀 잡아주시겠어요?");
	m_tCCTVchat[CCTV_USER_CITIZEN_ISLAND].push_back(L"당신의 그 찰나의 선한 마음에 대한\n존경의 표시로, 후한 보상을 드리겠습니다.");

	return S_OK;
}

HRESULT CUI_Manager::LoadSubTitleData(wstring filePath)
{
	std::ifstream file(filePath, std::ios::in);
	json jDataList;
	file >> jDataList;
	file.close();
	
	for (auto& jData : jDataList)
	{
		vector<wstring>* vecSubTitle = new vector<wstring>;

		for (auto& data : jData["content"])
			vecSubTitle->push_back(Convert_StrToWStr(data));

		m_UISubTitle.emplace(Convert_StrToWStr(jData["SubTitleName"]), vecSubTitle);
	}
	
	return S_OK;
}

HRESULT CUI_Manager::LoadUIData(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring filePath)
{
	/*
	* Load 할 때, TOOL에서 만든 UI_DESC 정보들 m_UIdescs 넣고
	* 객체 필요한 시점에 각 클래스에 맞게 Clone_GameObject.
						정적UI(기능X) 는 UI_BasicBase
	*					인스턴스UI 는 UI_InstanceBase
	* Clone_GameObject 했다면 UIMGR->PutUI(). m_UIs 에 넣어준다.
	*/

	std::ifstream file(Convert_WStrToStr(filePath), std::ios::binary);

	while (file) {

		CUI::UITYPE eUIType = CUI::UITYPE::UI_END;
		file.read(reinterpret_cast<char*>(&eUIType), sizeof(CUI::UITYPE));

		CUI::UI_DESC* pUIDesc = new CUI::UI_DESC;

		pUIDesc->eUIType = eUIType;
		file.read(reinterpret_cast<char*>(pUIDesc->szPrototpyeName), sizeof(_tchar) * MAX_PATH);
		file.read(reinterpret_cast<char*>(pUIDesc->szLayerName), sizeof(_tchar) * MAX_PATH);
		file.read(reinterpret_cast<char*>(&pUIDesc->WorldMat), sizeof(_float4x4));
		file.read(reinterpret_cast<char*>(&pUIDesc->iPass), sizeof(_int));
		file.read(reinterpret_cast<char*>(pUIDesc->szImgName), sizeof(_tchar) * MAX_PATH);

		if (eUIType == CUI::UITYPE::UI_BASIC)
		{
			m_CurLayTag = pUIDesc->szLayerName;
			m_UIdescs.emplace(pUIDesc->szPrototpyeName, pUIDesc);
			m_UILaytag.emplace_back(pUIDesc->szLayerName);
			m_UIComtag.emplace_back(pUIDesc->szPrototpyeName);
		}

		else if (eUIType == CUI::UITYPE::UI_INSTANCE)
		{
			CVIBuffer_UI_Rect_Instance::UIINSTANCE_DESC InstanceUIDesc = {};

			file.read(reinterpret_cast<char*>(&InstanceUIDesc.iCol), sizeof(_int));
			file.read(reinterpret_cast<char*>(&InstanceUIDesc.iRow), sizeof(_int));
			file.read(reinterpret_cast<char*>(&InstanceUIDesc.fScale), sizeof(_float));
			file.read(reinterpret_cast<char*>(&InstanceUIDesc.fMargin), sizeof(_float));
			file.read(reinterpret_cast<char*>(&InstanceUIDesc.fSpeed), sizeof(_float));
			file.read(reinterpret_cast<char*>(&InstanceUIDesc.vColor), sizeof(_float4));
			file.read(reinterpret_cast<char*>(&InstanceUIDesc.eAlign), sizeof(CVIBuffer_UI_Rect_Instance::UIALIGN));

			_uint* piAlpha = new _uint[(InstanceUIDesc.iCol * InstanceUIDesc.iRow)];

			for (_int col = 0; col < InstanceUIDesc.iCol; ++col)
			{
				for (_int row = 0; row < InstanceUIDesc.iRow; ++row)
				{
					_int idx = row + col * InstanceUIDesc.iRow;
					file.read(reinterpret_cast<char*>(&piAlpha[idx]), sizeof(_uint));
				}
			}

			InstanceUIDesc.vColorAlpha = piAlpha;

			if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, pUIDesc->szPrototpyeName,
				CVIBuffer_UI_Rect_Instance::Create(pDevice, pContext, &InstanceUIDesc))))
				return E_FAIL;

			m_UIdescs.emplace(pUIDesc->szPrototpyeName, pUIDesc);
			m_UILaytag.emplace_back(pUIDesc->szLayerName);
			m_UIComtag.emplace_back(pUIDesc->szPrototpyeName);

			Safe_Delete_Array(piAlpha);
		}

		else
			Safe_Delete(pUIDesc);
	}

	file.close();

	return S_OK;
}

void CUI_Manager::Release()
{
	Safe_Release(m_pGameInstance);

	for (auto& item : m_UIdescs)
		Safe_Delete(item.second);

	for (auto& items : m_UISubTitle)
		Safe_Delete(items.second);

	for (int i = 0;i < LEVEL_END; ++i)
	{
		for (auto& itemList : m_UIs[i])
		{
			for (auto item : itemList.second)
				Safe_Release(item);
		}
		for (auto itemList : m_Scenes[i])
		{
			Safe_Release(itemList.second);
		}

		m_Scenes[i].clear();
		m_UIs[i].clear();
	}

	Safe_Delete_Array(m_Scenes);
	Safe_Delete_Array(m_UIs);
}