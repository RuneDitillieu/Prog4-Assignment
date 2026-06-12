#include "InitUtils.h"

#include <SDL3/SDL.h>

#include "ArrowAnimComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "Scene.h"

#include "ServiceLocator.h"
#include "InputManager.h"

#include "Sounds.h"
#include "QBertScoreComponent.h"
#include "QBertActor.h"
#include "QBertCommands.h"
#include "DiscActor.h"
#include "CoilyActor.h"
#include "GameModeSelectionComp.h"
#include "HealthComponent.h"
#include "LevelManager.h"
#include "LivesDisplayComponent.h"
#include "QBertHighscoreComponent.h"
#include "ResourceManager.h"
#include "SlickSamActor.h"
#include "Tags.h"
#include "TextComponent.h"
#include "UggWrongwayActor.h"
#include "Colors.h"
#include "PlayerCoilyActor.h"

dae::GameObject* QBert::Utils::CreateLevel(dae::Scene& scene, std::vector<dae::Component*> comps, int tileType, bool revertableTiles,
                                           int startTile, int winTile, int intermediateTile)
	{
		auto go = std::make_unique<dae::GameObject>(dae::Tag(Tag::Level));
		go->SetLocalPosition(350.f, 150.f);
		go->SetScale(3.f);
		auto levelComp = go->AddComponent(std::make_unique<LevelBase>(go.get(), tileType, revertableTiles, startTile, winTile, intermediateTile));
		auto level{ go.get() };
		scene.Add(std::move(go));

		go = std::make_unique<dae::GameObject>();
		go->AddComponent(std::make_unique<LevelManager>(go.get(), levelComp,
			dynamic_cast<dae::SpriteComp*>(comps[0]), dynamic_cast<dae::TextComponent*>(comps[1]),
			dynamic_cast<dae::TextComponent*>(comps[2])));
		dae::InputManager::GetInstance().BindCommand(std::make_unique<SkipLevelCommand>(go.get()), SDL_SCANCODE_F1, SDL_EVENT_KEY_DOWN);
		scene.Add(std::move(go));

		return level;
	}

	std::unique_ptr<dae::GameObject> QBert::Utils::CreateNumberComp(float x, float y, bool enabled, std::vector<dae::SpriteComp*>& numberSprites)
	{
		auto score = std::make_unique<dae::GameObject>();
		score->SetLocalPosition(x, y);
		score->SetScale(3.f);
		auto rc = score->AddComponent(std::make_unique<dae::RenderComponent>(score.get(), "Fonts.png"));
		numberSprites.emplace_back(
		score->AddComponent(std::make_unique<dae::SpriteComp>(score.get(), rc, "Fonts.png",
			10, 1, rc->GetSize().x / 26.f, rc->GetSize().y / 8.f, glm::vec2(0, 0), false)));

		score->IsEnabled(enabled);

		return score;
	}

	std::unique_ptr<dae::GameObject> QBert::Utils::CreateLifeComp(float y, bool enabled, std::vector<dae::GameObject*>& lives)
	{
		auto life = std::make_unique<dae::GameObject>();
		life->SetLocalPosition(0.f, y);
		life->SetScale(3.f);
		auto rc = life->AddComponent(std::make_unique<dae::RenderComponent>(life.get(), "Fonts.png"));
		life->AddComponent(std::make_unique<dae::SpriteComp>(life.get(), rc, "Fonts.png", 1, 1,
			rc->GetSize().x / 26, rc->GetSize().y / 4, glm::vec2(96, rc->GetSize().y / 2), false));
		life->IsEnabled(enabled);
		lives.emplace_back(life.get());

		return life;
	}

	std::vector<dae::Component*> QBert::Utils::CreateUi(dae::Scene& scene, dae::TextComponent* player2ScoreText, std::vector<dae::GameObject*> player2Lives)
	{
		// TOP LEFT

		// animated text "Player"
		auto playerText = std::make_unique<dae::GameObject>();
		playerText->SetLocalPosition(20.f, 50.f);
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

	auto font = dae::ResourceManager::GetInstance().LoadFont("QBertFont.ttf", 20);

	auto score = std::make_unique<dae::GameObject>();
	auto tc = score->AddComponent(std::make_unique<dae::TextComponent>(score.get(), "0", font, QBert::ORANGE));
	score->SetLocalPosition(8.f, 40.f);
	score.release()->SetParent(playerText.get(), false);

	playerText->AddComponent(std::make_unique<QBert::ScoreComp>(playerText.get(), tc, player2ScoreText));

		// text "Change to:"
		auto changeToText = std::make_unique<dae::GameObject>();
		changeToText->SetLocalPosition(3.f, 85.f);
		changeToText->SetScale(3.f);
		rc = changeToText->AddComponent(std::make_unique<dae::RenderComponent>(changeToText.get(), "LevelUi.png"));
		changeToText->AddComponent(std::make_unique<dae::SpriteComp>(changeToText.get(), rc, "LevelUi.png",
			1, 1, rc->GetSize().x, rc->GetSize().y / 5.f, glm::vec2(0, rc->GetSize().y / 5.f * 4.f), false));

		std::vector<dae::GameObject*> arrows{};
		arrows.reserve(4);

		// pink arrows
		auto leftLeftArrow = std::make_unique<dae::GameObject>();
		leftLeftArrow->SetLocalPosition(0.f, 30.f);
		leftLeftArrow->SetScale(3.f);
		rc = leftLeftArrow->AddComponent(std::make_unique<dae::RenderComponent>(leftLeftArrow.get(), "LevelUi.png"));
		leftLeftArrow->AddComponent(std::make_unique<dae::SpriteComp>(leftLeftArrow.get(), rc, "LevelUi.png",
			1, 1, rc->GetSize().x / 5.f, rc->GetSize().y / 3.f, glm::vec2(rc->GetSize().x / 2.f, 0), false));

		arrows.push_back(leftLeftArrow.get());
		leftLeftArrow.release()->SetParent(changeToText.get(), false);

		auto leftArrow = std::make_unique<dae::GameObject>();
		leftArrow->SetLocalPosition(25.f, 30.f);
		leftArrow->SetScale(3.f);
		rc = leftArrow->AddComponent(std::make_unique<dae::RenderComponent>(leftArrow.get(), "LevelUi.png"));
		leftArrow->AddComponent(std::make_unique<dae::SpriteComp>(leftArrow.get(), rc, "LevelUi.png",
			1, 1, rc->GetSize().x / 5.f, rc->GetSize().y / 3.f, glm::vec2(rc->GetSize().x / 2.f, 0), false));

		arrows.push_back(leftArrow.get());
		leftArrow.release()->SetParent(changeToText.get(), false);

		auto rightArrow = std::make_unique<dae::GameObject>();
		rightArrow->SetLocalPosition(100.f, 30.f);
		rightArrow->SetScale(3.f);
		rc = rightArrow->AddComponent(std::make_unique<dae::RenderComponent>(rightArrow.get(), "LevelUi.png"));
		rightArrow->AddComponent(std::make_unique<dae::SpriteComp>(rightArrow.get(), rc, "LevelUi.png",
			1, 1, rc->GetSize().x / 5.f, rc->GetSize().y / 3.f, glm::vec2(rc->GetSize().x / 2.f + rc->GetSize().x / 5.f, 0), false));

		arrows.push_back(rightArrow.get());
		rightArrow.release()->SetParent(changeToText.get(), false);

		auto rightRightArrow = std::make_unique<dae::GameObject>();
		rightRightArrow->SetLocalPosition(125.f, 30.f);
		rightRightArrow->SetScale(3.f);
		rc = rightRightArrow->AddComponent(std::make_unique<dae::RenderComponent>(rightRightArrow.get(), "LevelUi.png"));
		rightRightArrow->AddComponent(std::make_unique<dae::SpriteComp>(rightRightArrow.get(), rc, "LevelUi.png",
			1, 1, rc->GetSize().x / 5.f, rc->GetSize().y / 3.f, glm::vec2(rc->GetSize().x / 2.f + rc->GetSize().x / 5.f, 0), false));

		arrows.push_back(rightRightArrow.get());
		rightRightArrow.release()->SetParent(changeToText.get(), false);

		changeToText->AddComponent(std::make_unique<QBert::ArrowAnimComp>(changeToText.get(), arrows));

		// goal tilecolor icon
		auto tileIcon = std::make_unique<dae::GameObject>();
		tileIcon->SetLocalPosition(53.f, 32.f);
		tileIcon->SetScale(3.f);
		rc = tileIcon->AddComponent(std::make_unique<dae::RenderComponent>(tileIcon.get(), "TileIcons.png"));
		auto tileIconSprite = tileIcon->AddComponent(std::make_unique<dae::SpriteComp>(tileIcon.get(), rc, "TileIcons.png",
			1, 1, rc->GetSize().x / 6, rc->GetSize().y / 3, glm::vec2{0, rc->GetSize().y / 3 }, false));
		tileIcon.release()->SetParent(changeToText.get(), false);

		changeToText.release()->SetParent(playerText.get(), false);

		scene.Add(std::move(playerText));


		// MIDDLE LEFT

		auto lives = std::make_unique<dae::GameObject>();
		lives->SetLocalPosition(20.f, 260.f);

		std::vector<dae::GameObject*> pLives{};
		pLives.reserve(5);

		CreateLifeComp(0.f, true, pLives).release()->SetParent(lives.get(), false);
		CreateLifeComp(50.f, true, pLives).release()->SetParent(lives.get(), false);
		CreateLifeComp(100.f, true, pLives).release()->SetParent(lives.get(), false);
		CreateLifeComp(150.f, true, pLives).release()->SetParent(lives.get(), false);
		CreateLifeComp(200.f, true, pLives).release()->SetParent(lives.get(), false);
		CreateLifeComp(250.f, true, pLives).release()->SetParent(lives.get(), false);

		lives->AddComponent(std::make_unique<QBert::LivesDisplay>(lives.get(), pLives, player2Lives));

		scene.Add(std::move(lives));

		// TOP RIGHT

		// text "Level:"
		auto levelText = std::make_unique<dae::GameObject>();
		levelText->SetLocalPosition(560.f, 150.f);
		levelText->SetScale(3.f);
		rc = levelText->AddComponent(std::make_unique<dae::RenderComponent>(levelText.get(), "LevelUi.png"));
		levelText->AddComponent(std::make_unique<dae::SpriteComp>(levelText.get(), rc, "LevelUi.png",
			1, 1, rc->GetSize().x / 3.f * 2.f, rc->GetSize().y / 5.f,
			glm::vec2(0, rc->GetSize().y / 5.f * 2.f), false));

		// nr of the current level
		auto levelNrText = std::make_unique<dae::GameObject>();
		levelNrText->SetLocalPosition(100.f, -7.f);
		auto levelNr = levelNrText->AddComponent(std::make_unique<dae::TextComponent>(levelNrText.get(), "1", font, QBert::ORANGE));
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
		auto roundNr = roundNrText->AddComponent(std::make_unique<dae::TextComponent>(roundNrText.get(), "1", font, QBert::ORANGE));

		roundNrText.release()->SetParent(roundText.get(), false);

		roundText.release()->SetParent(levelText.get(), false);

		scene.Add(std::move(levelText));

		return std::vector<dae::Component*>{tileIconSprite, levelNr, roundNr };
	}

