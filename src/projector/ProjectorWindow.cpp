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
#include <algorithm>
#include <cstdio>
#include "ProjectorWindow.h"
#include "ui_ProjectorWindow.h"
#include <ctemplate/template.h>
#include "data_types.h"
#include "util/xml_util.h"
#include "MainController.h"
#include "AuthorController.h"

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
	m_dict(new ctemplate::TemplateDictionary("main")),
	m_qDisplayDict(new ctemplate::TemplateDictionary("qdisplay")),
	m_rankDict(0),
	m_timer(ProjectorConfig::DEFAULT_TIME_PRECISION)
{
	connect(&m_timer, SIGNAL(timeUpdate(uint)), this, SLOT(setTimeLeft(uint)));
	m_ui->setupUi(this);
	this->setWindowTitle(WINDOW_TITLE);
	showFullScreen();
}

ProjectorWindow::~ProjectorWindow()
{
	delete m_qDisplayDict;
	delete m_rankDict;
	delete m_dict;
	delete m_controller;
	delete m_cfg;
	delete m_ui;
}

static const unsigned int MSEC_PER_SEC = 1000;

void ProjectorWindow::setTimeLeft(unsigned int val) {
	const QVariant &result =
			m_ui->webView->page()->mainFrame()->
			evaluateJavaScript(
					JS_TRIGGER_TIME.arg(QString::number(val)));

	static char buffer[1024];
	//format the thing differently if it's over a minute
	if ( val > MSEC_PER_SEC * 60 ) {
		std::sprintf(buffer, "%d:%.1lf",
					 val/(MSEC_PER_SEC * 60),
					 (double)(val % (MSEC_PER_SEC * 60))/MSEC_PER_SEC);

		ctemplate::TemplateDictionary::SetGlobalValue(
				"TIME_LEFT", buffer);
	} else {
		std::sprintf(buffer, "%.1lf", (double)val/MSEC_PER_SEC);

		ctemplate::TemplateDictionary::SetGlobalValue(
				"TIME_LEFT",
				buffer);
	}

	if ( result.toBool() ) return;
	//The page has no handler which accepts time update, refresh the page.
	refresh();
}

void ProjectorWindow::setTemplate(ctemplate::Template *tpl) {
	Q_ASSERT(tpl != 0);
	std::string buffer;
	tpl->Expand(&buffer, m_dict);
	m_ui->webView->setHtml(QString(buffer.c_str()), m_base_url);
}

void ProjectorWindow::setView(TemplateManager::TKey v) {
	setTemplate(m_tpl_mgr.getTemplate(m_tpl_key = v));
}

void ProjectorWindow::displayError(const char *brief, const char *detail, bool persist) {
	m_dict->SetValue("ERROR_BRIEF", brief);
	m_dict->SetValue("ERROR_DETAIL", detail);
	if ( persist )
		m_tpl_key = TemplateManager::ERROR;
	setTemplate(m_tpl_mgr.getTemplate(TemplateManager::ERROR));
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
	if ( !file.open(QIODevice::ReadOnly) ) {
		displayError("Unable to open configuration file", "", true);
		return;
	}
	try {
		ProjectorConfig cfg;
		XmlUtil::getInstance().readProjectorConfig(file.readAll(), cfg);
		setConfig(cfg);
	} catch ( XmlUtil::XmlException &e ) {
		displayError("Error while parsing configuration file", e.what(), true);
	}
}

void ProjectorWindow::setConfig(const ProjectorConfig &cfg) {
	bool change_ctrl = false;
	if ( cfg.hasAuthorMode() != m_cfg->hasAuthorMode() || m_controller == NULL )
		change_ctrl = true;

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

	ctemplate::TemplateDictionary::SetGlobalValue("CONTEST_NAME", cfg.contest_name.toStdString());
	m_timer.setInterval(cfg.time_precision);

	if ( change_ctrl ) {
		delete m_controller;
		if ( m_cfg->hasAuthorMode() )
			m_controller = new AuthorController(*this);
		else
			m_controller = new MainController(*this);
	}
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

void ProjectorWindow::refresh() {
	setTemplate(m_tpl_mgr.getTemplate(m_tpl_key));
}


void ProjectorWindow::flushQDisplay() {
	//ctemplate::TemplateDictionary has no "clear" function,
	//so we'll have to destroy it and recreate it ourselves
	delete m_qDisplayDict;
	m_qDisplayDict = new ctemplate::TemplateDictionary("qdisplay");
}

void ProjectorWindow::setQuestionNumber(int qnum) {
	ctemplate::TemplateDictionary::SetGlobalValue("QUESTION_NUMBER",
												  QString::number(qnum).toStdString());
}

void ProjectorWindow::setQuestionTotal(size_t nQuestions) {
	ctemplate::TemplateDictionary::SetGlobalValue("QUESTION_TOTAL",
												  QString::number(nQuestions).toStdString());
}

void ProjectorWindow::setAnswer(const QString &theAnswer) {
	m_qDisplayDict->SetValue("ANSWER", theAnswer.toStdString());
	m_qDisplayDict->ShowSection("ANSWER_SECTION");
}

void ProjectorWindow::setQuestion(const QString &theQuestion) {
	m_qDisplayDict->SetValue("QUESTION", theQuestion.toStdString());
	m_qDisplayDict->ShowSection("QUESTION_SECTION");
}

void ProjectorWindow::setStageNumber(int stageNumber) {
	ctemplate::TemplateDictionary::SetGlobalValue("STAGE_NUMBER", QString::number(stageNumber).toStdString());
}

void ProjectorWindow::setContestRanks( const vector<RankData>& rd ) {
	using ctemplate::TemplateDictionary;
	delete m_rankDict;
	m_rankDict = new TemplateDictionary("rankDict");

	for ( size_t i = 0; i < rd.size(); ++i ) {
		const RankData &r = rd[i];
		TemplateDictionary *contestant_section = m_rankDict->AddSectionDictionary("CONTESTANT_SECTION");
		contestant_section->SetIntValue("RANK", r.rank);
		contestant_section->SetValue("NAME", r.fullname.toStdString());
		contestant_section->SetValue("GROUP", r.teamname.toStdString());
		contestant_section->SetFormattedValue("SCORE", "%.2lf", r.score);
	}
}
