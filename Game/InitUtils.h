#ifndef INITUTILS_H
#define INITUTILS_H

#include "GameObject.h"
#include "RenderComponent.h"
#include "Scene.h"
#include "SpriteComponent.h"
#include "ServiceLocator.h"

#include "LevelBase.h"
#include "Sounds.h"
#include "QBertScoreComponent.h"

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

	std::unique_ptr<dae::GameObject> CreateNumberComp(float x, float y, bool enabled, std::vector<dae::SpriteComp*>& numberSprites)
	{
		auto score = std::make_unique<dae::GameObject>();
		score->SetLocalPosition(x, y);
		score->SetScale(3.f);
		auto rc = score->AddComponent(std::make_unique<dae::RenderComponent>(score.get(), "Fonts.png"));
		numberSprites.emplace_back(
		score->AddComponent(std::make_unique<dae::SpriteComp>(score.get(), rc, "Fonts.png",
			10, 1, rc->GetSize().x / 26.f, rc->GetSize().y / 8.f, glm::vec2(0, 0), false)));

		score->m_isEnabled = enabled;

		return score;
	}

	void CreateUi(dae::Scene& scene, dae::GameObject* player1)
	{
		// TOP LEFT
		
		// animated text "Player"
		auto playerText = std::make_unique<dae::GameObject>();
		playerText->SetLocalPosition(20.f, 80.f);
		playerText->SetScale(3.f);
		auto rc = playerText->AddComponent(std::make_unique<dae::RenderComponent>(playerText.get(), "PlayerTextSprites.png"));
		playerText->AddComponent(std::make_unique<dae::SpriteComp>(playerText.get(), rc, "PlayerTextSprites.png", 
			1, 6, rc->GetSize().x / 2.f, rc->GetSize().y / 6.f, glm::vec2(rc->GetSize().x / 2.f, 0)));

		// rect with player id
		auto playerId = std::make_unique<dae::GameObject>();
		playerId->SetLocalPosition(rc->GetSize().x / 2.f * 3.f + 20.f, -8.f);
		playerId->SetScale(3.f);
		rc = playerId->AddComponent(std::make_unique<dae::RenderComponent>(playerId.get(), "LevelUi.png"));
		playerId->AddComponent(std::make_unique<dae::SpriteComp>(playerId.get(), rc, "LevelUi.png", 
			2, 1, rc->GetSize().x / 5.f, rc->GetSize().y / 3.f, glm::vec2(0, 0), false));
		playerId.release()->SetParent(playerText.get(), false);

		std::vector<dae::SpriteComp*> numberSprites{};
		numberSprites.reserve(5);
		// score numbers
		CreateNumberComp(8.f, 25.f, true, numberSprites).release()->SetParent(playerText.get(), false);
		CreateNumberComp(28.f, 25.f, false, numberSprites).release()->SetParent(playerText.get(), false);
		CreateNumberComp(48.f, 25.f, false, numberSprites).release()->SetParent(playerText.get(), false);
		CreateNumberComp(68.f, 25.f, false, numberSprites).release()->SetParent(playerText.get(), false);
		CreateNumberComp(88.f, 25.f, false, numberSprites).release()->SetParent(playerText.get(), false);
		auto scoreComp = playerText->AddComponent(std::make_unique<QBert::ScoreComp>(playerText.get(), numberSprites));

		player1->GetSubject()->AddObserver(scoreComp);

		// text "Change to:"
		auto changeToText = std::make_unique<dae::GameObject>();
		changeToText->SetLocalPosition(3.f, 75.f);
		changeToText->SetScale(3.f);
		rc = changeToText->AddComponent(std::make_unique<dae::RenderComponent>(changeToText.get(), "LevelUi.png"));
		changeToText->AddComponent(std::make_unique<dae::SpriteComp>(changeToText.get(), rc, "LevelUi.png",
			1, 1, rc->GetSize().x, rc->GetSize().y / 5.f, glm::vec2(0, rc->GetSize().y / 5.f * 4.f), false));

		// pink arrows
		auto leftLeftArrow = std::make_unique<dae::GameObject>();
		leftLeftArrow->SetLocalPosition(0.f, 30.f);
		leftLeftArrow->SetScale(3.f);
		rc = leftLeftArrow->AddComponent(std::make_unique<dae::RenderComponent>(leftLeftArrow.get(), "LevelUi.png"));
		leftLeftArrow->AddComponent(std::make_unique<dae::SpriteComp>(leftLeftArrow.get(), rc, "LevelUi.png",
			1, 1, rc->GetSize().x / 5.f, rc->GetSize().y / 3.f, glm::vec2(rc->GetSize().x / 2.f, 0), false));
		leftLeftArrow.release()->SetParent(changeToText.get(), false);

		auto leftArrow = std::make_unique<dae::GameObject>();
		leftArrow->SetLocalPosition(25.f, 30.f);
		leftArrow->SetScale(3.f);
		rc = leftArrow->AddComponent(std::make_unique<dae::RenderComponent>(leftArrow.get(), "LevelUi.png"));
		leftArrow->AddComponent(std::make_unique<dae::SpriteComp>(leftArrow.get(), rc, "LevelUi.png",
			1, 1, rc->GetSize().x / 5.f, rc->GetSize().y / 3.f, glm::vec2(rc->GetSize().x / 2.f, 0), false));
		leftArrow.release()->SetParent(changeToText.get(), false);

		auto rightArrow = std::make_unique<dae::GameObject>();
		rightArrow->SetLocalPosition(100.f, 30.f);
		rightArrow->SetScale(3.f);
		rc = rightArrow->AddComponent(std::make_unique<dae::RenderComponent>(rightArrow.get(), "LevelUi.png"));
		rightArrow->AddComponent(std::make_unique<dae::SpriteComp>(rightArrow.get(), rc, "LevelUi.png",
			1, 1, rc->GetSize().x / 5.f, rc->GetSize().y / 3.f, glm::vec2(rc->GetSize().x / 2.f + rc->GetSize().x / 5.f, 0), false));
		rightArrow.release()->SetParent(changeToText.get(), false);

		auto rightRightArrow = std::make_unique<dae::GameObject>();
		rightRightArrow->SetLocalPosition(125.f, 30.f);
		rightRightArrow->SetScale(3.f);
		rc = rightRightArrow->AddComponent(std::make_unique<dae::RenderComponent>(rightRightArrow.get(), "LevelUi.png"));
		rightRightArrow->AddComponent(std::make_unique<dae::SpriteComp>(rightRightArrow.get(), rc, "LevelUi.png",
			1, 1, rc->GetSize().x / 5.f, rc->GetSize().y / 3.f, glm::vec2(rc->GetSize().x / 2.f + rc->GetSize().x / 5.f, 0), false));
		rightRightArrow.release()->SetParent(changeToText.get(), false);

		// goal tilecolor icon
		auto tileIcon = std::make_unique<dae::GameObject>();
		tileIcon->SetLocalPosition(50.f, 20.f);
		tileIcon->SetScale(3.f);
		rc = tileIcon->AddComponent(std::make_unique<dae::RenderComponent>(tileIcon.get(), "GroundPieces.png"));
		tileIcon->AddComponent(std::make_unique<dae::SpriteComp>(tileIcon.get(), rc, "GroundPieces.png",
			1, 1, rc->GetSize().x / 45.f, rc->GetSize().y / 6.f, glm::vec2(rc->GetSize().x / 11.25f, rc->GetSize().y / 6.f), false));
		tileIcon.release()->SetParent(changeToText.get(), false);

		changeToText.release()->SetParent(playerText.get(), false);

		scene.Add(std::move(playerText));


		// TOP RIGHT

		// text "Level:"
		auto levelText = std::make_unique<dae::GameObject>();
		levelText->SetLocalPosition(580.f, 150.f);
		levelText->SetScale(3.f);
		rc = levelText->AddComponent(std::make_unique<dae::RenderComponent>(levelText.get(), "LevelUi.png"));
		levelText->AddComponent(std::make_unique<dae::SpriteComp>(levelText.get(), rc, "LevelUi.png",
			1, 1, rc->GetSize().x / 3.f * 2.f, rc->GetSize().y / 5.f, glm::vec2(0, rc->GetSize().y / 5.f * 2.f), false));

		// nr of the current level
		auto levelNrText = std::make_unique<dae::GameObject>();
		levelNrText->SetLocalPosition(100.f, -7.f);
		levelNrText->SetScale(3.f);
		rc = levelNrText->AddComponent(std::make_unique<dae::RenderComponent>(levelNrText.get(), "Fonts.png"));
		levelNrText->AddComponent(std::make_unique<dae::SpriteComp>(levelNrText.get(), rc, "Fonts.png",
			10, 1, rc->GetSize().x / 26.f, rc->GetSize().y / 8.f, glm::vec2(0, 0), false));
		levelNrText.release()->SetParent(levelText.get(), false);

		// text "Round:"
		auto roundText = std::make_unique<dae::GameObject>();
		roundText->SetLocalPosition(0, 30.f);
		roundText->SetScale(3.f);
		rc = roundText->AddComponent(std::make_unique<dae::RenderComponent>(roundText.get(), "LevelUi.png"));
		roundText->AddComponent(std::make_unique<dae::SpriteComp>(roundText.get(), rc, "LevelUi.png",
			1, 1, rc->GetSize().x / 3.f * 2.f, rc->GetSize().y / 5.f, glm::vec2(0, rc->GetSize().y / 5.f * 3.f), false));

		// nr of the current round
		auto roundNrText = std::make_unique<dae::GameObject>();
		roundNrText->SetLocalPosition(100.f, -7.f);
		roundNrText->SetScale(3.f);
		rc = roundNrText->AddComponent(std::make_unique<dae::RenderComponent>(roundNrText.get(), "Fonts.png"));
		roundNrText->AddComponent(std::make_unique<dae::SpriteComp>(roundNrText.get(), rc, "Fonts.png",
			10, 1, rc->GetSize().x / 26.f, rc->GetSize().y / 8.f, glm::vec2(0, 0), false));
		roundNrText.release()->SetParent(roundText.get(), false);

		roundText.release()->SetParent(levelText.get(), false);

		scene.Add(std::move(levelText));
	}

	void AddSounds()
	{
		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::Jump1), "./Data/Sounds/jump.mp3");
		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::Jump2), "./Data/Sounds/jump-2.mp3");
		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::Jump3), "./Data/Sounds/jump-3.mp3");
		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::Jump4), "./Data/Sounds/jump-4.mp3");
	}
}

#endif // !INITUTILS_H