std::vector<dae::Component*> QBert::Utils::CreateCoopUi(dae::Scene& scene)
{
	// animated text "Player"
	auto playerText = std::make_unique<dae::GameObject>();
	playerText->SetLocalPosition(560.f, 50.f);
	playerText->SetScale(3.f);
	auto rc = playerText->AddComponent(std::make_unique<dae::RenderComponent>(playerText.get(), "PlayerTextSprites.png"));
	playerText->AddComponent(std::make_unique<dae::SpriteComp>(playerText.get(), rc, "PlayerTextSprites.png",
		1, 6, rc->GetSize().x / 2.f, rc->GetSize().y / 6.f, glm::vec2(0, 0)));

	// rect with player id
	auto playerId = std::make_unique<dae::GameObject>();
	playerId->SetLocalPosition(rc->GetSize().x / 2.f * 3.f + 20.f, -8.f);
	playerId->SetScale(3.f);
	rc = playerId->AddComponent(std::make_unique<dae::RenderComponent>(playerId.get(), "LevelUi.png"));
	playerId->AddComponent(std::make_unique<dae::SpriteComp>(playerId.get(), rc, "LevelUi.png",
		2, 1, rc->GetSize().x / 5.f, rc->GetSize().y / 3.f, glm::vec2(rc->GetSize().x / 5.f - 1.f, 0), false));
	playerId.release()->SetParent(playerText.get(), false);

	auto font = dae::ResourceManager::GetInstance().LoadFont("QBertFont.ttf", 20);

	auto score = std::make_unique<dae::GameObject>();
	auto tc = score->AddComponent(std::make_unique<dae::TextComponent>(score.get(), "0", font, QBert::ORANGE));
	score->SetLocalPosition(8.f, 40.f);
	score.release()->SetParent(playerText.get(), false);

	scene.Add(std::move(playerText));

	auto lives = std::make_unique<dae::GameObject>();
	lives->SetLocalPosition(750.f, 260.f);

	std::vector<dae::GameObject*> pLives{};
	pLives.reserve(5);

	CreateLifeComp(0.f, true, pLives).release()->SetParent(lives.get(), false);
	CreateLifeComp(50.f, true, pLives).release()->SetParent(lives.get(), false);
	CreateLifeComp(100.f, true, pLives).release()->SetParent(lives.get(), false);
	CreateLifeComp(150.f, true, pLives).release()->SetParent(lives.get(), false);
	CreateLifeComp(200.f, true, pLives).release()->SetParent(lives.get(), false);
	CreateLifeComp(250.f, true, pLives).release()->SetParent(lives.get(), false);

	scene.Add(std::move(lives));

	return CreateUi(scene, tc, pLives);
}

