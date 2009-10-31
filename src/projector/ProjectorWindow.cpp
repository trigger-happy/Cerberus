#include <QFile>
#include "ProjectorWindow.h"
#include "ui_ProjectorWindow.h"
#include <ctemplate/template.h>
#include "data_types.h"
#include "util/xml_util.h"

#define JS_TIMELEFT_EVENT "window.ontimeleft"

const QString WINDOW_TITLE = "Cerberus Contest System";

const QString JS_TRIGGER_TIME =
"if ( " JS_TIMELEFT_EVENT " != null ) {"
"	" JS_TIMELEFT_EVENT "(%1);"
"	return 1;"
"} else return 0;";

ProjectorWindow::ProjectorWindow(QWidget *parent) :
	QMainWindow(parent),
	m_ui(new Ui::ProjectorWindow),
	m_cfg(new ProjectorConfig)
{
	m_ui->setupUi(this);
	//m_ui->webView->setHtml(DEFAULT_PAGE);
	this->setWindowTitle(WINDOW_TITLE);
	showFullScreen();
}

void ProjectorWindow::setTimeLeft(unsigned int val) {

}

ProjectorWindow::~ProjectorWindow()
{
	delete m_ui;
	delete m_cfg;
}

#include <QKeyEvent>
void ProjectorWindow::keyReleaseEvent(QKeyEvent *event) {
	if ( event->key() == Qt::Key_F11 ) {
		if ( isFullScreen() )
			showNormal();
		else
			showFullScreen();
	} else
		QMainWindow::keyReleaseEvent(event);
}

void ProjectorWindow::loadConfigFromFile(const QString &file_path) {
	QFile file(file_path);
	file.open(QIODevice::ReadOnly);
	ProjectorConfig cfg;
	XmlUtil::getInstance().readProjectorConfig(file.readAll(), cfg);
	setConfig(cfg);
}

void ProjectorWindow::setConfig(const ProjectorConfig &cfg) {
	if ( m_cfg != &cfg )
		*m_cfg = cfg;
	m_base_url = QUrl::fromLocalFile(m_cfg->theme_path);
	m_tpl.setTemplatePath(m_cfg->theme_path);
	if ( m_cfg->contest_name.isEmpty() )
		setWindowTitle(WINDOW_TITLE);
	else
		setWindowTitle(QString("%1 - %2").
					   arg(WINDOW_TITLE).
					   arg(cfg.contest_name));

}

const ProjectorConfig& ProjectorWindow::getConfig() const { return *m_cfg; }

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
