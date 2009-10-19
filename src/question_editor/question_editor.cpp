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
	QFile f ("../resources/stage1_q.xml");
	f.open ( QIODevice::ReadOnly );
	QString xml = f.readAll();
	model=new QStandardItemModel(2,9);
	/*
	question score time ansa ansb ansc ansd boola boolb boolc boold boole
	*/
	for (int row=0;row<2;row++)
	{
		QString s="Question ";
		s.append(QString::number(row+1));
		QStandardItem* item=new QStandardItem(s);
		model->setItem(row,0,item);
		QStandardItem* another =new QStandardItem("This is a.");
		model->setItem(row,1,another);
	}
	q_ui->list_r1->setModel(model);
	connect(q_ui->list_r1, SIGNAL(clicked(QModelIndex)),this, SLOT(on_first_list()));
	connect(q_ui->button_addq_r1,SIGNAL(clicked()),this,SLOT(add_question_r1()));
	q_ui->textarea_welcome->setPlainText(xml);
	
	round2model=new QuestionModel(12);
	q_ui->list_r2->setModel(round2model);
	connect(q_ui->list_r2, SIGNAL(clicked(QModelIndex)),this, SLOT(on_r2_list()));
	connect(q_ui->button_addq_r2,SIGNAL(clicked()),this,SLOT(add_question_r2()));
	
}
QEditor::~QEditor()
{
	delete q_ui;
}

void QEditor::on_first_list()
{
	int index=q_ui->list_r1->currentIndex().row();
	q_ui->question1_text->setPlainText(q_ui->list_r1->currentIndex().data().toString());
	q_ui->question1_a->setText(model->item(index,1)->text());
	q_ui->question1_score->setValue(60);
}

void QEditor::on_r2_list()
{
	int index=q_ui->list_r2->currentIndex().row();
	q_ui->question_r2_text->setPlainText(q_ui->list_r2->currentIndex().data().toString());
	q_ui->question_r2_a->setText(round2model->item(index,1)->text());
	//q_ui->question1_score->setValue(60);
}

void QEditor::add_question_r1()
{
	model->appendRow(new QStandardItem("New Question"));
}

void QEditor::add_question_r2()
{
	round2model->addNewQuestion();
}

int main(int argc, char *argv[])
{
	QApplication app (argc, argv);
	QEditor q_App;
	q_App.show();
	return app.exec();
}
