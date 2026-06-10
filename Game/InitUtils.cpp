#include "InitUtils.h"

#include <SDL3/SDL.h>

#include "GameObject.h"
#include "RenderComponent.h"
#include "Scene.h"
#include "SceneManager.h"

#include "ServiceLocator.h"
#include "InputManager.h"

#include "Sounds.h"
#include "QBertScoreComponent.h"
#include "ArrowAnimComponent.h"
#include "QBertActor.h"
#include "QBertCommands.h"
#include "DiscActor.h"
#include "CoilyActor.h"
#include "HealthComponent.h"
#include "LevelManager.h"
#include "LivesDisplayComponent.h"
#include "SlickSamActor.h"
#include "Tags.h"
#include "UggWrongwayActor.h"

dae::GameObject* QBert::Utils::CreateLevel(dae::Scene& scene, int tileType, bool revertableTiles, int startTile, int winTile, int intermediateTile)
	{
		auto go = std::make_unique<dae::GameObject>(dae::Tag(Tag::Level));
		go->SetLocalPosition(350.f, 150.f);
		go->SetScale(3.f);
		auto levelComp = go->AddComponent(std::make_unique<LevelBase>(go.get(), tileType, revertableTiles, startTile, winTile, intermediateTile));
		auto level{ go.get() };
		scene.Add(std::move(go));

		go = std::make_unique<dae::GameObject>();
		go->AddComponent(std::make_unique<LevelManager>(go.get(), levelComp));
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

	void QBert::Utils::CreateUi(dae::Scene& scene)
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
		playerText->AddComponent(std::make_unique<QBert::ScoreComp>(playerText.get(), numberSprites));

		// text "Change to:"
		auto changeToText = std::make_unique<dae::GameObject>();
		changeToText->SetLocalPosition(3.f, 75.f);
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
		tileIcon->AddComponent(std::make_unique<dae::SpriteComp>(tileIcon.get(), rc, "TileIcons.png",
			1, 1, rc->GetSize().x / 6, rc->GetSize().y / 2, glm::vec2{0, 0}, false));
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

		lives->AddComponent(std::make_unique<QBert::LivesDisplay>(lives.get(), pLives));

		scene.Add(std::move(lives));

		// TOP RIGHT

		// text "Level:"
		auto levelText = std::make_unique<dae::GameObject>();
		levelText->SetLocalPosition(580.f, 150.f);
		levelText->SetScale(3.f);
		rc = levelText->AddComponent(std::make_unique<dae::RenderComponent>(levelText.get(), "LevelUi.png"));
		levelText->AddComponent(std::make_unique<dae::SpriteComp>(levelText.get(), rc, "LevelUi.png",
			1, 1, rc->GetSize().x / 3.f * 2.f, rc->GetSize().y / 5.f,
			glm::vec2(0, rc->GetSize().y / 5.f * 2.f), false));

		// nr of the current level
		auto levelNrText = std::make_unique<dae::GameObject>();
		levelNrText->SetLocalPosition(100.f, -7.f);
		levelNrText->SetScale(3.f);
		rc = levelNrText->AddComponent(std::make_unique<dae::RenderComponent>(levelNrText.get(), "Fonts.png"));
		auto levelNrSprite = levelNrText->AddComponent(std::make_unique<dae::SpriteComp>(levelNrText.get(), rc, "Fonts.png",
			10, 1, rc->GetSize().x / 26.f, rc->GetSize().y / 8.f,
			glm::vec2(0, 0), false));
		levelNrSprite->SetCurFrame(1);
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
		auto roundNrSprite = roundNrText->AddComponent(std::make_unique<dae::SpriteComp>(roundNrText.get(), rc, "Fonts.png",
			10, 1, rc->GetSize().x / 26.f, rc->GetSize().y / 8.f, glm::vec2(0, 0), false));
		roundNrSprite->SetCurFrame(1);
		roundNrText.release()->SetParent(roundText.get(), false);

		roundText.release()->SetParent(levelText.get(), false);

		scene.Add(std::move(levelText));
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

	std::unique_ptr<dae::GameObject> QBert::Utils::CreatePlayer(LevelBase* level)
	{
		auto player = std::make_unique<dae::GameObject>(dae::Tag(Tag::Player));
		player->SetRenderPriority(2);
		player->SetLocalPosition(100, 300);
		player->SetScale(3.f);

		auto rc = player->AddComponent(std::make_unique<dae::RenderComponent>(player.get(), "QBertSprites.png"));
		auto sprite = player->AddComponent(std::make_unique<dae::SpriteComp>(player.get(), "QBertSprites.png", 8, 1, false));
		auto move = player->AddComponent(std::make_unique<QBertMoveComp>(player.get(),
			glm::vec3((rc->GetSize().x / 8.f) * player->GetScale() / 2.f, (rc->GetSize().y * player->GetScale()) / 10.f * 9.f, 0),
			glm::vec2(0, 0), true, false));
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

std::unique_ptr<dae::GameObject> QBert::Utils::CreateCoily(LevelBase* level, QBertMoveComp* playerMove)
{
	auto coily = std::make_unique<dae::GameObject>(dae::Tag(QBert::Tag::Coily));
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
	coily->AddComponent(std::make_unique<QBert::CoilyActorComp>(coily.get(), level, playerMove));
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

std::unique_ptr<dae::GameObject> QBert::Utils::CreateSlick(LevelBase* level, QBertMoveComp* playerMove)
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

	slick->AddComponent(std::make_unique<QBertMoveComp>(slick.get(), glm::vec3(texSize.x / 20, texSize.y / 10.f * 9.f, 0), spawnTile, false, true));
	slick->AddComponent(std::make_unique<SlickSamActorComp>(slick.get(), level, playerMove));
	slick->AddComponent(std::make_unique<dae::HealthComponent>(slick.get(), 1, 1, 1));

	return slick;
}

	std::unique_ptr<dae::GameObject> QBert::Utils::CreateSam(LevelBase* level, QBertMoveComp* playerMove)
	{
		auto sam = CreateSlick(level, playerMove);
		sam->GetComponent<dae::RenderComponent>()->SetTexture("SamSprites.png");
		return sam;
	}

std::unique_ptr<dae::GameObject> QBert::Utils::CreateSlickSam(LevelBase* level, QBertMoveComp* playerMove)
{
	int randNr = rand() % 2;
	if (randNr == 0)
	{
		return CreateSlick(level, playerMove);
	}
	else
	{
		return CreateSam(level, playerMove);
	}
}

std::unique_ptr<dae::GameObject> QBert::Utils::CreateUgg(LevelBase* level, QBertMoveComp* playerMove)
{
		auto ugg = std::make_unique<dae::GameObject>(dae::Tag(Tag::UggWrongway));
		ugg->SetRenderPriority(2);
		auto rc = ugg->AddComponent(std::make_unique<dae::RenderComponent>(ugg.get(), "UggSprites.png"));
		ugg->SetLocalPosition(300, 300);
		ugg->SetScale(3.f);
		glm::vec2 texSize = rc->GetSize() * ugg->GetScale();
		ugg->AddComponent(std::make_unique<dae::SpriteComp>(ugg.get(), "UggSprites.png", 8, 1, false));
		ugg->AddComponent(std::make_unique<QBertMoveComp>(ugg.get(),
			glm::vec3(texSize.x / 12 - (0.5f * 32 * 3), texSize.y / 8.f * 7.f - (0.75f * 32 * 3), 0), glm::vec2(6, 0), false, true));
		ugg->AddComponent(std::make_unique<UggWrongwayActor>(ugg.get(), level, playerMove, false));

		return ugg;
}

std::unique_ptr<dae::GameObject> QBert::Utils::CreateWrongway(LevelBase* level, QBertMoveComp* playerMove)
{
	auto ugg = std::make_unique<dae::GameObject>(dae::Tag(Tag::UggWrongway));
	ugg->SetRenderPriority(2);
	auto rc = ugg->AddComponent(std::make_unique<dae::RenderComponent>(ugg.get(), "WrongwaySprites.png"));
	ugg->SetLocalPosition(300, 300);
	ugg->SetScale(3.f);
	glm::vec2 texSize = rc->GetSize() * ugg->GetScale();
	ugg->AddComponent(std::make_unique<dae::SpriteComp>(ugg.get(), "WrongwaySprites.png", 8, 1, false));
	ugg->AddComponent(std::make_unique<QBertMoveComp>(ugg.get(),
		glm::vec3(texSize.x / 20 - (-0.5f * 32 * 3), texSize.y / 10.f * 9.f - (0.75f * 32 * 3), 0), glm::vec2(0, 6), false, true));
	ugg->AddComponent(std::make_unique<UggWrongwayActor>(ugg.get(), level, playerMove, true));

	return ugg;
}

std::unique_ptr<dae::GameObject> QBert::Utils::CreateUggWrongway(LevelBase* level, QBertMoveComp* playerMove)
{
	int randNr = rand() % 2;
	if (randNr == 0)
	{
		return CreateUgg(level, playerMove);
	}
	else
	{
		return CreateWrongway(level, playerMove);
	}
}