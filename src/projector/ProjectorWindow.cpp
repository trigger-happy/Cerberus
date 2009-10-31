/*
Copyright (C) 2009 Wilhansen Li

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <QFile>
#include <QWebFrame>
#include <string>
#include "ProjectorWindow.h"
#include "ui_ProjectorWindow.h"
#include <ctemplate/template.h>
#include "data_types.h"
#include "util/xml_util.h"
#include "ProjectorController.h"

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
	m_cfg(new ProjectorConfig),
	m_tpl_key(TemplateManager::INDEX),
	m_controller(0),
	m_dict(new ctemplate::TemplateDictionary("main"))
{
	m_ui->setupUi(this);
	this->setWindowTitle(WINDOW_TITLE);
	showFullScreen();
}

ProjectorWindow::~ProjectorWindow()
{
	delete m_dict;
	delete m_controller;
	delete m_cfg;
	delete m_ui;
}

void ProjectorWindow::setTimeLeft(unsigned int val) {
	m_dict->SetIntValue("TIME_LEFT", val);
	const QVariant &result =
			m_ui->webView->page()->mainFrame()->
			evaluateJavaScript(
					JS_TRIGGER_TIME.arg(QString::number(val)));
	if ( result.toBool() ) return;
	//The page has no handler which accepts time update, refresh the page.
	setTemplate(m_tpl_mgr.getTemplate(m_tpl_key));
}

void ProjectorWindow::setTemplate(ctemplate::Template *tpl) {
	Q_ASSERT(tpl != 0);
	std::string buffer;
	tpl->Expand(&buffer, m_dict);
	m_ui->webView->setHtml(QString(buffer.c_str()), m_base_url);
}



void ProjectorWindow::displayError(const QString &brief, const QString &summary) {
	m_dict->SetValue("ERROR_BRIEF", brief.toStdString().c_str());
	m_dict->SetValue("ERROR_DETAIL", summary.toStdString().c_str());
	setTemplate(m_tpl_mgr.getTemplate(m_tpl_key));
}

#include <QKeyEvent>
void ProjectorWindow::keyReleaseEvent(QKeyEvent *event) {
	if ( event->key() == Qt::Key_F11 ) {
		if ( isFullScreen() )
			showNormal();
		else
			showFullScreen();
	} else if ( !(m_controller && m_controller->keyReleaseEvent(event)) )
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
	m_tpl_mgr.setTemplatePath(m_cfg->theme_path);
	if ( m_cfg->contest_name.isEmpty() )
		setWindowTitle(WINDOW_TITLE);
	else
		setWindowTitle(QString("%1 - %2").
					   arg(WINDOW_TITLE).
					   arg(cfg.contest_name));

	m_dict->SetValue("CONTEST_NAME", cfg.contest_name.toStdString());
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
