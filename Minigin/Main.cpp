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

#include "TextComponent.h"
#include "RenderComponent.h"
#include "FpsComponent.h"
#include "RotatorComponent.h"
#include "ImGuiComponent.h"
#include "MovementComponent.h"

#include "Commands.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	// background
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "background.png"));
	scene.Add(std::move(go));

	// logo
	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "logo.png"));
	go->SetLocalPosition(358, 180);
	scene.Add(std::move(go));

	// title
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), "Programming 4 Assignment", font));
	go->SetLocalPosition(292, 20);
	scene.Add(std::move(go));

	// fps
	go = std::make_unique<dae::GameObject>();
	go->SetLocalPosition(10, 10);
	go->AddComponent(std::make_unique<dae::FpsComponent>(go.get(), "0.0 FPS", font));
	scene.Add(std::move(go));

	// player
	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "Q_Bert.png"));
	go->SetLocalPosition(100, 100);
	go->SetScale(3.f);
	go->AddComponent(std::make_unique<dae::MovementComponent>(go.get(), 100.f));

	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(0, -1, 0)), SDL_SCANCODE_W, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(0, 1, 0)), SDL_SCANCODE_S, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(-1, 0, 0)), SDL_SCANCODE_A, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(1, 0, 0)), SDL_SCANCODE_D, SDL_EVENT_KEY_DOWN);

	scene.Add(std::move(go));


	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "Q_Bert_Enemy.png"));
	go->SetLocalPosition(300, 200);
	go->SetScale(3.f);
	go->AddComponent(std::make_unique<dae::MovementComponent>(go.get(), 100.f));

	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(0, -1, 0)), XINPUT_GAMEPAD_DPAD_UP, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(0, 1, 0)), XINPUT_GAMEPAD_DPAD_DOWN, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(-1, 0, 0)), XINPUT_GAMEPAD_DPAD_LEFT, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(1, 0, 0)), XINPUT_GAMEPAD_DPAD_RIGHT, 0);

	scene.Add(std::move(go));
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
