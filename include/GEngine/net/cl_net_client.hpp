/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet
** File description:
** cl_net_client
*/

#pragma once

#include "net_common.hpp"
#include "net_channel.hpp"

namespace Network
{
    class CLNetClient {
        public:
            CLNetClient();
            ~CLNetClient() = default;

            void init(void);
            void stop(void);

            void connectToServer(const Address &serverAddress);
            void disconnectFromServer(void);

            void handleTCPEvents(fd_set &readSet);
            void handleUDPEvents(SocketUDP &socket, const UDPMessage &msg, const Address &addr);

            void setChallenge(int challenge) { m_challenge = challenge; }
            int getChallenge(void) const { return m_challenge; }

        public:
            void pingLanServers(SocketUDP &socketUDP, AddressType type);
            void getPingResponse(const UDPMessage &msg, const Address &addr);

        private:
            int m_challenge = -1;

            bool enabled = false;

            SocketTCP m_socket;

            NetChannel m_netChannel;
            std::vector<UDPSV_PingResponse> m_pingedServers;
    };
} // namespace Network
