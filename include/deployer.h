#ifndef DEPLOYER_H
#define DEPLOYER_H

#include <QtGui>
#include <map>
#include <QString>

using namespace std;
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
    map <QString, QString> list;
    void installServer();
};


#endif // DEPLOYER_H