std::vector<dae::Component*> QBert::Utils::CreateVersusUi(dae::Scene& scene)
{
	// animated text "Player"
	auto playerText = std::make_unique<dae::GameObject>();
	playerText->SetLocalPosition(560.f, 50.f);
	playerText->SetScale(3.f);
	auto rc = playerText->AddComponent(std::make_unique<dae::RenderComponent>(playerText.get(), "PlayerTextSprites.png"));
	playerText->AddComponent(std::make_unique<dae::SpriteComp>(playerText.get(), rc, "PlayerTextSprites.png",
		1, 6, rc->GetSize().x / 2.f, rc->GetSize().y / 6.f, glm::vec2(0, 0)));

	// rect with player id
	auto playerId = std::make_unique<dae::GameObject>();
	playerId->SetLocalPosition(rc->GetSize().x / 2.f * 3.f + 20.f, -8.f);
	playerId->SetScale(3.f);
	rc = playerId->AddComponent(std::make_unique<dae::RenderComponent>(playerId.get(), "LevelUi.png"));
	playerId->AddComponent(std::make_unique<dae::SpriteComp>(playerId.get(), rc, "LevelUi.png",
		2, 1, rc->GetSize().x / 5.f, rc->GetSize().y / 3.f, glm::vec2(rc->GetSize().x / 5.f - 1.f, 0), false));
	playerId.release()->SetParent(playerText.get(), false);

	auto font = dae::ResourceManager::GetInstance().LoadFont("QBertFont.ttf", 20);

	// score
	auto score = std::make_unique<dae::GameObject>();
	auto tc = score->AddComponent(std::make_unique<dae::TextComponent>(score.get(), "0", font, QBert::ORANGE));
	score->SetLocalPosition(8.f, 40.f);
	score.release()->SetParent(playerText.get(), false);

	scene.Add(std::move(playerText));

	return CreateUi(scene, tc);
}

