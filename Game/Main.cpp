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

	std::function<void(dae::Scene&)>loadFunc = [](dae::Scene& scene)
	{
		QBert::Utils::CreateSelectionScreenUI(scene);
	};

	auto& scene = dae::SceneManager::GetInstance().CreateScene(dae::SceneName(QBert::SceneName::IntroScene), loadFunc);
	scene.Load();

	loadFunc = [](dae::Scene& scene)
	{
		// font
		auto font = dae::ResourceManager::GetInstance().LoadFont("QBertFont.ttf", 20);

		// fps
		auto go = std::make_unique<dae::GameObject>();
		go->SetLocalPosition(10, 10);
		go->AddComponent(std::make_unique<dae::FpsComponent>(go.get(), "0.0 FPS", font, SDL_Color{77, 206, 77, 255}));
		scene.Add(std::move(go));

		// level
		auto levelObj = QBert::Utils::CreateLevel(scene, 0, false, 0, 1);
		auto levelComp = levelObj->GetComponent<QBert::LevelBase>();

		// player
		go = QBert::Utils::CreatePlayer(levelComp);
		scene.Add(std::move(go));

		//#if defined(__EMSCRIPTEN__)
		//	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(0, -1, 0)), SDL_SCANCODE_W, SDL_EVENT_KEY_DOWN);
		//	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(0, 1, 0)), SDL_SCANCODE_S, SDL_EVENT_KEY_DOWN);
		//	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(-1, 0, 0)), SDL_SCANCODE_A, SDL_EVENT_KEY_DOWN);
		//	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(1, 0, 0)), SDL_SCANCODE_D, SDL_EVENT_KEY_DOWN);
		//
		//	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::TakeDamageCommand>(go.get(), 3), SDL_SCANCODE_Q, SDL_EVENT_KEY_DOWN);
		//	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::TurnTileCommand>(go.get(), 10), SDL_SCANCODE_T, SDL_EVENT_KEY_DOWN);
		//
		//#else
		//	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(0, -1, 0)), XINPUT_GAMEPAD_DPAD_UP, 0);
		//	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(0, 1, 0)), XINPUT_GAMEPAD_DPAD_DOWN, 0);
		//	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(-1, 0, 0)), XINPUT_GAMEPAD_DPAD_LEFT, 0);
		//	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(1, 0, 0)), XINPUT_GAMEPAD_DPAD_RIGHT, 0);
		//
		//	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::TakeDamageCommand>(go.get(), 3), XINPUT_GAMEPAD_A, 0);
		//	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::TurnTileCommand>(go.get(), 10), XINPUT_GAMEPAD_B, 0);
		//#endif // !EMSCRIPTEN

		dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MuteCommand>(), SDL_SCANCODE_F2, SDL_EVENT_KEY_DOWN);

		QBert::Utils::CreateUi(scene);
	};

	dae::SceneManager::GetInstance().CreateScene(dae::SceneName(QBert::SceneName::SinglePlayerScene), loadFunc);

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
