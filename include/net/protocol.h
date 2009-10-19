/*
Copyright (C) 2009 James Choa

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <QtGlobal>

#define PROTOCOL_MAJOR 0
#define PROTOCOL_MINOR 0
#define PROTOCOL_PATCH 1

enum CLIENT_ID {
        CLIENT_CONTESTANT = 0,
        CLIENT_PRESENTER,
        CLIENT_ADMIN
};

enum PROTOCOL_COMMAND {
        // error
        INF_ERROR = 0,

        // connection
        NET_INITIATE_CONNECTION,
        NET_CONNECTION_RESULT,
        NET_DISCONNECT,

        // authentication
        QRY_AUTHENTICATE,
        INF_AUTHENTICATE,

        // contest state
        QRY_CONTEST_STATE,
        INF_CONTEST_STATE,

        // question request
        QRY_QUESTION_REQUEST,
        INF_QUESTION_DATA,

        // answer submission
        QRY_ANSWER_SUBMIT,
        INF_ANSWER_REPLY

        // TODO: Add presenter commands here

        // TODO: Add admin commands here
};

enum CONTEST_STATUS {
        CONTEST_RUNNING = 0,
        CONTEST_STOPPED,
        CONTEST_PAUSED
};

struct p_version {
        p_version() {
                major = PROTOCOL_MAJOR;
                minor = PROTOCOL_MINOR;
                patch = PROTOCOL_PATCH;
        }
        uchar major;
        uchar minor;
        uchar patch;
};

struct p_ident {
        p_ident() {
                data[0] = 'C';
                data[1] = 'E';
                data[2] = 'R';
                data[3] = 'B';
        }
        unsigned char data[4];
};

struct p_header {
        unsigned short length;
        p_ident ident;
        p_version ver;
        unsigned char command;
};

bool is_proto_current ( const p_version& ver );

#endif //PROTOCOL_H
