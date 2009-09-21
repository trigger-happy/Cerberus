#ifndef CONTESTANT_NET_H
#define CONTESTANT_NET_H
#include <QtNetwork/QtNetwork>

enum CCSTATE{
	CCS_DISCONNECTED = 0,
	CCS_AUTHENTICATING,
	CCS_R1QDATA_REQUEST,
	CCS_R1ADATA_SEND,
	CCS_STANDBY
};

enum SERVER_REPLIES{
	SR_CONTEST_STATE = 0,
	SR_AUTHENTICATE,
	SR_QDATA,
	SR_ADATA
};

class ContestantNetwork : public QObject{
	Q_OBJECT;
public:
	ContestantNetwork(QObject* parent = 0);
	~ContestantNetwork();
	void connectToHost(const QString& ip, int port);
	void disconnectFromHost();
	void authenticate(const QString& user_name, const QString& pw);
	int r1QDataRequest();
	int r1ADataSend(const QString& xml);
	inline CCSTATE getState() const{
		return m_state;
	}
protected slots:
	void connected();
	void disconnected();
	void error(const QAbstractSocket::SocketError& err);
	void ready();
signals:
	void onContestStateChange(int state);
	void onError(const QAbstractSocket::SocketError& err);
	void onConnect();
	void onAuthenticate(bool);
	void onR1QData(const QString& xml);
	void onR1AData(bool);
protected:
	QTcpSocket* m_socket;
	bool m_authenticated;
	CCSTATE m_state;
	quint16 m_blocksize;
};

#endif //CONTESTANT_NET_H