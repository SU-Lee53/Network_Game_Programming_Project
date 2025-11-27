#pragma once
#include "Scene.h"
#include "D3DCore.h"

#include "RenderManager.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "GameTimer.h"
#include "AssimpLoader.h"
#include "GuiManager.h"
#include "NetworkManager.h"
#include "EffectManager.h"

class GameFramework {
public:
	GameFramework(BOOL bEnableDebugLayer, BOOL bEnableGBV);

	void Update();
	void Render();

private:
	//std::unique_ptr<Scene> m_pScene = nullptr;

public:
	// Core & Managers
	static std::unique_ptr<D3DCore>				g_pD3DCore;
	static std::unique_ptr<ResourceManager>		g_pResourceManager;
	static std::unique_ptr<RenderManager>		g_pRenderManager;
	static std::unique_ptr<ShaderManager>		g_pShaderManager;
	static std::unique_ptr<TextureManager>		g_pTextureManager;
	static std::unique_ptr<SceneManager>		g_pSceneManager;
	static std::unique_ptr<InputManager>		g_pInputManager;
	static std::unique_ptr<GameTimer>			g_pGameTimer;
	static std::unique_ptr<AssimpLoader>		g_pModelLoader;
	static std::unique_ptr<ModelManager>		g_pModelManager;
	static std::unique_ptr<GuiManager>			g_pGuiManager;
	static std::unique_ptr<NetworkManager>		g_pNetworkManager;
	static std::unique_ptr<EffectManager>		g_pEffectManager;

};

#define D3DCORE			GameFramework::g_pD3DCore
#define RESOURCE		GameFramework::g_pResourceManager
#define RENDER			GameFramework::g_pRenderManager
#define SHADER			GameFramework::g_pShaderManager
#define TEXTURE			GameFramework::g_pTextureManager
#define SCENE			GameFramework::g_pSceneManager
#define INPUT			GameFramework::g_pInputManager
#define TIMER			GameFramework::g_pInputManager
#define MODEL			GameFramework::g_pModelManager
#define GUI				GameFramework::g_pGuiManager
#define NETWORK			GameFramework::g_pNetworkManager
#define EFFECT			GameFramework::g_pEffectManager

#define MODELLOADER			GameFramework::g_pModelLoader


#define CUR_SCENE		GameFramework::g_pSceneManager->GetCurrentScene()
#define DT				GameFramework::g_pGameTimer->GetTimeElapsed()
