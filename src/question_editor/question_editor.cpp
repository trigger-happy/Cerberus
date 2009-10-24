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
	
	//connect(q_ui->list_r1, SIGNAL(clicked(QModelIndex)),this, SLOT(list_focus((int)1)));
	//connect(q_ui->button_add_r1,SIGNAL(clicked()),this,SLOT(add_question_r1()));
	//q_ui->textarea_welcome->setPlainText(xml);
	
	//round2model=new QuestionModel(2);
	//enabled_r2=false;
	//q_ui->list_r2->setModel(round2model);
	//disable_components(2);
	/*connect(q_ui->list_r2, SIGNAL(activated(QModelIndex)),this, SLOT(on_r2_list()));
	connect(q_ui->question_r2_text,SIGNAL(textChanged()),this,SLOT(changed_details_r2()));
	connect(q_ui->question_r2_a,SIGNAL(textChanged(QString)),this,SLOT(changed_details_r2()));
	connect(q_ui->question_r2_b,SIGNAL(textChanged(QString)),this,SLOT(changed_details_r2()));
	connect(q_ui->question_r2_c,SIGNAL(textChanged(QString)),this,SLOT(changed_details_r2()));
	connect(q_ui->question_r2_d,SIGNAL(textChanged(QString)),this,SLOT(changed_details_r2()));
	connect(q_ui->question_r2_score,SIGNAL(valueChanged(int)),this,SLOT(changed_details_r2()));
	connect(q_ui->question_r2_ans_a,SIGNAL(toggled(bool)),this,SLOT(changed_details_r2()));
	connect(q_ui->question_r2_ans_b,SIGNAL(toggled(bool)),this,SLOT(changed_details_r2()));
	connect(q_ui->question_r2_ans_c,SIGNAL(toggled(bool)),this,SLOT(changed_details_r2()));
	connect(q_ui->question_r2_ans_d,SIGNAL(toggled(bool)),this,SLOT(changed_details_r2()));
	
	connect(q_ui->button_add_r2,SIGNAL(clicked()),this,SLOT(add_question_r2()));
	connect(q_ui->button_update_r2,SIGNAL(clicked()),this,SLOT(update_question_r2()));
	
	q_ui->button_update_r1->setEnabled(false);
	q_ui->button_update_r2->setEnabled(false);
	q_ui->button_update_r3->setEnabled(false);
	q_ui->button_update_r4->setEnabled(false);*/
	
	roundmodel[0]=new QuestionModel(1);
	roundmodel[1]=new QuestionModel(2);
	roundmodel[2]=new QuestionModel(3);
	roundmodel[3]=new QuestionModel(4);
	
	question_list[0]=q_ui->list_r1;
	question_list[1]=q_ui->list_r2;
	question_list[2]=q_ui->list_r3;
	question_list[3]=q_ui->list_r4;
	
	question_num[0]=q_ui->question_r1_num;
	question_num[1]=q_ui->question_r2_num;
	question_num[2]=q_ui->question_r3_num;
	question_num[3]=q_ui->question_r4_num;
	
	question_text[0]=q_ui->question_r1_text;
	question_text[1]=q_ui->question_r2_text;
	question_text[2]=q_ui->question_r3_text;
	question_text[3]=q_ui->question_r4_text;
	
	question_a[0]=q_ui->question_r1_a;
	question_a[1]=q_ui->question_r2_a;
	question_a[2]=q_ui->question_r3_a;
	question_a[3]=q_ui->question_r4_a;
	
	question_b[0]=q_ui->question_r1_b;
	question_b[1]=q_ui->question_r2_b;
	question_b[2]=q_ui->question_r3_b;
	question_b[3]=q_ui->question_r4_b;
	
	question_c[0]=q_ui->question_r1_c;
	question_c[1]=q_ui->question_r2_c;
	question_c[2]=q_ui->question_r3_c;
	question_c[3]=q_ui->question_r4_c;
	
	question_d[0]=q_ui->question_r1_d;
	question_d[1]=q_ui->question_r2_d;
	question_d[2]=q_ui->question_r3_d;
	question_d[3]=q_ui->question_r4_d;
	
	question_e[0]=0;
	question_e[1]=0;
	question_e[2]=q_ui->question_r3_e;
	question_e[3]=q_ui->question_r4_e;
	
	question_score[0]=q_ui->question_r1_score;
	question_score[1]=q_ui->question_r2_score;
	question_score[2]=q_ui->question_r3_score;
	question_score[3]=q_ui->question_r4_score;
	
	question_time[0]=0;
	question_time[1]=0;
	question_time[2]=q_ui->question_r3_time;
	question_time[3]=q_ui->question_r4_time;
	
	question_ans_a[0]=q_ui->question_r1_ans_a;
	question_ans_a[1]=q_ui->question_r2_ans_a;
	question_ans_a[2]=q_ui->question_r3_ans_a;
	question_ans_a[3]=q_ui->question_r4_ans_a;
	
	question_ans_b[0]=q_ui->question_r1_ans_b;
	question_ans_b[1]=q_ui->question_r2_ans_b;
	question_ans_b[2]=q_ui->question_r3_ans_b;
	question_ans_b[3]=q_ui->question_r4_ans_b;
	
	question_ans_c[0]=q_ui->question_r1_ans_c;
	question_ans_c[1]=q_ui->question_r2_ans_c;
	question_ans_c[2]=q_ui->question_r3_ans_c;
	question_ans_c[3]=q_ui->question_r4_ans_c;
	
	question_ans_d[0]=q_ui->question_r1_ans_d;
	question_ans_d[1]=q_ui->question_r2_ans_d;
	question_ans_d[2]=q_ui->question_r3_ans_d;
	question_ans_d[3]=q_ui->question_r4_ans_d;
	
	question_ans_e[0]=0;
	question_ans_e[1]=0;
	question_ans_e[2]=q_ui->question_r3_ans_e;
	question_ans_e[3]=q_ui->question_r4_ans_e;
	
	button_update[0]=q_ui->button_update_pre;
	button_update[1]=q_ui->button_update_r1;
	button_update[2]=q_ui->button_update_r2;
	button_update[3]=q_ui->button_update_r3;
	button_update[4]=q_ui->button_update_r4;
	
	button_cancel[0]=q_ui->button_cancel_pre;
	button_cancel[1]=q_ui->button_cancel_r1;
	button_cancel[2]=q_ui->button_cancel_r2;
	button_cancel[3]=q_ui->button_cancel_r3;
	button_cancel[4]=q_ui->button_cancel_r4;
	
	button_add[0]=q_ui->button_add_r1;
	button_add[1]=q_ui->button_add_r2;
	button_add[2]=q_ui->button_add_r3;
	button_add[3]=q_ui->button_add_r4;
	
	button_remove[0]=q_ui->button_remove_r1;
	button_remove[1]=q_ui->button_remove_r2;
	button_remove[2]=q_ui->button_remove_r3;
	button_remove[3]=q_ui->button_remove_r4;
	
	sigToList=new QSignalMapper(this);
	sigToAdd=new QSignalMapper(this);
	sigToUpdate=new QSignalMapper(this);
	
	connect(sigToList,SIGNAL(mapped(int)),this,SLOT(list_focus(int)));
	connect(sigToAdd,SIGNAL(mapped(int)),this,SLOT(add_question(int)));
	connect(sigToUpdate,SIGNAL(mapped(int)),this,SLOT(update_question(int)));
	
	//round1model=new QuestionModel(1);
	//q_ui->list_r1->setModel(round1model);
	for (int ctr=0;ctr<4;ctr++)
	{
		sigToList->setMapping(question_list[ctr],ctr+1);
		sigToAdd->setMapping(button_add[ctr],ctr+1);
		
		connect(question_list[ctr],SIGNAL(clicked(QModelIndex)),sigToList,SLOT(map()));
		connect(button_add[ctr],SIGNAL(clicked(bool)),sigToAdd,SLOT(map()));
	}
	for (int ctr=0;ctr<5;ctr++)
	{
		sigToUpdate->setMapping(button_update[ctr],ctr);
	
		connect(button_update[ctr],SIGNAL(clicked(bool)),sigToUpdate,SLOT(map()));
	}
	
	for (int ctr=0;ctr<4;ctr++)
	{
		question_list[ctr]->setModel(roundmodel[ctr]);
	}
	
	
	//control_components(2,false);
	
}
QEditor::~QEditor()
{
	delete sigToAdd;
	delete sigToList;
	delete sigToUpdate;
	delete q_ui;
	//delete round1model;
	//delete round2model;
	for (int ctr=0;ctr<4;ctr++)
	{
		delete roundmodel[ctr];
	}
}

