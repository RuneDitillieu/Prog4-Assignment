#ifndef UGGWRONGWAYACTOR_H
#define UGGWRONGWAYACTOR_H

#include "Component.h"
#include "Events.h"
#include "IObserver.h"
#include "Subject.h"

namespace QBert
{
    class UggWrongwayState;
    class QBertMoveComp;
    class LevelBase;

    class UggWrongwayActor final : public dae::Component, public dae::IObserver
    {
    public:
        explicit UggWrongwayActor(dae::GameObject* pOwner, LevelBase* pLevel, const std::vector<QBertMoveComp*>& qbertMoves, bool goesRight);
        ~UggWrongwayActor();
        UggWrongwayActor(const UggWrongwayActor& other) = delete;
        UggWrongwayActor(UggWrongwayActor&& other) = delete;
        UggWrongwayActor& operator=(const UggWrongwayActor& other) = delete;
        UggWrongwayActor& operator=(UggWrongwayActor&& other) = delete;

        void Start() override;
        void Update() override;
        void Notify(dae::Event event, dae::Subject* subject) override;

        std::type_index GetType() const override;

    private:
		std::unique_ptr<UggWrongwayState> m_pState{ nullptr };

    };
}

#endif //UGGWRONGWAYACTOR_H
