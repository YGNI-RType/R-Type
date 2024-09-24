/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-basile.fouquet (Workspace)
** File description:
** msg_tcp_structs
*/

#pragma once

namespace Network
{
    enum {
        SV_DOWNLOAD,
        SV_UPLOAD, // TODO : add resriction on files (size, type, etc)
    };

    enum {
        CL_CONNECT_INFORMATION,
        CL_DOWNLOAD,
        CL_UPLOAD,
    };
} // namespace Network
