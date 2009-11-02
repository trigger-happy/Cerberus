/*
Copyright (C) 2009 Nicole Guloy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/



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
	if ( !m_sql.init ( text ) ) {
			showMessageDialog( QString::fromStdString("Failed to load database. Application will not work."));
	}

	m_team_table_w = new QDialog(this);
	m_team_table_wnd->setupUi(m_team_table_w);
	m_team_table_w->show();

	m_user_table_w = new QDialog(this);
	m_user_table_wnd->setupUi(m_user_table_w);
	m_user_table_w->hide();

	m_user_edit_w = new QDialog(this);
	m_user_edit_wnd->setupUi(m_user_edit_w);
	m_user_edit_w->hide();

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



	//start up the QLists
	refreshTeamList(teams);
}

void RegistrationApp::refreshTeamList(QStringList& teams){
	teams.clear();
	vector<TeamData> team_data;
	try{
		m_sql.getTeams(team_data);

		for (int i = 0; i < team_data.size(); i++){
			teams.append(team_data.at(i).teamname);
		}

		m_team_table_wnd->team_listview->clear();
		m_team_table_wnd->team_listview->addItems(teams);
		m_team_table_wnd->team_listview->setCurrentRow(0);

	}
	catch(SqlUtil::SqlUtilException e){
		showMessageDialog(  "Couldn't refresh teams: error " + e.msg);
	}
}

void RegistrationApp::refreshUserList(QString team, QStringList& users){
	users.clear();
	vector<UserData> user_data;
	try{
		m_sql.getTeamUsers(team, user_data);
		for (int i = 0; i < user_data.size(); i++){
			users.append(user_data.at(i).user_name);
		}
		m_user_table_wnd->user_listview->clear();
		m_user_table_wnd->user_listview->addItems(users);
		m_user_table_wnd->user_listview->setCurrentRow(0);
	}
	catch(SqlUtil::SqlUtilException e){
		showMessageDialog(  "Couldn't refresh users: error " + e.msg);
	}
}

bool RegistrationApp::addTeam(){
	const QString teamname_txt = m_team_table_wnd->teamname_txt->text(),
	teamschool_txt = m_team_table_wnd->teamschool_txt->text();
	QString teamname_forlist = teamname_txt;

	try{
		m_sql.addTeam(teamname_txt, teamschool_txt);

		refreshTeamList(teams);
		m_team_table_wnd->teamname_txt->setText("");

		showMessageDialog(QString::fromStdString( "Team successfully added"));
	}
	catch(SqlUtil::SqlUtilException e){
		showMessageDialog(  "Error adding team: "+ e.msg);
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
		 try {
			 m_sql.deleteTeam(team_nav);
			refreshTeamList(teams);
			showMessageDialog( QString::fromStdString("Team successfully deleted."));
		 }
		catch(SqlUtil::SqlUtilException e){
			showMessageDialog(  "Error deleting team: " + e.msg);
		}
	}
}



bool RegistrationApp::editTeamSchool(){
	//retrieve the new team name from the dialog box
	bool ok;
	QString new_team = QInputDialog::getText(this, tr("Team Edit"),
										  tr("Enter new team name here:"), QLineEdit::Normal,
										  team_nav, &ok);
	if (ok){
		try{
			m_sql.editTeamName(m_user_table_wnd->lbl_teamname->text(), new_team);
			showMessageDialog( QString::fromStdString("Team name successfully changed"));
			//refresh all fields that call this name in this particular window
			refreshTeamList(teams);
			m_user_table_wnd->lbl_teamname->setText(new_team);
		}
		catch(SqlUtil::SqlUtilException e){
			showMessageDialog(  "Error editing team name: " + e.msg);
		}
	}
}

bool RegistrationApp::addUser(){
	//should not work if there are already two users in the list
	//clear the text field
	//call add user
	user_nav = m_user_table_wnd->username_txt->text(); //TEMPORARY
	QString username_forlist = user_nav;
	m_user_table_wnd->username_txt->setText("");
	try{
		m_sql.addUser(user_nav, team_nav);
		refreshUserList(team_nav, users);
		showMessageDialog(QString::fromStdString( "User successfully added"));
	}
	catch(SqlUtil::SqlUtilException e){
		showMessageDialog(  "Error adding user: "+ e.msg);
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
	 QMessageBox msgBox;
	 msgBox.setText("You are about to delete " + user_nav +".");
	 msgBox.setWindowTitle("Confirm Deletion");
	 msgBox.setInformativeText("Are you sure?");
	 msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	 msgBox.setDefaultButton(QMessageBox::No);
	 int ret = msgBox.exec();
	 if(ret == QMessageBox::Yes){
		try{
			m_sql.deleteUser(user_nav);
			refreshUserList(team_nav, users);
			showMessageDialog( QString::fromStdString("User successfully deleted"));
		}
		catch(SqlUtil::SqlUtilException e){
			showMessageDialog( "Error deleting user: " + e.msg);
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

	QString fn_temp = ud.firstname;
	QString ln_temp = ud.lastname;
	QString pw_temp = ud.password;
	fn_temp.replace(" ", "");
	ln_temp.replace(" ", "");
	pw_temp.replace(" ", "");
	try{
		bool sure = false;
		if (fn_temp.compare(QString("")) == 0 || ln_temp.compare(QString("")) == 0 || pw_temp.compare(QString("")) == 0){
			 QMessageBox msgBox;
			 msgBox.setText("You are about to edit " + user_nav +" with blank fields.");
			 msgBox.setWindowTitle("Confirm Edit");
			 msgBox.setInformativeText("Are you sure you want to change these fields to blank?");
			 msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			 msgBox.setDefaultButton(QMessageBox::No);
			 int ret = msgBox.exec();
			 if (ret == QMessageBox::Yes)
				 sure = true;
		}
		else sure = true;

		if (sure){
			m_sql.editUser(ud.user_name, ud);
			showMessageDialog( "Changes Saved. Your password is '" + m_user_edit_wnd->password_txt->text() + "'." );
		}
		else showMessageDialog( QString("Did not edit user"));
	}
	catch(SqlUtil::SqlUtilException e){
		showMessageDialog( "Error editing user: " + e.msg);
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

void RegistrationApp::showMessageDialog(QString text){
	QMessageBox msg ( this );
		msg.setWindowTitle("");
		msg.setText ( text );
		msg.exec();
}


int main ( int argc, char* argv[] )
{
		//TODO: implement the app stuff here

		QApplication app ( argc, argv );

		RegistrationApp r_app;
		r_app.show();

		return app.exec();
}


