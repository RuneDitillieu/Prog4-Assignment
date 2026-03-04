#include "ImGuiRenderer.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include "imgui_plot.h"
#include <chrono>
#include <string>

dae::ImGuiRenderer::ImGuiRenderer(SDL_Window* window, SDL_Renderer* renderer)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
#if __EMSCRIPTEN__
	// For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
	// You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
	io.IniFilename = NULL;
#endif

	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);

	size_t amountSamples{ 100000 };
	m_dataInt.reserve(amountSamples);
	for (size_t idx{ 0 }; idx < m_dataInt.capacity(); ++idx)
	{
		m_dataInt.emplace_back(rand());
	}
	m_dataObj.reserve(amountSamples);
	for (size_t idx{ 0 }; idx < m_dataObj.capacity(); ++idx)
	{
		m_dataObj.emplace_back(GameObject3D());
	}
	m_dataObjAlt.reserve(amountSamples);
	for (size_t idx{ 0 }; idx < m_dataObj.capacity(); ++idx)
	{
		m_dataObjAlt.emplace_back(GameObject3DAlt());
	}
	m_results.reserve(11);
}

dae::ImGuiRenderer::~ImGuiRenderer()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

void dae::ImGuiRenderer::Render()
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	
	ImGui::Spacing();
	ImGui::SliderInt("Amount of Samples", &m_amountSamples, 3, 50);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("Thrash the Cash with integers");
	ImGui::Spacing();
	if(ImGui::Button("Calculate ex.1"))
		CalcExercise1();
	ImGui::Spacing();
	RenderPlot(m_avgResultsInt, 1);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Thrash the Cash with GameObjects");
	ImGui::Spacing();
	if (ImGui::Button("Calculate ex.2"))
		CalcExercise2(false);
	ImGui::Spacing();
	RenderPlot(m_avgResultsObj, 2);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Thrash the Cash with GameObjectAlts");
	ImGui::Spacing();
	if (ImGui::Button("Calculate ex.2.1"))
		CalcExercise2(true);
	ImGui::Spacing();
	RenderPlot(m_avgResultsObjAlt, 3);


	ImGui::Render();
}

void dae::ImGuiRenderer::RenderData(SDL_Renderer* renderer)
{
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
}

void dae::ImGuiRenderer::CalcExercise1()
{
	m_avgResultsInt.clear();
	m_results.clear();

	for (int loopNr{ 0 }; loopNr < m_amountSamples; ++loopNr)
	{
		std::vector<float> results{};
		results.reserve(m_amountSamples);

		for (size_t stepsize = 1; stepsize <= 1024; stepsize *= 2)
		{
			auto start = std::chrono::high_resolution_clock::now();

			for (size_t idx = 0; idx < m_dataInt.size(); idx += stepsize)
			{
				m_dataInt[idx] += 2;
			}

			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

			results.emplace_back(static_cast<float>(duration));
		}

		m_results.emplace_back(std::move(results));
	}

	GetAverageResults(m_avgResultsInt);
}

void dae::ImGuiRenderer::CalcExercise2(bool doAlt)
{
	if(doAlt)
		m_avgResultsObjAlt.clear();
	else
		m_avgResultsObj.clear();
	m_results.clear();

	for (int loopNr{ 0 }; loopNr < m_amountSamples; ++loopNr)
	{
		std::vector<float> results{};
		results.reserve(m_amountSamples);

		for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
		{
			auto start = std::chrono::high_resolution_clock::now();

			if (doAlt)
			{
				for (size_t idx = 0; idx < m_dataObjAlt.size(); idx += stepsize)
				{
					m_dataObjAlt[idx].ID += 2;
				}
			}
			else
			{
				for (size_t idx = 0; idx < m_dataObj.size(); idx += stepsize)
				{
					m_dataObj[idx].ID += 2;
				}
			}

			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

			results.emplace_back(static_cast<float>(duration));
		}

		m_results.emplace_back(std::move(results));
	}

	if (doAlt)
		GetAverageResults(m_avgResultsObjAlt);
	else
		GetAverageResults(m_avgResultsObj);
}

void dae::ImGuiRenderer::GetAverageResults(std::vector<float>& avgResultsVec)
{
	for (size_t idx{ 0 }; idx < 11; ++idx)
	{
		float avgResult{};
		float smallestResult{ m_results[0][idx] };
		float biggestResult{ };

		for (int loopNr{ 0 }; loopNr < m_amountSamples; ++loopNr)
		{
			float nr{ m_results[loopNr][idx] };
			avgResult += nr;
			if (nr > biggestResult)
				biggestResult = nr;
			if (nr < smallestResult)
				smallestResult = nr;
		}
		avgResult -= smallestResult;
		avgResult -= biggestResult;
		avgResult /= (m_amountSamples - 2);

		avgResultsVec.emplace_back(avgResult);
	}
}

void dae::ImGuiRenderer::RenderPlot(const std::vector<float>& results, int exerciseNr)
{
	if (results.size() > 0)
	{
		constexpr float xValues[]{ 1.f, 2.f, 4.f, 8.f, 16.f, 32.f, 64.f, 128.f, 256.f, 512.f, 1024.f };

		std::string name = "Avg results ex." + std::to_string(exerciseNr);
		ImGui::PlotConfig config{};
		config.overlay_text = name.c_str();
		config.values.xs = xValues;
		config.values.ys = results.data();
		config.values.count = static_cast<int>(results.size());
		switch (exerciseNr)
		{
		case 1:
			config.values.color = 0xFF'00'7F'FF;
			break;
		case 2:
			config.values.color = 0xFF'FF'7F'00;
			break;
		case 3:
			config.values.color = 0xFF'00'FF'7F;
			break;
		default:
			config.values.color = 0xFF'FF'FF'FF;
			break;
		}
		config.scale.min = 0;
		config.scale.max = *std::max_element(results.begin(), results.end());
		config.tooltip.show = true;
		config.tooltip.format = "x=%.0f, y=%.5f";
		config.grid_x.show = true;
		config.grid_y.show = true;
		config.frame_size = ImVec2(200, 100);
		config.line_thickness = 2.f;

		ImGui::Plot(config.overlay_text, config);
	}
}