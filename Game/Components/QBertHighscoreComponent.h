#ifndef QBERTHIGHSCORECOMPONENT_H
#define QBERTHIGHSCORECOMPONENT_H

#include <string>
#include <vector>

#include "Component.h"
#include "IObserver.h"
#include "Events.h"

namespace dae
{
    class TextComponent;
    class Subject;
}

namespace QBert
{
    struct Highscore
    {
        std::string fullString;
        std::string name;
        uint32_t score;
    };

    class QBertHighscoreComp final : public dae::Component, public dae::IObserver
    {
    public:
        QBertHighscoreComp(dae::GameObject* pOwner, std::vector<dae::TextComponent*> hsTextComps,
            std::vector<dae::TextComponent*> letterTextComps, std::vector<dae::GameObject*> arrows,
            dae::TextComponent* scoreTextComp);
        ~QBertHighscoreComp() = default;
        QBertHighscoreComp(const QBertHighscoreComp& other) = delete;
        QBertHighscoreComp(QBertHighscoreComp&& other) = delete;
        QBertHighscoreComp& operator=(const QBertHighscoreComp& other) = delete;
        QBertHighscoreComp& operator=(QBertHighscoreComp&& other) = delete;

        void Start() override;

        void Update() override;
        void Notify(dae::Event event, dae::Subject*) override;

        void PassScores(int score1, int score2 = 0);

        std::type_index GetType() const override;

    private:
        void ReadHighscores();
        void SetTextComps();
        void ConfirmLetter();
        void SetHighscore();
        void SaveHighscore();

        float m_secPassed{ 0.f };

        uint32_t m_scoreP1{ 0 };
        uint32_t m_scoreP2{ 0 };

        size_t m_curLetterIdx{ 0 };
        char m_curChar{ 'a' };
        std::string m_name{};
        bool m_savedHighscore{ false };

        std::vector<dae::TextComponent*> m_pHsTextComps;
        std::vector<dae::TextComponent*> m_pLetterTextComps;
        std::vector<dae::GameObject*> m_arrows;
        std::vector<Highscore> m_highscores{};
        dae::TextComponent* m_pScoreTextComp;
    };
}

#endif //QBERTHIGHSCORECOMPONENT_H
