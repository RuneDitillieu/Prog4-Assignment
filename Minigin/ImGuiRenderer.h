#pragma once
#include <vector>
#include <SDL3/SDL.h>

namespace dae
{
	struct DummyTransform
	{
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 };
	};

	class GameObject3D
	{
	public:
		DummyTransform transform;
		int ID{ 1 };
	};

	class GameObject3DAlt
	{
	public:
		DummyTransform* transform{ nullptr };
		int ID{ 1 };
	};

	class ImGuiRenderer final
	{
	public:
		ImGuiRenderer(SDL_Window* window, SDL_Renderer* renderer);
		~ImGuiRenderer();
		void Render();
		void RenderData(SDL_Renderer* renderer);

	private:
		void Calculate(int exerciseNr);
		void RenderPlot(const std::vector<float>& results, int exerciseNr);
		void GetAverageResults(std::vector<float>& avgResultsVec);

		int m_amountSamples{ 10 };
		const int m_amountOfData{ 100000 };
		std::vector<int> m_dataInt;
		std::vector<GameObject3D> m_dataObj;
		std::vector<GameObject3DAlt> m_dataObjAlt;
		std::vector<float> m_avgResultsInt;
		std::vector<float> m_avgResultsObj;
		std::vector<float> m_avgResultsObjAlt;
		std::vector<std::vector<float>> m_results;
	};
}