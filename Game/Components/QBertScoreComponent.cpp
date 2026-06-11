#include "QBertScoreComponent.h"
#include "GameObject.h"
#include "LevelManager.h"
#include "QBertHighscoreComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Tags.h"

QBert::ScoreComp::ScoreComp(dae::GameObject* pOwner, const std::vector<dae::SpriteComp*>& numberSprites)
	: dae::Component(pOwner)
	, m_pConnSpriteComps(numberSprites)
{ }

QBert::ScoreComp::~ScoreComp()
{
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::Player));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->RemoveObserver(this);
	}

	auto highscoreComp = dae::SceneManager::GetInstance().GetActiveScene()->GetFirstObjectByType<QBertHighscoreComp>();
	if (highscoreComp)
		highscoreComp->PassScores(m_score);
}

void QBert::ScoreComp::Start()
{
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::Player));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->AddObserver(this);
	}

	auto levelManager = dae::SceneManager::GetInstance().GetActiveScene()->GetFirstObjectByType<LevelManager>();
	levelManager->GetOwner()->GetSubject()->AddObserver(this);
}

void QBert::ScoreComp::Notify(dae::Event event, dae::Subject*)
{
	int startScore{ m_score };

	switch (event.id)
	{
	case dae::make_sdbm_hash("TILE_TURNED"):
		m_score += m_tileScore;
		break;
	case dae::make_sdbm_hash("COILY_KILLED"):
		m_score += m_coilyScore;
		break;
	case dae::make_sdbm_hash("SAM_SLICK_KILLED"):
		m_score += m_samSlickScore;
		break;
	case dae::make_sdbm_hash("BALL_CAUGHT"):
		m_score += m_greenBallScore;
		break;
	case dae::make_sdbm_hash("LEVEL_COMPLETED"):
		m_score += GetWinScore();
		break;
	}

	if (startScore == m_score)
		return;


	int sizeChecker{ 10000 };
	int divider{ 10 };
	int idx{ static_cast<int>(m_pConnSpriteComps.size()) };
	for (auto it = m_pConnSpriteComps.rbegin(); it != m_pConnSpriteComps.rend(); ++it) 
	{
		--idx;

		if (m_score / sizeChecker != 0)
		{
			int digit{ (m_score % divider) / (divider / 10) };
			m_pConnSpriteComps[idx]->GetOwner()->IsEnabled(true);
			m_pConnSpriteComps[idx]->SetCurFrame(digit);

			divider *= 10;
		}

		sizeChecker /= 10;
	}
}

int QBert::ScoreComp::GetWinScore() const
{
	return static_cast<int>(m_winScoreBase + (m_levelScoreMultiplier /* * level number */) + m_discScore * 0 /*amount of discs unused */ );
}

std::type_index QBert::ScoreComp::GetType() const
{
	return typeid(ScoreComp);
}
