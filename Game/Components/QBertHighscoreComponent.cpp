#include "QBertHighscoreComponent.h"

#include <filesystem>

#include "GameObject.h"
#include "TextComponent.h"
#include <fstream>

#include "Colors.h"

QBert::QBertHighscoreComp::QBertHighscoreComp(dae::GameObject* pOwner, std::vector<dae::TextComponent*> hsTextComps,
    std::vector<dae::TextComponent*> letterTextComps, std::vector<dae::GameObject*> arrows)
    : Component(pOwner)
    , m_pHsTextComps(hsTextComps)
    , m_pLetterTextComps(letterTextComps)
    , m_arrows(arrows)
{ }

void QBert::QBertHighscoreComp::Start()
{
    ReadHighscores();
    SetTextComps();
}

void QBert::QBertHighscoreComp::Notify(dae::Event event, dae::Subject*)
{
    if (m_savedHighscore)
        return;

    std::string c{};
    switch (event.id)
    {
    case dae::make_sdbm_hash("OnPreviousPressed"):
        if (m_curChar == '.')
        {
            m_curChar = 'a';
        }

        --m_curChar;
        if (m_curChar < 'a')
        {
            m_curChar = 'z';
        }
        c = m_curChar;
        m_pLetterTextComps[m_curLetterIdx]->SetText(c);
        break;
    case dae::make_sdbm_hash("OnNextPressed"):
        if (m_curChar == '.')
        {
            m_curChar = 'z';
        }

        ++m_curChar;
        if (m_curChar > 'z')
        {
            m_curChar = 'a';
        }
        c = m_curChar;
        m_pLetterTextComps[m_curLetterIdx]->SetText(c);
        break;
    case dae::make_sdbm_hash("OnConfirmPressed"):
        ConfirmLetter();
        break;
    }
}

void QBert::QBertHighscoreComp::ConfirmLetter()
{
    if (m_curLetterIdx < m_pLetterTextComps.size() - 1 && m_curChar != '.')
    {
        m_name = m_name + m_curChar;

        m_pLetterTextComps[m_curLetterIdx]->SetColor(QBert::GREEN);
        ++m_curLetterIdx;
        m_pLetterTextComps[m_curLetterIdx]->SetColor(QBert::ORANGE);
        m_curChar = '.';

        std::string c{m_curChar};
        m_pLetterTextComps[m_curLetterIdx]->SetText(c);

        glm::vec3 diff{ m_pLetterTextComps[m_curLetterIdx]->GetOwner()->GetLocalPosition()
        - m_pLetterTextComps[m_curLetterIdx - 1]->GetOwner()->GetLocalPosition() };
        for (auto arrow : m_arrows)
        {
            arrow->SetLocalPosition(arrow->GetLocalPosition() + diff);
        }
    }
    else if (m_curLetterIdx != 0)
    {
        m_savedHighscore = true;
        if (m_curChar != '.')
        {
            m_name = m_name + m_curChar;
        }

        m_pLetterTextComps[m_curLetterIdx]->SetColor(QBert::GREEN);
        for (auto arrow : m_arrows)
        {
            arrow->IsEnabled(false);
        }
        SetHighscore();
    }
}

void QBert::QBertHighscoreComp::SetHighscore()
{
    std::string highscore = m_name + " ";

    if (m_scoreP1 < 10000)
        highscore += "0";
    if (m_scoreP1 < 1000)
        highscore += "0";
    if (m_scoreP1 < 100)
        highscore += "0";
    if (m_scoreP1 < 10)
        highscore += "0";

    highscore += std::to_string(m_scoreP1);

    m_highscores.emplace_back(Highscore{.fullString = highscore, .name = m_name, .score = m_scoreP1});
    SetTextComps();
    SaveHighscore();
}

void QBert::QBertHighscoreComp::SaveHighscore()
{
#if __EMSCRIPTEN__
    fs::path data_location = "";
#else
    std::filesystem::path data_location = "./Data/";
    if(!std::filesystem::exists(data_location))
        data_location = "../Data/";
#endif

    const auto fullPath = data_location/"Highscores.txt";
    std::ofstream highscoreFile(fullPath.c_str());
    for (auto hs : m_highscores)
    {
        highscoreFile << hs.fullString << '\n';
    }
}

void QBert::QBertHighscoreComp::PassScores(int score1, int score2)
{
    m_scoreP1 = score1;
    m_scoreP2 = score2;
}

void QBert::QBertHighscoreComp::ReadHighscores()
{
#if __EMSCRIPTEN__
    fs::path data_location = "";
#else
    std::filesystem::path data_location = "./Data/";
    if(!std::filesystem::exists(data_location))
        data_location = "../Data/";
#endif

    const auto fullPath = data_location/"Highscores.txt";
    std::ifstream highscoreFile(fullPath.c_str());
    std::string highscoreStr;

    while (getline(highscoreFile, highscoreStr))
    {
        Highscore highscore{};
        highscore.fullString = highscoreStr;

        std::stringstream s(highscoreStr);
        std::getline(s, highscore.name, ' ');

        std::string scoreStr;
        std::getline(s, scoreStr);

        if (std::all_of(std::begin(scoreStr), std::end(scoreStr),
        [](unsigned char c) { return std::isdigit(c); }))
        {
            highscore.score = stoi(scoreStr);
        }

        m_highscores.emplace_back(highscore);
    }
}

void QBert::QBertHighscoreComp::SetTextComps()
{
    std::sort(m_highscores.begin(), m_highscores.end(),
        [](const Highscore& s1, const Highscore& s2){ return s1.score > s2.score; });

    size_t Idx{ 0 };
    for (Highscore& highscore : m_highscores)
    {
        if (Idx >= m_pHsTextComps.size())
        {
            m_highscores.erase(m_highscores.begin() + Idx, m_highscores.end());
            break;
        }

        m_pHsTextComps[Idx]->SetText(highscore.fullString);

        ++Idx;
    }
}

std::type_index QBert::QBertHighscoreComp::GetType() const
{
    return typeid(QBertHighscoreComp);
}