void QBert::Utils::AddSounds()
	{
		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::Jump1),
			"./Data/Sounds/Jump-1.mp3", 0.5f);

		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::Jump2),
			"./Data/Sounds/Jump-2.mp3", 0.5f);

		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::Jump3),
			"./Data/Sounds/Jump-3.mp3", 0.5f);

		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::Jump4),
			"./Data/Sounds/Jump-4.mp3", 0.5f);

		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::Coin),
			"./Data/Sounds/Coin.mp3", 0.5f);

		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::QBertFall),
			"./Data/Sounds/QBertFall.mp3", 0.5f);

		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::CoilyFall),
			"./Data/Sounds/CoilyFall.mp3", 0.5f);

		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::Lift),
			"./Data/Sounds/Lift.mp3", 0.5f);

		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::Speech1),
			"./Data/Sounds/Speech-1.mp3", 0.5f);

		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::Speech2),
			"./Data/Sounds/Speech-2.mp3", 0.5f);

		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::Speech3),
			"./Data/Sounds/Speech-3.mp3", 0.5f);

		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::LevelStart),
			"./Data/Sounds/LevelStart.mp3", 0.5f);

		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::Tune),
			"./Data/Sounds/Tune.mp3", 0.5f);

		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::Victory),
			"./Data/Sounds/Victory.mp3", 0.5f);

		dae::ServiceLocator::GetSoundSystem().AddSound(dae::SoundId(QBert::Sound::Prize),
			"./Data/Sounds/Prize.mp3", 0.5f);
	}

	std::unique_ptr<dae::GameObject> QBert::Utils::CreatePlayer(LevelBase* level, const glm::vec2& startPos)
	{
		auto player = std::make_unique<dae::GameObject>(dae::Tag(Tag::QBert));
		player->SetRenderPriority(2);
		player->SetLocalPosition(100, 300);
		player->SetScale(3.f);

		auto rc = player->AddComponent(std::make_unique<dae::RenderComponent>(player.get(), "QBertSprites.png"));
		auto sprite = player->AddComponent(std::make_unique<dae::SpriteComp>(player.get(), "QBertSprites.png", 8, 1, false));
		auto move = player->AddComponent(std::make_unique<QBertMoveComp>(player.get(),
			glm::vec3((rc->GetSize().x / 8.f) * player->GetScale() / 2.f, (rc->GetSize().y * player->GetScale()) / 10.f * 9.f, 0),
			startPos, true, false));
		sprite->SetCurFrame(5);
		player->AddComponent(std::make_unique<QBertActorComp>(player.get(), sprite, move, level));

		player->AddComponent(std::make_unique<dae::HealthComponent>(player.get(), 1, 6, 6));

		auto speechBubble = std::make_unique<dae::GameObject>();
		auto speechRc = speechBubble->AddComponent(std::make_unique<dae::RenderComponent>(speechBubble.get(),
			"SpeechBubble.png"));
		speechBubble->SetScale(2.f);
		speechBubble->SetLocalPosition((rc->GetSize().x / 16.f) * player->GetScale() - (speechRc->GetSize().x / 2.f) * speechBubble->GetScale(),
			-speechRc->GetSize().y * speechBubble->GetScale());
		speechBubble->IsEnabled(false);
		speechBubble.release()->SetParent(player.get(), false);

		return player;
	}

