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
#include "util/sql_util.h"
#include <QCryptographicHash>

bool SqlUtil::init ( const QString& dbname )
{
        db = QSqlDatabase::addDatabase ( "QSQLITE", "contestdb" ); // contestdb = conenction name
        db.setHostName ( "server" );
        db.setDatabaseName ( dbname );
        db.setUserName ( "admin" );
        db.setPassword ( "admin" );
        bool ok = db.open();
        query = new QSqlQuery ( db );

        return ok;
}

int SqlUtil::addTeam ( const QString& team_name, const QString& school )
{
        return query->exec ( "INSERT INTO team(team_name, school) "
                             "VALUES ('" + team_name + "', '" + school + "')" );
}

int SqlUtil::addUser ( const UserData& ud )
{
        QString pwd = QCryptographicHash::hash ( ud.password.toAscii(), QCryptographicHash::Sha1 );
        QString sql = QString ( "INSERT INTO user(username, team_name, "
                                "firstname, lastname, password) "
                                "VALUES ('%1', '%2', '%3', '%4', '%5')" )
                      .arg ( QString ( ud.user_name ) )
                      .arg ( QString ( ud.teamname ) )
                      .arg ( QString ( ud.firstname ) )
                      .arg ( QString ( ud.lastname ) )
                      .arg ( QString ( pwd ) );
        return query->exec ( sql );
}

double SqlUtil::getScore ( const QString& user_name )
{
        query->exec ( "SELECT score FROM user u, scores s "
                      "WHERE s.username = '" + user_name + "' AND u.username = s.username" );
        query->next();
        double score = query->value ( 0 ).toDouble();
        return score;
}

int SqlUtil::setScore ( const QString& user_name, double score )
{
        QString sql = QString ( "" );
        int size = 0;
        int result = 0;
        result = query->exec ( "SELECT username FROM user "
                               "WHERE username = '" + user_name + "'" );
        if ( result != 0 ) {
                return result;
        }
        while ( query->next() ) {
                size++;
        }
        if ( size == 1 ) {
                result = query->exec ( "SELECT username FROM scores "
                                       "WHERE username = '" + user_name + "'" );
                if ( result != 0 ) {
                        return result;
                }
                size = 0;
                while ( query->next() ) {
                        size++;
                }
                if ( size == 1 ) {
                        sql = QString ( "UPDATE scores SET score = '%1' "
                                        "WHERE username='%2'" ).arg ( score ).arg ( user_name );
                } else {
                        sql = QString ( "INSERT INTO scores (username, score) "
                                        "VALUES ('%1','%2')" )
                              .arg ( user_name )
                              .arg ( score );
                }
        } else {
                return -1;
        }
        return query->exec( sql );
}

bool SqlUtil::authenticate ( const QString& user_name, const QString& password )
{
        QString sql = QString ( "SELECT username FROM user WHERE username = '%1' AND password = '%2'" )
                      .arg ( user_name )
                      .arg ( password );
        query->exec ( sql );
        int size = 0;
        while ( query->next() ) {
                size++;
        }
        if ( size == 1 )
                return true;
        else
                return false;
}
