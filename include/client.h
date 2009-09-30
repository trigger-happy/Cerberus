#ifndef CLIENT_H
#define CLIENT_H
#include <QtGui>
#include <QtNetwork>
#include "net/contestant_net.h"

namespace Ui
{
class client_dlg;
}

class ClientDlg : public QDialog
{
        Q_OBJECT;
public:
        ClientDlg ( QWidget* parent = 0 );
        ~ClientDlg();
private slots:
        void onAuthReply ( bool result );
        void onAuthBtn();
        void onConnectBtn();
        void onQuitBtn();
        void onConnect();
        void onError ( QAbstractSocket::SocketError error );
        void onQDR();
        void onADS();
        void onR1QData ( const QString& xml );
        void onR1AData ( bool result );
	void onContestStateChange(int c);
private:
        ContestantNetwork* m_net;
        Ui::client_dlg* m_dlg;
};

#endif
