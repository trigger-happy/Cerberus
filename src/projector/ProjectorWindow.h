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

#ifndef PROJECTOR_PROJECTORWINDOW_H_
#define PROJECTOR_PROJECTORWINDOW_H_

#include <QtGui/QMainWindow>
#include <QUrl>
#include <vector>
#include "data_types.h"
#include "TemplateManager.h"
#include "util/ContestTimer.h"

namespace Ui {
	class ProjectorWindow;
}

namespace ctemplate {
	class TemplateDictionary;
}

struct ProjectorConfig;
class ProjectorController;

class ProjectorWindow : public QMainWindow {
	Q_OBJECT
public:
	ProjectorWindow(QWidget *parent = 0);
	~ProjectorWindow();

	void loadConfigFromFile(const QString &file_path);
	void setConfig(const ProjectorConfig &cfg);
	const ProjectorConfig& getConfig() const;

	/************************
	 * View state setters.
	 * Note that these just sets the internal states of the view...
	 * IT DOES NOT alter the view itself. To make the changes
	 * in the internal state affect the view, call refresh()
	 * after calling any of them.
	 */
	void setQuestion(const QString &theQuestion);
	void setAnswer(const QString &theAnswer);
	void flushQDisplay();
	void setStageNumber(int stageNumber);
	void setQuestionNumber(int qnum);
	void setQuestionTotal(size_t nQuestions);
	void setContestRanks( const std::vector<RankData>& rd, bool showNames = true );

	ContestTimer& getContestTimer() { return m_timer; }

	TemplateManager::TKey getView() const { return m_tpl_key; }

	void displayError(const char *brief, const char *detail, bool persist = false);
protected:
	void changeEvent(QEvent *e);
	void keyReleaseEvent(QKeyEvent *event);
private:
	Ui::ProjectorWindow *m_ui;
	ProjectorConfig *m_cfg;
	QUrl m_base_url;
	TemplateManager m_tpl_mgr;
	TemplateManager::TKey m_tpl_key;
	ProjectorController *m_controller;
	ctemplate::TemplateDictionary *m_dict, *m_qDisplayDict, *m_rankDict;
	ContestTimer m_timer;

	void setTemplate(ctemplate::Template *tpl, ctemplate::TemplateDictionary *dict);
public slots:
	void setTimeLeft(unsigned int);
	void refresh();
	void setView(TemplateManager::TKey v);
};

#endif // PROJECTOR_PROJECTORWINDOW_H_
