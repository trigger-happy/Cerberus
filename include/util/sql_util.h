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
#include <vector>
#include "data_types.h"
#include "patterns/singleton.h"

using namespace std;
/*!
\brief The SqlUtil class presents an interface for the SQLite database.
This class takes care of the low level interactions with the SQLite DB
(using the QtSql module).
*/
class SqlUtil : public Singleton<SqlUtil>
{
public:
        /*!
        Initialize the connection to the database specified in dbname
        \param dbname A QString containing the path to the database
        \return true on success,false on error.
        \note This function assumes that the database tables have been made.
        */
        bool init ( const QString& dbname );

        /*!
        Add a team to the database.
        \param team_name The name of the team.
        \param school The name of the school.
        \return 0 on success, a QSqlQuery error otherwise.
        */
        int addTeam ( const QString& team_name, const QString& school );

        /*!
        Add a user to the database.
        \param ud A const reference to a filled up UserData struct.
        \return 0 on success, a QSqlQuery error otherwise.
        */
        int addUser ( const UserData& ud );

        /*!
        Returns the score of the user in user_name.
        \param user_name The user_name of the score to query->
        \return The score in double.
        */
        double getScore ( const QString& user_name );

        /*!
        Set the score for user_name to score.
        \param user_name The user name who's score should be updated.
        \param score The score of the user in double.
        \return 0 on success, an error code otherwise.
        */
        int setScore ( const QString& user_name, double score );

        /*!
        Authenticate a user by verifying the match of user_name and password
        in the database.
        \param user_name The user name to verify.
        \param password The password
        \return true if a match was found, false otherwise.
        */
        bool authenticate ( const QString& user_name, const QString& password );

        /*!
        Get all the available teams in the database.
        \param out A reference to a vector\<TeamData\> to output the data to.
        \return true on success, false otherwise.
        */
        bool getTeams ( vector<TeamData>& out );

        /*!
        Get all the available users in the database.
        \param out A reference to a vector\<UserData\> to output the data to.
        \return true on success, false otherwise.
        */
        bool getUsers ( vector<UserData>& out );

        /*!
        Get all the available score data in the database.
        \param out A reference to a vector\<UserData\> to output the data to.
        \return true on success, false otherwise.
        */
        bool getScores ( vector<ScoreData>& out );

        /*!
        Get all the available admin data in the database.
        \param out A reference to a vector\<AdminData\> to output the data to.
        \return true on success, false otherwise.
        */
        bool getAdmins ( vector<AdminData>& out );

        /*!
        Add an administrator account into the database.
        \param a The AdminData to enter into the DB.
        \return true on success, false otherwise.
        */
        bool addAdmin ( const AdminData& a );

        /*!
        Verify if the database tables are existent.
        \param sl QStringList of tables.
        \return true of valid, false otherwise.
        */
        bool verifyDB ( const QStringList& sl );
private:
        /*!
        Create the database tables if they don't exist
        */
        void createDBTables();

        QSqlDatabase db;
        QSqlQuery* query;
};
#endif //SQL_UTIL_H
