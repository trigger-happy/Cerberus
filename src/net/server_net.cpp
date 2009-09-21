#include <algorithm>
#include <cassert>
#include <iostream>
#include "net/server_net.h"

using namespace std;

ContestantConnection::ContestantConnection(QObject* parent) : QObject(parent){
	m_blocksize = 0;
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
	QDataStream in(m_socket);
	in.setVersion(QDataStream::Qt_4_5);
	if(m_blocksize == 0){
		if(m_socket->bytesAvailable() < (int)sizeof(quint16)){
			return;
		}
		in >> m_blocksize;
	}
	
	if(m_socket->bytesAvailable() < m_blocksize){
		return;
	}
	
	// check what command was sent by the server and react accordingly
	quint16 command;
	in >> command;
	switch(command){
		case CR_CONTEST_STATE:
			//contestant is asking for the contest state.
			{
			}
			break;
		case CR_AUTHENTICATE:
			//contestant wants to authenticate
			{
				QString buffer, user, pass;
				in >> buffer;
				user = buffer.section(",", 0, 0);
				pass = buffer.section(",", 1, 1);
				bool result = SqlUtil::getInstance().authenticate(user, pass);
				authenticationReply(result);
			}
			break;
		case CR_QDATA:
			//contestant is asking for question data
			{
				sendR1QData(*m_r1qdata);
			}
			break;
		case CR_ADATA:
			//contestant has submitted their answers
			{
				QString buffer;
				in >> buffer;
				//TODO: have the buffer contents sent and checked
				//let's reply with something for now
				sendR1AReply(true);
			}
			break;
		default:
			cout << command << endl;
			assert(false);
	}
	m_blocksize = 0;
}

void ContestantConnection::disconnected(){
	//do something about our disconnection here
}

void ContestantConnection::authenticationReply(bool res){
	//construct the packet and send it
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_5);
	out << (quint16)0 << (quint16)1;
	out << res;
	out.device()->seek(0);
	out << (quint16)(block.size()-sizeof(quint16));
	m_socket->write(block);
}

void ContestantConnection::sendR1QData(const QString& xml){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_5);
	out << (quint16)0 << (quint16)2;
	out << xml;
	out.device()->seek(0);
	out << (quint16)(block.size()-sizeof(quint16));
	m_socket->write(block);
}

void ContestantConnection::sendR1AReply(bool res){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_5);
	out << (quint16)0 << (quint16)3;
	out << res;
	out.device()->seek(0);
	out << (quint16)(block.size()-sizeof(quint16));
	m_socket->write(block);
}

void ContestantConnection::sendContestState(quint16 state){
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_5);
	out << (quint16)0 << (quint16)0;
	out << state;
	out.device()->seek(0);
	out << (quint16)(block.size()-sizeof(quint16));
	m_socket->write(block);
}

ServerNetwork::ServerNetwork(QObject* parent) : QObject(parent){
	//initialize stuff here
	m_server = new QTcpServer(this);
	connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
	connect(m_server, SIGNAL(newConnection()), this, SIGNAL(onNewConnection()));
}

ServerNetwork::~ServerNetwork(){
}

void ServerNetwork::listen(quint16 port){
	m_server->listen(QHostAddress::Any, port);
}

void ServerNetwork::newConnection(){
	ContestantConnection* cc = new ContestantConnection(this);
	cc->setSocket(m_server->nextPendingConnection());
	connect(cc, SIGNAL(contestantDisconnect(ContestantConnection*)), 
	this, SLOT(contestantDisconnect(ContestantConnection*)));
	m_contestants.insert(m_contestants.end(), cc);
	//TODO: add more stuff here for when a new client connects
}

void ServerNetwork::contestantDisconnect(ContestantConnection* c){
	//TODO: insert penalty calculation here
	
	//remove it from the list
	concon_list::iterator i = m_contestants.begin();
	while(i != m_contestants.end()){
		if(*i == c){
			break;
		}
		i++;
	}
	if(*i == c){
		delete *i;
		m_contestants.erase(i);
	}
}
