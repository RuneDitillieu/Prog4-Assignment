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

#include "Components/ComponentsInclude.h"
#include "Subject.h"
#include "Observers.h"

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


	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	// ui
	auto uiLives = std::make_unique<dae::GameObject>();
	uiLives->AddComponent(std::make_unique<dae::TextComponent>(uiLives.get(), "# lives: 3", font));
	uiLives->SetLocalPosition(15, 250);
	auto uiScore = std::make_unique<dae::GameObject>();
	uiScore->AddComponent(std::make_unique<dae::TextComponent>(uiScore.get(), "score: 0", font));
	uiScore->SetLocalPosition(15, 280);

	auto livesDisplay = std::make_unique<dae::RemainingLivesDisplay>(uiLives.get());
	auto scoreDisplay = std::make_unique<dae::ScoreDisplay>(uiScore.get());

	// player
	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "Q_Bert.png"));
	go->SetLocalPosition(100, 300);
	go->SetScale(3.f);
	go->AddComponent(std::make_unique<dae::MovementComponent>(go.get(), 100.f));
	go->AddComponent(std::make_unique<dae::HealthComponent>(go.get(), 3, 3));
	go->AddComponent(std::make_unique<dae::ScoreComponent>(go.get()));
	go->InitSubject();
	go->GetSubject()->AddObserver(livesDisplay.get());
	go->GetSubject()->AddObserver(scoreDisplay.get());

	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(0, -1, 0)), SDL_SCANCODE_W, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(0, 1, 0)), SDL_SCANCODE_S, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(-1, 0, 0)), SDL_SCANCODE_A, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(1, 0, 0)), SDL_SCANCODE_D, SDL_EVENT_KEY_DOWN);

	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::TakeDamageCommand>(go.get(), 3), SDL_SCANCODE_E, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::TurnTileCommand>(go.get(), 10), SDL_SCANCODE_R, SDL_EVENT_KEY_DOWN);

	scene.Add(std::move(go));

	scene.Add(std::move(uiLives));
	scene.Add(std::move(uiScore));
	scene.Add(std::move(livesDisplay));
	scene.Add(std::move(scoreDisplay));

	uiLives = std::make_unique<dae::GameObject>();
	uiLives->AddComponent(std::make_unique<dae::TextComponent>(uiLives.get(), "# lives: 3", font));
	uiLives->SetLocalPosition(15, 310);
	uiScore = std::make_unique<dae::GameObject>();
	uiScore->AddComponent(std::make_unique<dae::TextComponent>(uiScore.get(), "score: 0", font));
	uiScore->SetLocalPosition(15, 340);

	livesDisplay = std::make_unique<dae::RemainingLivesDisplay>(uiLives.get());
	scoreDisplay = std::make_unique<dae::ScoreDisplay>(uiScore.get());

	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "Q_Bert_Enemy.png"));
	go->SetLocalPosition(300, 300);
	go->SetScale(3.f);
	go->AddComponent(std::make_unique<dae::MovementComponent>(go.get(), 100.f));
	go->AddComponent(std::make_unique<dae::HealthComponent>(go.get(), 3, 3));
	go->AddComponent(std::make_unique<dae::ScoreComponent>(go.get()));
	go->InitSubject();
	go->GetSubject()->AddObserver(livesDisplay.get());
	go->GetSubject()->AddObserver(scoreDisplay.get());

#if defined(__EMSCRIPTEN__)
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(0, -1, 0)), SDL_SCANCODE_W, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(0, 1, 0)), SDL_SCANCODE_S, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(-1, 0, 0)), SDL_SCANCODE_A, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(1, 0, 0)), SDL_SCANCODE_D, SDL_EVENT_KEY_DOWN);

	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::TakeDamageCommand>(go.get(), 3), SDL_SCANCODE_Q, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::TurnTileCommand>(go.get(), 10), SDL_SCANCODE_T, SDL_EVENT_KEY_DOWN);

#else
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(0, -1, 0)), XINPUT_GAMEPAD_DPAD_UP, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(0, 1, 0)), XINPUT_GAMEPAD_DPAD_DOWN, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(-1, 0, 0)), XINPUT_GAMEPAD_DPAD_LEFT, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(1, 0, 0)), XINPUT_GAMEPAD_DPAD_RIGHT, 0);

	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::TakeDamageCommand>(go.get(), 3), XINPUT_GAMEPAD_A, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::TurnTileCommand>(go.get(), 10), XINPUT_GAMEPAD_B, 0);
#endif // !EMSCRIPTEN


	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), "Use WASD to move Q*Bert, Q to inflict damage, R to turn tiles", font));
	go->SetLocalPosition(15, 150);
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), "Use DPAD to move the green guy, E to inflict damage, T to turn tiles", font));
	go->SetLocalPosition(15, 200);
	scene.Add(std::move(go));

	scene.Add(std::move(uiLives));
	scene.Add(std::move(uiScore));
	scene.Add(std::move(livesDisplay));
	scene.Add(std::move(scoreDisplay));
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
