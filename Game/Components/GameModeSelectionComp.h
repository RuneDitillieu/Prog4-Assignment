#ifndef GAMEMODESELECTIONCOMP_H
#define GAMEMODESELECTIONCOMP_H

#include "Component.h"
#include "IObserver.h"
#include "Events.h"

namespace dae
{
    class TextComponent;
}

namespace QBert
{
    class GameModeSelectionComp final : public dae::Component, public dae::IObserver
    {
    public:
        GameModeSelectionComp(dae::GameObject* pOwner, const std::vector<dae::TextComponent*>& pTextComps);
        ~GameModeSelectionComp() = default;
        GameModeSelectionComp(const GameModeSelectionComp& other) = delete;
        GameModeSelectionComp(GameModeSelectionComp&& other) = delete;
        GameModeSelectionComp& operator=(const GameModeSelectionComp& other) = delete;
        GameModeSelectionComp& operator=(GameModeSelectionComp&& other) = delete;

        void Notify(dae::Event event, dae::Subject*) override;

        std::type_index GetType() const override;

    private:
        void UpdateSelectionColors();

        uint16_t m_selectedGameMode{ 0 };
        uint16_t m_prevSelection{ 0 };
        std::vector<dae::TextComponent*> m_pConnTextComps;
    };
}

#endif //GAMEMODESELECTIONCOMP_H