std::unique_ptr<dae::GameObject> QBert::Utils::AddKeyboardBindings(std::unique_ptr<dae::GameObject> player)
{
	dae::InputManager::GetInstance().BindCommand(std::make_unique<QBert::QBertMoveCommand>(player.get(),
			glm::vec3(0, -1, 0)), SDL_SCANCODE_W, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<QBert::QBertMoveCommand>(player.get(),
		glm::vec3(0, 1, 0)), SDL_SCANCODE_S, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<QBert::QBertMoveCommand>(player.get(),
		glm::vec3(-1, 0, 0)), SDL_SCANCODE_A, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<QBert::QBertMoveCommand>(player.get(),
		glm::vec3(1, 0, 0)), SDL_SCANCODE_D, SDL_EVENT_KEY_DOWN);

	return player;
}

std::unique_ptr<dae::GameObject> QBert::Utils::AddController1Bindings(std::unique_ptr<dae::GameObject> player)
{
	dae::InputManager::GetInstance().BindCommand(std::make_unique<QBertMoveCommand>(player.get(),
		glm::vec3(0, -1, 0)), XINPUT_GAMEPAD_DPAD_UP, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<QBertMoveCommand>(player.get(),
		glm::vec3(0, 1, 0)), XINPUT_GAMEPAD_DPAD_DOWN, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<QBertMoveCommand>(player.get(),
		glm::vec3(-1, 0, 0)), XINPUT_GAMEPAD_DPAD_LEFT, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<QBertMoveCommand>(player.get(),
		glm::vec3(1, 0, 0)), XINPUT_GAMEPAD_DPAD_RIGHT, 0);

	return player;
}

std::unique_ptr<dae::GameObject> QBert::Utils::AddController2Bindings(std::unique_ptr<dae::GameObject> player)
{
	dae::InputManager::GetInstance().BindCommand(std::make_unique<QBertMoveCommand>(player.get(),
		glm::vec3(0, -1, 0)), XINPUT_GAMEPAD_DPAD_UP, 1);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<QBertMoveCommand>(player.get(),
		glm::vec3(0, 1, 0)), XINPUT_GAMEPAD_DPAD_DOWN, 1);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<QBertMoveCommand>(player.get(),
		glm::vec3(-1, 0, 0)), XINPUT_GAMEPAD_DPAD_LEFT, 1);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<QBertMoveCommand>(player.get(),
		glm::vec3(1, 0, 0)), XINPUT_GAMEPAD_DPAD_RIGHT, 1);

	return player;
}

std::unique_ptr<dae::GameObject> QBert::Utils::CreatePlayerCoily(LevelBase* level, QBertMoveComp* qbertMove)
{
	auto playerCoily = std::make_unique<dae::GameObject>(dae::Tag(Tag::PlayerCoily));
	playerCoily->SetRenderPriority(2);
	auto rc = playerCoily->AddComponent(std::make_unique<dae::RenderComponent>(playerCoily.get(), "CoilySprites.png"));
	playerCoily->SetLocalPosition(300, 300);
	playerCoily->SetScale(3.f);

	glm::vec2 texSize = rc->GetSize() * playerCoily->GetScale();
	playerCoily->AddComponent(std::make_unique<dae::SpriteComp>(playerCoily.get(), "CoilySprites.png", 10, 1, false));

	int randNr = rand() % 2;
	glm::vec2 spawnTile{0, 0};
	if (randNr == 0)
	{
		spawnTile.x = 1;
	}
	else
	{
		spawnTile.y = 1;
	}

	playerCoily->AddComponent(std::make_unique<QBertMoveComp>(playerCoily.get(), glm::vec3(texSize.x / 20, texSize.y / 10.f * 9.f, 0),
		spawnTile, false, false, true));
	playerCoily->AddComponent(std::make_unique<PlayerCoilyActorComp>(playerCoily.get(), level, qbertMove));
	playerCoily->AddComponent(std::make_unique<dae::HealthComponent>(playerCoily.get(), 1, 1, 1));

	dae::InputManager::GetInstance().BindCommand(std::make_unique<QBertMoveCommand>(playerCoily.get(),
		glm::vec3(0, -1, 0)), XINPUT_GAMEPAD_DPAD_UP, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<QBertMoveCommand>(playerCoily.get(),
		glm::vec3(0, 1, 0)), XINPUT_GAMEPAD_DPAD_DOWN, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<QBertMoveCommand>(playerCoily.get(),
		glm::vec3(-1, 0, 0)), XINPUT_GAMEPAD_DPAD_LEFT, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<QBertMoveCommand>(playerCoily.get(),
		glm::vec3(1, 0, 0)), XINPUT_GAMEPAD_DPAD_RIGHT, 0);

	return playerCoily;
}

