#ifndef LIVESDISPLAYCOMPONENT_H
#define LIVESDISPLAYCOMPONENT_H

#include "Component.h"
#include "IObserver.h"
#include "Events.h"
#include <vector>

namespace QBert
{
	class GameObject;
	class LivesDisplay final : public dae::Component, public dae::IObserver
	{
	public:
		explicit LivesDisplay(dae::GameObject* pOwner, std::vector<dae::GameObject*> livesP1, std::vector<dae::GameObject*> livesP2);
		~LivesDisplay();
		LivesDisplay(const LivesDisplay& other) = delete;
		LivesDisplay(LivesDisplay&& other) = delete;
		LivesDisplay& operator=(const LivesDisplay& other) = delete;
		LivesDisplay& operator=(LivesDisplay&& other) = delete;

		void Start() override;
		void Notify(dae::Event event, dae::Subject*) override;

		std::type_index GetType() const override;

	private:
		void UpdateUI(int amLives, const std::vector<dae::GameObject*>& livesVec) const;

		std::vector<dae::GameObject*> m_pLivesP1;
		dae::GameObject* m_player1{ nullptr };
		std::vector<dae::GameObject*> m_pLivesP2;
		dae::GameObject* m_player2{ nullptr };
	};
}

#endif // !LIVESDISPLAYCOMPONENT_H

