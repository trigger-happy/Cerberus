#ifndef SERVER_NET_H
#define SERVER_NET_H
#include <QtNetwork/QtNetwork>
#include <list>
#include "patterns/singleton.h"

using std::list;

class ContestantConnection : public QObject{
	Q_OBJECT;
public:
	ContestantConnection(QObject* parent = 0);
	void setSocket(QTcpSocket* m_socket);
public slots:
	void error(const QAbstractSocket::SocketError& err);
	void ready();
	void disconnected();
signals:
private:
	QString* m_r1qdata;
	QTcpSocket* m_socket;
};

typedef list<ContestantConnection> concon_list;

class ServerNetwork : public QObject, public Singleton<ServerNetwork>{
	Q_OBJECT;
public:
	ServerNetwork(QObject* parent = 0);
	~ServerNetwork();
	void listen(quint16 port);
public slots:
	void newConnection();
	void contestantDisconnect(ContestantConnection* c);
signals:
protected:
	//server socket
	QTcpServer* m_server;
	//extra data, we may need a resource manager for these
	QString* m_r1qdata;
	//connections
	concon_list m_contestants;
};

#endif //SERVER_NET_H