#ifndef CONFIG_EDITOR_H
#define CONFIG_EDITOR_H

#include <QtGui/QMainWindow>

namespace Ui
{
    class config_editor;
}

class ConfigEditor : public QMainWindow
{
    Q_OBJECT

public:
    ConfigEditor(QWidget *parent = 0);
    ~ConfigEditor();
    bool askConfirmation( QString& s );
    bool showInfo( QString s);
public slots:
    void save();
    void load();
    void Port1Changed();
    void Port2Changed();

private:
    Ui::config_editor *ui;
};


#endif // CONFIG_EDITOR_H
