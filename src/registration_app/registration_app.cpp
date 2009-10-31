#include "registration_app.h"

#include "ui_team_table.h"
#include "ui_user_table.h"
#include "ui_user_edit.h"
#include "util/sql_util.h"
#include "data_types.h"
#include <QListWidget>

RegistrationApp::RegistrationApp(QWidget* parent) :
	m_team_table_wnd( new Ui::team_table_wnd ),
	m_user_table_wnd(new Ui::user_table_wnd),
	m_user_edit_wnd(new Ui::user_edit_wnd),
	m_sql(SqlUtil::getInstance())
{
	this->setWindowTitle("Registration");
	this->hide();
	bool ok;
	QString text = QInputDialog::getText(this, tr("Database Directory"),
										  tr("Database Path:"), QLineEdit::Normal,
										  "resources/test.db", &ok);
	if (!text.isEmpty())
		 m_sql.init(text);


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

	bool result = m_sql.init ( text );
		if ( !result ) {
				QMessageBox msg ( this );
				msg.setWindowTitle("");
				msg.setText ( "Failed to load database" );
				msg.exec();
		}

	//starting up the QLists
	refreshTeamList(teams);
}

void RegistrationApp::refreshTeamList(QStringList& teams){
	teams.clear();
	vector<TeamData> team_data;
	bool test = m_sql.getTeams(team_data);
	if (!test){
				QMessageBox team_msg ( this );
				team_msg.setWindowTitle("");
				team_msg.setText ( "Couldn't get teams" );
				team_msg.exec();
	}
	else{
		for (int i = 0; i < team_data.size(); i++){
			teams.append(team_data.at(i).teamname);
		}

		m_team_table_wnd->team_listview->clear();
		m_team_table_wnd->team_listview->addItems(teams);
		m_team_table_wnd->team_listview->setCurrentRow(0);
	}
}

void RegistrationApp::refreshUserList(QString team, QStringList& users){
	users.clear();
	vector<UserData> user_data;
	bool test = m_sql.getTeamUsers(team, user_data);
	if (!test){
				QMessageBox team_msg ( this );
				team_msg.setWindowTitle("");
				team_msg.setText ( "Couldn't get users" );
				team_msg.exec();
	}
	else{
		for (int i = 0; i < user_data.size(); i++){
			users.append(user_data.at(i).user_name);
		}
		m_user_table_wnd->user_listview->clear();
		m_user_table_wnd->user_listview->addItems(users);
		m_user_table_wnd->user_listview->setCurrentRow(0);
	}

}

bool RegistrationApp::addTeam(){
	const QString teamname_txt = m_team_table_wnd->teamname_txt->text(),
	teamschool_txt = m_team_table_wnd->teamschool_txt->text();
	QString teamname_forlist = teamname_txt;
	int success = m_sql.addTeam(teamname_txt, teamschool_txt);
	if (success){
				refreshTeamList(teams);
				m_team_table_wnd->teamname_txt->setText("");
				//m_team_table_wnd->team_listview->addItem(teamname_forlist);
				QMessageBox addteam_msg ( this );
				addteam_msg.setWindowTitle("");
				addteam_msg.setText ( "Team successfully added" );
				addteam_msg.exec();
	}
}

bool RegistrationApp::goToEditTeam(){
	//change this to access from the list view
	team_nav = m_team_table_wnd->team_listview->currentItem()->text();

	m_team_table_wnd->teamname_txt->setText("");
	m_team_table_wnd->teamschool_txt->setText("");
	refreshUserList(team_nav, users);
	m_team_table_w->hide();
	m_user_table_w->show();
	m_user_table_wnd->lbl_teamname->setText(team_nav);
	m_user_table_wnd->lbl_schoolname->setText(m_sql.getTeamSchool(m_user_table_wnd->lbl_teamname->text()));

}

bool RegistrationApp::deleteTeam(){
	//gets the selected school in the list view
	team_nav = m_team_table_wnd->team_listview->currentItem()->text();
	//show pop up dialog if they are sure they want to delete that team
	 QMessageBox msgBox;
	 msgBox.setText("You are about to delete team " + team_nav +".");
	 msgBox.setInformativeText("Are you sure?");
	 msgBox.setWindowTitle("Confirm Deletion");
	 msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	 msgBox.setDefaultButton(QMessageBox::No);
	 int ret = msgBox.exec();
	 if(ret == QMessageBox::Yes){
		int success = m_sql.deleteTeam(team_nav);
		if (success){
					refreshTeamList(teams);
					QMessageBox deleteteam_msg ( this );
					deleteteam_msg.setWindowTitle("");
					deleteteam_msg.setText ( "Team successfully deleted" );
					deleteteam_msg.exec();
		}
	}
}


