#ifndef SERVER_NET_H
#define SERVER_NET_H
#include <QtNetwork/QtNetwork>
#include <list>
#include "patterns/singleton.h"
#include "sql_util.h"

using std::list;

enum CONTESTANT_REQUEST{
	CR_CONTEST_STATE = 0,
	CR_AUTHENTICATE,
	CR_QDATA,
	CR_ADATA
};

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
	void contestantDisconnect(ContestantConnection* cc);
private:
	//some private functions
	void authenticationReply(bool res);
	void sendR1QData(const QString& xml);
	void sendR1AReply(bool res);
	void sendContestState(quint16 state);
	
	//private fields
	QString* m_r1qdata;
	QTcpSocket* m_socket;
	quint16 m_blocksize;
};

typedef list<ContestantConnection*> concon_list;

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