std::unique_ptr<dae::GameObject> QBert::Utils::CreateCoily(LevelBase* level, const std::vector<QBertMoveComp*>& playerMoves)
{
	auto coily = std::make_unique<dae::GameObject>(dae::Tag(Tag::Coily));
	coily->SetRenderPriority(2);
	auto rc = coily->AddComponent(std::make_unique<dae::RenderComponent>(coily.get(), "CoilySprites.png"));
	coily->SetLocalPosition(300, 300);
	coily->SetScale(3.f);
	glm::vec2 texSize = rc->GetSize() * coily->GetScale();
	coily->AddComponent(std::make_unique<dae::SpriteComp>(coily.get(), "CoilySprites.png", 10, 1, false));

	int randNr = rand() % 2;
	glm::vec2 spawnTile{0, 0};
	if (randNr == 0)
	{
		spawnTile.x = 1;
	}
	else
	{
		spawnTile.y = 1;
	}

	coily->AddComponent(std::make_unique<QBert::QBertMoveComp>(coily.get(), glm::vec3(texSize.x / 20, texSize.y / 10.f * 9.f, 0),
		spawnTile, false, false));
	coily->AddComponent(std::make_unique<QBert::CoilyActorComp>(coily.get(), level, playerMoves));
	coily->AddComponent(std::make_unique<dae::HealthComponent>(coily.get(), 1, 1, 1));

	return coily;
}

	std::unique_ptr<dae::GameObject> QBert::Utils::CreateDisc(const glm::vec2& tile, LevelBase* level)
	{
		auto disc = std::make_unique<dae::GameObject>(dae::Tag(QBert::Tag::Disc));
		disc->SetScale(3.f);

		auto rc = disc->AddComponent(std::make_unique<dae::RenderComponent>(disc.get(), "DiscSprites.png"));
		auto sprite = disc->AddComponent(std::make_unique<dae::SpriteComp>(disc.get(), rc, "DiscSprites.png", 4, 1,
			rc->GetSize().x / 45, rc->GetSize().y / 2, glm::vec2(0, 0), true));
		disc->AddComponent(std::make_unique<QBert::DiscActorComp>(disc.get(), tile));

		disc->SetLocalPosition(level->GetMiddlePosOfTile(static_cast<int>(tile.x), static_cast<int>(tile.y))
			- glm::vec3((sprite->GetSpriteSize().x * disc->GetScale() / 2.f),
			sprite->GetSpriteSize().y * disc->GetScale() / 2.f, 0));

		return disc;
	}

std::unique_ptr<dae::GameObject> QBert::Utils::CreateSlick(LevelBase* level, const std::vector<QBertMoveComp*>& playersMoves)
{
	auto slick = std::make_unique<dae::GameObject>(dae::Tag(Tag::SlickSam));
	slick->SetRenderPriority(2);
	auto rc = slick->AddComponent(std::make_unique<dae::RenderComponent>(slick.get(), "SlickSprites.png"));
	slick->SetLocalPosition(300, 300);
	slick->SetScale(3.f);
	glm::vec2 texSize = rc->GetSize() * slick->GetScale();
	slick->AddComponent(std::make_unique<dae::SpriteComp>(slick.get(), "SlickSprites.png", 8, 1, false));

	int randNr = rand() % 2;
	glm::vec2 spawnTile{ 0, 0};
	if (randNr == 0)
	{
		spawnTile.x = 1;
	}
	else
	{
		spawnTile.y = 1;
	}

	slick->AddComponent(std::make_unique<QBertMoveComp>(slick.get(),
		glm::vec3(texSize.x / 20, texSize.y / 10.f * 9.f, 0), spawnTile, false, true));
	slick->AddComponent(std::make_unique<SlickSamActorComp>(slick.get(), level, playersMoves));
	slick->AddComponent(std::make_unique<dae::HealthComponent>(slick.get(), 1, 1, 1));

	return slick;
}

	std::unique_ptr<dae::GameObject> QBert::Utils::CreateSam(LevelBase* level, const std::vector<QBertMoveComp*>& playersMoves)
	{
		auto sam = CreateSlick(level, playersMoves);
		sam->GetComponent<dae::RenderComponent>()->SetTexture("SamSprites.png");
		return sam;
	}

