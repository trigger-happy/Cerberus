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
#include "TemplateManager.h"

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
	ctemplate::TemplateDictionary *m_dict;
	void setTemplate(ctemplate::Template *tpl);
	void displayError(const char *brief, const char *detail);
public slots:
	void setTimeLeft(unsigned int);
};

#endif // PROJECTOR_PROJECTORWINDOW_H_
