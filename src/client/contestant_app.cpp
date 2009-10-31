/*
Copyright (C) 2009 Michael Ybanez, John Eric Sy

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
#include "ui_semifinals.h"
#include "ui_summary.h"
#include <iostream>

ContestantApp::ContestantApp ( QWidget* parent )
		: QDialog ( parent ),
		DISCONNECT_INFORMATION ( tr ( "There will be a penalty for disconnecting." ) ),
		DISCONNECT_QUESTION ( tr ( "Are you sure you want to exit the program?" ) ),
		UNAUTH_TEXT ( tr ( "Unable to obtain authorization." ) ),
		UNAUTH_INFORMATION ( tr ( "Username or password may be incorrect." ) )
{
	m_login_dlg = new Ui::login_dlg;
	m_welcome_dlg = new Ui::welcome_dlg;
	m_reconnect_dlg = new Ui::reconnect_dlg;
	m_elims_dlg = new Ui::elims_dlg;
    m_semifinals_dlg = new Ui::semifinals_dlg;
    m_summary_dlg = new Ui::summary_dlg;

	this->hide();
	m_welcome_w = new QDialog( this );
	m_welcome_dlg->setupUi( m_welcome_w );
	m_welcome_w->hide();

	m_reconnect_w = new QDialog( this );
	m_reconnect_dlg->setupUi( m_reconnect_w );
	m_reconnect_w->hide();

	m_elims_w = new QDialog( this );
	m_elims_dlg->setupUi( m_elims_w );
    m_elims_w->hide();

    m_semifinals_w = new QDialog( this );
    m_semifinals_dlg->setupUi( m_semifinals_w );
    m_semifinals_w->hide();

	m_summary_w = new QDialog( this );
	m_summary_dlg->setupUi( m_summary_w );
	m_summary_w->hide();

	m_login_w = new QDialog( this );
	m_login_dlg->setupUi( m_login_w );
	m_login_w->hide();

	m_network = new ContestantNetwork ( this );


	connect( m_network, SIGNAL ( onConnect() ), this, SLOT ( onConnect() ) );
	connect( m_network, SIGNAL ( onDisconnect() ), this, SLOT ( onDisconnect() ) );
	connect( m_network, SIGNAL ( onAuthenticate ( bool ) ), this, SLOT ( onAuthenticate ( bool ) ) );

	connect( m_network, SIGNAL ( onContestStateChange ( int, CONTEST_STATUS ) ),
	         this, SLOT ( onContestStateChange ( int, CONTEST_STATUS ) ) );
	connect( m_network, SIGNAL ( onQuestionStateChange ( ushort, ushort, QUESTION_STATUS ) ),
	         this, SLOT ( onQuestionStateChange ( ushort, ushort, QUESTION_STATUS ) ) );
	connect( m_network, SIGNAL ( onContestTime ( ushort ) ), this, SLOT ( onContestTime ( ushort ) ) );
	connect( m_network, SIGNAL ( onQData ( QString ) ), this, SLOT ( onQData ( QString ) ) );
	connect( m_network, SIGNAL ( onAData ( bool ) ), this, SLOT ( onAData ( bool ) ) );

	connect( m_network, SIGNAL ( onContestError ( ERROR_MESSAGES ) ),
	         this, SLOT ( onContestError ( ERROR_MESSAGES ) ) );
	connect( m_network, SIGNAL ( onError ( QAbstractSocket::SocketError ) ),
	         this, SLOT ( onError ( QAbstractSocket::SocketError ) ) );


	// connections for the login dialog
	connect( m_login_dlg->login_btn, SIGNAL ( clicked() ), this, SLOT ( login() ) );
	connect( m_login_dlg->exit_btn, SIGNAL ( clicked() ), this, SLOT ( exit() ) );

	// connections for the welcome dialog
	connect( m_welcome_dlg->start_btn, SIGNAL ( clicked() ), this, SLOT ( welcomeStart() ) );

	// connections for the reconnect dialog
	connect( m_reconnect_dlg->try_btn, SIGNAL ( clicked() ), this, SLOT ( reconnectTry() ) );
	connect( m_reconnect_dlg->cancel_btn, SIGNAL ( clicked() ), this, SLOT ( reconnectCancel() ) );

    // connections for the elims and semis dialog
    connect( m_elims_dlg->prev_btn, SIGNAL ( clicked() ), this, SLOT ( elimSemiPrev() ) );
    connect( m_elims_dlg->next_btn, SIGNAL ( clicked() ), this, SLOT ( elimSemiNext() ) );
    connect( m_semifinals_dlg->prev_btn, SIGNAL ( clicked() ), this, SLOT ( elimSemiPrev() ) );
    connect( m_semifinals_dlg->next_btn, SIGNAL ( clicked() ), this, SLOT ( elimSemiNext() ) );

	//connections for summary dialog
	connect( m_summary_dlg->review_btn, SIGNAL( clicked() ), this, SLOT( review() ) );
	connect( m_summary_dlg->submit_btn, SIGNAL( clicked() ), this, SLOT( submit() ) );

	// Get the client configuration from XmlUtil
    /*
    QString xml;
	QFile file("client_config.xml");
	if (!file.open (IO_ReadOnly))
	    //display error message
	QTextStream stream( &file );
	QString line;
	while( !stream.eof() ) {
	    line = stream.readLine();
	    xml += line;
	}

	ClientConfig config;
    XMLUtil::getInstance().readNetConfig( xml, config ); */

	// Connect to the server here
	//m_network->connectToHost ( config.ip , config.port );
	m_network->connectToHost ( "localhost" , 2652 );

    qCount = 0;
}

