#include "ImGuiComponent.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include "ImGuiPlot.h"
#include <chrono>
#include <string>
#include "Transform.h"
#include "Renderer.h"

dae::ImGuiComponent::ImGuiComponent(GameObject* pOwner)
	: Component(pOwner)
{
	m_sdlRenderer = Renderer::GetInstance().GetSDLRenderer();

	m_dataInt.reserve(m_amountOfData);
	for (size_t idx{ 0 }; idx < m_dataInt.capacity(); ++idx)
	{
		m_dataInt.emplace_back(rand());
	}
	m_dataObj.reserve(m_amountOfData);
	for (size_t idx{ 0 }; idx < m_dataObj.capacity(); ++idx)
	{
		m_dataObj.emplace_back(GameObject3D());
	}
	m_dataObjAlt.reserve(m_amountOfData);
	for (size_t idx{ 0 }; idx < m_dataObjAlt.capacity(); ++idx)
	{
		m_dataObjAlt.emplace_back(GameObject3DAlt());
	}
	m_results.reserve(11);
}

void dae::ImGuiComponent::Render(const Transform& transform)
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	
	// exercise 1
	ImGui::SetNextWindowPos(ImVec2(transform.GetPosition().x, transform.GetPosition().y)); 
	ImGui::SetNextWindowSize(ImVec2(300, 250));
	ImGui::Begin("Exercise 1");

	ImGui::Spacing();

	ImGui::SliderInt(" # Samples", &m_amountSamples, 3, 50);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Thrash the Cash with integers");

	ImGui::Spacing();

	if (ImGui::Button("Calculate ex.1"))
		Calculate(1);

	ImGui::Spacing();

	RenderPlot(m_avgResultsInt, 1);
	ImGui::End();


	// exercise 2
	ImGui::SetNextWindowPos(ImVec2(transform.GetPosition().x + 320, transform.GetPosition().y)); 
	ImGui::SetNextWindowSize(ImVec2(300, 400));
	ImGui::Begin("Exercise 2");

	ImGui::Spacing();

	ImGui::SliderInt(" # Samples", &m_amountSamples, 3, 50);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Thrash the Cash with GameObjects");

	ImGui::Spacing();

	if (ImGui::Button("Calculate ex.2"))
		Calculate(2);

	ImGui::Spacing();

	RenderPlot(m_avgResultsObj, 2);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Thrash the Cash with GameObjectAlts");

	ImGui::Spacing();

	if (ImGui::Button("Calculate ex.2.1"))
		Calculate(3);

	ImGui::Spacing();

	RenderPlot(m_avgResultsObjAlt, 3);
	ImGui::End();

	ImGui::Render();

	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_sdlRenderer);
}

void dae::ImGuiComponent::Calculate(int exerciseNr)
{
	switch (exerciseNr)
	{
	case 1:
		m_avgResultsInt.clear();
		break;
	case 2:
		m_avgResultsObj.clear();
		break;
	case 3:
		m_avgResultsObjAlt.clear();
		break;
	default:
		return;
	}

	m_results.clear();

	for (int loopNr{ 0 }; loopNr < m_amountSamples; ++loopNr)
	{
		std::vector<float> results{};
		results.reserve(m_amountSamples);

		for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
		{
			auto start = std::chrono::high_resolution_clock::now();

			for (int idx = 0; idx < m_amountOfData; idx += stepsize)
			{
				switch (exerciseNr)
				{
				case 1:
					m_dataInt[idx] += 2;
					break;
				case 2:
					m_dataObj[idx].ID += 2;
					break;
				case 3:
					m_dataObjAlt[idx].ID += 2;
					break;
				}
			}

			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

			results.emplace_back(static_cast<float>(duration));
		}

		m_results.emplace_back(std::move(results));
	}

	switch (exerciseNr)
	{
	case 1:
		GetAverageResults(m_avgResultsInt);
		break;
	case 2:
		GetAverageResults(m_avgResultsObj);
		break;
	case 3:
		GetAverageResults(m_avgResultsObjAlt);
		break;
	}
}

void dae::ImGuiComponent::GetAverageResults(std::vector<float>& avgResultsVec)
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

void dae::ImGuiComponent::RenderPlot(const std::vector<float>& results, int exerciseNr)
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

std::type_index dae::ImGuiComponent::GetType() const
{
	return typeid(ImGuiComponent);
}