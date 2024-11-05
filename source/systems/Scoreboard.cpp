/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Scoreboard.cpp
*/

#include "systems/Scoreboard.hpp"

#include <fstream>
#include <iostream>

namespace rtype::system {
Scoreboard::Scoreboard(const std::string &saveFile)
    : m_saveFile(saveFile) {
}

void Scoreboard::init(void) {
    subscribeToEvent<gengine::system::event::StartEngine>(&Scoreboard::loadScores);
    subscribeToEvent<event::GameOver>(&Scoreboard::saveScore);
}

void Scoreboard::loadScores(gengine::system::event::StartEngine &) {
    std::ifstream scoreFile(m_saveFile);
    if (!scoreFile)
        THROW_WARNING("File: " + m_saveFile + " does not exist");

    std::string line;
    while (std::getline(scoreFile, line)) {
        std::istringstream iss(line);
        int number;
        std::string text;

        if (!(iss >> number) || !(iss.ignore(3)) || !std::getline(iss, text))
            continue;

        m_scores.emplace(number, text);
    }

    scoreFile.close();
}

void Scoreboard::saveScore(event::GameOver &e) {
    auto &scores = getComponents<component::Score>();
    auto &players = getComponents<gengine::interface::component::RemoteLocal>();

    int totalScore = 0;

    for (auto [entity, score, player] : gengine::Zip(scores, players))
        totalScore += score.score;

    m_scores.emplace(std::make_pair(totalScore, currentDateTimeString()));

    saveScoresToFile();
}

void Scoreboard::saveScoresToFile(void) {
    std::ofstream scoreFile(m_saveFile);
    if (!scoreFile)
        THROW_WARNING("File: " + m_saveFile + " does not exist");

    for (const auto &[score, date] : m_scores)
        scoreFile << score << " - " << date << std::endl;

    scoreFile.close();
}

std::string currentDateTimeString(void) {
    auto now = std::chrono::system_clock::now();

    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm tm;
#ifdef _WIN32
    localtime_s(&tm, &now_time); // Windows-specific thread-safe local time
#else
    localtime_r(&now_time, &tm); // POSIX (Linux, macOS)
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
} // namespace rtype::system