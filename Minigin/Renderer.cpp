#include <stdexcept>
#include <cstring>
#include <iostream>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "imgui.h"
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include "imgui_plot.h"
#include <chrono>

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;

	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

#if defined(__EMSCRIPTEN__)
	m_renderer = SDL_CreateRenderer(window, nullptr);
#else
	m_renderer = SDL_CreateRenderer(window, nullptr);
#endif

	if (m_renderer == nullptr)
	{
		std::cout << "Failed to create the renderer: " << SDL_GetError() << "\n";
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGuiIO& io = ImGui::GetIO(); (void)io;
//	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//#if __EMSCRIPTEN__
//	// For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
//	// You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
//	io.IniFilename = NULL;
//#endif
//
//	ImGui_ImplSDL3_InitForSDLRenderer(window, m_renderer);
//	ImGui_ImplSDLRenderer3_Init(m_renderer);

	m_ImGuiRenderer = new ImGuiRenderer(window, m_renderer);
}

void dae::Renderer::Render() const
{
	//ImGui_ImplSDLRenderer3_NewFrame();
	//ImGui_ImplSDL3_NewFrame();
	//ImGui::NewFrame();

	////ImGui::ShowDemoWindow(); // For demonstration purposes, do not keep this in your engine

	//RenderExercise1();

	//ImGui::Render();
	m_ImGuiRenderer->Render();

	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	//ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);
	m_ImGuiRenderer->RenderData(m_renderer);

	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	/*ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();*/

	delete m_ImGuiRenderer;

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);
	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	dst.w = width;
	dst.h = height;
	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }

void dae::Renderer::RenderExercise1() const
{
	std::vector<int> vec{ };
	for (size_t idx{ 0 }; idx < 1000000; ++idx)
	{
		vec.emplace_back(rand());
	}

	std::vector<float> results{};
	for (size_t stepsize = 1; stepsize <= 1024; stepsize *= 2)
	{
		auto start = std::chrono::high_resolution_clock::now();

		for (size_t idx = 0; idx < vec.size(); idx += stepsize)
		{
			vec[idx] *= 2;
		}

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

		results.emplace_back(static_cast<float>(duration));
	}

	// plotting
	constexpr float xValues[]{ 1.f, 2.f, 4.f, 8.f, 16.f, 32.f, 64.f, 128.f, 256.f, 512.f, 1024.f };

	ImGui::PlotConfig config{};
	config.values.xs = xValues;
	config.values.ys = results.data();
	config.values.count = static_cast<int>(results.size());
	config.values.color = 0xFF'00'7F'FF;
	config.scale.min = 0;
	config.scale.max = *std::max_element(results.begin(), results.end());
	config.tooltip.show = true;
	config.tooltip.format = "x=%.0f, y=%.5f";
	config.grid_x.show = true;
	config.grid_y.show = true;
	config.frame_size = ImVec2(200, 100);
	config.line_thickness = 2.f;

	ImGui::Plot("results", config);
}
