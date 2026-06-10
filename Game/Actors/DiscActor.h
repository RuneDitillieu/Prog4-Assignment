#ifndef DISCACTOR_H
#define DISCACTOR_H

#include "Component.h"
#include "IObserver.h"
#include "Events.h"
#include "DiscStates.h"
#include <memory>

namespace QBert
{
	class GameObject;
	class DiscActorComp final : public dae::Component, public dae::IObserver
	{
	public:
		DiscActorComp(dae::GameObject* pOwner, const glm::vec2& tile);
		~DiscActorComp();
		DiscActorComp(const DiscActorComp& other) = delete;
		DiscActorComp(DiscActorComp&& other) = delete;
		DiscActorComp& operator=(const DiscActorComp& other) = delete;
		DiscActorComp& operator=(DiscActorComp&& other) = delete;

		void Start() override;
		void Update() override;
		void Notify(dae::Event event, dae::Subject* subject) override;
		glm::vec2 GetTile() const { return m_tile; }

		std::type_index GetType() const override;

	private:
		glm::vec2 m_tile;
		std::unique_ptr<DiscState> m_pState;
	};
}

#endif // DISCACTOR_H

