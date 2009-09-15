#ifndef CLIENT_H
#define CLIENT_H
#include <QtGui/QDialog>
#include <QtNetwork/QtNetwork>

namespace Ui{
	class client_dlg;
}

class ClientDlg : public QDialog{
	Q_OBJECT;
public:
	ClientDlg(QWidget* parent = 0);
	~ClientDlg();
private slots:
	void onConnect();
	void onQuit();
private:
	Ui::client_dlg* m_dlg;
};

#endif