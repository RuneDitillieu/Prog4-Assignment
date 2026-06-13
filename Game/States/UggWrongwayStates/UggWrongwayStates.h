#ifndef UGGWRONGWAYSTATES_H
#define UGGWRONGWAYSTATES_H

#include "GameObject.h"
#include "SpriteComponent.h"

namespace QBert
{
    class QBertMoveComp;
    class LevelBase;

    // base state
    class UggWrongwayState
    {
    public:
        explicit UggWrongwayState(dae::GameObject* uggWrongway, dae::SpriteComp* spriteComp,
            QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps, bool goesRight);
        virtual ~UggWrongwayState() = default;
        virtual void OnEnter() {};
        virtual void OnExit() {};
        virtual std::unique_ptr<UggWrongwayState> Update();
        virtual std::unique_ptr<UggWrongwayState> OnNotify(dae::Event, dae::Subject*);

        bool m_goesRight;
    protected:
        dae::GameObject* m_uggWrongway;
        dae::SpriteComp* m_pConnSprite;
        QBertMoveComp* m_pMoveComp;
        LevelBase* m_pConnLevel;
        std::vector<QBertMoveComp*> m_pQBertMoveComps;
    };

    class IdleUggWrongwayState final : public UggWrongwayState
    {
    public:
        explicit IdleUggWrongwayState(dae::GameObject* uggWrongway, dae::SpriteComp* spriteComp,
            QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps, bool goesRight);

        void OnEnter() override;
        std::unique_ptr<UggWrongwayState> Update() override;

    private:
        float m_secPassed{ 0.f };
        const float m_idleSec{ 0.5f };
    };

    class JumpingUggWrongwayState final : public UggWrongwayState
    {
    public:
        explicit JumpingUggWrongwayState(dae::GameObject* uggWrongway, dae::SpriteComp* spriteComp,
            QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps, bool goesRight);

        void OnEnter() override;
        std::unique_ptr<UggWrongwayState> Update() override;
        std::unique_ptr<UggWrongwayState> OnNotify(dae::Event, dae::Subject*) override;
    };

    class FallingUggWrongwayState final : public UggWrongwayState
    {
    public:
        explicit FallingUggWrongwayState(dae::GameObject* uggWrongway, dae::SpriteComp* spriteComp,
            QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps, bool goesRight);

        std::unique_ptr<UggWrongwayState> Update() override;
    };

    class StunnedUggWrongwayState final : public UggWrongwayState
    {
    public:
        explicit StunnedUggWrongwayState(dae::GameObject* uggWrongway, dae::SpriteComp* spriteComp,
            QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps, bool goesRight);

        void OnEnter() override;
        std::unique_ptr<UggWrongwayState> Update() override;

    private:
        float m_secPassed{ 0.f };
        const float m_maxSec{ 1.5f };
    };
}

#endif //UGGWRONGWAYSTATES_H
