#pragma once

#include "Config.h"
#include "Singleton.h"

class Scene;
class SceneManager : public Singleton<SceneManager>
{
private:
	map<wstring, Scene*> mapScenes;
	map<wstring, Scene*> mapLoadingScenes;

public:
	virtual ~SceneManager() {}
	static Scene* currScene;		// ���� ��� ���� ��
	static Scene* readyScene;		// �غ� ���� ��
	static Scene* loadingScene;		// �ε� ��

	void Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	void AddScene(const wstring& key, Scene* scene);
	void AddLoadingScene(const wstring& key, Scene* loadingScene);

	HRESULT ChangeScene(const wstring& sceneName);
	HRESULT ChangeScene(const wstring& sceneName, const wstring& loadingSceneName);
};

