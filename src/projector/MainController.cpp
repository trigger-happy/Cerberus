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
#include "util/xml_util.h"

static const int CONNECT_DELAY = 1000;
static const int RECONNECT_DELAY = 3000;

MainController::MainController(ProjectorWindow &target) :
		ProjectorController(target), m_net(new ProjectorNet(this)), m_stageData(0),
		m_connected(false), m_view(TemplateManager::INDEX), m_activeQuestionIndex(0) {
	connect(m_net, SIGNAL(onConnect()), this, SLOT(onConnect()));
	connect(m_net, SIGNAL(onContestState(ushort,CONTEST_STATUS)), this, SLOT(onContestState(ushort,CONTEST_STATUS)));
	connect(m_net, SIGNAL(onContestTime(ushort)), this, SLOT(onContestTime(ushort)));
	connect(m_net, SIGNAL(onDisconnect()), this, SLOT(onDisconnect()));
	connect(m_net, SIGNAL(onError(QString)), this, SLOT(onError(QString)));
	connect(m_net, SIGNAL(onQuestionState(ushort,ushort,QUESTION_STATUS)), this, SLOT(onQuestionState(ushort,ushort,QUESTION_STATUS)));
	connect(m_net, SIGNAL(onShowAnswer()), this, SLOT(onShowAnswer()));
	connect(m_net, SIGNAL(onShowContestRanks(vector<RankData>)), this, SLOT(onShowContestRanks(vector<RankData>)));
	connect(m_net, SIGNAL(onShowContestTime()), this, SLOT(onShowContestTime()));
	connect(m_net, SIGNAL(onShowQuestion()), this, SLOT(onShowQuestion()));
	connect(m_net, SIGNAL(onStageData(QString)), this, SLOT(onStageData(QString)));

	QTimer::singleShot(CONNECT_DELAY, this, SLOT(connectToServer()));
}

MainController::~MainController() {
	delete m_stageData;
}

bool MainController::keyReleaseEvent(QKeyEvent *event) {
	if ( event->key() == Qt::Key_F5 ) {
		m_target.refresh();
		return true;
	}
	return false;
}

void MainController::onConnect() {
	(qDebug() << "MainController::onConnect\n");
	m_net->sendReadyState();
	m_connected = true;
	//show the last thing that was viewed
	m_target.setView(m_view);
}

void MainController::onDisconnect() {
	(qDebug() << "MainController::onDisconnect\n");
	m_connected = false;
	m_target.displayError("Connection error. Reconnecting...", m_lastError.toStdString().c_str());
	QTimer::singleShot(RECONNECT_DELAY, this, SLOT(connectToServer()));
}

void MainController::onError( const QString& error ) {
	(qDebug() << "MainController::onError\n");
	m_lastError = error;
	if ( m_connected ) {
		//assume onDisconnect will get called...
		m_target.displayError("Connection error.", error.toStdString().c_str());
	} else {
		onDisconnect();
	}

}

void MainController::onContestState( ushort round, CONTEST_STATUS status ) {
	(qDebug() << "MainController::onContestState(" << round << ", " << status << ")\n");
	m_target.setStageNumber(round);
	ContestTimer &tmr = m_target.getContestTimer();
	if ( m_stageData == 0 )
		qWarning() << "MainController::onContestState called without valid stage data.";
	switch ( status ) {
		case CONTEST_RUNNING:
			//start the thingo! (make sure we set the time first)
			if ( m_stageData && m_stageData->hasContestTime() ) {
				tmr.setDuration(m_stageData->contest_time);
				tmr.start();
				m_target.setView(m_view = TemplateManager::TIMEBOARD);
			} else {
				//the contest has no timer?!
				//it must be one of those per-question rounds!
			}
			break;
		case CONTEST_PAUSED:
			tmr.pause();
			break;
		case CONTEST_STOPPED:
			m_net->getStageData(round);
			tmr.stop();
			break;
	}
}

void MainController::onStageData(const QString &xml) {
	(qDebug() << "MainController::onStageData\n");
	//it's time to load a new stage!
	m_target.getContestTimer().stop();
	delete m_stageData;

	m_stageData = new StageData();
	try {
		XmlUtil::getInstance().readStageData(xml, *m_stageData);
		//fukitol: don't do anything else...
		//showing the actual shiznit is left to the other signals
	} catch ( XmlUtil::XmlException &e ) {
		m_target.displayError("Error in parsing stage data", e.what());
	}
}

void MainController::onContestTime( ushort time ) {
	(qDebug() << "MainController::onContestTime(" << time << ")\n");
	m_target.getContestTimer().restart(time * 1000u);
}

void MainController::onShowContestTime() {
	(qDebug() << "MainController::onShowContestTime\n");
	m_target.setTimeLeft(m_target.getContestTimer().timeLeft());
	m_target.setView(m_view = TemplateManager::TIMEBOARD);
}

void MainController::onShowContestRanks( const vector<RankData>& rd ) {
	(qDebug() << "MainController::onShowContestRanks");
	m_target.setContestRanks(rd);
	m_target.setView(m_view = TemplateManager::SCOREBOARD);
}

#include <QDebug>
bool MainController::assertQuestionState(ushort qnum) const {
	if ( m_stageData == 0 ) {
		m_target.displayError("No Stage Data", "No stage data received.", true);
		return false;
	}
	if ( m_stageData->questions.size() <= qnum ) {
		m_target.displayError("Question Request Out of Bounds",
							  QString("Requested for question %1 but stage data only has %2 questions.").
							  arg(QString::number(qnum)).arg(QString::number(m_stageData->questions.size())).
							  toStdString().c_str(),
							  true);
		return false;
	}

	if ( m_stageData->hasContestTime() )
		qWarning() << "assertQuestionState(" << qnum << ") called in a stage that has a contest time (probably stages 3 and 4).";

	return true;
}

void MainController::onQuestionState( ushort qnum, ushort time, QUESTION_STATUS status ) {
	(qDebug() << "MainController::onQuestionState(" << qnum << ", " << time << ", " << status << ")\n");
	if ( !assertQuestionState(qnum) ) return;
	ContestTimer &tmr = m_target.getContestTimer();
	Question &q = m_stageData->questions[qnum];

	m_target.setQuestionNumber(qnum);
	m_target.setQuestionTotal(m_stageData->questions.size());

	switch ( status ) {
		case QUESTION_RUNNING:
			if ( q.hasTimeLimit() ) {
				//we trust that the time sent to us is the "correct"
				//time, so we prefer it over what we have.
				tmr.restart(time * 1000u);
				m_target.setView(m_view = TemplateManager::QDISPLAY);
			}
			//Don't do anything else: since we have the showQuestion and showAnswer
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
			break;
	}
	if ( qnum != m_activeQuestionIndex )
		m_target.flushQDisplay();
	m_activeQuestionIndex = qnum;
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
	(qDebug() << "MainController::onShowAnswer\n");
	if ( !assertQuestionState(m_activeQuestionIndex) ) return;
	m_target.setAnswer(printfAnswerKey(m_stageData->questions[m_activeQuestionIndex]));
	m_target.setView(m_view = TemplateManager::QDISPLAY);
}

void MainController::onShowQuestion() {
	(qDebug() << "MainController::onShowQuestion\n");
	if ( !assertQuestionState(m_activeQuestionIndex) ) return;
	m_target.setQuestion(m_stageData->questions[m_activeQuestionIndex].question);
	m_target.setView(m_view = TemplateManager::QDISPLAY);
}

void MainController::connectToServer() {
	if ( m_connected ) return;
	m_net->connectToHost(m_target.getConfig().ip, m_target.getConfig().port);
}
