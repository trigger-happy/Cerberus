#include "net/contestant_net.h"

ContestantNetwork::ContestantNetwork(QObject* parent) : QObject(parent){
	m_socket = new QTcpSocket();
	connect(m_socket, SIGNAL(connected()), this, SLOT(connected()));
	connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
}

ContestantNetwork::~ContestantNetwork(){
	delete m_socket;
}

void ContestantNetwork::connectToHost(const QString& ip, int port){
	m_socket->connectToHost(ip, port);
}

void ContestantNetwork::disconnectFromHost(){
}

void ContestantNetwork::authenticate(const QString& user_name, const QString& pw){
}

int ContestantNetwork::r1QDataRequest(){
}

int ContestantNetwork::r1ADataSend(const QString& xml){
}

void ContestantNetwork::connected(){
}

void ContestantNetwork::disconnected(){
}

void ContestantNetwork::error(const QAbstractSocket::SocketError& err){
}

