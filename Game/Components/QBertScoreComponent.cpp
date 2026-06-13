#include "QBertScoreComponent.h"
#include "GameObject.h"
#include "LevelManager.h"
#include "QBertHighscoreComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Tags.h"
#include "TextComponent.h"

QBert::ScoreComp::ScoreComp(dae::GameObject* pOwner, dae::TextComponent* score1Text, dae::TextComponent* score2Text)
	: dae::Component(pOwner)
	, m_pScore1Text(score1Text)
	, m_pScore2Text(score2Text)
{ }

QBert::ScoreComp::~ScoreComp()
{
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::QBert));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->RemoveObserver(this);
	}

	if (dae::SceneManager::GetInstance().GetActiveScene()->GetSceneName() != dae::SceneName(SceneName::HighscoreScene))
	{
		return;
	}

	auto highscoreComp = dae::SceneManager::GetInstance().GetActiveScene()->GetFirstObjectByType<QBertHighscoreComp>();
	if (highscoreComp)
	{
		if (m_player2) highscoreComp->PassScores(m_score1, m_score2);
		else highscoreComp->PassScores(m_score1);
	}
}

void QBert::ScoreComp::Start()
{
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::QBert));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->AddObserver(this);
	}

	m_player1 = qberts[0];
	if (qberts.size() > 1) m_player2 = qberts[1];
	else
	{
		auto coilyPlayer = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::PlayerCoily));
		if (!coilyPlayer.empty()) m_player2 = coilyPlayer[0];
	}

	auto levelManager = dae::SceneManager::GetInstance().GetActiveScene()->GetFirstObjectByType<LevelManager>();
	levelManager->GetOwner()->GetSubject()->AddObserver(this);
}

void QBert::ScoreComp::Notify(dae::Event event, dae::Subject* subject)
{
	int startScore1{ m_score1 };
	int startScore2{ m_score2 };

	switch (event.id)
	{
	case dae::make_sdbm_hash("TILE_TURNED"):
		if (subject->GetOwner() == m_player1)
		{
			m_score1 += m_tileScore;
		}
		else if (subject->GetOwner() == m_player2)
		{
			m_score2 += m_tileScore;
		}
		break;

	case dae::make_sdbm_hash("COILY_KILLED"):
		m_score1 += m_coilyScore;

		if (m_player2 &&
			m_player2->GetTag() != dae::Tag(Tag::PlayerCoily))
		{
			m_score2 += m_coilyScore;
		}
		break;

	case dae::make_sdbm_hash("SAM_SLICK_KILLED"):
		if (event.args->object == m_player1)
		{
			m_score1 += m_tileScore;
		}
		else if (event.args->object == m_player2)
		{
			m_score2 += m_tileScore;
		}
		break;

	case dae::make_sdbm_hash("QBERT_KILLED"):
		if (subject->GetOwner() == m_player1
			&& m_player2
			&& m_player2->GetTag() == dae::Tag(Tag::PlayerCoily))
		{
			m_score2 += m_coilyScore;
		}
		break;

	case dae::make_sdbm_hash("LEVEL_COMPLETED"):
		m_score1 += GetWinScore(event.args[0].nr, event.args[1].nr);
		if (m_player2 && m_player2->GetTag() != dae::Tag(Tag::PlayerCoily))
			m_score2 += GetWinScore(event.args[0].nr, event.args[1].nr);
		break;
	}

	if (startScore1 == m_score1 && startScore2 == m_score2) return;

	m_pScore1Text->SetText(std::to_string(m_score1));
	if (m_pScore2Text) m_pScore2Text->SetText(std::to_string(m_score2));
}

int QBert::ScoreComp::GetWinScore(int level, int amDiscsUnused) const
{
	return static_cast<int>(m_winScoreBase + (m_levelScoreMultiplier * level) + m_discScore * amDiscsUnused );
}

std::type_index QBert::ScoreComp::GetType() const
{
	return typeid(ScoreComp);
}