void QEditor::list_focus(int round)
{
	int ptr=round-1;
	int index=question_list[ptr]->currentIndex().row();
	question_num[ptr]->setText("Question "+QString::number(index+1));
	question_text[ptr]->setPlainText(roundmodel[ptr]->getQuestion(index));
	question_a[ptr]->setText(roundmodel[ptr]->getA(index));
	question_b[ptr]->setText(roundmodel[ptr]->getB(index));
	question_c[ptr]->setText(roundmodel[ptr]->getC(index));
	question_d[ptr]->setText(roundmodel[ptr]->getD(index));
	if (round > 2) question_e[ptr]->setText(roundmodel[ptr]->getE(index));
	question_score[ptr]->setValue(roundmodel[ptr]->getScore(index));
	if (round > 2) question_time[ptr]->setValue(roundmodel[ptr]->getTime(index));
	bool* ans=new bool[5];
	roundmodel[ptr]->getAnskey(index,ans);
	question_ans_a[ptr]->setChecked(ans[0]);
	question_ans_b[ptr]->setChecked(ans[1]);
	question_ans_c[ptr]->setChecked(ans[2]);
	question_ans_d[ptr]->setChecked(ans[3]);
	if (round > 2) question_ans_e[ptr]->setChecked(ans[4]);
	delete ans;
}

