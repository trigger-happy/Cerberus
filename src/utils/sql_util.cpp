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
                        "VALUES (\'" + team_name + "\', \'" + school + "\')");
}

int SqlUtil::addUser(const UserData& ud){
    QString sql = QString("INSERT INTO user(username, team_name, "
                          "firstname, lastname, password) "
                          "VALUES ('%1', '%2', '%3', '%4', '%5')")
            .arg(QString(ud.user_name.c_str()))
            .arg(QString(ud.teamname.c_str()))
            .arg(QString(ud.firstname.c_str()))
            .arg(QString(ud.lastname.c_str()))
            .arg(QString(ud.password.c_str()));
    return query->exec(sql);
}

int SqlUtil::getScore(const QString& user_name){
    return query->exec("SELECT score FROM user u, scores s "
                       "WHERE s.username = \'" + user_name + "\' AND u.username = s.username");
}

void SqlUtil::setScore(const QString& user_name, int score){
    QString sql = QString("UPDATE scores SET score = '%1' "
                          "WHERE username='%2'").arg(score).arg(user_name);
    query->exec(sql);
}

bool SqlUtil::authenticate(const QString& user_name, const QString& password){

    query->exec("SELECT username FROM user "
                "WHERE username = '" + user_name + "' AND password = '" + password + "'");
    int size = query->size();
    if (size == 1)
        return true;
    else
        return false;
}