std::unique_ptr<dae::GameObject> QBert::Utils::CreateSlickSam(LevelBase* level, const std::vector<QBertMoveComp*>& playersMoves)
{
	int randNr = rand() % 2;
	if (randNr == 0)
	{
		return CreateSlick(level, playersMoves);
	}
	else
	{
		return CreateSam(level, playersMoves);
	}
}

std::unique_ptr<dae::GameObject> QBert::Utils::CreateUgg(LevelBase* level, const std::vector<QBertMoveComp*>& playersMoves)
{
		auto ugg = std::make_unique<dae::GameObject>(dae::Tag(Tag::UggWrongway));
		ugg->SetRenderPriority(2);
		auto rc = ugg->AddComponent(std::make_unique<dae::RenderComponent>(ugg.get(), "UggSprites.png"));
		ugg->SetLocalPosition(300, 300);
		ugg->SetScale(3.f);
		glm::vec2 texSize = rc->GetSize() * ugg->GetScale();
		ugg->AddComponent(std::make_unique<dae::SpriteComp>(ugg.get(), "UggSprites.png", 8, 1, false));
		ugg->AddComponent(std::make_unique<QBertMoveComp>(ugg.get(),
			glm::vec3(texSize.x / 12 - (0.5f * 32 * 3), texSize.y / 8.f * 7.f - (0.75f * 32 * 3), 0),
			glm::vec2(6, 0), false, false));
		ugg->AddComponent(std::make_unique<UggWrongwayActor>(ugg.get(), level, playersMoves, false));

		return ugg;
}

std::unique_ptr<dae::GameObject> QBert::Utils::CreateWrongway(LevelBase* level, const std::vector<QBertMoveComp*>& playersMoves)
{
	auto ugg = std::make_unique<dae::GameObject>(dae::Tag(Tag::UggWrongway));
	ugg->SetRenderPriority(2);
	auto rc = ugg->AddComponent(std::make_unique<dae::RenderComponent>(ugg.get(), "WrongwaySprites.png"));
	ugg->SetLocalPosition(300, 300);
	ugg->SetScale(3.f);
	glm::vec2 texSize = rc->GetSize() * ugg->GetScale();
	ugg->AddComponent(std::make_unique<dae::SpriteComp>(ugg.get(), "WrongwaySprites.png", 8, 1, false));
	ugg->AddComponent(std::make_unique<QBertMoveComp>(ugg.get(),
		glm::vec3(texSize.x / 20 - (-0.5f * 32 * 3), texSize.y / 10.f * 9.f - (0.75f * 32 * 3), 0),
		glm::vec2(0, 6), false, false));
	ugg->AddComponent(std::make_unique<UggWrongwayActor>(ugg.get(), level, playersMoves, true));

	return ugg;
}

std::unique_ptr<dae::GameObject> QBert::Utils::CreateUggWrongway(LevelBase* level, const std::vector<QBertMoveComp*>& playersMoves)
{
	int randNr = rand() % 2;
	if (randNr == 0)
	{
		return CreateUgg(level, playersMoves);
	}
	else
	{
		return CreateWrongway(level, playersMoves);
	}
}

void QBert::Utils::CreateSelectionScreenUI(dae::Scene& scene)
{
	auto title = std::make_unique<dae::GameObject>();
	auto rc = title->AddComponent(std::make_unique<dae::RenderComponent>(title.get(), "Fonts.png"));
	title->AddComponent(std::make_unique<dae::SpriteComp>(title.get(), rc, "Fonts.png", 1, 1,
		rc->GetSize().x / 20.f * 9.f, rc->GetSize().y / 4, glm::vec2(0, rc->GetSize().y / 2), false));
	title->SetScale(5.f);
	title->SetLocalPosition(180, 100);

	std::vector<dae::TextComponent*> textComps{};
	textComps.reserve(3);

	auto font = dae::ResourceManager::GetInstance().LoadFont("QBertFont.ttf", 18);
	auto go = std::make_unique<dae::GameObject>();
	auto tc = go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), "Start Single Player game", font, QBert::ORANGE));
	textComps.emplace_back(tc);
	go->SetLocalPosition(180, 350);
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	tc = go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), "Start Coop game", font, QBert::GREEN));
	textComps.emplace_back(tc);
	go->SetLocalPosition(180, 410);
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	tc = go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), "Start Versus game", font, QBert::GREEN));
	textComps.emplace_back(tc);
	go->SetLocalPosition(180, 470);
	scene.Add(std::move(go));

	auto sc = title->AddComponent(std::make_unique<GameModeSelectionComp>(title.get(), textComps));
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::PrevCommand>(sc), SDL_SCANCODE_UP, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::NextCommand>(sc), SDL_SCANCODE_DOWN, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::ConfirmCommand>(sc), SDL_SCANCODE_SPACE, SDL_EVENT_KEY_DOWN);

	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::PrevCommand>(sc), XINPUT_GAMEPAD_DPAD_UP, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::NextCommand>(sc), XINPUT_GAMEPAD_DPAD_DOWN, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::ConfirmCommand>(sc), XINPUT_GAMEPAD_B, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::ConfirmCommand>(sc), XINPUT_GAMEPAD_A, 0);

	scene.Add(std::move(title));
}

