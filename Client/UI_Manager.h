#pragma once

#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "UI_Define.h"



BEGIN(Engine)
class CGameInstance;
class CGameObject;
END

BEGIN(Client)
class CUI_Manager
{
	DECLARE_SINGLETON(CUI_Manager)

private:
	CUI_Manager();
	~CUI_Manager();

public:
	HRESULT Initialize();

public:
	void Active_UI(const wstring& strLayerTag);
	void Inactive_UI(const wstring& strLayerTag);
	void Insert_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strUIName);
	void Set_ReturnToPool(const wstring& strLayerTag, _bool b);

	/* Scene */
	class CUIScene* GetScene(wstring SceneName);
	void PutScene(_int eLevel, wstring SceneName, class CUIScene* pScene);
	void RemoveScene(wstring SceneName);

	/* SubTitle */
	vector<wstring>* GetSubTitle(wstring TitleName);

	void SceneInit(wstring SceneName);
	void SceneOut(wstring SceneName);

	/* UI Object */
	_bool PutUI(_int eLevel, wstring UIName, vector<class CUI*>& pObjvec); 
	void RemoveUI(wstring UIName); 
	vector<class CUI*> GetUIVec(wstring UIName) {
		if (0 >= m_UIs[GET_CURLEVEL][UIName].size())
			return {};

		return m_UIs[GET_CURLEVEL][UIName];
	}
	class CUI* GetUI(wstring UIName, _int idx) { 
		if (0 >= m_UIs[GET_CURLEVEL][UIName].size())
			return NULL;

		return m_UIs[GET_CURLEVEL][UIName][idx];
	}

	HRESULT LoadUISavedData(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT ReadyCCTVChatData();
	HRESULT LoadSubTitleData(wstring filePath);
	HRESULT LoadUIData(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring filePath);
	CUI::UI_DESC* GetDesc(wstring UIDescName) { return m_UIdescs[UIDescName]; } // 각 UI이름(ComTag) 에 해당하는 정보Desc 가져오기 


	/* Change Level */
	void ChangeLevel();

	/* UI_Common_BattleScencOut*/
	void BatteryCharge();
	void BatteryMinus();
	void BattleButtonIn();
	void BattleButtonOut();
	void FinalSlashSkillInit(_uint _eDir);
	void FinalSlashSkillOut();
	void CommonBattleEnd(INS_LETTER eLetterType = INS_LETTER_END);
	void CommonBattleEndLetterDisappear(INS_LETTER eLetterType = INS_LETTER_END);

	/* CCTV */
	vector<wstring> GetChatingContent(CCTV_USERTYPE eUserType) { return m_tCCTVchat[(_int)eUserType]; }
	void SetCCTVinfo(CCTV_USER tUserInfo);
	CCTV_USER GetCCTVuser(CCTV_USERTYPE eUserType) { return m_tCCTVuser[(_int)eUserType]; }

	/* SubTitle */
	void ShowSubTitle(wstring wstrContent = L"");
	void ShowSubTitle_Timer(wstring wstrContent = L"", _float _fTotalTime = 0.f, _float _fInterval = 1.f);
	void HideSubTitle();
	void NextSubTitle();
	void Insert_ExitFunc(function<void()> _Func);

private:
	CGameInstance*						m_pGameInstance = {};

	wstring								m_CurLayTag = L" ";
	vector<wstring>						m_UILaytag = {};
	vector<wstring>						m_UIComtag = {};

	/* CCTV */
	vector<vector<wstring>>				m_tCCTVchat = {};
	vector<CCTV_USER>					m_tCCTVuser = {};


	map<const wstring, vector<wstring>*>		m_UISubTitle = {};
	map<const wstring, CUI::UI_DESC*>			m_UIdescs = {};
	map<const wstring, class CUIScene*>*		m_Scenes = {};
	map<const wstring, vector<class CUI*>>*		m_UIs = {};

private:
	void Release();
};

END


