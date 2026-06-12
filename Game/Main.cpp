#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "InputManager.h"
#include "Commands.h"
#include "ServiceLocator.h"
#include "SdlSoundSystem.h"

#include "ComponentsInclude.h"

#include "InitUtils.h"
#include "DiscActor.h"
#include "Tags.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

std::unique_ptr<dae::SoundSystem> dae::ServiceLocator::_ss_instance{ std::make_unique<dae::NullSoundSystem>() };

static void load()
{
	// sound
	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SdlSoundSystem>());
	dae::ServiceLocator::GetSoundSystem().Init();
	QBert::Utils::AddSounds();

	// SELECTION SCENE
	std::function<void(dae::Scene&)>loadFunc = [](dae::Scene& scene)
	{
		QBert::Utils::CreateSelectionScreenUI(scene);

		dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MuteCommand>(), SDL_SCANCODE_F2, SDL_EVENT_KEY_DOWN);
	};

	auto& scene = dae::SceneManager::GetInstance().CreateScene(dae::SceneName(QBert::SceneName::IntroScene), loadFunc);
	scene.Load();

	// SINGLEPLAYER SCENE
	loadFunc = [](dae::Scene& scene)
	{
		dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MuteCommand>(), SDL_SCANCODE_F2, SDL_EVENT_KEY_DOWN);

		auto tileIconSprite = QBert::Utils::CreateUi(scene);

		// level
		auto levelObj = QBert::Utils::CreateLevel(scene, tileIconSprite, 0, false, 0, 1);
		auto levelComp = levelObj->GetComponent<QBert::LevelBase>();

		// player
		auto go = QBert::Utils::CreatePlayer(levelComp, glm::vec2(0, 0));
		go = QBert::Utils::AddKeyboardBindings(std::move(go));
		go = QBert::Utils::AddController1Bindings(std::move(go));
		scene.Add(std::move(go));
	};

	dae::SceneManager::GetInstance().CreateScene(dae::SceneName(QBert::SceneName::SinglePlayerScene), loadFunc);

	// COOP SCENE
	loadFunc = [](dae::Scene& scene)
	{
		dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MuteCommand>(), SDL_SCANCODE_F2, SDL_EVENT_KEY_DOWN);

		auto tileIconSprite = QBert::Utils::CreateCoopUi(scene);

		// level
		auto levelObj = QBert::Utils::CreateLevel(scene, tileIconSprite, 0, false, 0, 1);
		auto levelComp = levelObj->GetComponent<QBert::LevelBase>();

		// player
		auto go = QBert::Utils::CreatePlayer(levelComp, glm::vec2(0, 6));
		go = QBert::Utils::AddKeyboardBindings(std::move(go));
		go = QBert::Utils::AddController2Bindings(std::move(go));
		scene.Add(std::move(go));

		go = QBert::Utils::CreatePlayer(levelComp, glm::vec2(6, 0));
		go = QBert::Utils::AddController1Bindings(std::move(go));
		scene.Add(std::move(go));
	};

	dae::SceneManager::GetInstance().CreateScene(dae::SceneName(QBert::SceneName::CoopScene), loadFunc);

	// HIGHSCORE SCENE
	loadFunc = [](dae::Scene& scene)
	{
		QBert::Utils::CreateHighscoreScreenUI(scene);
	};

	dae::SceneManager::GetInstance().CreateScene(dae::SceneName(QBert::SceneName::HighscoreScene), loadFunc);
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
