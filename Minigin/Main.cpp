#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "FpsComponent.h"
#include "RotatorComponent.h"
#include "Scene.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "background.png"));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "logo.png"));
	go->SetLocalPosition(358, 180);
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), "Programming 4 Assignment", font));
	go->SetLocalPosition(292, 20);
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->SetLocalPosition(10, 10);
	go->AddComponent(std::make_unique<dae::FpsComponent>(go.get(), "0.0 FPS", font));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "Q_Bert.png"));
	go->SetLocalPosition(200, 300);
	go->SetScale(3.f);
	go->AddComponent(std::make_unique<dae::RotatorComponent>(go.get(), glm::vec3{ 200.f, 280.f, 0.f }, 2.f));

	auto child = std::make_unique<dae::GameObject>();
	child->AddComponent(std::make_unique<dae::RenderComponent>(child.get(), "Q_Bert_Enemy.png"));
	child->SetLocalPosition(50, 0);
	child->SetScale(3.f);
	child->AddComponent(std::make_unique<dae::RotatorComponent>(child.get(), glm::vec3{ 0.f, 0.f, 0.f }, -2.f));
	child->SetParent(go.get(), false);
	scene.Add(std::move(go));
	scene.Add(std::move(child));
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
