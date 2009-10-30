#ifndef PROJECTORWINDOW_H
#define PROJECTORWINDOW_H

#include <QtGui/QMainWindow>

namespace Ui {
    class ProjectorWindow;
}

class ProjectorWindow : public QMainWindow {
    Q_OBJECT
public:
    ProjectorWindow(QWidget *parent = 0);
    ~ProjectorWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ProjectorWindow *m_ui;
};

#endif // PROJECTORWINDOW_H
