/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** Leaderboard.hpp
*/

#pragma once

#include <ctime>
#include <set>

#include "GEngine/interface/components/RemoteLocal.hpp"
#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

#include "components/Score.hpp"
#include "events/GameOver.hpp"

namespace rtype::system {

class Scoreboard : public gengine::System<Scoreboard, component::Score, gengine::interface::component::RemoteLocal>,
                   public gengine::RemoteSystem {
public:
    Scoreboard(const std::string &saveFile);

    void init(void) override;

    void loadScores(gengine::system::event::StartEngine &);
    void saveScore(event::GameOver &);

    void saveScoresToFile(void);

private:
    std::string m_saveFile;

    struct CompareByInt {
        bool operator()(const std::pair<int, std::string> &a, const std::pair<int, std::string> &b) const {
            return a.first > b.first;
        }
    };
    std::multiset<std::pair<int, std::string>, CompareByInt> m_scores;
};

std::string currentDateTimeString(void);
} // namespace rtype::system