#include "registration_app.h"

#include "ui_team_table.h"
#include "ui_user_table.h"
#include "ui_user_edit.h"
#include "util/sql_util.h"
#include "data_types.h"

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

	//experimenting with qlistview
	//m_team_table_wnd->team_listview->addColumn("Users");
	//QListViewItem element = new QListViewItem( element, qName, namespaceURI );

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

	bool result = m_sql.init ( "resources/test.db" ); //change parameter to 'text' when done testing
		if ( !result ) {
				QMessageBox msg ( this );
				msg.setText ( "Failed to load database" );
				msg.exec();
		}
}

bool RegistrationApp::addTeam(){
	const QString teamname_txt = m_team_table_wnd->teamname_txt->text(),
	teamschool_txt = m_team_table_wnd->teamschool_txt->text();
	int success = m_sql.addTeam(teamname_txt, teamschool_txt);
	if (success){
				QMessageBox addteam_msg ( this );
				addteam_msg.setText ( "Team successfully added" );
				addteam_msg.exec();
	}

	//refresh the list view here

}

bool RegistrationApp::goToEditTeam(){
	//change this to access from the list view
	team_nav = m_team_table_wnd->teamname_txt->text();
	//clear all text fields in the view you are moving to
	m_team_table_w->hide();
	m_user_table_w->show();
	m_user_table_wnd->lbl_teamname->setText(team_nav);
	m_user_table_wnd->lbl_schoolname->setText(m_sql.getTeamSchool(team_nav));

}

bool RegistrationApp::deleteTeam(){
	//gets the selected school in the list view
	team_nav = m_user_table_wnd->lbl_schoolname->text();
	//show pop up dialog if they are sure they want to delete that team
	int success = m_sql.deleteTeam(team_nav);
	if (success){
				QMessageBox deleteteam_msg ( this );
				deleteteam_msg.setText ( "Team successfully deleted" );
				deleteteam_msg.exec();
	}

}


//the following fields will need to access the team name from... the label na lang. haha.

bool RegistrationApp::editTeamSchool(){
	//retrieve the new team name from the dialog box
	bool ok;
	QString new_team = QInputDialog::getText(this, tr("Team Edit"),
										  tr("Enter new team name here:"), QLineEdit::Normal,
										  QDir::home().dirName(), &ok);
	if (ok){
		m_sql.editTeamName(m_user_table_wnd->lbl_schoolname->text(), new_team);
		//refresh all fields that call this name in this particular window
		m_user_table_wnd->lbl_schoolname->setText(new_team);
	}
}

bool RegistrationApp::addUser(){
	//get the new username from the text field
	//clear the text field
	//call add user
	user_nav = m_user_table_wnd->username_txt->text(); //TEMPORARY
	m_user_table_wnd->username_txt->setText("");
	bool success = m_sql.addUser(user_nav, team_nav);
	if (success){
				QMessageBox adduser_msg ( this );
				adduser_msg.setText ( "User successfully added" );
				adduser_msg.exec();
	}

}

bool RegistrationApp::editUser(){
	//pass the username of the selected value in the list view to user_nav

	//get the original data of that user
	UserData ud;
	m_sql.getSpecificUser(user_nav, ud);
	m_user_edit_wnd->firstname_txt->setText(ud.firstname);
	m_user_edit_wnd->lastname_txt->setText(ud.lastname);

	//move to the user edit window
	m_user_edit_wnd->lbl_username->setText(user_nav);
	m_user_edit_wnd->username_txt->setText(user_nav);
	m_user_edit_w->show();
}

bool RegistrationApp::deleteUser(){
	//gets the selected user in the list view
	//show pop up dialog if they are sure they want to delete that user
	int success = m_sql.deleteUser(user_nav);
	if (success){
				QMessageBox deleteuser_msg ( this );
				deleteuser_msg.setText ( "User successfully deleted" );
				deleteuser_msg.exec();
	}

}


bool RegistrationApp::backToTeam(){
	//goes back to the teams from specific team view, with cleared text fields.
	m_team_table_w->show();
	m_user_table_w->hide();
}

bool RegistrationApp::saveUserEdit(){
	UserData ud;
	ud.user_name = m_user_edit_wnd->username_txt->text();
	ud.teamname = team_nav;
	ud.firstname = m_user_edit_wnd->firstname_txt->text();
	ud.lastname = m_user_edit_wnd->lastname_txt->text();
	ud.password = m_user_edit_wnd->password_txt->text();
	//display a dialog whether successful or not
	int success = m_sql.editUser(user_nav, ud); //is not woooorking...
	if (success){
				QMessageBox edituser_msg ( this );
				edituser_msg.setText ( "User successfully edited" );
				edituser_msg.exec();
	}

	//go back to the specific team view
	backToUser();
}

bool RegistrationApp::backToUser(){
	//clear the text fields
	m_user_edit_wnd->firstname_txt->setText("");
	m_user_edit_wnd->lastname_txt->setText("");
	m_user_edit_wnd->password_txt->setText("");
	//go back to the specific team view
	m_user_edit_w->hide();


}


int main ( int argc, char* argv[] )
{
		//TODO: implement the app stuff here

		QApplication app ( argc, argv );

		RegistrationApp r_app;
		r_app.show();

		return app.exec();
}
