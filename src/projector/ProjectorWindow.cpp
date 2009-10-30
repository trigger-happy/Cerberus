#include "ProjectorWindow.h"
#include "ui_ProjectorWindow.h"

ProjectorWindow::ProjectorWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::ProjectorWindow)
{
    m_ui->setupUi(this);
}

ProjectorWindow::~ProjectorWindow()
{
    delete m_ui;
}

void ProjectorWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
