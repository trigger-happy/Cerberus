/*
Copyright (C) 2009 Michael Ybanez

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
#include "contestant_app.h"
#include "error_defs.h"
#include "ui_login.h"
#include "ui_welcome.h"
#include "ui_reconnect.h"
#include "ui_elims.h"
#include "ui_summary.h"
#include "util/xml_util.h"
#include <iostream>

ContestantApp::ContestantApp ( QWidget* parent )
                : QDialog ( parent ),
                DISCONNECT_INFORMATION ( tr ( "There will be a penalty for disconnecting." ) ),
                DISCONNECT_QUESTION ( tr ( "Are you sure you want to exit the program?" ) ),
				CON_TEXT ( tr ( "Connected to Server." ) ),
				UNAUTH_TEXT ( tr ( "Unable to obtain authorization." ) ),
                UNAUTH_INFORMATION ( tr ( "Username or password may be incorrect." ) )
{
        m_login_dlg = new Ui::login_dlg;
        m_welcome_dlg = new Ui::welcome_dlg;
        m_reconnect_dlg = new Ui::reconnect_dlg;
		m_elims_dlg = new Ui::elims_dlg;
        m_summary_dlg = new Ui::summary_dlg;

        this->hide();
        m_welcome_w = new QDialog ( this );
        m_welcome_dlg->setupUi ( m_welcome_w );
        m_welcome_w->hide();

        m_reconnect_w = new QDialog ( this );
        m_reconnect_dlg->setupUi ( m_reconnect_w );
        m_reconnect_w->hide();

		m_elims_w = new QDialog ( this );
		m_elims_dlg->setupUi ( m_elims_w );
		m_elims_w->hide();

        m_summary_w = new QDialog ( this );
        m_summary_dlg->setupUi ( m_summary_w );
        m_summary_w->hide();

        m_login_w = new QDialog ( this );
        m_login_dlg->setupUi ( m_login_w );
        m_login_w->show();

        m_network = new ContestantNetwork ( this );

        connect ( m_network, SIGNAL ( onAuthenticate ( bool ) ), this, SLOT ( netAuthenticate ( bool ) ) );
        connect ( m_network, SIGNAL ( onConnect() ), this, SLOT ( netConnect() ) );
		connect ( m_network, SIGNAL ( onContestStateChange ( int, CONTEST_STATUS ) ), this, SLOT ( netContestStateChange ( int, CONTEST_STATUS ) ) );
        connect ( m_network, SIGNAL ( onContestError ( quint16 ) ), this, SLOT ( netContestError ( quint16 ) ) );
        connect ( m_network, SIGNAL ( onError ( QAbstractSocket::SocketError ) ), this, SLOT ( netError ( QAbstractSocket::SocketError ) ) );
		connect ( m_network, SIGNAL ( onAData ( bool ) ), this, SLOT ( netAData ( bool ) ) );
		connect ( m_network, SIGNAL ( onQData ( QString ) ), this, SLOT ( netQData ( QString ) ) );

        // connections for the login dialog
        connect ( m_login_dlg->login_btn, SIGNAL ( clicked() ), this, SLOT ( loginLogin() ) );
        connect ( m_login_dlg->exit_btn, SIGNAL ( clicked() ), this, SLOT ( loginExit() ) );

        // connections for the welcome dialog
        connect ( m_welcome_dlg->start_btn, SIGNAL ( clicked() ), this, SLOT ( welcomeStart() ) );

        // connections for the reconnect dialog
        connect ( m_reconnect_dlg->try_btn, SIGNAL ( clicked() ), this, SLOT ( reconnectTry() ) );
        connect ( m_reconnect_dlg->cancel_btn, SIGNAL ( clicked() ), this, SLOT ( reconnectCancel() ) );

        // connections for the round 1 dialog
		connect ( m_elims_dlg->prev_btn, SIGNAL ( clicked() ), this, SLOT ( elimsPrevious() ) );
		connect ( m_elims_dlg->next_btn, SIGNAL ( clicked() ), this, SLOT ( elimsNext() ) );

		//connections for summary dialog
		connect ( m_summary_dlg->review_btn, SIGNAL( clicked() ), this, SLOT( review() ) );


		// TODO: get the client configuration from XmlUtil


        // TODO: connect to the server here
		m_network->connectToHost ( "localhost" , 2652 );

        // question data types, to be used all throughout the round
        /*r1qdata = new QuestionData;
        r1question = new Question;*/

		r1Count = 0;
}

ContestantApp::~ContestantApp()
{
        delete m_network;
        delete m_login_w;
        delete m_welcome_w;
        delete m_reconnect_w;
		delete m_elims_w;

        /*delete r1qdata;
        delete r1question;*/
}

void ContestantApp::netContestStateChange ( int r, CONTEST_STATUS s )
{
        //TODO: do something here for when the contest state changes
		m_network->qDataRequest( r );
		round = r;
}

void ContestantApp::netError ( const QAbstractSocket::SocketError& err )
{
        //TODO: do something here when there's a network error.
}

