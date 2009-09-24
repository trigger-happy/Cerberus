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
        int getScore ( const QString& user_name );
        void setScore ( const QString& user_name, int score );
        bool authenticate ( const QString& user_name, const QString& password );
public slots:
private:
        QSqlDatabase db;
        QSqlQuery* query;
};
#endif //SQL_UTIL_H
