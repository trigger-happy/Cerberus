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
	
	round1model=new QuestionModel(1);
	q_ui->list_r1->setModel(round1model);
	connect(q_ui->list_r1, SIGNAL(clicked(QModelIndex)),this, SLOT(on_first_list()));
	connect(q_ui->button_add_r1,SIGNAL(clicked()),this,SLOT(add_question_r1()));
	q_ui->textarea_welcome->setPlainText(xml);
	
	round2model=new QuestionModel(2);
	
	enabled_r2=false;
	q_ui->list_r2->setModel(round2model);
	disable_components(2);
	connect(q_ui->list_r2, SIGNAL(activated(QModelIndex)),this, SLOT(on_r2_list()));
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
	q_ui->button_update_r4->setEnabled(false);
	
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
	
}
QEditor::~QEditor()
{
	delete q_ui;
	delete round1model;
	delete round2model;
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
	question_a[ptr]->setText(roundmodel[ptr]->getA(index));
	question_b[ptr]->setText(roundmodel[ptr]->getB(index));
	question_c[ptr]->setText(roundmodel[ptr]->getC(index));
	question_d[ptr]->setText(roundmodel[ptr]->getD(index));
	if (round > 2) question_e[ptr]->setText(roundmodel[ptr]->getE(index));
	question_score[ptr]->setValue(roundmodel[ptr]->getScore(index));
	if (round > 2) question_time[ptr]->setValue(roundmodel[ptr]->getTime(index));
	bool* ans=round1model->getAnskey(index);
	question_ans_a[ptr]->setChecked(ans[0]);
	question_ans_b[ptr]->setChecked(ans[1]);
	question_ans_c[ptr]->setChecked(ans[2]);
	question_ans_d[ptr]->setChecked(ans[3]);
	if (round > 2) question_ans_e[ptr]->setChecked(ans[4]);
	delete ans;
}

void QEditor::on_first_list()
{
	int index=q_ui->list_r1->currentIndex().row();
	q_ui->question_r1_num->setText("Question "+QString::number(index+1));
	q_ui->question_r1_text->setPlainText(q_ui->list_r1->currentIndex().data().toString());
	q_ui->question_r1_a->setText(round1model->getA(index));
	q_ui->question_r1_b->setText(round1model->getB(index));
	q_ui->question_r1_c->setText(round1model->getC(index));
	q_ui->question_r1_d->setText(round1model->getD(index));
	q_ui->question_r1_score->setValue(round1model->getScore(index));
	bool* ans=round1model->getAnskey(index);
	q_ui->question_r1_ans_a->setChecked(ans[0]);
	q_ui->question_r1_ans_b->setChecked(ans[1]);
	q_ui->question_r1_ans_c->setChecked(ans[2]);
	q_ui->question_r1_ans_d->setChecked(ans[3]);
	delete[] ans;
}

void QEditor::on_r2_list()
{
	int index=q_ui->list_r2->currentIndex().row();
	q_ui->question_r2_num->setText("Question "+QString::number(index+1));
	q_ui->question_r2_text->setPlainText(q_ui->list_r2->currentIndex().data().toString());
	q_ui->question_r2_a->setText(round2model->getA(index));
	q_ui->question_r2_b->setText(round2model->getB(index));
	q_ui->question_r2_c->setText(round2model->getC(index));
	q_ui->question_r2_d->setText(round2model->getD(index));
	q_ui->question_r2_score->setValue(round2model->getScore(index));
	bool* ans=round2model->getAnskey(index);
	q_ui->question_r2_ans_a->setChecked(ans[0]);
	q_ui->question_r2_ans_b->setChecked(ans[1]);
	q_ui->question_r2_ans_c->setChecked(ans[2]);
	q_ui->question_r2_ans_d->setChecked(ans[3]);
	delete[] ans;
	//QString
}

void QEditor::add_question_r1()
{
	round1model->addNewQuestion();
}

void QEditor::add_question_r2()
{
	round2model->addNewQuestion();
}

void QEditor::changed_details_r2()
{
	if (q_ui->list_r2->currentIndex().row()!=-1)
		q_ui->button_update_r2->setEnabled(true);
}

void QEditor::update_question(int round)
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
	if (round>2) {
		if (question_ans_e[ptr]->isChecked()) anskey.append("1");
		else anskey.append("0");
	}
	else {
		anskey.append("0");
	}
	
	QString score=QString::number(question_score[ptr]->value());
	QString time;
	if (round>2) {
		time=QString::number(question_time[ptr]->value());
	}
	else {
		time="0";
	}
	roundmodel[ptr]->updateQuestion(index,question,a,b,c,d,e,anskey,score,time);
	q_ui->button_update_r2->setEnabled(false);
}

void QEditor::update_question_r2()
{
	int index=q_ui->list_r2->currentIndex().row();
	QString question=q_ui->question_r2_text->toPlainText();
	QString a=q_ui->question_r2_a->text();
	QString b=q_ui->question_r2_b->text();
	QString c=q_ui->question_r2_c->text();
	QString d=q_ui->question_r2_d->text();
	QString anskey="";
	if (q_ui->question_r2_ans_a->isChecked()) anskey.append("1");
	else anskey.append("0");
	if (q_ui->question_r2_ans_b->isChecked()) anskey.append("1");
	else anskey.append("0");
	if (q_ui->question_r2_ans_c->isChecked()) anskey.append("1");
	else anskey.append("0");
	if (q_ui->question_r2_ans_d->isChecked()) anskey.append("1");
	else anskey.append("0");
	anskey.append("0");
	QString score=QString::number(q_ui->question_r2_score->value());
	round2model->updateQuestion(index,question,a,b,c,d,"",anskey,score,"0");
	q_ui->button_update_r2->setEnabled(false);
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
