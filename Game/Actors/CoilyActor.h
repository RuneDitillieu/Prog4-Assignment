#ifndef COILYACTOR_H
#define COILYACTOR_H

#include "Component.h"
#include "CoilyStates.h"
#include "Events.h"
#include "IObserver.h"

namespace QBert
{
    class GameObject;
	class CoilyActorComp : public dae::Component, public dae::IObserver
	{
    public:
        CoilyActorComp(dae::GameObject* pOwner, LevelBase* pLevel);
        ~CoilyActorComp() = default;
        CoilyActorComp(const CoilyActorComp& other) = delete;
        CoilyActorComp(CoilyActorComp&& other) = delete;
        CoilyActorComp& operator=(const CoilyActorComp& other) = delete;
        CoilyActorComp& operator=(CoilyActorComp&& other) = delete;

        void Update() override;
        void Notify(dae::Event event, dae::Subject* subject) override;

        std::type_index GetType() const override;

    private:
        std::unique_ptr<CoilyState> m_pState;
	};
}

#endif // !COILY_H