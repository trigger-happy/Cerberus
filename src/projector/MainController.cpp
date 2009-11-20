/*
Copyright (C) 2009 Wilhansen Li

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


#include "MainController.h"
#include "ProjectorWindow.h"
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>
#include "util/xml_util.h"

static const int CONNECT_DELAY = 1000;
static const int RECONNECT_DELAY = 3000;

MainController::MainController(ProjectorWindow &target) :
		ProjectorController(target), m_net(new ProjectorNet(this)), m_stageDataCounter(0),
		m_connected(false), m_view(TemplateManager::INDEX),
		m_activeQuestionIndex(0), m_activeRound (0) {
	connect(m_net, SIGNAL(onConnect()), this, SLOT(onConnect()));
	connect(m_net, SIGNAL(onContestState(ushort,CONTEST_STATUS)), this, SLOT(onContestState(ushort,CONTEST_STATUS)));
	connect(m_net, SIGNAL(onContestTime(ushort)), this, SLOT(onContestTime(ushort)));
	connect(m_net, SIGNAL(onDisconnect()), this, SLOT(onDisconnect()));
	connect(m_net, SIGNAL(onError(QString)), this, SLOT(onError(QString)));
	connect(m_net, SIGNAL(onQuestionState(ushort,ushort,QUESTION_STATUS)), this, SLOT(onQuestionState(ushort,ushort,QUESTION_STATUS)));
	connect(m_net, SIGNAL(onShowAnswer()), this, SLOT(onShowAnswer()));
	connect(m_net, SIGNAL(onHideAnswer()), this, SLOT(onHideAnswer()));
	connect(m_net, SIGNAL(onShowContestRanks(vector<RankData>)), this, SLOT(onShowContestRanks(vector<RankData>)));
	connect(m_net, SIGNAL(onShowContestTime()), this, SLOT(onShowContestTime()));
	connect(m_net, SIGNAL(onShowMainScreen()), this, SLOT(onShowMainScreen()));
	connect(m_net, SIGNAL(onStageData(ushort, QString)), this, SLOT(onStageData(ushort, QString)));
	connect(m_net, SIGNAL(onNumRounds(ushort)), this, SLOT(onRoundsReceived(ushort)));

	QTimer::singleShot(CONNECT_DELAY, this, SLOT(connectToServer()));
}

MainController::~MainController() {

}

bool MainController::keyReleaseEvent(QKeyEvent *event) {
	if ( event->key() == Qt::Key_F5 ) {
		m_target.refresh();
		return true;
	}
	return false;
}

void MainController::resetData() {
	m_activeQuestionIndex = 0;
	m_stageDataCounter = 0;
	m_activeRound = 0;

	m_stageData.clear();

	m_target.flushQDisplay();
}

StageData& MainController::getCurrentStage() {
	if ( m_activeRound <= 0 || m_stageData.size() < m_activeRound ) {
		qWarning() << "Round data request out of bounds "
				   << "(round: " << m_activeRound << ") (maximum: " << m_stageData.size() << ")";
	}
	return m_stageData[m_activeRound-1];
}

void MainController::onConnect() {
	(qDebug() << "MainController::onConnect");
	resetData();

	m_net->sendReadyState();
	m_net->getNumRounds();

	m_connected = true;
	//show the last thing that was viewed
	m_target.setView(m_view);
}

void MainController::onDisconnect() {
	(qDebug() << "MainController::onDisconnect");
	//Reset the current data that we hold, the server might change them when we reconnect
	resetData();
	m_connected = false;
	m_target.displayError("Connection error. Reconnecting...", m_lastError.toStdString().c_str());
	QTimer::singleShot(RECONNECT_DELAY, this, SLOT(connectToServer()));
}

void MainController::onError( const QString& error ) {
	(qDebug() << "MainController::onError");
	m_lastError = error;
	if ( m_connected ) {
		//assume onDisconnect will get called...
		m_target.displayError("Connection error.", error.toStdString().c_str());
	} else {
		onDisconnect();
	}

}

void MainController::onRoundsReceived(const ushort nRounds) {
	qDebug() << "MainController::onRoundsReceived(" << nRounds << ")";
	m_stageDataCounter = nRounds;
	m_stageData.resize(nRounds);

        for ( int i = 1; i <= nRounds; ++i ) {
            qDebug() << "Sending stage data request: " << i;
            m_net->getStageData(i);
        }
}

void MainController::onContestState( ushort round, CONTEST_STATUS status ) {
	(qDebug() << "MainController::onContestState(" << round << ", " << status << ")");

	if ( m_stageData.size() > round ) {
		qWarning() << "Round data request out of bounds "
				   << "(round: " << round << ") (maximum: " << m_stageData.size() << ")";
	}

	//if we changed rounds, flush the question display since we might have the leftover
	//question
	if ( m_activeRound != round ) {
		m_target.flushQDisplay();
		m_target.setStageNumber(round);
	}

	m_activeRound = round;

	ContestTimer &tmr = m_target.getContestTimer();

	switch ( status ) {
		case CONTEST_RUNNING:
			//start the thingo! (make sure we set the time first)
			if ( getCurrentStage().hasContestTime() ) {
				tmr.start();
			} else {
				//the contest has no timer?!
				//it must be one of those per-question rounds!
				//stop the timer and set it to "indefinite"
				tmr.stop();
				tmr.setDuration(ContestTimer::INDEFINITE);
			}
			//Show the timeboard regardless.
			m_target.setView(m_view = TemplateManager::TIMEBOARD);
			break;
		case CONTEST_PAUSED:
			tmr.pause();
			m_target.setView(m_view = TemplateManager::TIMEBOARD);
			break;
		case CONTEST_STOPPED:
			tmr.stop();
			m_target.setView(m_view = TemplateManager::INDEX);
			break;
	}
}

#include <sstream>
void MainController::onStageData(ushort round, const QString &xml) {
	(qDebug() << "MainController::onStageData(" << round << ", ...)");

	try {
		XmlUtil::getInstance().readStageData(xml, m_stageData[round-1]);
		if ( --m_stageDataCounter ) {
			//we got everything, now, request for the contest state themselves
			m_net->getContestState();
			m_net->getContestTime();
		}
		//fukitol: don't do anything else...
		//showing the actual shiznit is left to the other signals
	} catch ( XmlUtil::XmlException &e ) {
		std::ostringstream oss;
		oss << "Error in parsing stage data #" << round;
		m_target.displayError(oss.str().c_str(), e.what());
	}
}

void MainController::onContestTime( ushort time ) {
	(qDebug() << "MainController::onContestTime(" << time << ")");
	m_target.getContestTimer().setDuration(time * 1000u);
}

void MainController::onShowContestTime() {
	(qDebug() << "MainController::onShowContestTime");
	m_target.setTimeLeft(m_target.getContestTimer().timeLeft());
	(qDebug() << "\ttime left:" << m_target.getContestTimer().timeLeft());
	m_target.setView(m_view = TemplateManager::TIMEBOARD);
}

void MainController::onShowContestRanks( const vector<RankData>& rd ) {
	(qDebug() << "MainController::onShowContestRanks");
	//TODO: WTF WTF HAX.
	bool showNames = (m_activeRound == 1);
	m_target.setContestRanks(rd, showNames);
	m_target.setView(m_view = TemplateManager::SCOREBOARD);
}

bool MainController::assertQuestionState(ushort qnum) const {
	if ( m_activeRound == 0 ) {
		m_target.displayError("No Stage Data", "No stage data received.", true);
		return false;
	}
	if ( getCurrentStage().questions.size() <= qnum ) {
		m_target.displayError("Question Request Out of Bounds",
							  QString("Requested for question %1 but stage data only has %2 questions.").
							  arg(QString::number(qnum)).arg(QString::number(getCurrentStage().questions.size())).
							  toStdString().c_str(),
							  true);
		return false;
	}

	if ( getCurrentStage().hasContestTime() )
		qWarning() << "assertQuestionState(" << qnum << ") called in a stage that has a contest time (probably stages 3 and 4).";

	return true;
}

void MainController::onQuestionState( ushort qnum, ushort time, QUESTION_STATUS status ) {
	(qDebug() << "MainController::onQuestionState(" << qnum << ", " << time << ", " << status << ")");
	if ( !assertQuestionState(qnum) ) return;
	const StageData &sd = getCurrentStage();
	ContestTimer &tmr = m_target.getContestTimer();
	//note: Question indexes are 0-based
	const Question &q = sd.questions[qnum];

	m_target.setQuestionNumber(qnum + 1);
	m_target.setQuestionTotal(sd.questions.size());

	if ( qnum != m_activeQuestionIndex ) {
		m_target.flushQDisplay();
		m_activeQuestionIndex = qnum;
	}
	m_target.setQuestion(q.question);

	switch ( status ) {
		case QUESTION_RUNNING:
			if ( q.hasTimeLimit() ) {
				//we trust that the time sent to us is the "correct"
				//time, so we prefer it over what we have.
				tmr.restart(time * 1000u);
			} else {
				//we "don't" need the time, so pause it it.
				tmr.stop();
				tmr.setDuration(ContestTimer::INDEFINITE);
			}
			//Don't do anything else: since we have the hideAnser and showAnswer
			//signals, we'll leave it to those signals so show the actual question.
			//Yes, it's kinda stupid that we can start the time without showing the
			//question but meh, don't look at me. I'm just following the protocol..
			break;
		case QUESTION_PAUSED:
			//Again, we trust that the time the server is more "accurate" (though
			//not necessarily more precise) than what we have. so we "pause" the
			//timer at the received time. By stopping and setting the duration
			//at the passed time.
			tmr.stop();
			tmr.setDuration(time * 1000u);
			break;
		case QUESTION_STOPPED:
			tmr.stop();
			tmr.setDuration(0);
			break;
	}
	m_target.setView(m_view = TemplateManager::QDISPLAY);
}

QString MainController::printfAnswerKey(const Question &q) {
	QStringList ret;
	for ( size_t i = 0; i < q.answer_key.size(); ++i) {
		if ( q.answer_key[i].is_answer )
			ret.push_back(q.answer_key[i].c);
	}

	if ( q.type == Question::CHOOSE_ANY )
		return ret.join(" AND ");
	else
		return ret.join(" OR ");
}

void MainController::onShowAnswer() {
	(qDebug() << "MainController::onShowAnswer");
	if ( !assertQuestionState(m_activeQuestionIndex) ) return;
	m_target.setAnswer(printfAnswerKey(getCurrentStage().questions[m_activeQuestionIndex]));
	m_target.setView(m_view = TemplateManager::QDISPLAY);
}

void MainController::onHideAnswer() {
	(qDebug() << "MainController::onHideAnswer");
	if ( !assertQuestionState(m_activeQuestionIndex) ) return;
	m_target.flushQDisplay();
	m_target.setQuestion(getCurrentStage().questions[m_activeQuestionIndex].question);
	m_target.setView(m_view = TemplateManager::QDISPLAY);
}

void MainController::onShowMainScreen() {
	m_target.setView(m_view = TemplateManager::INDEX);
}

void MainController::connectToServer() {
	if ( m_connected ) return;
	m_net->connectToHost(m_target.getConfig().ip, m_target.getConfig().port);
}