//the following fields will need to access the team name from... the label na lang. haha.

bool RegistrationApp::editTeamSchool(){
	//retrieve the new team name from the dialog box
	bool ok;
	QString new_team = QInputDialog::getText(this, tr("Team Edit"),
										  tr("Enter new team name here:"), QLineEdit::Normal,
										  team_nav, &ok);
	if (ok){
		int success = m_sql.editTeamName(m_user_table_wnd->lbl_teamname->text(), new_team);
		if (success){
				QMessageBox editname_msg ( this );
				editname_msg.setWindowTitle("");
				editname_msg.setText ( "Team name successfully changed." );
				editname_msg.exec();
		}
		//refresh all fields that call this name in this particular window
		refreshTeamList(teams);
		m_user_table_wnd->lbl_teamname->setText(new_team);
	}
}

bool RegistrationApp::addUser(){
	//should not work if there are already two users in the list
	//clear the text field
	//call add user
	user_nav = m_user_table_wnd->username_txt->text(); //TEMPORARY
	QString username_forlist = user_nav;
	m_user_table_wnd->username_txt->setText("");
	bool success = m_sql.addUser(user_nav, team_nav);
	if (success){
				refreshUserList(team_nav, users);
				//m_user_table_wnd->user_listview->addItem(username_forlist);
				QMessageBox adduser_msg ( this );
				adduser_msg.setWindowTitle("");
				adduser_msg.setText ( "User successfully added" );
				adduser_msg.exec();
	}

}

bool RegistrationApp::editUser(){
	//pass the username of the selected value in the list view to user_nav
	user_nav = m_user_table_wnd->user_listview->currentItem()->text();
	//get the original data of that user
	UserData ud;
	m_sql.getSpecificUser(user_nav, ud);
	m_user_edit_wnd->firstname_txt->setText(ud.firstname);
	m_user_edit_wnd->lastname_txt->setText(ud.lastname);

	//move to the user edit window
	m_user_edit_wnd->lbl_username->setText(user_nav);
	m_user_edit_wnd->lbl_userteam->setText(ud.teamname);
	m_user_edit_wnd->lbl_userschool->setText(m_sql.getTeamSchool(ud.teamname));
	m_user_edit_w->show();
}

bool RegistrationApp::deleteUser(){
	//gets the selected user in the list view
	user_nav = m_user_table_wnd->user_listview->currentItem()->text();
	//show pop up dialog if they are sure they want to delete that user
	int success = m_sql.deleteUser(user_nav);
	 QMessageBox msgBox;
	 msgBox.setText("You are about to delete " + user_nav +".");
	 msgBox.setWindowTitle("Confirm Deletion");
	 msgBox.setInformativeText("Are you sure?");
	 msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	 msgBox.setDefaultButton(QMessageBox::No);
	 int ret = msgBox.exec();
	 if(ret == QMessageBox::Yes){
		if (success){
					refreshUserList(team_nav, users);
					QMessageBox deleteuser_msg ( this );
					deleteuser_msg.setWindowTitle("");
					deleteuser_msg.setText ( "User successfully deleted" );
					deleteuser_msg.exec();
		}
	}
}


bool RegistrationApp::backToTeam(){
	//goes back to the teams from specific team view, with cleared text fields.
	refreshTeamList(teams);
	m_team_table_w->show();
	m_user_table_w->hide();
}

bool RegistrationApp::saveUserEdit(){
	UserData ud;
	ud.user_name = user_nav;
	ud.teamname = team_nav;
	ud.firstname = m_user_edit_wnd->firstname_txt->text();
	ud.lastname = m_user_edit_wnd->lastname_txt->text();
	ud.password = m_user_edit_wnd->password_txt->text();
	int success = m_sql.editUser(ud.user_name, ud); //is not woooorking...
	if (success){
				QMessageBox edituser_msg ( this );
				edituser_msg.setWindowTitle("");
				edituser_msg.setText ( "Changes Saved. Your password is '" + m_user_edit_wnd->password_txt->text() + "'." );
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
