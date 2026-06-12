#include "LevelManager.h"

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

QBert::LevelManager::LevelManager(dae::GameObject* pOwner, LevelBase* level, dae::SpriteComp* tileIconSprite)
	: dae::Component(pOwner)
	, m_pConnLevel(level)
	, m_pWinTileIndicator(tileIconSprite)
{
	LevelParams levelParams{};

	// 1 1
	levelParams.coilySpawns = std::vector<float>{ 1.f };
	levelParams.samSlickSpawns = std::vector<float>{ 3.f };
	levelParams.discSpawns = std::vector<glm::vec2>{ {-1, 4}, {4, -1} };
	levelParams.tileParams = TileParams{0, false, 0, 1};
	m_levelParams.emplace_back(levelParams);

	// 1 2
	levelParams.coilySpawns = std::vector<float>{ 1.f };
	levelParams.discSpawns = std::vector<glm::vec2>{{-1, 5}, {6, -1}};
	levelParams.tileParams = TileParams{ 1, true, 1, 0 };
	m_levelParams.emplace_back(levelParams);

	auto activeScene = dae::SceneManager::GetInstance().GetActiveScene();

	std::vector<DiscActorComp*> discs{};
	auto disc = Utils::CreateDisc(m_levelParams[0].discSpawns[0], m_pConnLevel);
	discs.emplace_back(disc->GetComponent<DiscActorComp>());
	activeScene->Add(std::move(disc));

	disc = Utils::CreateDisc(m_levelParams[0].discSpawns[1], m_pConnLevel);
	discs.emplace_back(disc->GetComponent<DiscActorComp>());
	activeScene->Add(std::move(disc));

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
	auto players = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::Player));
	for (auto player : players)
	{
		m_pPlayersMove.emplace_back(player->GetComponent<QBertMoveComp>());
	}
}

void QBert::LevelManager::Update()
{
	m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();

	if (m_doingLevelTransition)
	{
		if (m_secPassed >= 1.5f)
		{
			m_doingLevelTransition = false;
			GoToNextLevel();
		}

		return;
	}

	auto activeScene = dae::SceneManager::GetInstance().GetActiveScene();

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

	for (auto& uggWrongwaySpawn : m_levelParams[m_curLevelParams].uggWrongwaySpawns)
	{
		if (uggWrongwaySpawn >= 0 && m_secPassed >= uggWrongwaySpawn)
		{
			auto uggWrongway = Utils::CreateUggWrongway(m_pConnLevel, m_pPlayersMove);
			uggWrongway->Start();
			activeScene->Add(std::move(uggWrongway));
			uggWrongwaySpawn = -1;
		}
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

	size_t Idx{ static_cast<size_t>(m_currentLevel * 3 + m_currentRound) };
	if (Idx < m_levelParams.size())
	{
		++m_curLevelParams;
		m_secPassed = 0.f;
		MarkAllCreaturesForRemoval();

		if (m_pPlayersMove.size() == 1)
		{
			m_pPlayersMove[0]->Reset(glm::vec2(0, 0), false);
		}
		else
		{
			m_pPlayersMove[0]->Reset(glm::vec2(0, 6), false);
			m_pPlayersMove[1]->Reset(glm::vec2(6, 0), false);
		}

		m_pConnLevel->ResetBase(m_levelParams[m_curLevelParams].tileParams,
			m_levelParams[m_curLevelParams].discSpawns[0], m_levelParams[m_curLevelParams].discSpawns[1]);
		m_pWinTileIndicator->SetStartPos(glm::vec2(m_pWinTileIndicator->GetSpriteSize().x * m_levelParams[m_curLevelParams].tileParams.tileType,
			m_pWinTileIndicator->GetSpriteSize().y * m_levelParams[m_curLevelParams].tileParams.winTile));
	}
	else
	{
		dae::SceneManager::GetInstance().SetActiveScene(dae::SceneName(SceneName::HighscoreScene));
	}
}

void QBert::LevelManager::MarkAllCreaturesForRemoval() const
{
	auto coilys = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::Coily));
	for (auto coily : coilys)
	{
		GetOwner()->GetSubject()->RemoveObserver(coily->GetComponent<CoilyActorComp>());
		coily->IsEnabled(false);
		coily->MarkForRemoval();
	}

	auto slickSams = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::SlickSam));
	for (auto slickSam : slickSams)
	{
		GetOwner()->GetSubject()->RemoveObserver(slickSam->GetComponent<SlickSamActorComp>());
		slickSam->IsEnabled(false);
		slickSam->MarkForRemoval();
	}

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

}

void QBert::LevelManager::FreezeCreatures() const
{
	for (auto player : m_pPlayersMove)
	{
		player->m_isEnabled = false;
	}
}

std::type_index QBert::LevelManager::GetType() const
{
	return typeid(LevelManager);
}