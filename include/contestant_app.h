#ifndef CONTESTANT_APP_H
#define CONTESTANT_APP_H
#include <QtGui/QtGui>
#include <QtNetwork/QtNetwork>

namespace Ui{
	class contestant_dlg;
}

class ContestantNetwork;

class ContestantDlg : public QWidget{
	Q_OBJECT;
public:
	ContestantDlg(QWidget* parent = 0);
	~ContestantDlg();
private slots:
	
	/*!
	Slot for when the server says the contest state changes.
	\param state The state that the contest is in now.
	*/
	void netContestStateChange(int state);
	
	/*!
	Emitted when there's an error with the connection.
	\param err SocketError
	*/
	void netError(const QAbstractSocket::SocketError& err);
	
	/*!
	Emitted when there's a contest error.
	\param err ERROR_MESSAGES indicating the error.
	*/
	void netContestError(quint16 err);
	
	/*!
	Called when the client is able to connect.
	*/
	void netConnect();
	
	/*!
	Slot for when the authentication reply has arrived.
	\param result true if we're in, false if not
	*/
	void netAuthenticate(bool result);
	
	/*!
	When round 1 questions arrive, this gets called.
	\param xml Round 1 question data in xml format.
	*/
	void netR1QData(const QString& xml);
	
	/*!
	Slot for when the server replies to our sent answers.
	\param result true on success, false on failure
	*/
	void netR1AData(bool result);
private:
	Ui::contestant_dlg* m_dlg;
	ContestantNetwork* m_network;
};

#endif //CONTESTANT_APP_H