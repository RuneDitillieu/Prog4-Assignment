#include "LevelManager.h"

#include <filesystem>
#include <fstream>
#include <cassert>

#include "CoilyActor.h"
#include "DeltaTime.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Tags.h"
#include "QBertMoveComponent.h"
#include "InitUtils.h"
#include "DiscActor.h"
#include "SlickSamActor.h"
#include "UggWrongwayActor.h"
#include "HealthComponent.h"

QBert::LevelManager::LevelManager(dae::GameObject* pOwner, LevelBase* level, dae::SpriteComp* tileIconSprite,
                                  dae::TextComponent* levelNrText, dae::TextComponent* roundNrText)
	: dae::Component(pOwner)
	, m_pConnLevel(level)
	, m_pWinTileIndicator(tileIconSprite)
	, m_pLevelNrText(levelNrText)
	, m_pRoundNrText(roundNrText)
{
	LoadLevelParams();

	auto activeScene = dae::SceneManager::GetInstance().GetActiveScene();

	// initialize discs
	std::vector<DiscActorComp*> discs{};
	auto disc = Utils::CreateDisc(m_levelParams[0].discSpawns[0], m_pConnLevel);
	discs.emplace_back(disc->GetComponent<DiscActorComp>());
	activeScene->Add(std::move(disc));

	disc = Utils::CreateDisc(m_levelParams[0].discSpawns[1], m_pConnLevel);
	discs.emplace_back(disc->GetComponent<DiscActorComp>());
	activeScene->Add(std::move(disc));

	disc = Utils::CreateDisc(m_levelParams[0].discSpawns[2], m_pConnLevel);
	disc->IsEnabled(false);
	discs.emplace_back(disc->GetComponent<DiscActorComp>());
	activeScene->Add(std::move(disc));

	disc = Utils::CreateDisc(m_levelParams[0].discSpawns[3], m_pConnLevel);
	disc->IsEnabled(false);
	discs.emplace_back(disc->GetComponent<DiscActorComp>());
	activeScene->Add(std::move(disc));

	// set level params
	m_pConnLevel->SetDiscs(std::move(discs));
	m_pWinTileIndicator->SetStartPos(glm::vec2(m_pWinTileIndicator->GetSpriteSize().x * m_levelParams[0].tileParams.tileType,
		m_pWinTileIndicator->GetSpriteSize().y * m_levelParams[0].tileParams.winTile));
}

QBert::LevelManager::~LevelManager()
{

}

void QBert::LevelManager::Start()
{
	m_pConnLevel->GetOwner()->GetSubject()->AddObserver(this);
	auto players = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::QBert));
	for (auto player : players)
	{
		m_pPlayersMove.emplace_back(player->GetComponent<QBertMoveComp>());
	}

	if (players.size() == 1
		&& dae::SceneManager::GetInstance().GetActiveScene()->GetSceneName() == dae::SceneName(SceneName::VersusScene))
	{
		auto playerCoily = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::PlayerCoily));
		if (!playerCoily.empty()) m_pPlayersMove.emplace_back(playerCoily[0]->GetComponent<QBertMoveComp>());
	}

	for (auto player : m_pPlayersMove)
	{
		if (player)
		{
			m_pPlayersHealth.emplace_back(player->GetOwner()->GetComponent<dae::HealthComponent>());
		}
	}
}