void QBert::Utils::CreateHighscoreScreenUI(dae::Scene& scene)
{
	auto bigFont = dae::ResourceManager::GetInstance().LoadFont("QBertFont.ttf", 48);
	auto title = std::make_unique<dae::GameObject>();
	title->AddComponent(std::make_unique<dae::TextComponent>(title.get(), "HIGH SCORES",
		bigFont, QBert::PURPLE));
	title->SetLocalPosition(150, 100);

	auto normalFont = dae::ResourceManager::GetInstance().LoadFont("QBertFont.ttf", 28);

	std::vector<dae::TextComponent*> hsTextComps{};
	hsTextComps.reserve(5);

	for (size_t Idx{ 0 }; Idx < 5; ++Idx)
	{
		auto go = std::make_unique<dae::GameObject>();
		go->SetLocalPosition(240, 220 + (Idx * 50.f));
		auto tc = go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), " ", normalFont, QBert::ORANGE));
		hsTextComps.emplace_back(tc);
		scene.Add(std::move(go));
	}

	std::vector<dae::TextComponent*> letterTextComps{};
	letterTextComps.reserve(5);

	for (size_t Idx{ 0 }; Idx < 5; ++Idx)
	{
		auto go = std::make_unique<dae::GameObject>();
		go->SetLocalPosition(250 + (Idx * 70.f), 620);
		auto tc = go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), ".", bigFont, QBert::GREEN));
		if (Idx == 0)
		{
			tc->SetColor(QBert::ORANGE);
			tc->SetText("A");
		}
		letterTextComps.emplace_back(tc);
		scene.Add(std::move(go));
	}

	std::vector<dae::GameObject*> arrows{};
	auto go = std::make_unique<dae::GameObject>();
	auto rc = go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "Triangles.png"));
	go->AddComponent(std::make_unique<dae::SpriteComp>(go.get(), rc, "Triangles.png", 1, 1, rc->GetSize().x, rc->GetSize().y / 2,
		glm::vec2(0, 0), false));
	go->SetScale(2.f);
	go->SetLocalPosition(255, 598);
	arrows.emplace_back(go.get());
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	rc = go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "Triangles.png"));
	go->AddComponent(std::make_unique<dae::SpriteComp>(go.get(), rc, "Triangles.png", 1, 1, rc->GetSize().x, rc->GetSize().y / 2,
		glm::vec2(0, rc->GetSize().y / 2), false));
	go->SetScale(2.f);
	go->SetLocalPosition(255, 680);
	arrows.emplace_back(go.get());
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	auto tc = go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), " ", normalFont, QBert::ORANGE));
	go->SetLocalPosition(415, 710);

	auto hs = title->AddComponent(std::make_unique<QBertHighscoreComp>(title.get(), hsTextComps,
		letterTextComps, arrows, tc));

	scene.Add(std::move(go));

	auto smallFont = dae::ResourceManager::GetInstance().LoadFont("QBertFont.ttf", 16);

	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), "Your score", smallFont, QBert::PURPLE));
	go->SetLocalPosition(230, 720);
	scene.Add(std::move(go));

	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::PrevCommand>(hs), SDL_SCANCODE_UP, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::NextCommand>(hs), SDL_SCANCODE_DOWN, SDL_EVENT_KEY_DOWN);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::ConfirmCommand>(hs), SDL_SCANCODE_SPACE, SDL_EVENT_KEY_DOWN);

	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::PrevCommand>(hs), XINPUT_GAMEPAD_DPAD_UP, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::NextCommand>(hs), XINPUT_GAMEPAD_DPAD_DOWN, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::ConfirmCommand>(hs), XINPUT_GAMEPAD_B, 0);
	dae::InputManager::GetInstance().BindCommand(std::make_unique<dae::ConfirmCommand>(hs), XINPUT_GAMEPAD_A, 0);

	scene.Add(std::move(title));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), "Fill in your name to save your highscore", smallFont, QBert::PURPLE));
	go->SetLocalPosition(90, 550);
	scene.Add(std::move(go));
}