ContestantApp::~ContestantApp()
{
	delete m_network;
	delete m_login_w;
	delete m_welcome_w;
	delete m_reconnect_w;
	delete m_elims_w;
    delete m_semifinals_w;
}


void ContestantApp::onConnect()
{
	QMessageBox msg;
	msg.setWindowTitle( "Information" );
	msg.setText( "Connected to Server." );
	msg.setStandardButtons( QMessageBox::Ok );
	msg.setDefaultButton( QMessageBox::Ok );
	msg.setIcon( QMessageBox::Information );
	msg.exec();

	m_login_w->show();
}

void ContestantApp::onDisconnect()
{

}

void ContestantApp::onAuthenticate ( bool result )
{
	//TODO: do something here for authorization replies.

	if ( result )
	{
		m_login_w->hide();
		m_welcome_w->show();
		m_network->getContestState();
	}
	else
	{
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

void ContestantApp::onContestStateChange ( int r, CONTEST_STATUS s )
{
	m_network->qDataRequest( r );
	round = r;
}

void ContestantApp::onQuestionStateChange( ushort q, ushort time, QUESTION_STATUS status )
{

}

void ContestantApp::onContestTime( ushort time )
{

}

void ContestantApp::onQData ( const QString& xml )
{
    sd.questions.clear();
	XmlUtil::getInstance().readStageData( xml, sd );
	m_welcome_dlg->instructions_txt->setPlainText( sd.welcome_msg );
}

void ContestantApp::onAData ( bool result )
{
	//TODO: do something here for replies to answer uploads.
}

void ContestantApp::onContestError ( ERROR_MESSAGES err )
{

}


void ContestantApp::onError ( const QAbstractSocket::SocketError& err )
{
	//TODO: do something here when there's a network error.
}

//widget slots

void ContestantApp::login()
{
	m_network->authenticate ( m_login_dlg->username_line->text(), m_login_dlg->password_line->text() );
}

void ContestantApp::exit()
{
	this->close();
}

void ContestantApp::welcomeStart()
{
	m_welcome_w->hide();
    if( round == 1 )
        m_elims_w->show();
    else if( round == 2 )
        m_semifinals_w->show();

	// changing the question text to the first question
    displayQuestionAndChoices();
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

	switch ( disconnect )
	{
		case QMessageBox::Yes:
			this->close();
			break;

		case QMessageBox::No:
			break;
	}
}

void ContestantApp::elimSemiNext()
{
	bool atLastQuestion = ( ( sd.questions.size() - 1 ) == qCount );

	if ( atLastQuestion )
	{
        if( round == 1 )
            m_elims_w->hide();
        else if( round == 2 )
            m_semifinals_w->hide();
		m_summary_w->show();
	}
	else
	{
		qCount++;
		displayQuestionAndChoices();
	}
}

void ContestantApp::elimSemiPrev()
{
	bool atFirstQuestion = ( qCount == 0 );

	// if first question, show the welcome/instructions page
	if ( atFirstQuestion )
	{
        if( round == 1 )
            m_elims_w->hide();
        else if( round == 2 )
            m_semifinals_w->hide();
		m_welcome_w->show();
	}
	else
	{
		qCount--;
		displayQuestionAndChoices();
	}
}

void ContestantApp::review()
{
	m_summary_w->hide();

	if ( round == 1 )
	{
		m_elims_w->show();
		qCount = sd.questions.size() - 1;
	}

	displayQuestionAndChoices();
}

void ContestantApp::submit()
{

}

void ContestantApp::displayQuestionAndChoices()
{
	Question q = sd.questions[qCount];
	if ( round == 1 )
	{
		m_elims_dlg->question_lbl->setText ( q.question );
        m_elims_dlg->a_choice->setText ( q.answer_key[0].c );
        m_elims_dlg->b_choice->setText ( q.answer_key[1].c );
        m_elims_dlg->c_choice->setText ( q.answer_key[2].c );
        m_elims_dlg->d_choice->setText ( q.answer_key[3].c );
	}
    else if( round == 2 )
    {
        m_semifinals_dlg->question_lbl->setText ( q.question );
        m_semifinals_dlg->a_choice->setText ( q.answer_key[0].c );
        m_semifinals_dlg->b_choice->setText ( q.answer_key[1].c );
        m_semifinals_dlg->c_choice->setText ( q.answer_key[2].c );
        m_semifinals_dlg->d_choice->setText ( q.answer_key[3].c );
    }
}

int main ( int argc, char* argv[] )
{
	QApplication app ( argc, argv );

	ContestantApp c_app;
    //c_app.show();

	return app.exec();
}
