#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "Component.h"
#include "IObserver.h"
#include "LevelBase.h"
#include "TileComponent.h"

namespace dae
{
	class TextComponent;
}

namespace QBert
{
	class QBertMoveComp;

	struct LevelParams
	{
		std::vector<float> coilySpawns{};
		std::vector<glm::vec2> discSpawns{};
		std::vector<float> uggWrongwaySpawns{};
		std::vector<float> samSlickSpawns{};

		TileParams tileParams;
	};

	class LevelManager final : public dae::Component, public dae::IObserver
	{
	public:
		LevelManager(dae::GameObject* pOwner, LevelBase* level, dae::SpriteComp* tileIconSprite,
			dae::TextComponent* levelNrText, dae::TextComponent* roundNrText);
		~LevelManager();
		LevelManager(const LevelManager& other) = delete;
		LevelManager(LevelManager&& other) = delete;
		LevelManager& operator=(const LevelManager& other) = delete;
		LevelManager& operator=(LevelManager&& other) = delete;

		void Start() override;
		void Update() override;
		void Notify(dae::Event event, dae::Subject*) override;

		int GetCurrentRound() const { return m_currentRound; }
		int GetCurrentLevel() const { return m_currentLevel; }

		void GoToNextLevel();

		std::type_index GetType() const override;

	private:
		void MarkAllCreaturesForRemoval() const;
		void LoadLevelParams();
		void FreezeCreatures() const;

		int m_curUggWrongwayIdx{ 0 };
		float m_curUggWrongwayThreshold{ 0.f };
		int m_currentRound{ 0 };
		int m_currentLevel{ 0 };
		int m_curLevelParams{ 0 };
		float m_secPassed{ 0.f };
		bool m_doingLevelTransition{ false };

		LevelBase* m_pConnLevel;
		dae::SpriteComp* m_pWinTileIndicator;
		dae::TextComponent* m_pLevelNrText;
		dae::TextComponent* m_pRoundNrText;
		std::vector<LevelParams> m_levelParams{};
		std::vector<QBertMoveComp*> m_pPlayersMove{};
	};
}

#endif // !LEVELMANAGERCOMPONENT_H
