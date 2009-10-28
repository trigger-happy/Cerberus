#include "registration_app.h"

#include "ui_team_table.h"
#include "ui_user_table.h"
#include "ui_user_edit.h"
#include "util/sql_util.h"

RegistrationApp::RegistrationApp(QWidget* parent) :
	m_team_table_wnd( new Ui::team_table_wnd ),
	m_user_table_wnd(new Ui::user_table_wnd),
	m_user_edit_wnd(new Ui::user_edit_wnd),
	m_sql(SqlUtil::getInstance())
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("Database Directory"),
										  tr("Database Path:"), QLineEdit::Normal,
										  QDir::home().dirName(), &ok);
	if (!text.isEmpty())
		 m_sql.init(text);
	this->hide();
	m_team_table_w = new QDialog(this);
	m_team_table_wnd->setupUi(m_team_table_w);
	m_team_table_w->show();

	m_user_table_w = new QDialog(this);
	m_user_table_wnd->setupUi(m_user_table_w);
	m_user_table_w->hide();

	m_user_edit_w = new QDialog(this);
	m_user_edit_wnd->setupUi(m_user_edit_w);
	m_user_edit_w->hide();
	//network?

	//connecting stuff

	//connections for the buttons

	//team table
	connect(m_team_table_wnd->add_team_btn, SIGNAL(clicked()), this, SLOT(addTeam()));
	connect(m_team_table_wnd->team_edit_btn, SIGNAL(clicked()), this, SLOT(goToEditTeam()));
	connect(m_team_table_wnd->team_delete_btn, SIGNAL(clicked()), this, SLOT(deleteTeam()));

	//user table
	connect(m_user_table_wnd->edit_teamname_btn, SIGNAL(clicked()), this, SLOT(editTeamSchool()));
	connect(m_user_table_wnd->add_user_btn, SIGNAL(clicked()), this, SLOT(addUser()));
	connect(m_user_table_wnd->edit_user_btn, SIGNAL(clicked()), this, SLOT(editUser()));
	connect(m_user_table_wnd->delete_user_btn, SIGNAL(clicked()), this, SLOT(deleteUser()));
	connect(m_user_table_wnd->backtoteams_btn, SIGNAL(clicked()), this, SLOT(backToTeam()));

	//user edit
	connect(m_user_edit_wnd->save_userchange_btn, SIGNAL(clicked()), this, SLOT(saveUserEdit()));
	connect(m_user_edit_wnd->backtouser_btn, SIGNAL(clicked()), this, SLOT(backToUser()));

	bool result = SqlUtil::getInstance().init ( text );
		if ( !result ) {
				QMessageBox msg ( this );
				msg.setText ( "Failed to load database" );
				msg.exec();
		}
}

bool RegistrationApp::addTeam(){
	const QString teamname_txt = m_team_table_wnd->teamname_txt->text(),
	teamschool_txt = m_team_table_wnd->teamschool_txt->text();
	m_sql.addTeam(teamname_txt, teamschool_txt);
	//refresh the list view here

}

bool RegistrationApp::goToEditTeam(){
	//make sure at least one thing in the list view is selected
	m_team_table_w->hide();
	m_user_table_w->show();
	m_user_table_wnd->lbl_schoolname->setText(m_team_table_wnd->teamname_txt->text());

}

bool RegistrationApp::deleteTeam(){
	//m_sql.deleteTeam(); //gets the selected school in the list view
}

bool RegistrationApp::editTeamSchool(){

}

bool RegistrationApp::addUser(){
}

bool RegistrationApp::editUser(){
}

bool RegistrationApp::deleteUser(){
}

bool RegistrationApp::backToTeam(){
}

bool RegistrationApp::saveUserEdit(){
}

bool RegistrationApp::backToUser(){
}








int main ( int argc, char* argv[] )
{
		//TODO: implement the app stuff here

		QApplication app ( argc, argv );

		RegistrationApp r_app;
		r_app.show();

		return app.exec();
}
