/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** net_client
*/

#pragma once

#include "net_channel.hpp"

#include <memory>
#include <string>
#include <vector>

namespace Network {

#define MAX_SZ_NAME 64
#define PACKET_BACKUP 32

/*
this info is transmitted to network, WE NEED TO USE C ARRAYS
this is global TO ALL TYPE OF GAMES, we need to set custom data information one day
*/
class NetClientInfo {
public:
    NetClientInfo() = default;
    ~NetClientInfo() = default;

    void setName(const std::string &name);
    const std::string &getName(void) const;

private:
    char m_name[MAX_SZ_NAME];
};

/* Since the engine and the snapshots are not defined in advance, we store them in heap with a predefined size */
class NetClientSnapshot {
public:
    NetClientSnapshot(size_t size);
    ~NetClientSnapshot() = default;

private:
    std::vector<byte_t> m_data;
};

/* likely composed */
class NetClient {

public:
    NetClient(std::unique_ptr<Address> addr, SocketTCP &socket);
    ~NetClient() = default;

    const NetChannel &getChannel(void) const { return m_channel; }

private:
    NetChannel m_channel;

    int m_challenge = -1; /* challenge for authoring / avoid ddos */
    clientState m_state = CS_FREE;
    connectionState m_connectionState = CON_UNINITIALIZED;

    // NetClientSnapshot m_snapshots[PACKET_BACKUP];

    /* sends CMD, not any data */

    uint16_t ping;
    uint16_t rate;         /* bytes per second */
    uint16_t snapshotRate; /* rate of snapshots (msec)*/
};
} // namespace Network
