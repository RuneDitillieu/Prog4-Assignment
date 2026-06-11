#include "GameModeSelectionComp.h"

#include "SceneManager.h"
#include "Subject.h"
#include "Tags.h"
#include "TextComponent.h"

QBert::GameModeSelectionComp::GameModeSelectionComp(dae::GameObject* pOwner, const std::vector<dae::TextComponent*>& pTextComps)
    : Component(pOwner)
    , m_pConnTextComps(pTextComps)
{ }

void QBert::GameModeSelectionComp::Notify(dae::Event event, dae::Subject*)
{
    switch (event.id)
    {
    case dae::make_sdbm_hash("OnPreviousPressed"):
        if (m_selectedGameMode == 0)
        {
            m_selectedGameMode = static_cast<uint16_t>(m_pConnTextComps.size() -1);
        }
        else
        {
            --m_selectedGameMode;
        }
        UpdateSelectionColors();
        break;

    case dae::make_sdbm_hash("OnNextPressed"):
        ++m_selectedGameMode;
        m_selectedGameMode %= m_pConnTextComps.size();
        UpdateSelectionColors();
        break;

    case dae::make_sdbm_hash("OnConfirmPressed"):
        SceneName sceneName = static_cast<SceneName>(m_selectedGameMode + 1);
        dae::SceneManager::GetInstance().SetActiveScene(dae::SceneName(sceneName));
        break;
    }
}

void QBert::GameModeSelectionComp::UpdateSelectionColors()
{
    m_pConnTextComps[m_prevSelection]->SetColor(SDL_Color(77, 206, 77, 255));
    m_pConnTextComps[m_selectedGameMode]->SetColor(SDL_Color(255, 120, 0, 255));

    m_prevSelection = m_selectedGameMode;
}

std::type_index QBert::GameModeSelectionComp::GetType() const
{
    return typeid(GameModeSelectionComp);
}
