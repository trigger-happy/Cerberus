/*
Copyright (C) 2009 John Eric Sy, Michael Ybanez

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
#include "ui_elims.h"
#include "ui_semifinals.h"
#include "ui_finalsChoice.h"
#include "ui_finalsIdent.h"
#include "ui_summary.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <vector>
#include <cassert>

ContestantApp::ContestantApp ( QWidget* parent )
		: QDialog ( parent ),
		DISCONNECT_INFORMATION ( tr ( "There will be a penalty for disconnecting." ) ),
		DISCONNECT_QUESTION ( tr ( "Are you sure you want to exit the program?" ) ),
		UNAUTH_TEXT ( tr ( "Unable to obtain authorization." ) ),
		UNAUTH_INFORMATION ( tr ( "Username or password may be incorrect." ) )
{
	m_login_dlg = new Ui::login_dlg;
	m_welcome_dlg = new Ui::welcome_dlg;
	m_elims_dlg = new Ui::elims_dlg;
    m_semifinals_dlg = new Ui::semifinals_dlg;
    m_finalsChoice_dlg = new Ui::finalsChoice_dlg;
    m_finalsIdent_dlg = new Ui::finalsIdent_dlg;
    m_summary_dlg = new Ui::summary_dlg;

	this->hide();
	m_welcome_w = new QDialog( this );
	m_welcome_dlg->setupUi( m_welcome_w );
	m_welcome_w->hide();

	m_elims_w = new QDialog( this );
	m_elims_dlg->setupUi( m_elims_w );
    m_elims_w->hide();

    m_semifinals_w = new QDialog( this );
    m_semifinals_dlg->setupUi( m_semifinals_w );
    m_semifinals_w->hide();

    m_finalsChoice_w = new QDialog( this );
    m_finalsChoice_dlg->setupUi( m_finalsChoice_w );
    m_finalsChoice_w->hide();

    m_finalsIdent_w = new QDialog( this );
    m_finalsIdent_dlg->setupUi( m_finalsIdent_w );
    m_finalsIdent_w->hide();

	m_summary_w = new QDialog( this );
	m_summary_dlg->setupUi( m_summary_w );
	m_summary_w->hide();

	m_login_w = new QDialog( this );
	m_login_dlg->setupUi( m_login_w );
	m_login_w->hide();

	m_network = new ContestantNetwork ( this );
    timer = new QTimer( this );

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
	connect( m_network, SIGNAL(onError(QString)),
	         this, SLOT ( onError ( QString ) ) );


	// connections for the login dialog
	connect( m_login_dlg->login_btn, SIGNAL ( clicked() ), this, SLOT ( login() ) );
	connect( m_login_dlg->exit_btn, SIGNAL ( clicked() ), this, SLOT ( exit() ) );

	// connections for the welcome dialog
	connect( m_welcome_dlg->start_btn, SIGNAL ( clicked() ), this, SLOT ( welcomeStart() ) );

    // connections for the elims and semis dialog
    connect( m_elims_dlg->prev_btn, SIGNAL ( clicked() ), this, SLOT ( elimSemiPrev() ) );
    connect( m_elims_dlg->next_btn, SIGNAL ( clicked() ), this, SLOT ( elimSemiNext() ) );
    connect( m_semifinals_dlg->prev_btn, SIGNAL ( clicked() ), this, SLOT ( elimSemiPrev() ) );
    connect( m_semifinals_dlg->next_btn, SIGNAL ( clicked() ), this, SLOT ( elimSemiNext() ) );

    // connections for summary dialog
	connect( m_summary_dlg->review_btn, SIGNAL( clicked() ), this, SLOT( review() ) );
	connect( m_summary_dlg->submit_btn, SIGNAL( clicked() ), this, SLOT( submit() ) );

    // connections for finals dialog
    connect( m_finalsChoice_dlg->submit_btn, SIGNAL( clicked() ), this, SLOT( finalsSubmit() ) );
    connect( m_finalsIdent_dlg->submit_btn, SIGNAL( clicked() ), this, SLOT( finalsSubmit() ) );

    // slot for timer
    connect( timer, SIGNAL( timeout() ), this, SLOT( updateTimer() ) );

	// Get the client configuration from XmlUtil

    QString xml;
    QFile file( QString("resources/client_config.xml") );
    if( !file.exists() )
    {
        showInfo( 1, "client_config.xml does not exist", "Make sure file is ready" );
        exit();
    }
    else if( !file.open( QIODevice::ReadOnly ) )
    {
        showInfo( 1, "Can't open client_config.xml", "Make sure file is ready" );
        exit();
    }
	QTextStream stream( &file );
	QString line;
    do {
	    line = stream.readLine();
        xml.append( line );
    } while( !line.isNull() );

	ClientConfig config;
    XmlUtil::getInstance().readNetConfig( xml, config );

    m_network->connectToHost ( config.ip , config.port );

    qCount = 0;
    time = 0;
    status = CONTEST_STOPPED;
    qStatus = QUESTION_STOPPED;
    loggedIn = false;
    closing = false;
}

ContestantApp::~ContestantApp()
{
	delete m_network;
	delete m_login_w;
	delete m_welcome_w;
    delete m_elims_w;
    delete m_semifinals_w;
    delete m_finalsChoice_w;
    delete m_finalsIdent_w;
    delete m_summary_w;
    delete timer;
}


void ContestantApp::onConnect()
{
    showInfo( 0, "Connected to server", "" );
	m_login_w->show();
}

void ContestantApp::onDisconnect()
{
    if( !closing )
    {
        showInfo( 1, "Disconnected from server", "Please reconnect if still in the middle of the contest" );
        m_welcome_w->hide();
        m_elims_w->hide();
        m_semifinals_w->hide();
        m_finalsChoice_w->hide();
        m_finalsIdent_w->hide();
        m_summary_w->hide();
        m_login_w->show();
        round = 0;
    }
}

void ContestantApp::onAuthenticate ( bool result )
{
	if ( result )
	{
        loggedIn = true;
		m_login_w->hide();
		m_welcome_w->show();
        m_network->getContestState();

        QString name = m_login_dlg->username_line->text();
        m_welcome_dlg->name_lbl->setText( name );
        m_elims_dlg->name_lbl->setText( name );
        m_semifinals_dlg->name_lbl->setText( name );
        m_finalsChoice_dlg->name_lbl->setText( name );
        m_finalsIdent_dlg->name_lbl->setText( name );
	}
	else
	{
        showInfo( 1, UNAUTH_TEXT, UNAUTH_INFORMATION );
        m_login_w->show();
	}
}

void ContestantApp::onContestStateChange ( int r, CONTEST_STATUS s )
{
    if( !loggedIn )
        return;

    if( round != r )
    {
        round = r;
        ad.clear();
        qCount = 0;
        time = 0;
        status = CONTEST_STOPPED;
        qStatus = QUESTION_STOPPED;
    }

    m_network->qDataRequest( r );

    if( round == 3 || round == 4 )
        m_welcome_dlg->start_btn->setEnabled( false );
    if( round == 4 )
    {
        m_finalsChoice_w->setWindowTitle("Tie-Breaker Round");
        m_finalsIdent_w->setWindowTitle("Tie-Breaker Round");
    }
    status = s;

    if( status == CONTEST_RUNNING )
    {
        displayStatus();
        if( round == 1 || round == 2 )
            m_network->getContestTime();
    }
    else if( status == CONTEST_STOPPED )
        stopContest();
    else if( status == CONTEST_PAUSED )
        pauseContest();
}

void ContestantApp::onQuestionStateChange( ushort q, ushort t, QUESTION_STATUS s )
{
    if( !loggedIn )
        return;
    if( q >= sd.questions.size() )
        return;

    if( status == CONTEST_RUNNING )
    {
        m_welcome_w->hide();
        time = t;

        qCount = q;
        m_finalsChoice_dlg->time_lbl->setText("");
        m_finalsIdent_dlg->time_lbl->setText("");

        qStatus = s;

        displayQuestionAndChoices();

        if( qStatus == QUESTION_STOPPED )
            stopQuestion();
        else if( qStatus == QUESTION_RUNNING )
            runQuestion();
        else if( qStatus == QUESTION_PAUSED )
            pauseQuestion();
        else
            stopQuestion();
    }    
}

void ContestantApp::onContestTime( ushort t )
{
    time = t;
    runContest();
}

void ContestantApp::onQData ( const QString& xml )
{
    if( !loggedIn )
        return;

    sd.questions.clear();
	XmlUtil::getInstance().readStageData( xml, sd );
	m_welcome_dlg->instructions_txt->setPlainText( sd.welcome_msg );  


    if( status == CONTEST_RUNNING || status == CONTEST_PAUSED )
        if ( round == 3 || round == 4 )
            m_network->getQuestionState();

}

void ContestantApp::onAData ( bool result )
{
    if( result )
    {
        showInfo( 0, "Answers successfully sent to server", "" );
        if( round == 1 || round == 2 )
        {
            m_summary_w->hide();
            m_welcome_w->show();
            m_welcome_dlg->start_btn->setEnabled( false );
        }
    }
    else
        showInfo( 1, "Answers not sent. Please try again.", "" );
}

void ContestantApp::onContestError ( ERROR_MESSAGES err )
{
    switch ( err )
    {
        case ERR_NOTAUTHORIZED:
            showInfo( 1, "Server returned that we're not authorized", "" );
            break;
        case ERR_BADCOMMAND:
            showInfo( 1, "Server returned bad command", "" );
            break;
        case ERR_CONTEST_STOPPED:
            showInfo( 1, "Contest is stopped", "" );
            break;
        case ERR_UNKNOWN:
            showInfo( 1, "Server returned unknown error", "" );
            break;
        default:
            assert ( false );
    }
}

void ContestantApp::onError ( const QString& err )
{
    showInfo( 1, err, "" );
}

void ContestantApp::updateTimer()
{
    time--;
    if( time <= 0 )
    {
        if( round == 1 || round == 2 )
        {
            status = CONTEST_STOPPED;
            stopContest();
            m_elims_dlg->time_lbl->setText("");
            m_semifinals_dlg->time_lbl->setText("");   
        }
        if( round == 3 || round == 4 )
        {
            qStatus = QUESTION_PAUSED;
            pauseQuestion();
            m_finalsChoice_dlg->time_lbl->setText("");
            m_finalsIdent_dlg->time_lbl->setText("");
        }
        m_welcome_dlg->time_lbl->setText("");
        return;
    }

    int minute = time/60;
    int second = time - minute*60;
    QString t;
    if( minute > 0 )
    {
        t.append( QString::number(minute) );
        t.append(":");
    }

    if( minute > 0 && second < 10 )
        t.append("0");

    t.append( QString::number(second) );

    m_welcome_dlg->time_lbl->setText( t );
    if( round == 1 )
        m_elims_dlg->time_lbl->setText( t );
    else if( round == 2 )
        m_semifinals_dlg->time_lbl->setText( t );
    else if( round == 3 || round == 4 )
    {
        m_finalsChoice_dlg->time_lbl->setText( t );
        m_finalsIdent_dlg->time_lbl->setText( t );
    }
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
    if( !loggedIn )
        return;

	m_welcome_w->hide();
    if( round == 1 )
        m_elims_w->show();
    else if( round == 2 )
        m_semifinals_w->show();

    //finals/tiebreaker dialog showing handled by displayQuestionAndChoices
    if( round == 1 || round == 2 )
        initializeAnswerData();
    displayQuestionAndChoices();
}

void ContestantApp::elimSemiNext()
{
    recordAnswer();
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
        displayAnswer();
	}
}

void ContestantApp::elimSemiPrev()
{
    recordAnswer();
    bool atFirstQuestion = ( qCount == 0 );

    if( atFirstQuestion )
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
        displayAnswer();
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
    else if( round == 2 )
    {
        m_semifinals_w->show();
        qCount = sd.questions.size() - 1;
    }
	displayQuestionAndChoices();
}

void ContestantApp::submit()
{
    if( !loggedIn )
        return;

    m_network->aDataSend( round, ad );
}

void ContestantApp::finalsSubmit()
{
    QMessageBox msg;
    msg.setWindowTitle("Confirm answer");
    msg.setText("Is this your final answer?");
    msg.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok );
    msg.setDefaultButton(QMessageBox::Cancel);
    int ret = msg.exec();

    if( ret == QMessageBox::Ok )
    {
        recordAnswer();
        m_network->aDataSend( round, ad );
    }
}

//convenience methods

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
    else if( round == 3 || round == 4 )
    {
        if( q.type == Question::CHOOSE_ANY || q.type == Question::CHOOSE_ONE )
        {
            m_finalsChoice_dlg->question_lbl->setText ( q.question );
            m_finalsChoice_dlg->a_choice->setText ( q.answer_key[0].c );
            m_finalsChoice_dlg->b_choice->setText ( q.answer_key[1].c );
            m_finalsChoice_dlg->c_choice->setText ( q.answer_key[2].c );
            m_finalsChoice_dlg->d_choice->setText ( q.answer_key[3].c );

            m_finalsIdent_w->hide();
            m_finalsChoice_w->show();
        }
        else if( q.type == Question::IDENTIFICATION )
        {
            m_finalsIdent_dlg->question_lbl->setText( q.question );

            m_finalsChoice_w->hide();
            m_finalsIdent_w->show();
        }
    }
}

void ContestantApp::displayAnswer()
{
    if( round == 1 )
    { 
        //to ensure no button is checked by default (previous dialog selection)
        if( ad[qCount].multi_choice.size() == 0 )
        {
            m_elims_dlg->a_choice->setCheckable(false);
            m_elims_dlg->b_choice->setCheckable(false);
            m_elims_dlg->c_choice->setCheckable(false);
            m_elims_dlg->d_choice->setCheckable(false);
            m_elims_dlg->a_choice->setCheckable(true);
            m_elims_dlg->b_choice->setCheckable(true);
            m_elims_dlg->c_choice->setCheckable(true);
            m_elims_dlg->d_choice->setCheckable(true);
        }

        for( int i = 0; i < ad[qCount].multi_choice.size(); i++ )
        {
            if( ad[qCount].multi_choice[i] == 1 )
                m_elims_dlg->a_choice->setChecked( true );
            else if( ad[qCount].multi_choice[i] == 2 )
                m_elims_dlg->b_choice->setChecked( true );
            else if( ad[qCount].multi_choice[i] == 3 )
                m_elims_dlg->c_choice->setChecked( true );
            else if( ad[qCount].multi_choice[i] == 4 )
                m_elims_dlg->d_choice->setChecked( true );
        }
    }
    else if( round == 2 )
    {
        std::vector<ushort> ans;

        for( int i = 0; i < ad[qCount].multi_choice.size(); i++ )
            ans.push_back( ad[qCount].multi_choice[i] );

        m_semifinals_dlg->a_choice->setChecked(false);
        m_semifinals_dlg->b_choice->setChecked(false);
        m_semifinals_dlg->c_choice->setChecked(false);
        m_semifinals_dlg->d_choice->setChecked(false);

        for( int i = 0; i < ans.size(); i++ )
        {
            if( ans[i] == 1 )
                m_semifinals_dlg->a_choice->setChecked( true );
            else if( ans[i] == 2 )
                m_semifinals_dlg->b_choice->setChecked( true );
            else if( ans[i] == 3 )
                m_semifinals_dlg->c_choice->setChecked( true );
            else if( ans[i] == 4 )
                m_semifinals_dlg->d_choice->setChecked( true );
        }
    }
}

void ContestantApp::displayStatus()
{
    QString s;
    if( status == CONTEST_RUNNING )
        s = "RUNNING";
    else if( status == CONTEST_STOPPED )
        s = "STOPPED";
    else if( status == CONTEST_PAUSED )
        s = "PAUSED";

    m_welcome_dlg->status_lbl->setText( s );
    m_elims_dlg->status_lbl->setText( s );
    m_semifinals_dlg->status_lbl->setText( s );
    m_finalsChoice_dlg->status_lbl->setText( s );
    m_finalsIdent_dlg->status_lbl->setText( s );
}

void ContestantApp::recordAnswer()
{
    if( round == 1 )
    {
        ad[qCount].multi_choice.clear();
        if( m_elims_dlg->a_choice->isChecked() )
            ad[qCount].multi_choice.push_back(1);
        else if( m_elims_dlg->b_choice->isChecked() )
            ad[qCount].multi_choice.push_back(2);
        else if( m_elims_dlg->c_choice->isChecked() )
            ad[qCount].multi_choice.push_back(3);
        else if( m_elims_dlg->d_choice->isChecked() )
            ad[qCount].multi_choice.push_back(4);
    }
    else if( round == 2 )
    {
        ad[qCount].multi_choice.clear();
        if( m_semifinals_dlg->a_choice->isChecked() )
            ad[qCount].multi_choice.push_back(1);
        if( m_semifinals_dlg->b_choice->isChecked() )
            ad[qCount].multi_choice.push_back(2);
        if( m_semifinals_dlg->c_choice->isChecked() )
            ad[qCount].multi_choice.push_back(3);
        if( m_semifinals_dlg->d_choice->isChecked() )
            ad[qCount].multi_choice.push_back(4);
    }
    else if( round == 3 || round == 4 )
    {
        ad.clear();
        Answer a;
        if( sd.questions[qCount].type == Question::IDENTIFICATION )
            a.ans_type = Question::IDENTIFICATION;
        else
            a.ans_type = sd.questions[qCount].type;

        a.id_answer = m_finalsIdent_dlg->answer_line->text();

        if( m_finalsChoice_dlg->a_choice->isChecked() )
            a.multi_choice.push_back(1);
        if( m_finalsChoice_dlg->b_choice->isChecked() )
            a.multi_choice.push_back(2);
        if( m_finalsChoice_dlg->c_choice->isChecked() )
            a.multi_choice.push_back(3);
        if( m_finalsChoice_dlg->d_choice->isChecked() )
            a.multi_choice.push_back(4);
        ad.push_back( a );
    }
}

void ContestantApp::initializeAnswerData()
{
    if( sd.questions.size() == ad.size() )
        return;

    for( int i = 0; i < sd.questions.size(); i++ )
    {
        Answer a;
        a.ans_type = sd.questions[i].type;
        ad.push_back( a );
    }
}

void ContestantApp::stopContest()
{
    timer->stop();

    m_welcome_dlg->time_lbl->setText("");
    m_elims_w->hide();
    m_semifinals_w->hide();
    m_finalsChoice_w->hide();
    m_finalsIdent_w->hide();
    m_summary_w->hide();

    qCount = 0;
    m_welcome_dlg->start_btn->setEnabled( false );
    m_welcome_w->show();

    displayStatus();
}

void ContestantApp::pauseContest()
{
    timer->stop();

    m_welcome_dlg->start_btn->setEnabled( false );
    m_elims_dlg->prev_btn->setEnabled( false );
    m_elims_dlg->next_btn->setEnabled( false );

    m_semifinals_dlg->prev_btn->setEnabled( false );
    m_semifinals_dlg->next_btn->setEnabled( false );

    m_summary_dlg->submit_btn->setEnabled( false );

    pauseQuestion();
    displayStatus();
}

void ContestantApp::runContest()
{
    if( round == 1 || round == 2 )
        m_welcome_dlg->start_btn->setEnabled( true );
    else if( round == 3 || round == 4 )
        m_welcome_dlg->start_btn->setEnabled( false );

    m_elims_dlg->prev_btn->setEnabled( true );
    m_elims_dlg->next_btn->setEnabled( true );

    m_semifinals_dlg->prev_btn->setEnabled( true );
    m_semifinals_dlg->next_btn->setEnabled( true );

    m_summary_dlg->submit_btn->setEnabled( true );

    if( round == 1 || round == 2 )
        timer->start( 1000 );
    displayStatus();
}

void ContestantApp::stopQuestion()
{
    timer->stop();

    m_finalsChoice_dlg->a_choice->setChecked(false);
    m_finalsChoice_dlg->b_choice->setChecked(false);
    m_finalsChoice_dlg->c_choice->setChecked(false);
    m_finalsChoice_dlg->d_choice->setChecked(false);
    m_finalsChoice_dlg->a_choice->setCheckable(false);
    m_finalsChoice_dlg->b_choice->setCheckable(false);
    m_finalsChoice_dlg->c_choice->setCheckable(false);
    m_finalsChoice_dlg->d_choice->setCheckable(false);
    m_finalsChoice_dlg->submit_btn->setEnabled(false);

    m_finalsIdent_dlg->answer_line->setText("");
    m_finalsIdent_dlg->answer_line->setEnabled(false);
    m_finalsIdent_dlg->submit_btn->setEnabled(false);

    displayStatus();
}

void ContestantApp::pauseQuestion()
{
    timer->stop();

    m_finalsChoice_dlg->a_choice->setEnabled(false);
    m_finalsChoice_dlg->b_choice->setEnabled(false);
    m_finalsChoice_dlg->c_choice->setEnabled(false);
    m_finalsChoice_dlg->d_choice->setEnabled(false);
    m_finalsChoice_dlg->submit_btn->setEnabled(false);

    m_finalsIdent_dlg->answer_line->setEnabled(false);
    m_finalsIdent_dlg->submit_btn->setEnabled(false);

    displayStatus();
}

void ContestantApp::runQuestion()
{
    m_finalsChoice_dlg->a_choice->setEnabled(true);
    m_finalsChoice_dlg->b_choice->setEnabled(true);
    m_finalsChoice_dlg->c_choice->setEnabled(true);
    m_finalsChoice_dlg->d_choice->setEnabled(true);
    m_finalsChoice_dlg->a_choice->setCheckable(true);
    m_finalsChoice_dlg->b_choice->setCheckable(true);
    m_finalsChoice_dlg->c_choice->setCheckable(true);
    m_finalsChoice_dlg->d_choice->setCheckable(true);
    m_finalsChoice_dlg->submit_btn->setEnabled(true);

    m_finalsIdent_dlg->answer_line->setEnabled(true);
    m_finalsIdent_dlg->submit_btn->setEnabled(true);

    timer->start( 1000 );
    displayStatus();
}

void ContestantApp::showInfo( int i, QString s, QString inform )
{
    QMessageBox msg;
    if( i == 0 )
        msg.setWindowTitle( "Success" );
    else if( i == 1 )
        msg.setWindowTitle( "Error" );
    else if( i == 2 )
        msg.setWindowTitle( "Information" );

    msg.setText( s );
    msg.setInformativeText( inform );
    msg.setStandardButtons( QMessageBox::Ok );
    msg.setDefaultButton( QMessageBox::Ok );
    msg.setIcon( QMessageBox::Information );
    msg.exec();
}

void ContestantApp::closeEvent ( QCloseEvent * event )
{
    closing = true;
    event->accept();
}

int main ( int argc, char* argv[] )
{
	QApplication app ( argc, argv );

	ContestantApp c_app;
    c_app.show();
    c_app.resize( 500, 500 );
	return app.exec();
}
