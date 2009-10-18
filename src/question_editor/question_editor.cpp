/*
Copyright (C) 2009 Ken Lee

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
#include <QtGui>
#include <QtGui/QApplication>
#include "question_editor.h"
#include "ui_question_editor.h"
#include "util/xml_util.h"
#include <iostream>

using namespace std;

QEditor::QEditor(QWidget *parent) : QMainWindow(parent), q_ui(new Ui::q_editor)
{
	q_ui->setupUi(this);
	QFile f ("resources/stage1_q.xml");
	f.open ( QIODevice::ReadOnly );
	QString xml = f.readAll();
	//q_ui->first_round_list
	//cout << xml;
	model=new QStandardItemModel(2,9);
	/*
	question score time ansa ansb ansc ansd boola boolb boolc boold boole
	*/
	for (int row=0;row<2;row++)
	{
		QString s="Question ";
		s.append(QString::number(row+1));
		QStandardItem* item=new QStandardItem(s);
		// QStandardItem* child = new QStandardItem(QString("Item %0").arg(i) );
		model->setItem(row,0,item);
	}
	q_ui->first_round_list->setModel(model);
	connect(q_ui->first_round_list, SIGNAL(clicked(QModelIndex)),this, SLOT(on_first_list()));
	connect(q_ui->add_q_round_1,SIGNAL(clicked()),this,SLOT(add_question_r1()));
	
	//connect(q_ui->toolButton,SIGNAL(clicked()),this,SLOT(on_first_list()));
	
}
QEditor::~QEditor()
{
	delete q_ui;
}

void QEditor::on_first_list()
{
	q_ui->question1_text->setPlainText(q_ui->first_round_list->currentIndex().data().toString());
	q_ui->question1_a->setText("@_@");
	//q_ui->question1_time->;
	q_ui->question1_score->setValue(60);
}

void QEditor::add_question_r1()
{
	model->appendRow(new QStandardItem("New Question"));
}

int main(int argc, char *argv[])
{
	QApplication app (argc, argv);
	QEditor q_App;
	q_App.show();
	return app.exec();
}
