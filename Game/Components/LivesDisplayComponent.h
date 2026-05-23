#ifndef LIVESDISPLAYCOMPONENT_H
#define LIVESDISPLAYCOMPONENT_H

#include "Component.h"
#include "IObserver.h"
#include "Events.h"
#include <vector>

namespace QBert
{
	class GameObject;
	class LivesDisplay : public dae::Component, public dae::IObserver
	{
	public:
		LivesDisplay(dae::GameObject* pOwner, std::vector<dae::GameObject*> lives);
		~LivesDisplay();
		LivesDisplay(const LivesDisplay& other) = delete;
		LivesDisplay(LivesDisplay&& other) = delete;
		LivesDisplay& operator=(const LivesDisplay& other) = delete;
		LivesDisplay& operator=(LivesDisplay&& other) = delete;

		void Start() override;
		void Notify(dae::Event event, dae::Subject*) override;

		std::type_index GetType() const override;

	private:
		std::vector<dae::GameObject*> m_pLives;
	};
}

#endif // !LIVESDISPLAYCOMPONENT_H

