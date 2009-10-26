#ifndef REGISTRATION_APP_H
#define REGISTRATION_APP_H

#include <QtGui/QtGui>
#include <QtNetwork/QtNetwork>
#include "util/sql_util.h"

namespace Ui
{
	class add_team_wnd;
	class team_view_wnd;
	class teammember_edit_wnd;
}

class RegistrationApp : public QDialog
{
	Q_OBJECT;
public:
	RegistrationApp( QWidget* parent = 0);

//signals:


protected slots:
	/*Slot for the add team button on add_team.ui*/
	void addTeam();
	/*Slot once you click a team on the add_team.ui table ... if I can get it to work*/
	void viewTeam();
	/*for the add user button while adding to the team.*/
	void addUser();
	/*for editing the team member*/
	void editTeamMember();
	//returns to the add teammembers screen
	void backToAddTeam();

	//needs no extra signals because you're using the basic ones:
	// may probably need some new signals because

private:
	Ui::add_team_wnd* m_add_team_wnd;
	Ui::teammember_edit_wnd* m_teammember_edit_wnd;
	Ui::team_view_wnd* m_team_view_wnd;

	QMainWindow* m_addteam_w;
	QDialog* m_teammember_w;
	QDialog* m_teamview_w;

};

#endif // REGISTRATION_APP_H