void QBert::LevelManager::Update()
{
	m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();

	bool isPlayerAlive{ false };
	for (auto health : m_pPlayersHealth)
	{
		if (health && health->GetCurLives() > 0)
			isPlayerAlive = true;
	}

	if (isPlayerAlive == false)
	{
		dae::SceneManager::GetInstance().SetActiveScene(dae::SceneName(SceneName::HighscoreScene));
	}

	// stop level transition
	if (m_doingLevelTransition)
	{
		if (m_secPassed >= 1.5f)
		{
			m_doingLevelTransition = false;
			GoToNextLevel();
		}

		return;
	}

	// spawn enemies
	auto activeScene = dae::SceneManager::GetInstance().GetActiveScene();

	// coily
	if (activeScene->GetSceneName() != dae::SceneName(SceneName::VersusScene))
	{
		for (auto& coilySpawn : m_levelParams[m_curLevelParams].coilySpawns)
		{
			if (coilySpawn >= 0 && m_secPassed >= coilySpawn)
			{
				auto coily = Utils::CreateCoily(m_pConnLevel, m_pPlayersMove);
				coily->Start();
				activeScene->Add(std::move(coily));
				coilySpawn = -1;
			}
		}
	}

	// slick & sam
	for (auto& slickSamSpawn : m_levelParams[m_curLevelParams].samSlickSpawns)
	{
		if (slickSamSpawn >= 0 && m_secPassed >= slickSamSpawn)
		{
			auto slickSam = Utils::CreateSlickSam(m_pConnLevel, m_pPlayersMove);
			slickSam->Start();
			activeScene->Add(std::move(slickSam));
			slickSamSpawn = -1;
		}
	}

	// ugg & wrongway
	size_t amUWSpawns{ m_levelParams[m_curLevelParams].uggWrongwaySpawns.size() };
	if (amUWSpawns == 0) return;

	float curSecNeeded{ m_levelParams[m_curLevelParams].uggWrongwaySpawns[m_curUggWrongwayIdx] };
	if (m_secPassed >= curSecNeeded + m_curUggWrongwayThreshold)
	{
		auto uggWrongway = Utils::CreateUggWrongway(m_pConnLevel, m_pPlayersMove);
		uggWrongway->Start();
		activeScene->Add(std::move(uggWrongway));
		++m_curUggWrongwayIdx;
		m_curUggWrongwayIdx %= amUWSpawns;
		m_curUggWrongwayThreshold += curSecNeeded;
	}
}

void QBert::LevelManager::Notify(dae::Event event, dae::Subject*)
{
	if (event.id == dae::make_sdbm_hash("LEVEL_COMPLETED"))
	{
		m_doingLevelTransition = true;
		m_secPassed = 0.f;
		m_pConnLevel->DoLevelCompletionAnim();

		// notify score manager
		event.args->nr = m_pConnLevel->GetActiveDiscAmount();
		GetOwner()->GetSubject()->NotifyObservers(event);
	}
}

void QBert::LevelManager::GoToNextLevel()
{
	if (m_currentRound < 3)
	{
		++m_currentRound;
	}
	else
	{
		m_currentRound = 0;
		++m_currentLevel;
	}

	// if last level hasn't been reached
	size_t Idx{ static_cast<size_t>(m_currentLevel * 4 + m_currentRound) };
	if (Idx < m_levelParams.size())
	{
		++m_curLevelParams;
		m_secPassed = 0.f;
		MarkAllCreaturesForRemoval();

		// reset players
		if (m_pPlayersMove.size() == 1
			|| dae::SceneManager::GetInstance().GetActiveScene()->GetSceneName() == dae::SceneName(SceneName::VersusScene))
		{
			// reset singular qbert player
			m_pPlayersMove[0]->Reset(glm::vec2(0, 0), false);

			// reset possible coily player
			if (m_pPlayersMove.size() > 1)
			{
				int randNr = rand() % 2;
				glm::vec2 spawnTile{0, 0};
				if (randNr == 0) spawnTile.x = 1;
				else spawnTile.y = 1;

				m_pPlayersMove[1]->Reset(spawnTile, false);
			}
		}
		else
		{
			// reset coop qberts
			m_pPlayersMove[0]->Reset(glm::vec2(0, 6), false);
			m_pPlayersMove[1]->Reset(glm::vec2(6, 0), false);
		}

		// reset tiles to new params
		m_pConnLevel->ResetBase(m_levelParams[m_curLevelParams].tileParams,
			m_levelParams[m_curLevelParams].discSpawns);

		// update UI
		m_pLevelNrText->SetText(std::to_string(m_currentLevel + 1));
		m_pRoundNrText->SetText(std::to_string(m_currentRound + 1));
		m_pWinTileIndicator->SetStartPos(glm::vec2(m_pWinTileIndicator->GetSpriteSize().x * m_levelParams[m_curLevelParams].tileParams.tileType,
			m_pWinTileIndicator->GetSpriteSize().y * m_levelParams[m_curLevelParams].tileParams.winTile));
	}
	else
	{
		// go to highscore scene
		dae::SceneManager::GetInstance().SetActiveScene(dae::SceneName(SceneName::HighscoreScene));
	}
}

