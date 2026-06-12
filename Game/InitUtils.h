#ifndef INITUTILS_H
#define INITUTILS_H

#include "SpriteComponent.h"
#include "Scene.h"
#include "LevelBase.h"
#include "QBertMoveComponent.h"
#include "TextComponent.h"

class GameObject;
//class LevelBase;
//class QBertMoveComp;

namespace QBert::Utils
{
	dae::GameObject* CreateLevel(dae::Scene& scene, dae::SpriteComp* tileIconSprite, int tileType, bool revertableTiles,
		int startTile, int winTile, int intermediateTile = -1);

	std::unique_ptr<dae::GameObject> CreateNumberComp(float x, float y, bool enabled, std::vector<dae::SpriteComp*>& numberSprites);

	std::unique_ptr<dae::GameObject> CreateLifeComp(float y, bool enabled, std::vector<dae::GameObject*>& lives);

	dae::SpriteComp* CreateUi(dae::Scene& scene, dae::TextComponent* player2ScoreText = nullptr, std::vector<dae::GameObject*> p2Lives = std::vector<dae::GameObject*>{});
	dae::SpriteComp* CreateCoopUi(dae::Scene& scene);

	void AddSounds();

	std::unique_ptr<dae::GameObject> CreatePlayer(LevelBase* level, const glm::vec2& startPos);
	std::unique_ptr<dae::GameObject> AddKeyboardBindings(std::unique_ptr<dae::GameObject> player);
	std::unique_ptr<dae::GameObject> AddController1Bindings(std::unique_ptr<dae::GameObject> player);
	std::unique_ptr<dae::GameObject> AddController2Bindings(std::unique_ptr<dae::GameObject> player);

	std::unique_ptr<dae::GameObject> CreateCoily(LevelBase* level, const std::vector<QBertMoveComp*>& playerMoves);

	std::unique_ptr<dae::GameObject> CreateDisc(const glm::vec2& tile, LevelBase* level);

	std::unique_ptr<dae::GameObject> CreateSlick(LevelBase* level, const std::vector<QBertMoveComp*>& playersMoves);

	std::unique_ptr<dae::GameObject> CreateSam(LevelBase* level, const std::vector<QBertMoveComp*>& playersMoves);

	std::unique_ptr<dae::GameObject> CreateSlickSam(LevelBase* level, const std::vector<QBertMoveComp*>& playersMoves);

	std::unique_ptr<dae::GameObject> CreateUgg(LevelBase* level, const std::vector<QBertMoveComp*>& playersMoves);

	std::unique_ptr<dae::GameObject> CreateWrongway(LevelBase* level, const std::vector<QBertMoveComp*>& playersMoves);

	std::unique_ptr<dae::GameObject> CreateUggWrongway(LevelBase* level, const std::vector<QBertMoveComp*>& playersMoves);

	void CreateSelectionScreenUI(dae::Scene& scene);

	void CreateHighscoreScreenUI(dae::Scene& scene);
}

#endif // !INITUTILS_H