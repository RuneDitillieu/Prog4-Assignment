#include "LevelManager.h"

#include "DeltaTime.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Tags.h"
#include "QBertMoveComponent.h"
#include "InitUtils.h"

QBert::LevelManager::LevelManager(dae::GameObject* pOwner, LevelBase* level)
	: dae::Component(pOwner)
	, m_pConnLevel(level)
{
	LevelParams levelParams{};

	// 1 1
	levelParams.coilySpawns = std::vector<float>{ 1.f };
	levelParams.samSlickSpawns = std::vector<float>{ 3.f };
	levelParams.discSpawns = std::vector<glm::vec2>{ {-1, 4}, {4, -1} };
	levelParams.tileParams = TileParams{0, false, 1, 0};
	m_levelParams.emplace_back(levelParams);

	// 1 2
	levelParams.coilySpawns = std::vector<float>{ 1.f };
	levelParams.discSpawns = std::vector<glm::vec2>{{-1, 5}, {6, -1}};
	levelParams.tileParams = TileParams{ 1, false, 1, 0 };
	m_levelParams.emplace_back(levelParams);
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
		m_pPlayers.emplace_back(player->GetComponent<QBertMoveComp>());
	}
}

void QBert::LevelManager::Update()
{
	m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();

	auto activeScene = dae::SceneManager::GetInstance().GetActiveScene();

	int Idx{ -1 };
	for (auto& coilySpawn : m_levelParams[m_curLevelParams].coilySpawns)
	{
		++Idx;
		if (coilySpawn >= 0 && m_secPassed >= coilySpawn)
		{
			auto coily = Utils::CreateCoily(m_pConnLevel, m_pPlayers[0]);
			coily->Start();
			activeScene->Add(std::move(coily));
			coilySpawn = -1;
		}
	}
	Idx = -1;

	for (auto& slickSamSpawn : m_levelParams[m_curLevelParams].samSlickSpawns)
	{
		++Idx;
		if (slickSamSpawn >= 0 && m_secPassed >= slickSamSpawn)
		{
			auto slickSam = Utils::CreateSlick(m_pConnLevel, m_pPlayers[0]);
			slickSam->Start();
			activeScene->Add(std::move(slickSam));
			slickSamSpawn = -1;
		}
	}
	Idx = -1;

	for (auto& uggWrongwaySpawn : m_levelParams[m_curLevelParams].uggWrongwaySpawns)
	{
		++Idx;
		if (uggWrongwaySpawn >= 0 && m_secPassed >= uggWrongwaySpawn)
		{

			uggWrongwaySpawn = -1;
		}
	}
}

void QBert::LevelManager::Notify(dae::Event event, dae::Subject*)
{
	if (event.id == dae::make_sdbm_hash("LEVEL_COMPLETED"))
	{
		GoToNextLevel();

		// notify score manager
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

		for (auto player : m_pPlayers)
		{
			player->Reset(glm::vec2(0, 0), false);
		}

		m_pConnLevel->ResetBase(m_levelParams[m_curLevelParams].tileParams);
	}
}

void QBert::LevelManager::MarkAllCreaturesForRemoval() const
{
	auto coilys = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::Coily));
	for (auto coily : coilys)
	{
		coily->IsEnabled(false);
		coily->MarkForRemoval();
	}

	auto slickSams = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::SlickSam));
	for (auto slickSam : slickSams)
	{
		slickSam->IsEnabled(false);
		slickSam->MarkForRemoval();
	}

	auto uggWrongways = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::UggWrongway));
	for (auto uggWrongway : uggWrongways)
	{
		uggWrongway->IsEnabled(false);
		uggWrongway->MarkForRemoval();
	}
}

std::type_index QBert::LevelManager::GetType() const
{
	return typeid(LevelManager);
}