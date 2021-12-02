#include "SceneManager.h"
#include "Scene.h"

Scene* SceneManager::currScene = nullptr;
Scene* SceneManager::readyScene = nullptr;
Scene* SceneManager::loadingScene = nullptr;

DWORD CALLBACK LoadingThread(LPVOID pvParam)
{
	// ������� �ʱ�ȭ �Ѵ�.
	SceneManager::readyScene->Init();
	SceneManager::currScene = SceneManager::readyScene;

	SceneManager::loadingScene->Release();
	SceneManager::loadingScene = nullptr;
	SceneManager::readyScene = nullptr;

	return 0;
}


void SceneManager::Update()
{
	if (currScene)
		currScene->Update();
}

void SceneManager::Render(HDC hdc)
{
	if (currScene)
		currScene->Render(hdc);
}

void SceneManager::Release()
{
	for(auto& elem : mapScenes)
	{
		SAFE_RELEASE(elem.second);
	}
	mapScenes.clear();
}

void SceneManager::AddScene(string key, Scene* scene)
{
	if (scene == nullptr) return;

	if (mapScenes.find(key) != mapScenes.end())
	{
		return;
	}

	mapScenes.insert(make_pair(key, scene));
}

void SceneManager::AddLoadingScene(string key, Scene* loadingScene)
{
	if (loadingScene == nullptr) return;

	if (mapLoadingScenes.find(key) != mapLoadingScenes.end())
	{
		return;
	}

	mapLoadingScenes.insert(make_pair(key, loadingScene));
}

HRESULT SceneManager::ChangeScene(string sceneName)
{
	map<string, Scene*>::iterator it = mapScenes.find(sceneName);

	if (it == mapScenes.end())
		return E_FAIL;

	if (SUCCEEDED((it->second)->Init()))
	{
		if (currScene)
			currScene->Release();
		
		currScene = it->second;

		return S_OK;
	}

	return E_FAIL;
}

HRESULT SceneManager::ChangeScene(string sceneName, string loadingSceneName)
{
	map<string, Scene*>::iterator it = mapScenes.find(sceneName);

	// �� Ȯ��
	if (it == mapScenes.end())
		return E_FAIL;

	// �ε��� Ȯ��
	map<string, Scene*>::iterator itLoading = mapLoadingScenes.find(loadingSceneName);
	if (itLoading == mapLoadingScenes.end())
	{
		return ChangeScene(sceneName);
	}

	// �ε��� ���� ��
	if (SUCCEEDED((itLoading->second)->Init()))
	{
		if (currScene)
			currScene->Release();

		readyScene = it->second;
		loadingScene = itLoading->second;

		currScene = loadingScene;

		// ��Ƽ������ ó��
		DWORD loadThread;
		HANDLE hThread = CreateThread(NULL, 0,
			LoadingThread /*���� ��ų �Լ� ������(�Լ��̸�)*/,
			NULL /*���� ��ų �Լ��� �� �Ű� ����*/,
			0, &loadThread /*Thread ID*/);

		CloseHandle(hThread); // ���� �ڵ� �ݾ��ִ� ����

		return S_OK;
	}

	return E_FAIL;
}
