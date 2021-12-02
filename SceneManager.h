#pragma once

#include "Config.h"
#include "Singleton.h"

class Scene;
class SceneManager : public Singleton<SceneManager>
{
private:
	map<string, Scene*> mapScenes;
	map<string, Scene*> mapLoadingScenes;

public:
	virtual ~SceneManager() {}
	static Scene* currScene;		// ���� ��� ���� ��
	static Scene* readyScene;		// �غ� ���� ��
	static Scene* loadingScene;		// �ε� ��

	void Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	void AddScene(string key, Scene* scene);
	void AddLoadingScene(string key, Scene* loadingScene);

	HRESULT ChangeScene(string sceneName);
	HRESULT ChangeScene(string sceneName, string loadingSceneName);

};

