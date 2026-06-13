#ifndef QBERTSCORECOMPONENT_H
#define QBERTSCORECOMPONENT_H

#include "IObserver.h"
#include "Component.h"
#include "Events.h"

namespace dae
{
	class TextComponent;
}

namespace QBert
{
	class GameObject;
	class ScoreComp final : public dae::Component, public dae::IObserver
	{
	public:
		explicit ScoreComp(dae::GameObject* pOwner, dae::TextComponent* score1Text, dae::TextComponent* score2Text = nullptr);
		~ScoreComp();
		ScoreComp(const ScoreComp& other) = delete;
		ScoreComp(ScoreComp&& other) = delete;
		ScoreComp& operator=(const ScoreComp& other) = delete;
		ScoreComp& operator=(ScoreComp&& other) = delete;

		void Start() override;

		std::type_index GetType() const override;

		void Notify(dae::Event event, dae::Subject*) override;

	private:
		int GetWinScore(int level, int amDiscsUnused) const;

		int m_score1{ 0 };
		int m_score2{ 0 };

		dae::GameObject* m_player1{ nullptr };
		dae::GameObject* m_player2{ nullptr };

		const int m_tileScore{ 25 };
		const int m_coilyScore{ 500 };
		const int m_samSlickScore{ 300 };
		const int m_winScoreBase{ 750 }; 
		const int m_levelScoreMultiplier{ 250 };
		const int m_discScore{ 50 };

		dae::TextComponent* m_pScore1Text;
		dae::TextComponent* m_pScore2Text;
	};
}

#endif // !QBERTSCORECOMPONENT_H

