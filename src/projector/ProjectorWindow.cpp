#include <QFile>
#include "ProjectorWindow.h"
#include "ui_ProjectorWindow.h"
#include <ctemplate/template.h>
#include "data_types.h"
#include "util/xml_util.h"


const char * const DEFAULT_PAGE =
"<html>"
"	<head><title>No Style Specified</title></head>"
"	<body><h1>No style specified in projector_config.xml</h1></body>"
"</html>";
const QString WINDOW_TITLE = "Cerberus Projector";

ProjectorWindow::ProjectorWindow(QWidget *parent) :
	QMainWindow(parent),
	m_ui(new Ui::ProjectorWindow)
{
	m_ui->setupUi(this);
	m_ui->webView->setHtml(DEFAULT_PAGE);
	this->setWindowTitle(WINDOW_TITLE);
	showFullScreen();
	m_cfg = new ProjectorConfig();
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
	XmlUtil::getInstance().readProjectorConfig(file.readAll(), *m_cfg);
	setConfig(*m_cfg);
}

void ProjectorWindow::setConfig(const ProjectorConfig &cfg) {
	if ( m_cfg != &cfg )
		*m_cfg = cfg;

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
