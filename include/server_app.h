#ifndef SERVER_APP_H
#define SERVER_APP_H
#include <QtGui/QtGui>

namespace Ui
{
class server_dlg;
}

class ServerNetwork;

class ServerApp : public QDialog
{
        Q_OBJECT;
public:
        ServerApp ( QWidget* parent = 0 );
        ~ServerApp();
private slots:
        /*!
        Called when there is a new connection
        */
        void netNewConnection();
private:
        ServerNetwork* m_network;
        Ui::server_dlg* m_dlg;

};

#endif //SERVER_APP_H
