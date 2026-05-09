#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "LevelBase.h"
#include "SpriteComponent.h"
#include "RenderComponent.h"

namespace QBert::Utils
{
	void CreateLevel(dae::Scene& scene, int tileType, bool revertableTiles, int startTile, int winTile, int intermediateTile = -1)
	{
		auto go = std::make_unique<dae::GameObject>();
		go->SetLocalPosition(350.f, 150.f);
		go->SetScale(3.f);
		go->AddComponent(std::make_unique<QBert::LevelBase>(go.get(), tileType, revertableTiles, startTile, winTile, intermediateTile));
		scene.Add(std::move(go));
	}

	void CreateUi(dae::Scene& scene)
	{
		// topleft

		auto playerText = std::make_unique<dae::GameObject>();
		playerText->SetLocalPosition(20.f, 80.f);
		playerText->SetScale(3.f);
		auto rc = playerText->AddComponent(std::make_unique<dae::RenderComponent>(playerText.get(), "PlayerTextSprites.png"));
		playerText->AddComponent(std::make_unique<dae::SpriteComp>(playerText.get(), rc, "PlayerTextSprites.png", 
			1, 6, rc->GetSize().x / 2.f, rc->GetSize().y / 6.f, glm::vec2(rc->GetSize().x / 2.f, 0)));

		auto playerId = std::make_unique<dae::GameObject>();
		playerId->SetLocalPosition(rc->GetSize().x / 2.f * 3.f + 20.f, -8.f);
		playerId->SetScale(3.f);
		rc = playerId->AddComponent(std::make_unique<dae::RenderComponent>(playerId.get(), "LevelUi.png"));
		playerId->AddComponent(std::make_unique<dae::SpriteComp>(playerId.get(), rc, "LevelUi.png", 
			2, 1, rc->GetSize().x / 5.f, rc->GetSize().y / 3.f, glm::vec2(0, 0), false));
		playerId.release()->SetParent(playerText.get(), false);

		auto changeToText = std::make_unique<dae::GameObject>();
		changeToText->SetLocalPosition(3.f, 70.f);
		changeToText->SetScale(3.f);
		rc = changeToText->AddComponent(std::make_unique<dae::RenderComponent>(changeToText.get(), "LevelUi.png"));
		changeToText->AddComponent(std::make_unique<dae::SpriteComp>(changeToText.get(), rc, "LevelUi.png",
			1, 1, rc->GetSize().x, rc->GetSize().y / 5.f, glm::vec2(0, rc->GetSize().y / 5.f * 4.f), false));
		changeToText.release()->SetParent(playerText.get(), false);

		scene.Add(std::move(playerText));


		// topright

		auto levelText = std::make_unique<dae::GameObject>();
		levelText->SetLocalPosition(580.f, 150.f);
		levelText->SetScale(3.f);
		rc = levelText->AddComponent(std::make_unique<dae::RenderComponent>(levelText.get(), "LevelUi.png"));
		levelText->AddComponent(std::make_unique<dae::SpriteComp>(levelText.get(), rc, "LevelUi.png",
			1, 1, rc->GetSize().x / 3.f * 2.f, rc->GetSize().y / 5.f, glm::vec2(0, rc->GetSize().y / 5.f * 2.f), false));

		auto roundText = std::make_unique<dae::GameObject>();
		roundText->SetLocalPosition(0, 30.f);
		roundText->SetScale(3.f);
		rc = roundText->AddComponent(std::make_unique<dae::RenderComponent>(roundText.get(), "LevelUi.png"));
		roundText->AddComponent(std::make_unique<dae::SpriteComp>(roundText.get(), rc, "LevelUi.png",
			1, 1, rc->GetSize().x / 3.f * 2.f, rc->GetSize().y / 5.f, glm::vec2(0, rc->GetSize().y / 5.f * 3.f), false));
		roundText.release()->SetParent(levelText.get(), false);

		scene.Add(std::move(levelText));
	}
}