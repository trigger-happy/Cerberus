#ifndef DEPLOYER_H
#define DEPLOYER_H

#include <QtGui/QMainWindow>

namespace Ui
{
    class deployerUI;
}

class Installer : public QMainWindow
{
    Q_OBJECT

public:
    Installer (QWidget *parent = 0);
    ~Installer();

public slots:
    void install();

private:
    Ui::deployerUI *ui;
};


#endif // DEPLOYER_H
