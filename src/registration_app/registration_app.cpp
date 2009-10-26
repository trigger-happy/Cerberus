#include "registration_app.h"

#include "ui_add_team.h"
#include "ui_team_view.h"
#include "ui_teammember_edit.h"
#include "util/sql_util.h"

RegistrationApp::RegistrationApp(QWidget* parent) : m_add_team_wnd( new Ui::add_team_wnd ),
	m_team_view_wnd(new Ui::team_view_wnd),
	m_teammember_edit_wnd(new Ui::teammember_edit_wnd)
{

	this->hide();
	m_addteam_w = new QMainWindow(this);
	m_add_team_wnd->setupUi(m_addteam_w);
	m_addteam_w->show();

	m_teammember_w = new QDialog(this);
	m_teammember_edit_wnd->setupUi(m_teammember_w);
	m_teammember_w->hide();

	m_teamview_w = new QDialog(this);
	m_team_view_wnd->setupUi(m_teamview_w);
	m_teamview_w->hide();
	//network?

	//connecting stuff

	//connections for the buttons
	connect(m_add_team_wnd->add_team_btn, SIGNAL(clicked()), this, SLOT(addTeam()));
	connect(m_teammember_edit_wnd->add_member_btn,SIGNAL(clicked()), this, SLOT(addUser()));
	connect(m_teammember_edit_wnd->view_team_info_btn, SIGNAL(clicked()), this, SLOT(viewTeam()));
	connect(m_team_view_wnd->back_to_edit_btn, SIGNAL(clicked()), this, SLOT(backToAddTeam()));


}

void RegistrationApp::editTeamMember(){
	const QString teamname = m_add_team_wnd->teamname->text(),
	teamschool = m_add_team_wnd->teamschool->text();
	SqlUtil s;
	s.addTeam(teamname, teamschool);
}

void RegistrationApp::addTeam(){

}

void RegistrationApp::addUser(){

}

void RegistrationApp::viewTeam(){
}

void RegistrationApp::backToAddTeam(){
}

int main ( int argc, char* argv[] )
{
		//TODO: implement the app stuff here

		QApplication app ( argc, argv );

		RegistrationApp r_app;
		r_app.show();

		return app.exec();
}