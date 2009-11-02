/*
Copyright (C) 2009 James Choa

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
#ifndef TEST_PROJECTOR_H
#define TEST_PROJECTOR_H
#include <QtGui>
#include <QtNetwork>
#include "net/projector_net.h"

namespace Ui {

	class projector_dlg;
}

class TestProjector : public QDialog {
	Q_OBJECT

public:
	TestProjector( QDialog* parent = 0 );

public slots:
	void onConnectBtn();
	void onQuitBtn();
	// networking slots
	void onConnect();
	void onDisconnect();
	void onContestState( ushort round, CONTEST_STATUS status );
	void onContestTime( ushort time );
	void onShowContestTime();
	// NOTE: params for this will change
	void onShowContestRanks();
	void onQuestionTime();
	void onQuestionState( ushort qnum, ushort time, QUESTION_STATUS status );
	void onShowAnswer();

private:
	void writeLog( const QString& log );
	Ui::projector_dlg* m_dlg;
	ProjectorNet* m_network;
};

#endif //TEST_PROJECTOR_H