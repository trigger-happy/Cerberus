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

#ifndef PROJECTOR_MAINCONTROLLER_H
#define PROJECTOR_MAINCONTROLLER_H

#include "ProjectorController.h"
#include "TemplateManager.h"
#include <QObject>
#include "net/projector_net.h"
#include "data_types.h"
#include <vector>

/**
 * \brief Projector controller for the actual thing.
 */
class MainController : public QObject, public ProjectorController
{
	Q_OBJECT
	ProjectorNet *m_net;
	std::vector<StageData> m_stageData;
	///Counter to keep track the number of stage data received.
	int m_stageDataCounter;

	bool m_connected;
	///The last view shown by the system (so we can easily re-show it when the server reconnects)
	TemplateManager::TKey m_view;
	int m_activeQuestionIndex;
	int m_activeRound;
	QString m_lastError;

	bool assertQuestionState(ushort qnum) const;
	static QString printfAnswerKey(const Question &q);

	void resetData();
	StageData& getCurrentStage();
	const StageData& getCurrentStage() const {
		return const_cast<MainController*>(this)->getCurrentStage();
	}
public:
	MainController(ProjectorWindow &target);
	~MainController();
	virtual bool keyReleaseEvent(QKeyEvent *event);

public slots:
	void onConnect();
	void onDisconnect();
	void onError( const QString& error );

	void onStageData(ushort round, const QString &xml);
	void onContestState( ushort round, CONTEST_STATUS status );
	void onContestTime( ushort time );
	void onShowContestTime();
	void onShowContestRanks( const vector<RankData>& rd );
	void onQuestionState( ushort qnum, ushort time, QUESTION_STATUS status );
	void onShowAnswer();
	void onHideAnswer();
	void onShowMainScreen();
	void onRoundsReceived(const ushort nRounds);

	void connectToServer();
};

#endif // PROJECTOR_MAINCONTROLLER_H
