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

class MainController : public QObject, public ProjectorController
{
	Q_OBJECT
	ProjectorNet *m_net;
	StageData *m_stageData;
	bool m_connected;
	TemplateManager::TKey m_view;
	int m_activeQuestionIndex;
	QString m_lastError;

	bool assertQuestionState(ushort qnum) const;
	static QString printfAnswerKey(const Question &q);
public:
	MainController(ProjectorWindow &target);
	~MainController();
	virtual bool keyReleaseEvent(QKeyEvent *event);

public slots:
	void onConnect();
	void onDisconnect();
	void onError( const QString& error );

	void onStageData(const QString &xml);
	void onContestState( ushort round, CONTEST_STATUS status );
	void onContestTime( ushort time );
	void onShowContestTime();
	void onShowContestRanks( const vector<RankData>& rd );
	void onQuestionState( ushort qnum, ushort time, QUESTION_STATUS status );
	void onShowAnswer();
	void onShowQuestion();

	void connectToServer();
};

#endif // PROJECTOR_MAINCONTROLLER_H
