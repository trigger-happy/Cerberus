#ifndef PROJECTOR_PROJECTORWINDOW_H_
#define PROJECTOR_PROJECTORWINDOW_H_

#include <QtGui/QMainWindow>
#include <QUrl>
#include "TemplateManager.h"

namespace Ui {
	class ProjectorWindow;
}

struct ProjectorConfig;

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
	TemplateManager m_tpl;
public slots:
	void setTimeLeft(unsigned int);
};

#endif // PROJECTOR_PROJECTORWINDOW_H_
