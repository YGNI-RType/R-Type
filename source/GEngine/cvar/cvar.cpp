/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** cvar
*/

#include "GEngine/cvar/cvar.hpp"

namespace CVar {

std::unordered_map<std::string, ConVar *> ConVar::g_hmAllCommands = {};
std::unordered_map<std::string, ConVar *> ConVar::g_hmPublicCommands = {};

ConVar sv_cheats("sv_cheats", "0", ConVar::CHEAT | ConVar::SERVER);

////////////////////////////////////////

ConVar::ConVar(const std::string &name, const std::string &value, short flags, const std::string &description) : m_value(value), m_intValue(std::stoi(value)), m_description(description), m_flags(flags) {
    g_hmAllCommands[name] = this;

    if (flags & ROM || flags & CMD)
        return;
    g_hmPublicCommands[name] = this;
}

void ConVar::setValue(const std::string &value, bool isUserInput) {
    if (isUserInput) {
        if (m_flags & ROM || m_flags & CMD)
            return;

        #ifndef SERVER
            if (m_flags & CHEAT && !sv_cheats.getIntValue())
                return;
            if (m_flags & SERVER)
                return; // TODO: notifies the server that the client want the value changed !!
        #endif
    }

    m_value = value;
}

} // namespace RType
