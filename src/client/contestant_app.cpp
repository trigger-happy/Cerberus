#include "contestant_app.h"
#include "net/contestant_net.h"
#include "error.h"
#include "ui_contestant_app.h"

ContestantDlg::ContestantDlg(QWidget* parent){
	m_network = new ContestantNetwork(this);
}

ContestantDlg::~ContestantDlg(){
	delete m_network;
}

void ContestantDlg::netContestStateChange(int state){
	//TODO: do something here for when the contest state changes
}

void ContestantDlg::netError(const QAbstractSocket::SocketError& err){
	//TODO: do something here when there's a network error.
}

void ContestantDlg::netContestError(quint16 err){
	//TODO: do something here for contest errors.
}

void ContestantDlg::netConnect(){
	//TODO: implement actions for when we have a connection
}

void ContestantDlg::netAuthenticate(bool result){
	//TODO: do something here for authorization replies.
}

void ContestantDlg::netR1QData(const QString& xml){
	//TODO: do something here for question data arriving.
}

void ContestantDlg::netR1AData(bool result){
	//TODO: do something here for replies to answer uploads.
}

int main(int argc, char* argv[]){
	//TODO: implement the app stuff here
}