void QBert::LevelManager::MarkAllCreaturesForRemoval() const
{
	// remove coilys
	auto coilys = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::Coily));
	for (auto coily : coilys)
	{
		GetOwner()->GetSubject()->RemoveObserver(coily->GetComponent<CoilyActorComp>());
		coily->IsEnabled(false);
		coily->MarkForRemoval();
	}

	// remove slick & sams
	auto slickSams = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::SlickSam));
	for (auto slickSam : slickSams)
	{
		GetOwner()->GetSubject()->RemoveObserver(slickSam->GetComponent<SlickSamActorComp>());
		slickSam->IsEnabled(false);
		slickSam->MarkForRemoval();
	}

	// remove ugg & wrongways
	auto uggWrongways = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::UggWrongway));
	for (auto uggWrongway : uggWrongways)
	{
		GetOwner()->GetSubject()->RemoveObserver(uggWrongway->GetComponent<UggWrongwayActor>());
		uggWrongway->IsEnabled(false);
		uggWrongway->MarkForRemoval();
	}
}

void QBert::LevelManager::LoadLevelParams()
{
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	std::filesystem::path data_location = "./Data/";
	if(!std::filesystem::exists(data_location))
		data_location = "../Data/";
#endif

	// open file
	const auto fullPath = data_location/"LevelSetup.json";
	std::ifstream levelSetupFile(fullPath.c_str(), std::ifstream::binary);
	assert(levelSetupFile.is_open() || "Couldn't open file");

	// create needed vars
	Json::Value data;
	Json::CharReaderBuilder readerBuilder;
	std::string errs;

	// parse and close file
	Json::parseFromStream(readerBuilder, levelSetupFile, &data, &errs);
	levelSetupFile.close();

	assert(errs.empty() || "Failed to parse json data");

	for (int Idx{ 1 }; Idx < 4; ++Idx)
	{
		std::string levelName{ "Level" + std::to_string(Idx) };
		ParseLevel(data, levelName);
	}
}

void QBert::LevelManager::ParseLevel(Json::Value& data, const std::string& levelName)
{
	for (int Idx{ 1 }; Idx < 5; ++Idx)
	{
		std::string roundName{ "Round" + std::to_string(Idx) };
		ParseRound(data, levelName, roundName);
	}
}

void QBert::LevelManager::ParseRound(Json::Value& data, const std::string& levelName, const std::string& roundName)
{
	Json::Value round = data[levelName][roundName];

	LevelParams levelParams{};

	// parse enemies
	Json::Value coilySpawns = round["CoilySpawns"];
	for (const auto& spawn : coilySpawns)
	{
		levelParams.coilySpawns.emplace_back(spawn.asFloat());
	}

	Json::Value SamSlickSpawns = round["SamSlickSpawns"];
	for (const auto& spawn : SamSlickSpawns)
	{
		levelParams.samSlickSpawns.emplace_back(spawn.asFloat());
	}

	Json::Value UggWrongwaySpawns = round["UggWrongwaySpawns"];
	for (const auto& spawn : UggWrongwaySpawns)
	{
		levelParams.uggWrongwaySpawns.emplace_back(spawn.asFloat());
	}

	// parse discs
	glm::vec2 spawn{};
	Json::Value DiscSpawn1 = round["DiscSpawn1"];
	spawn.x = DiscSpawn1[0].asFloat();
	spawn.y = DiscSpawn1[1].asFloat();
	levelParams.discSpawns.emplace_back(spawn);

	Json::Value DiscSpawn2 = round["DiscSpawn2"];
	spawn.x = DiscSpawn2[0].asFloat();
	spawn.y = DiscSpawn2[1].asFloat();
	levelParams.discSpawns.emplace_back(spawn);

	Json::Value DiscSpawn3 = round["DiscSpawn3"];
	spawn.x = DiscSpawn3[0].asFloat();
	spawn.y = DiscSpawn3[1].asFloat();
	levelParams.discSpawns.emplace_back(spawn);

	Json::Value DiscSpawn4 = round["DiscSpawn4"];
	spawn.x = DiscSpawn4[0].asFloat();
	spawn.y = DiscSpawn4[1].asFloat();
	levelParams.discSpawns.emplace_back(spawn);

	// tile params
	TileParams tileParams{};
	Json::Value TileParams = round["TileParams"];
	tileParams.tileType = TileParams["TileType"].asInt();
	tileParams.revertable = TileParams["Revertable"].asBool();
	tileParams.startTile = TileParams["StartTile"].asInt();
	tileParams.winTile = TileParams["WinTile"].asInt();
	tileParams.middleTile = TileParams["MiddleTile"].asInt();

	levelParams.tileParams = tileParams;

	m_levelParams.emplace_back(levelParams);
}

void QBert::LevelManager::FreezeCreatures() const
{
	// disable movement during level transition animation
	for (auto player : m_pPlayersMove)
	{
		player->m_isEnabled = false;
	}
}

std::type_index QBert::LevelManager::GetType() const
{
	return typeid(LevelManager);
}