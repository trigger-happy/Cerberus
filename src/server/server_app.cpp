#include "server_app.h"
#include "ui_server_app.h"
#include "net/server_net.h"

ServerApp::ServerApp(QWidget* parent) : QDialog(parent), m_dlg(new Ui::server_dlg){
	m_dlg->setupUi(this);
}

ServerApp::~ServerApp(){
	
}

int main(int argc, char* argv[]){
}
