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
#ifndef DATA_TYPES_H
#define DATA_TYPES_H
#include <vector>
#include <map>
#include <algorithm>
#include <QString>

using std::vector;
using std::map;
using std::pair;
using std::equal;

struct R1Question {
        bool operator== ( const R1Question& rhs ) {
                if ( this->question != rhs.question ) {
                        return false;
                }
                if ( this->choices.size() == rhs.choices.size() ) {
                        map<int, QString>::const_iterator lit, rit;
                        lit = this->choices.begin();
                        rit = rhs.choices.begin();
                        while ( lit != this->choices.end() ) {
                                if ( lit->first != rit->first || lit->second != rit->second ) {
                                        return false;
                                }
                                lit++;
                                rit++;
                        }
                }
                return true;
        }
        int number;
        int score;
        QString question;
        map<int, QString> choices;
};

struct R1QData {
        bool operator== ( const R1QData& rhs ) {
                if ( this->welcome_msg != rhs.welcome_msg ) {
                        return false;
                }
                if ( this->questions.size() == rhs.questions.size() ) {
                        for ( int i = 0; i < this->questions.size(); i++ ) {
                                if ( ! ( this->questions[i] == rhs.questions[i] ) ) {
                                        return false;
                                }
                        }
                } else {
                        return false;
                }
                return true;
        }
        QString welcome_msg;
        vector<R1Question> questions;
};

typedef map<int,int> R1Answers;

struct NetworkConfig {
        bool operator== ( const NetworkConfig& rhs ) {
                if ( this->ip == rhs.ip && this->port == rhs.port ) {
                        return true;
                }
                return false;
        }
        QString ip;
        int port;
};

struct ClientConfig {
};

struct PresenterConfig {
};

struct AdminConfig {
};

struct StageData {
        bool operator== ( const StageData& rhs ) {
                if ( question_file == rhs.question_file
                     && answer_file == rhs.answer_file ) {
                        return true;
                }
                return false;
        }
        QString question_file;
        QString answer_file;
};

struct ServerConfig {
        bool operator== ( const ServerConfig& rhs ) {
                if ( port == rhs.port && db_path == rhs.db_path ) {
                        if ( stage_data.size() != rhs.stage_data.size() ) {
                                return false;
                        }
                        if ( equal ( stage_data.begin() ,stage_data.end(), rhs.stage_data.begin() ) ) {
                                return true;
                        }
                }
                return false;
        }
        int port;
        QString db_path;
        vector<StageData> stage_data;
};

struct UserData {
        QString user_name;
        QString password;
        QString firstname;
        QString lastname;
        QString teamname;
};

struct TeamData {
        QString teamname;
        QString school;
};

struct ScoreData {
        QString user_name;
        double score;
};

struct AdminData {
        QString user_name;
        QString password;
};

#endif //DATA_TYPES_H
