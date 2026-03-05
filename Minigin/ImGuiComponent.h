#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "Component.h"

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

	class GameObject;
	class Transform;
	class ImGuiComponent final : public Component
	{
	public:
		ImGuiComponent(GameObject* pOwner);
		~ImGuiComponent() = default;
		void Render(const Transform& transform) override;
		std::type_index GetType() const override;

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

		SDL_Renderer* m_sdlRenderer{ nullptr }; // non-owning
	};
}