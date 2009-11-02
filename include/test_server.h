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
#ifndef SERVER_H
#define SERVER_H
#include <QtGui>
#include <QtNetwork>
#include "net/server_net.h"

namespace Ui {

	class server_dlg;
}

class ServerDlg : public QDialog {
	Q_OBJECT;

public:
	ServerDlg ( QWidget* parent = 0 );
	~ServerDlg();

private slots:
	void onQuestionBtn();
	void onTimeBtn();
	void onStartBtn();
	void onStopBtn();
	void onPauseBtn();
	void onRoundChangeBtn();
	void onQuitBtn();
	void newContestant ( ContestantConnection* cc );
	void badClient ( TempConnection* tc );
	void contestantDisconnect ( ContestantConnection* cc );
	void onAuthenticate( ContestantConnection* cc, const QString& username );
	void onAnswerSubmit( ContestantConnection* cc, int round, const AnswerData& data );
	void onContestTimeRequest( ContestantConnection* cc );
	void newProjector( ProjectorConnection* pc );
	void projectorDisconnect( ProjectorConnection* pc );
	void onQTimeBtn();
	void onShowRanksBtn();
	void onShowQTimeBtn();
	void onShowAnswerBtn();
	void onShowQuestionBtn();
	void onStartQuestionBtn();
	void onStopQuestionBtn();
	void onPauseQuestionBtn();

private:
	void writeLog ( const QString& s );
	QString log;
	Ui::server_dlg* m_dlg;
	ServerNetwork* m_server;
	vector<QString> m_questions;
};

#endif // SERVER_H