void QEditor::add_question(int round)
{
	//cout << round;
	roundmodel[round-1]->addNewQuestion();
}


void QEditor::changed_details_r2()
{
	if (q_ui->list_r2->currentIndex().row()!=-1)
		q_ui->button_update_r2->setEnabled(true);
}

void QEditor::update_question(int round)
{
	if (round==0)
	{
	      
	}
	else
	{ 
		int ptr=round-1;
		int index=question_list[ptr]->currentIndex().row();
		QString question=question_text[ptr]->toPlainText();
		QString a=question_a[ptr]->text();
		QString b=question_b[ptr]->text();
		QString c=question_c[ptr]->text();
		QString d=question_d[ptr]->text();
		QString e;
		if (round > 2) e=question_e[ptr]->text();
		else e="";
		QString anskey="";
		if (question_ans_a[ptr]->isChecked()) anskey.append("1");
		else anskey.append("0");
		if (question_ans_b[ptr]->isChecked()) anskey.append("1");
		else anskey.append("0");
		if (question_ans_c[ptr]->isChecked()) anskey.append("1");
		else anskey.append("0");
		if (question_ans_d[ptr]->isChecked()) anskey.append("1");
		else anskey.append("0");
		
		QString score=QString::number(question_score[ptr]->value());
		QString time;
		if (round>2) {
			if (question_ans_e[ptr]->isChecked()) anskey.append("1");
			else anskey.append("0");
			time=QString::number(question_time[ptr]->value());
		}
		else {
			anskey.append("0");
			time="0";
		}
		roundmodel[ptr]->updateQuestion(index,question,a,b,c,d,e,anskey,score,time);
	}
}

void QEditor::enable_components(int round)
{
	
	if (round==2)
	{
		bool enable=false;
		q_ui->question_r2_num->setEnabled(enable);
		q_ui->question_r2_text->setEnabled(enable);
		q_ui->question_r2_a->setEnabled(enable);
		q_ui->question_r2_b->setEnabled(enable);
		q_ui->question_r2_c->setEnabled(enable);
		q_ui->question_r2_d->setEnabled(enable);
		q_ui->question_r2_score->setEnabled(enable);
		q_ui->question_r2_ans_a->setEnabled(enable);
		q_ui->question_r2_ans_b->setEnabled(enable);
		q_ui->question_r2_ans_c->setEnabled(enable);
		q_ui->question_r2_ans_d->setEnabled(enable);	
	}
}

void QEditor::control_components(int round,bool enable)
{
	int ptr=round-1;
	question_text[ptr]->setEnabled(enable);
	question_a[ptr]->setEnabled(enable);
	question_b[ptr]->setEnabled(enable);
	question_c[ptr]->setEnabled(enable);
	question_d[ptr]->setEnabled(enable);
	question_ans_a[ptr]->setEnabled(enable);
	question_ans_b[ptr]->setEnabled(enable);
	question_ans_c[ptr]->setEnabled(enable);
	question_ans_d[ptr]->setEnabled(enable);
	question_score[ptr]->setEnabled(enable);
	if (round > 2)
	{
		question_e[ptr]->setEnabled(enable);
		question_ans_e[ptr]->setEnabled(enable);
		question_time[ptr]->setEnabled(enable);
	}
}

void QEditor::disable_components(int round)
{
	//int ptr=round-1;
	//question_num[ptr]->setEnabled(
  
	if (round==2)
	{
		bool enable=true;
		q_ui->question_r2_num->setEnabled(enable);
		q_ui->question_r2_text->setEnabled(enable);
		q_ui->question_r2_a->setEnabled(enable);
		q_ui->question_r2_b->setEnabled(enable);
		q_ui->question_r2_c->setEnabled(enable);
		q_ui->question_r2_d->setEnabled(enable);
		q_ui->question_r2_score->setEnabled(enable);
		q_ui->question_r2_ans_a->setEnabled(enable);
		q_ui->question_r2_ans_b->setEnabled(enable);
		q_ui->question_r2_ans_c->setEnabled(enable);
		q_ui->question_r2_ans_d->setEnabled(enable);	
	}
}

int main(int argc, char *argv[])
{
	QApplication app (argc, argv);
	QEditor q_App;
	q_App.show();
	return app.exec();
}
