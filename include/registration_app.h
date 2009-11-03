#ifndef REGISTRATION_APP_H
#define REGISTRATION_APP_H

#include <QtGui/QtGui>
#include <QtNetwork/QtNetwork>
#include "util/sql_util.h"

namespace Ui
{
	class team_table_wnd;
	class user_table_wnd;
	class user_edit_wnd;
}

class RegistrationApp : public QDialog
{
	Q_OBJECT;
public:
	RegistrationApp( QWidget* parent = 0);

//signals:


protected slots:
	//team table
	bool addTeam();
	bool goToEditTeam();
	bool deleteTeam();

	//user table
	bool editTeamSchool();
	bool addUser();
	bool editUser();
	bool deleteUser();
	bool backToTeam();

	//user edit
	bool saveUserEdit();
	bool backToUser();

	//convenience methods
	void showMessageDialog(QString text);

	//needs no extra signals because you're using the basic ones:


private:
	void refreshTeamList(QStringList& teams); //a function to refresh the list of teams in the respective QListWidget
	void refreshUserList(QString team, QStringList& users);

	Ui::team_table_wnd* m_team_table_wnd;
	Ui::user_table_wnd* m_user_table_wnd;
	Ui::user_edit_wnd* m_user_edit_wnd;

	QString team_nav;//for saving the name of the team who you are currently accessing
	QString user_nav;

	QStringList teams;
	QStringList users;

	QDialog* m_team_table_w;
	QDialog* m_user_table_w;
	QDialog* m_user_edit_w;

	SqlUtil& m_sql;
};

#endif // REGISTRATION_APP_H
