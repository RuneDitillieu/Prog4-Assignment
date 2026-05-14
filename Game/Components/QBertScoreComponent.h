#ifndef QBERTSCORECOMPONENT_H
#define QBERTSCORECOMPONENT_H

#include "Observers.h"
#include "Component.h"
#include "SpriteComponent.h"
#include "Events.h"

namespace QBert
{
	class GameObject;
	class ScoreComp : public dae::Component, public dae::IObserver
	{
	public:
		ScoreComp(dae::GameObject* pOwner, const std::vector<dae::SpriteComp*>& numberSprites);
		~ScoreComp() = default;
		ScoreComp(const ScoreComp& other) = delete;
		ScoreComp(ScoreComp&& other) = delete;
		ScoreComp& operator=(const ScoreComp& other) = delete;
		ScoreComp& operator=(ScoreComp&& other) = delete;

		std::type_index GetType() const override;

		void Notify(dae::Event event, dae::Subject*) override;

	private:
		int GetWinScore() const;

		int m_score{ 0 };

		const int m_tileScore{ 25 };
		const int m_coilyScore{ 500 };
		const int m_samSlickScore{ 300 };
		const int m_greenBallScore{ 100 };
		const int m_winScoreBase{ 750 }; 
		const int m_levelScoreMultiplier{ 250 };
		const int m_discScore{ 50 };

		std::vector<dae::SpriteComp*> m_pConnSpriteComps;
	};
}

#endif // !QBERTSCORECOMPONENT_H

