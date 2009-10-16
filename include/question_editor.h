#ifndef QUESTION_EDITOR_H
#define QUESTION_EDITOR_H
#include <QtGui>

namespace Ui
{
class question_editor;
}

class QEditor : public QMainWindow
{
	Q_Object;
public:
	QEditor(QWidget *parent = 0);
	~QEditor();
private: 
	Ui::question_editor q_ui;
}