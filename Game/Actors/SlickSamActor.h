#ifndef SLICKSAMACTOR_H
#define SLICKSAMACTOR_H

#include <memory>

#include "Component.h"
#include "IObserver.h"
#include "SlickSamStates.h"

namespace QBert
{
	class QBertMoveComp;
	class LevelBase;

	class SlickSamActorComp final : public dae::Component, public dae::IObserver
	{
	public:
		explicit SlickSamActorComp(dae::GameObject* pOwner, LevelBase* pLevel, const std::vector<QBertMoveComp*>& qbertMoves);
		~SlickSamActorComp();
		SlickSamActorComp(const SlickSamActorComp& other) = delete;
		SlickSamActorComp(SlickSamActorComp&& other) = delete;
		SlickSamActorComp& operator=(const SlickSamActorComp& other) = delete;
		SlickSamActorComp& operator=(SlickSamActorComp&& other) = delete;

		void Start() override;
		void Update() override;
		void Notify(dae::Event event, dae::Subject* subject) override;

		std::type_index GetType() const override;

	private:
		std::unique_ptr<SlickSamState> m_pState{ nullptr };
	};
}

#endif // !SLICKSAMACTOR_H

