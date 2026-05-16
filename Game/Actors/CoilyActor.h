#ifndef COILYACTOR_H
#define COILYACTOR_H

#include "Component.h"
#include "CoilyStates.h"

namespace QBert
{
    class GameObject;
	class CoilyActorComp : public dae::Component
	{
    public:
        CoilyActorComp(dae::GameObject* pOwner, LevelBase* pLevel);
        ~CoilyActorComp() = default;
        CoilyActorComp(const CoilyActorComp& other) = delete;
        CoilyActorComp(CoilyActorComp&& other) = delete;
        CoilyActorComp& operator=(const CoilyActorComp& other) = delete;
        CoilyActorComp& operator=(CoilyActorComp&& other) = delete;

        void Update() override;

        std::type_index GetType() const override;

    private:
        std::unique_ptr<CoilyState> m_pState;
	};
}

#endif // !COILY_H