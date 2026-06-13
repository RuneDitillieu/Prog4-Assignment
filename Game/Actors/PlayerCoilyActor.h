#ifndef MINIGIN_PLAYERCOILYACTOR_H
#define MINIGIN_PLAYERCOILYACTOR_H

#include "Component.h"
#include "IObserver.h"
#include "PlayerCoilyStates.h"

namespace QBert
{
    class QBertMoveComp;
    class LevelBase;

    class PlayerCoilyActorComp : public dae::Component, public dae::IObserver
    {
    public:
        explicit PlayerCoilyActorComp(dae::GameObject* pOwner, LevelBase* pLevel, QBertMoveComp* qbertMoveComp);
        ~PlayerCoilyActorComp();
        PlayerCoilyActorComp(const PlayerCoilyActorComp& other) = delete;
        PlayerCoilyActorComp(PlayerCoilyActorComp&& other) = delete;
        PlayerCoilyActorComp& operator=(const PlayerCoilyActorComp& other) = delete;
        PlayerCoilyActorComp& operator=(PlayerCoilyActorComp&& other) = delete;

        void Start() override;
        void Update() override;
        void Notify(dae::Event event, dae::Subject* subject) override;

        std::type_index GetType() const override;

    private:
        std::unique_ptr<PlayerCoilyState> m_pState;
    };
}

#endif //MINIGIN_PLAYERCOILYACTOR_H
