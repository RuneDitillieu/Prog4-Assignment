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
	go->SetPosition(358, 180);
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), "Programming 4 Assignment", font));
	go->SetPosition(292, 20);
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), "0.0 FPS", font));
	go->SetPosition(10, 10);
	go->GetComponent<dae::TextComponent>()->AddFpsComponent();
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