void ContestantApp::netContestError ( quint16 err )
{
        //TODO: do something here for contest errors.
		//Undefined, ignore for now
}

void ContestantApp::netConnect()
{
        //TODO: implement actions for when we have a connection
		QMessageBox msg;
		msg.setWindowTitle ( "Information" );
		msg.setText ( CON_TEXT );
		msg.setStandardButtons ( QMessageBox::Ok );
		msg.setDefaultButton ( QMessageBox::Ok );
		msg.setIcon ( QMessageBox::Information );
		msg.exec();
}

void ContestantApp::netAuthenticate ( bool result )
{
        //TODO: do something here for authorization replies.

		if ( result ) {
				m_login_w->hide();
				m_welcome_w->show();
				m_network->getContestState();
        } else {
                QMessageBox msg;
                msg.setWindowTitle ( "Error" );
                msg.setText ( UNAUTH_TEXT );
                msg.setInformativeText ( UNAUTH_INFORMATION );
                msg.setStandardButtons ( QMessageBox::Ok );
                msg.setDefaultButton ( QMessageBox::Ok );
                msg.setIcon ( QMessageBox::Information );
                msg.exec();
        }

}

void ContestantApp::netQData ( const QString& xml )
{
		XmlUtil::getInstance().readQuestionData( round, xml, qd );
		m_welcome_dlg->instructions_txt->setPlainText( qd.welcome_msg );
}

void ContestantApp::netAData ( bool result )
{
        //TODO: do something here for replies to answer uploads.
}

void ContestantApp::loginLogin()
{
        m_network->authenticate ( m_login_dlg->username_line->text(), m_login_dlg->password_line->text() );
}

void ContestantApp::loginExit()
{
        this->close();
}

void ContestantApp::welcomeStart()
{
        // to do later
        m_welcome_w->hide();
		m_elims_w->show();

        // changing the question text to the first question
		Question q = qd.questions[r1Count];
		m_elims_dlg->question_lbl->setText ( q.question );
		m_elims_dlg->a_radio->setText ( q.choices[1] );
		m_elims_dlg->b_radio->setText ( q.choices[2] );
		m_elims_dlg->c_radio->setText ( q.choices[3] );
		m_elims_dlg->d_radio->setText ( q.choices[4] );

}

void ContestantApp::reconnectTry()
{

}

void ContestantApp::reconnectCancel()
{
        QMessageBox msgBox;
        msgBox.setWindowTitle ( "Confirm Disconnection" );
        msgBox.setText ( DISCONNECT_QUESTION );
        msgBox.setInformativeText ( DISCONNECT_INFORMATION );
        msgBox.setStandardButtons ( QMessageBox::Yes | QMessageBox::No );
        msgBox.setDefaultButton ( QMessageBox::No );
        msgBox.setIcon ( QMessageBox::Question );
        int disconnect = msgBox.exec();

        switch ( disconnect ) {
        case QMessageBox::Yes:
                this->close();
                break;

        case QMessageBox::No:
                break;
        }
}

void ContestantApp::elimsNext()
{
		bool atLastQuestion = ( (qd.questions.size()-1) == r1Count );
		if ( atLastQuestion )
		{
			m_elims_w->hide();
			m_summary_w->show();
		}
		else
		{
			r1Count++;
			Question q = qd.questions[r1Count];
			m_elims_dlg->question_lbl->setText ( q.question );
			m_elims_dlg->a_radio->setText ( q.choices[1] );
			m_elims_dlg->b_radio->setText ( q.choices[2] );
			m_elims_dlg->c_radio->setText ( q.choices[3] );
			m_elims_dlg->d_radio->setText ( q.choices[4] );
		}
}

void ContestantApp::elimsPrevious()
{
		bool atFirstQuestion = ( r1Count == 0 );

        // if first question, show the welcome/instructions page
        if ( atFirstQuestion ) {
			m_elims_w->hide();
			m_welcome_w->show();
        } else {
			r1Count--;
			Question q = qd.questions[r1Count];
			m_elims_dlg->question_lbl->setText ( q.question );
			m_elims_dlg->a_radio->setText ( q.choices[1] );
			m_elims_dlg->b_radio->setText ( q.choices[2] );
			m_elims_dlg->c_radio->setText ( q.choices[3] );
			m_elims_dlg->d_radio->setText ( q.choices[4] );
        }
}

void ContestantApp::review()
{
	m_summary_w->hide();
	if( round == 1 )
	{
		m_elims_w->show();
		r1Count = qd.questions.size()-1;
		Question q = qd.questions[r1Count];
		m_elims_dlg->question_lbl->setText ( q.question );
		m_elims_dlg->a_radio->setText ( q.choices[1] );
		m_elims_dlg->b_radio->setText ( q.choices[2] );
		m_elims_dlg->c_radio->setText ( q.choices[3] );
		m_elims_dlg->d_radio->setText ( q.choices[4] );
	}



}

int main ( int argc, char* argv[] )
{
        //TODO: implement the app stuff here

        QApplication app ( argc, argv );

        ContestantApp c_app;
		c_app.show();

        return app.exec();
}
