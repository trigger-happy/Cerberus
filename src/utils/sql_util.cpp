/*
Copyright (C) 2009 Janlebrad Ang, Ken Lee and Nicole Guloy

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
#include <iostream>

bool SqlUtil::init ( const QString& dbname )
{
        db = QSqlDatabase::addDatabase ( "QSQLITE", "contestdb" ); // contestdb = conenction name
        db.setHostName ( "server" );
        db.setDatabaseName ( dbname );
        db.setUserName ( "admin" );
        db.setPassword ( "admin" );
        bool ok = db.open();
        query = new QSqlQuery ( db );
        QStringList sl = db.tables();
        if ( !verifyDB ( sl ) ) {
                createDBTables();
        }
        return ok;
}

void SqlUtil::createDBTables()
{
        // assume that db is connected but does not contain the valid tables.
        QFile file ( "resources/server.sql" );
        QString s;
        file.open ( QIODevice::ReadOnly );
        s = file.readAll();
        query->exec ( s );
}

void SqlUtil::addTeam ( const QString& team_name, const QString& school )
{
		//check input values here if they are all valid?
		if (countTeamsPerSchool(school) >= MAX_TEAMS_PER_SCHOOL){
			//QString& temp = new QString();
			throw SqlUtilException(QString::fromStdString("Cannot add team (max 2 teams per school)"));
		}
		else{
			bool success =  query->exec ( "INSERT INTO team(team_name, school) "
								 "VALUES ('" + team_name + "', '" + school + "')" );
			if (!success)
				throw SqlUtilException(query->lastError().databaseText());
		}
}

void SqlUtil::addUser(const QString& user_name, const QString& team_name)
{
	if (countUsersPerTeam(team_name) >= MAX_USERS_PER_TEAM){
		//QString temp = new QString();
		throw SqlUtilException(QString::fromStdString("cannot add user (max 2 users per team)"));
	}
	else{
		//insert into user(username,team_name,firstname,lastname,password) values ('123','team','','','')
		QString sql = QString ( "INSERT INTO user(username, team_name, firstname, lastname, password) "
					"VALUES ('%1','%2','','','')" )
				  .arg ( QString (user_name) )
				  .arg ( QString (team_name) );
		bool success = query->exec( sql );
		if (!success)
				throw SqlUtilException(query->lastError().databaseText());
	}
}

//not used, i think
void SqlUtil::addUser ( const UserData& ud )
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
		bool success= query->exec ( sql );
		if (!success)
			throw SqlUtilException(query->lastError().databaseText());
}

void SqlUtil::editUser ( const QString& user_name, const UserData& ud )
{
	//update user set firstname = 'A', lastname = 'B' , password = 'C' where username = 'username';
	QString pwd = QCryptographicHash::hash ( ud.password.toAscii(), QCryptographicHash::Sha1 );
	QString sql = QString ( "UPDATE user SET firstname = '%1', lastname = '%2', password = '%3' WHERE username = '%4'" )
		      .arg ( QString ( ud.firstname ) )
		      .arg ( QString ( ud.lastname ) )
		      .arg ( QString ( pwd ) )
		      .arg ( QString ( user_name ) );
	bool success= query->exec ( sql );
	if (!success)
			throw SqlUtilException(query->lastError().databaseText());
}

void SqlUtil::editTeamName ( const QString& team_name_old, const QString& team_name_new )
{
	//UPDATE team SET team_name= 'new' WHERE team_name = 'old'
	QString sql = QString ( "UPDATE team SET team_name = '%1' WHERE team_name = '%2'" )
		      .arg ( QString ( team_name_new ) )
		      .arg ( QString ( team_name_old ) );
	//UPDATE user SET team_name= 'new' WHERE team_name = 'team'
	
	if ( query->exec ( sql ) == false)
			throw SqlUtilException(query->lastError().databaseText());
	sql = QString ( "UPDATE user SET team_name = '%1' WHERE team_name = '%2'" )
		      .arg ( QString ( team_name_new ) )
		      .arg ( QString ( team_name_old ) );
	bool success = query->exec ( sql );
	if (!success)
			throw SqlUtilException(query->lastError().databaseText());
}

void SqlUtil::deleteUser ( const QString& user_name )
{
	//DELETE from user where username = '123'
	QString sql = QString ( "DELETE FROM user WHERE username = '%1'" )
		      .arg ( QString ( user_name ) );
	bool success= query->exec ( sql );
	if (!success)
			throw SqlUtilException(query->lastError().databaseText());
}

void SqlUtil::deleteTeam ( const QString& team_name )
{
	//QString team_name2 = team_name;
	//DELETE from user where team_name='B'
	QString sql = QString ( "DELETE FROM user WHERE team_name = '%1'" )
		      .arg ( QString ( team_name ) );
	if ( query->exec ( sql ) == false)
			throw SqlUtilException(query->lastError().databaseText());
	
	//DELETE from team where team_name='B'
	sql = QString ( "DELETE FROM team WHERE team_name = '%1'" )
			  .arg ( QString ( team_name ) );
	//std::cout << query->lastError().text().toStdString() << std::endl;
	bool success= query->exec ( sql );
	if (!success)
			throw SqlUtilException(query->lastError().databaseText());
}

int SqlUtil::countTeamsPerSchool(const QString& school){
		query->exec ( "SELECT count(*) FROM team "
					  "WHERE school = '" + school + "'");
		query->next();
		int tally = query->value ( 0 ).toInt();
		return tally;
}

int SqlUtil::countUsersPerTeam(const QString& team){
	query->exec ( "SELECT count(*) FROM user "
					  "WHERE team_name = '" + team + "'");
		query->next();
		int tally = query->value ( 0 ).toInt();
		return tally;
}

void SqlUtil::getTeamUsers ( const QString& team_name, vector<UserData>& out )
{
	//SELECT username,team_name,firstname,lastname FROM user WHERE team_name = 'B'
	QString sql = QString ( "SELECT username, team_name, firstname, lastname "
				"FROM user WHERE team_name = '%1'" )
		      .arg ( QString ( team_name ) );
	if ( !query->exec ( sql ) )
		 throw SqlUtilException(query->lastError().databaseText());
	else
	{
		while ( query->next() )
		{
			UserData ud;
			ud.user_name = query->value(0).toString();
			ud.teamname = query->value(1).toString();
			ud.firstname = query->value(2).toString();
			ud.lastname = query->value(3).toString();
			out.push_back(ud);
		}
	}
}

void SqlUtil::getSpecificUser ( const QString& user_name, UserData& out )
{
	//SELECT username,team_name,firstname,lastname FROM user WHERE username = 'D'
	QString sql = QString ( "SELECT username, team_name, firstname, lastname FROM user WHERE username = '%1'" )
		      .arg( QString ( user_name ) );
	if ( !query->exec(sql) )
			throw SqlUtilException(query->lastError().databaseText());
	else
	{
		if (!query->next())
			throw SqlUtilException(query->lastError().databaseText());
		else
		{
			out.user_name = query->value(0).toString();
			out.teamname = query->value(1).toString();
			out.firstname = query->value(2).toString();
			out.lastname = query->value(3).toString();			
		}
	}
}

QString SqlUtil::getTeamSchool ( const QString& team_name )
{
	//select school from team where team_name = 'B'
	QString sql = QString ( "SELECT school FROM team WHERE team_name = '%1'" )
		      .arg( QString ( team_name ) );
	if ( !query->exec(sql) )
		return "Some weird error occurred. I don't know what ^_^;";
	else
	{
		if (!query->next())
			return "None";
		else
			return query->value(0).toString();
	}
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
        bool result = false;
        result = query->exec ( "SELECT username FROM user "
                               "WHERE username = '" + user_name + "'" );
        if ( !result ) {
                return -1;
        }
        while ( query->next() ) {
                size++;
        }
        if ( size == 1 ) {
                result = query->exec ( "SELECT username FROM scores "
                                       "WHERE username = '" + user_name + "'" );
                if ( !result ) {
                        return -2;
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
                return -3;
        }
        result =  query->exec ( sql );
        if ( result ) {
                return 0;
        } else {
                return -4;
        }
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

void SqlUtil::getTeams ( vector<TeamData>& out )
{
        out.clear();
        QString sql = "SELECT school, team_name FROM team";
        bool result = query->exec ( sql );
        if ( result ) {
                while ( query->next() ) {
                        TeamData temp;
                        temp.school = query->value ( 0 ).toString();
                        temp.teamname = query->value ( 1 ).toString();
                        out.push_back ( temp );
				}
        } else {
			throw SqlUtilException(query->lastError().databaseText());
        }
}

void SqlUtil::getUsers ( vector<UserData>& out )
{
        out.clear();
        QString sql = "SELECT username, team_name, firstname, lastname, password FROM user";
        bool result = query->exec ( sql );
        if ( result ) {
                while ( query->next() ) {
                        UserData temp;
                        temp.user_name = query->value ( 0 ).toString();
                        temp.teamname = query->value ( 1 ).toString();
                        temp.firstname = query->value ( 2 ).toString();
                        temp.lastname = query->value ( 3 ).toString();
                        temp.password = query->value ( 4 ).toString();
                        out.push_back ( temp );
                }
        } else {
			throw SqlUtilException(query->lastError().databaseText());
        }
}

void SqlUtil::getScores ( vector<ScoreData>& out )
{
        out.clear();
        QString sql = "SELECT username, score FROM scores";
        bool result = query->exec ( sql );
        if ( result ) {
                while ( query->next() ) {
                        ScoreData temp;
                        temp.user_name = query->value ( 0 ).toString();
                        temp.score = query->value ( 1 ).toDouble();
                        out.push_back ( temp );
                }
        } else {
			throw SqlUtilException(query->lastError().databaseText());
        }
}

void SqlUtil::getAdmins ( vector<AdminData>& out )
{
        out.clear();
        QString sql = "SELECT username, password FROM admin";
        bool result = query->exec ( sql );
        if ( result ) {
                while ( query->next() ) {
                        AdminData temp;
                        temp.user_name = query->value ( 0 ).toString();
                        temp.password = query->value ( 1 ).toString();
                        out.push_back ( temp );
                }
        } else {
			throw SqlUtilException(query->lastError().databaseText());
        }
}

void SqlUtil::addAdmin ( const AdminData& a )
{
        QString sql = QString ( "INSERT INTO \"admin\" VALUES('%1', '%2')" )
                      .arg ( a.user_name ).arg ( a.password );
		bool success = query->exec ( sql );
		if (!success)
			throw SqlUtilException(query->lastError().databaseText());
}

bool SqlUtil::verifyDB ( const QStringList& sl )
{
        if ( sl.size() < 4 ) {
                return false;
        }
        bool result = true;
        result &= ( bool ) sl.contains ( "admin" );
        result &= ( bool ) sl.contains ( "user" );
        result &= ( bool ) sl.contains ( "scores" );
        result &= ( bool ) sl.contains ( "team" );
        return result;
}
