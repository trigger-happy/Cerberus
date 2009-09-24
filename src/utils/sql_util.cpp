#include "util/sql_util.h"

bool SqlUtil::init(const QString& dbname){
    db = QSqlDatabase::addDatabase("QSQLITE", "contestdb"); // contestdb = conenction name
    db.setHostName("server");
    db.setDatabaseName(dbname);
    db.setUserName("admin");
    db.setPassword("admin");
    bool ok = db.open();
    query = new QSqlQuery(db);

        return ok;
}

int SqlUtil::addTeam(const QString& team_name, const QString& school){
    return query->exec("INSERT INTO team(team_name, school) "
			"VALUES (\"" + team_name + "\", \"" + school + "\")");
}

int SqlUtil::addUser(const UserData& ud){
	return 0;
}

int SqlUtil::getScore(const QString& user_name){
	return 0;
}

void SqlUtil::setScore(const QString& user_name, int score){
}

bool SqlUtil::authenticate(const QString& user_name, const QString& password){
	return true;
}
