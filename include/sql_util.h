#ifndef SQL_UTIL_H
#define SQL_UTIL_H
#include <QtSql/QtSql>
#include "data_types.h"

class SqlUtil : public QObject{
	Q_OBJECT;
public:
	bool init(const QString& dbname);
	int addTeam(const QString& team_name, const QString& school);
	int addUser(const UserData& ud);
	int getScore(const QString& user_name);
	void setScore(const QString& user_name, int score);
	bool authenticate(const QString& user_name, const QString& password);
private:
};
#endif //SQL_UTIL_H