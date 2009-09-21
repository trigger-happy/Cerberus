#include <algorithm>
#include "net/server_net.h"

using namespace std;

ContestantConnection::ContestantConnection(QObject* parent) : QObject(parent){
}

void ContestantConnection::setSocket(QTcpSocket* socket){
	m_socket = socket;
	connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(error(QAbstractSocket::SocketError)));
	connect(m_socket, SIGNAL(readyRead()), this, SLOT(ready()));
	connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void ContestantConnection::error(const QAbstractSocket::SocketError& err){
	//do something about the error here
}

void ContestantConnection::ready(){
	//read the socket data
}

void ContestantConnection::disconnected(){
	//do something about our disconnection here
}

ServerNetwork::ServerNetwork(QObject* parent) : QObject(parent){
	//initialize stuff here
	m_server = new QTcpServer(this);
}

ServerNetwork::~ServerNetwork(){
}

void ServerNetwork::listen(quint16 port){
	m_server->listen(QHostAddress::Any, port);
}

void ServerNetwork::newConnection(){
}

void ServerNetwork::contestantDisconnect(ContestantConnection* c){
	concon_list::iterator i = m_contestants.begin();
	while(i != m_contestants.end()){
		if(&(*i) == c){
			break;
		}
		i++;
	}
	if(&(*i) == c){
		m_contestants.erase(i);
	}
}
