/*
Copyright (C) 2009 Janlebrad Ang

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
#ifndef SQL_UTIL_H
#define SQL_UTIL_H
#include <QtSql/QtSql>
#include "data_types.h"
#include "patterns/singleton.h"

class SqlUtil : public QObject, public Singleton<SqlUtil>
{
        Q_OBJECT
public:
        bool init ( const QString& dbname );
        int addTeam ( const QString& team_name, const QString& school );
        int addUser ( const UserData& ud );
        double getScore ( const QString& user_name );
        void setScore ( const QString& user_name, int score );
        bool authenticate ( const QString& user_name, const QString& password );
public slots:
private:
        QSqlDatabase db;
        QSqlQuery* query;
};
#endif //SQL_UTIL_H
