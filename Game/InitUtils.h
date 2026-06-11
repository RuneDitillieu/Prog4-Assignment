#ifndef INITUTILS_H
#define INITUTILS_H

#include "SpriteComponent.h"
#include "Scene.h"
#include "LevelBase.h"
#include "QBertMoveComponent.h"

class GameObject;
//class LevelBase;
//class QBertMoveComp;

namespace QBert::Utils
{
	dae::GameObject* CreateLevel(dae::Scene& scene, int tileType, bool revertableTiles,
		int startTile, int winTile, int intermediateTile = -1);

	std::unique_ptr<dae::GameObject> CreateNumberComp(float x, float y, bool enabled, std::vector<dae::SpriteComp*>& numberSprites);

	std::unique_ptr<dae::GameObject> CreateLifeComp(float y, bool enabled, std::vector<dae::GameObject*>& lives);

	void CreateUi(dae::Scene& scene);

	void AddSounds();

	std::unique_ptr<dae::GameObject> CreatePlayer(LevelBase* level);

	std::unique_ptr<dae::GameObject> CreateCoily(LevelBase* level, QBertMoveComp* playerMove);

	std::unique_ptr<dae::GameObject> CreateDisc(const glm::vec2& tile, LevelBase* level);

	std::unique_ptr<dae::GameObject> CreateSlick(LevelBase* level, QBertMoveComp* playerMove);

	std::unique_ptr<dae::GameObject> CreateSam(LevelBase* level, QBertMoveComp* playerMove);

	std::unique_ptr<dae::GameObject> CreateSlickSam(LevelBase* level, QBertMoveComp* playerMove);

	std::unique_ptr<dae::GameObject> CreateUgg(LevelBase* level, QBertMoveComp* playerMove);

	std::unique_ptr<dae::GameObject> CreateWrongway(LevelBase* level, QBertMoveComp* playerMove);

	std::unique_ptr<dae::GameObject> CreateUggWrongway(LevelBase* level, QBertMoveComp* playerMove);

	void CreateSelectionScreenUI(dae::Scene& scene);
}

#endif // !INITUTILS_H