#ifndef PROJECTOR_PROJECTORWINDOW_H_
#define PROJECTOR_PROJECTORWINDOW_H_

#include <QtGui/QMainWindow>

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
};

#endif // PROJECTOR_PROJECTORWINDOW_H_
