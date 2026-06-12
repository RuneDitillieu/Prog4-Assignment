#include "GameModeSelectionComp.h"

#include "SceneManager.h"
#include "Tags.h"
#include "TextComponent.h"
#include "Colors.h"
#include "DeltaTime.h"

QBert::GameModeSelectionComp::GameModeSelectionComp(dae::GameObject* pOwner, const std::vector<dae::TextComponent*>& pTextComps)
    : Component(pOwner)
    , m_pConnTextComps(pTextComps)
{ }

void QBert::GameModeSelectionComp::Update()
{
    m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();
}

void QBert::GameModeSelectionComp::Notify(dae::Event event, dae::Subject*)
{
    if (m_secPassed < 0.2f) return;

    switch (event.id)
    {
    case dae::make_sdbm_hash("OnPreviousPressed"):
        // update selection
        if (m_selectedGameMode == 0)
        {
            m_selectedGameMode = static_cast<uint16_t>(m_pConnTextComps.size() -1);
        }
        else
        {
            --m_selectedGameMode;
        }

        UpdateSelectionColors();
        m_secPassed = 0.f;
        break;

    case dae::make_sdbm_hash("OnNextPressed"):
        // update selection
        ++m_selectedGameMode;
        m_selectedGameMode %= m_pConnTextComps.size();

        UpdateSelectionColors();
        m_secPassed = 0.f;
        break;

    case dae::make_sdbm_hash("OnConfirmPressed"):
        // load scene
        SceneName sceneName = static_cast<SceneName>(m_selectedGameMode + 1);
        dae::SceneManager::GetInstance().SetActiveScene(dae::SceneName(sceneName));
        m_secPassed = 0.f;
        break;
    }
}

void QBert::GameModeSelectionComp::UpdateSelectionColors()
{
    m_pConnTextComps[m_prevSelection]->SetColor(QBert::GREEN);
    m_pConnTextComps[m_selectedGameMode]->SetColor(QBert::ORANGE);

    m_prevSelection = m_selectedGameMode;
}

std::type_index QBert::GameModeSelectionComp::GetType() const
{
    return typeid(GameModeSelectionComp);
}
