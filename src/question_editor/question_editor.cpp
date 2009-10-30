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
#include "ui_question_editor_v2.h"
#include "data_types.h"
#include <iostream>

#define CHOICE 4

using namespace std;
//TODO: remove methods only relevant to that,  use of constants

QEditor::QEditor(QWidget *parent) : QMainWindow(parent), q_ui(new Ui::q_editor)
{
	q_ui->setupUi(this);
	QFile f ("../resources/stage1_q.xml");
	f.open ( QIODevice::ReadOnly );
	QString xml = f.readAll();
	
	
	//declaration of pointers to components frequently accessed
	
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
	
	welcome[0]=q_ui->welcome_r1;
	welcome[1]=q_ui->welcome_r2;
	welcome[2]=q_ui->welcome_r3;
	welcome[3]=q_ui->welcome_r4;
	
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
	
	question_iden[0]=0;
	question_iden[1]=0;
	question_iden[2]=q_ui->iden_r3;
	question_iden[3]=q_ui->iden_r4;
	
	question_multi[0]=0;
	question_multi[1]=0;
	question_multi[2]=q_ui->multi_r3;
	question_multi[3]=q_ui->multi_r4;
	
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
	
	button_update[0]=q_ui->button_update_r1;
	button_update[1]=q_ui->button_update_r2;
	button_update[2]=q_ui->button_update_r3;
	button_update[3]=q_ui->button_update_r4;
	
	button_cancel[0]=q_ui->button_cancel_r1;
	button_cancel[1]=q_ui->button_cancel_r2;
	button_cancel[2]=q_ui->button_cancel_r3;
	button_cancel[3]=q_ui->button_cancel_r4;
	
	button_add[0]=q_ui->button_add_r1;
	button_add[1]=q_ui->button_add_r2;
	button_add[2]=q_ui->button_add_r3;
	button_add[3]=q_ui->button_add_r4;
	
	button_remove[0]=q_ui->button_remove_r1;
	button_remove[1]=q_ui->button_remove_r2;
	button_remove[2]=q_ui->button_remove_r3;
	button_remove[3]=q_ui->button_remove_r4;
	
	button_up[0]=q_ui->button_up_r1;
	button_up[1]=q_ui->button_up_r2;
	button_up[2]=q_ui->button_up_r3;
	button_up[3]=q_ui->button_up_r4;
	
	button_down[0]=q_ui->button_down_r1;
	button_down[1]=q_ui->button_down_r2;
	button_down[2]=q_ui->button_down_r3;
	button_down[3]=q_ui->button_down_r4;
	
	duration[0]=q_ui->duration_r1;
	duration[1]=q_ui->duration_r2;
	duration[2]=q_ui->duration_r3;
	duration[3]=q_ui->duration_r4;
	
	//slot and signal connections
	
	sigToList=new QSignalMapper(this);
	sigToAdd=new QSignalMapper(this);
	sigToRemove=new QSignalMapper(this);
	sigToUpdate=new QSignalMapper(this);
	sigToCancel=new QSignalMapper(this);
	sigToGeneralUpdate=new QSignalMapper(this);
	sigToDetailUpdate=new QSignalMapper(this);
	sigToUp=new QSignalMapper(this);
	sigToDown=new QSignalMapper(this);
	sigToVisible=new QSignalMapper(this);
	
	connect(sigToList,SIGNAL(mapped(int)),this,SLOT(list_focus(int)));
	connect(sigToAdd,SIGNAL(mapped(int)),this,SLOT(add_question(int)));
	connect(sigToRemove,SIGNAL(mapped(int)),this,SLOT(remove_question(int)));
	connect(sigToUpdate,SIGNAL(mapped(int)),this,SLOT(update_question(int)));
	connect(sigToCancel,SIGNAL(mapped(int)),this,SLOT(cancel_update(int)));
	connect(sigToGeneralUpdate,SIGNAL(mapped(int)),this,SLOT(changed_general(int)));
	connect(sigToDetailUpdate,SIGNAL(mapped(int)),this,SLOT(changed_details(int)));
	connect(sigToUp,SIGNAL(mapped(int)),this,SLOT(move_up(int)));
	connect(sigToDown,SIGNAL(mapped(int)),this,SLOT(move_down(int)));
	connect(sigToVisible,SIGNAL(mapped(int)),this,SLOT(visibility(int)));
	
	for (int ctr=0;ctr<CHOICE;ctr++)
	{
		sigToList->setMapping(question_list[ctr],ctr+1);
		sigToAdd->setMapping(button_add[ctr],ctr+1);
		sigToRemove->setMapping(button_remove[ctr],ctr+1);
		sigToGeneralUpdate->setMapping(welcome[ctr],ctr+1);
		sigToDetailUpdate->setMapping(question_text[ctr],ctr+1);
		sigToDetailUpdate->setMapping(question_a[ctr],ctr+1);
		sigToDetailUpdate->setMapping(question_b[ctr],ctr+1);
		sigToDetailUpdate->setMapping(question_c[ctr],ctr+1);
		sigToDetailUpdate->setMapping(question_d[ctr],ctr+1);
		sigToDetailUpdate->setMapping(question_score[ctr],ctr+1);
		sigToDetailUpdate->setMapping(question_ans_a[ctr],ctr+1);
		sigToDetailUpdate->setMapping(question_ans_b[ctr],ctr+1);
		sigToDetailUpdate->setMapping(question_ans_c[ctr],ctr+1);
		sigToDetailUpdate->setMapping(question_ans_d[ctr],ctr+1);
		sigToGeneralUpdate->setMapping(duration[ctr],ctr+1);
		sigToUp->setMapping(button_up[ctr],ctr+1);
		sigToDown->setMapping(button_down[ctr],ctr+1);
		sigToUpdate->setMapping(button_update[ctr],ctr+1);
		sigToCancel->setMapping(button_cancel[ctr],ctr+1);
		
		connect(question_list[ctr],SIGNAL(activated(QModelIndex)),sigToList,SLOT(map()));
		connect(button_add[ctr],SIGNAL(clicked(bool)),sigToAdd,SLOT(map()));
		connect(button_remove[ctr],SIGNAL(clicked(bool)),sigToRemove,SLOT(map()));
		connect(welcome[ctr],SIGNAL(textChanged()),sigToGeneralUpdate,SLOT(map()));
		connect(question_text[ctr],SIGNAL(textChanged()),sigToDetailUpdate,SLOT(map()));
		connect(question_a[ctr],SIGNAL(textEdited(QString)),sigToDetailUpdate,SLOT(map()));
		connect(question_b[ctr],SIGNAL(textEdited(QString)),sigToDetailUpdate,SLOT(map()));
		connect(question_c[ctr],SIGNAL(textEdited(QString)),sigToDetailUpdate,SLOT(map()));
		connect(question_d[ctr],SIGNAL(textEdited(QString)),sigToDetailUpdate,SLOT(map()));
		connect(question_score[ctr],SIGNAL(valueChanged(int)),sigToDetailUpdate,SLOT(map()));
		connect(question_ans_a[ctr],SIGNAL(toggled(bool)),sigToDetailUpdate,SLOT(map()));
		connect(question_ans_b[ctr],SIGNAL(toggled(bool)),sigToDetailUpdate,SLOT(map()));
		connect(question_ans_c[ctr],SIGNAL(toggled(bool)),sigToDetailUpdate,SLOT(map()));
		connect(question_ans_d[ctr],SIGNAL(toggled(bool)),sigToDetailUpdate,SLOT(map()));
		connect(duration[ctr],SIGNAL(valueChanged(int)),sigToGeneralUpdate,SLOT(map()));
		connect(button_up[ctr],SIGNAL(clicked(bool)),sigToUp,SLOT(map()));
		connect(button_down[ctr],SIGNAL(clicked(bool)),sigToDown,SLOT(map()));
		connect(button_update[ctr],SIGNAL(clicked(bool)),sigToUpdate,SLOT(map()));
		connect(button_cancel[ctr],SIGNAL(clicked(bool)),sigToCancel,SLOT(map()));
		
		
		if (ctr>1)
		{
			sigToDetailUpdate->setMapping(question_e[ctr],ctr+1);
			sigToDetailUpdate->setMapping(question_time[ctr],ctr+1);
			sigToDetailUpdate->setMapping(question_iden[ctr],ctr+1);
			sigToDetailUpdate->setMapping(question_multi[ctr],ctr+1);
			sigToVisible->setMapping(question_iden[ctr],ctr+1);
			sigToVisible->setMapping(question_multi[ctr],ctr+1);
			
			connect(question_e[ctr],SIGNAL(textEdited(QString)),sigToDetailUpdate,SLOT(map()));
			connect(question_time[ctr],SIGNAL(valueChanged(int)),sigToDetailUpdate,SLOT(map()));
			connect(question_iden[ctr],SIGNAL(toggled(bool)),sigToDetailUpdate,SLOT(map()));
			connect(question_multi[ctr],SIGNAL(toggled(bool)),sigToDetailUpdate,SLOT(map()));
			connect(question_iden[ctr],SIGNAL(toggled(bool)),sigToVisible,SLOT(map()));
			connect(question_multi[ctr],SIGNAL(toggled(bool)),sigToVisible,SLOT(map()));
			
			question_e[ctr]->setVisible(false);
			question_e[ctr]->installEventFilter(this);
		}
		
		welcome[ctr]->setTabChangesFocus(true);
		question_list[ctr]->setModel(roundmodel[ctr]);
		question_list[ctr]->setEditTriggers(QAbstractItemView::NoEditTriggers);
		question_text[ctr]->setTabChangesFocus(true);
		
		question_text[ctr]->installEventFilter(this);
		question_a[ctr]->installEventFilter(this);
		question_b[ctr]->installEventFilter(this);
		question_c[ctr]->installEventFilter(this);
		question_d[ctr]->installEventFilter(this);
		
		
		fully_updated[ctr]=true;
		qmod[ctr]=false;
		
		control_components(ctr+1,false);
		button_update[ctr]->setEnabled(false);
		button_cancel[ctr]->setEnabled(false);
	}
	
	
	//for menu/keyboard commands
	q_ui->act_new->setShortcut(QKeySequence::New);
	q_ui->act_import->setShortcut(QKeySequence(tr("ctrl+i")));
	q_ui->act_save_xml->setShortcut(QKeySequence::Save);
	q_ui->act_save_as->setShortcut(QKeySequence::SaveAs);
	q_ui->act_load_xml->setShortcut(QKeySequence::Open);
	q_ui->act_exit->setShortcut(QKeySequence::Close);
	q_ui->act_new->setStatusTip("Creates an empty file");
	q_ui->act_import->setStatusTip("Adds questions read from xml file");
	q_ui->act_save_xml->setStatusTip("Saves questions to xml files");
	q_ui->act_save_as->setStatusTip("Saves questions to some other xml files");
	q_ui->act_load_xml->setStatusTip("Load xml files containing the questions.");
	q_ui->act_exit->setStatusTip("Closes the program");
	
	connect(q_ui->act_new,SIGNAL(triggered(bool)),this,SLOT(newfile()));
	connect(q_ui->act_import,SIGNAL(triggered(bool)),this,SLOT(import()));
	connect(q_ui->act_save_xml,SIGNAL(triggered(bool)),this,SLOT(save()));
	connect(q_ui->act_save_as,SIGNAL(triggered(bool)),this,SLOT(saveAs()));
	connect(q_ui->act_load_xml,SIGNAL(triggered(bool)),this,SLOT(load()));
	connect(q_ui->act_exit,SIGNAL(triggered(bool)),this,SLOT(exit()));
	
	this->setWindowTitle("untitled - QEditor");
}

QEditor::~QEditor()
{
	delete sigToAdd;
	delete sigToRemove;
	delete sigToList;
	delete sigToUpdate;
	delete sigToCancel;
	delete sigToGeneralUpdate;
	delete sigToDetailUpdate;
	delete sigToUp;
	delete sigToDown;
	
	delete q_ui;
	for (int ctr=0;ctr<CHOICE;ctr++)
	{
		delete roundmodel[ctr];
	}
}

void QEditor::visibility(int round)
{
	int ptr=round -1;
	bool visi=question_multi[ptr]->isChecked();
	question_ans_a[ptr]->setVisible(visi);
	question_ans_b[ptr]->setVisible(visi);
	question_ans_c[ptr]->setVisible(visi);
	question_ans_d[ptr]->setVisible(visi);
	question_e[ptr]->setVisible(!visi);
}

void QEditor::list_focus(int round)
{
	int ptr=round-1;
	//control_components(ptr,true);
	int index=question_list[ptr]->currentIndex().row();
	
	if (index!=-1)
	{
		bool change=true;
		if (button_update[ptr]->isEnabled() && index!=history[ptr] && qmod[ptr])
		{
			QMessageBox conf;
			conf.setWindowTitle("Notification - QEditor");
			conf.setText("Details of question has been modified, but have not been updated");
			conf.setInformativeText("What do you want to do?");
			conf.setIcon(QMessageBox::Question);
			QPushButton *update=conf.addButton(tr("Update then proceed"),QMessageBox::ActionRole);
			QPushButton *canpro=conf.addButton(tr("Cancel then proceed"),QMessageBox::ActionRole);
			QPushButton *goback=conf.addButton(tr("Cancel Action"),QMessageBox::ActionRole);
			conf.setDefaultButton(update);
			conf.exec();
			
			if (conf.clickedButton()==update)
			{
				QModelIndex curr=question_list[ptr]->currentIndex();
				QModelIndex child=roundmodel[ptr]->index(history[ptr],0);
				question_list[ptr]->setCurrentIndex(child);
				update_question(round);
				question_list[ptr]->setCurrentIndex(curr);
			}
			else if (conf.clickedButton()==goback)
			{
				QModelIndex curr=question_list[ptr]->currentIndex();
				QModelIndex child=roundmodel[ptr]->index(history[ptr],0);
				question_list[ptr]->setCurrentIndex(child);
				change=false;
			}
		}
		if (change)
		{
			question_num[ptr]->setText("Question "+QString::number(index+1));
			question_text[ptr]->setPlainText(roundmodel[ptr]->getQuestion(index));
			question_a[ptr]->setText(roundmodel[ptr]->getA(index));
			question_b[ptr]->setText(roundmodel[ptr]->getB(index));
			question_c[ptr]->setText(roundmodel[ptr]->getC(index));
			question_d[ptr]->setText(roundmodel[ptr]->getD(index));
			question_score[ptr]->setValue(roundmodel[ptr]->getScore(index));
			bool* ans=new bool[5];
			roundmodel[ptr]->getAnskey(index,ans);
			question_ans_a[ptr]->setChecked(ans[0]);
			question_ans_b[ptr]->setChecked(ans[1]);
			question_ans_c[ptr]->setChecked(ans[2]);
			question_ans_d[ptr]->setChecked(ans[3]);
			if (round > 2)
			{
				question_e[ptr]->setText(roundmodel[ptr]->getE(index));
				question_time[ptr]->setValue(roundmodel[ptr]->getTime(index));
				if (roundmodel[ptr]->isIdentification(index))
				{
					question_iden[ptr]->setChecked(true);
				}
				else
				{
					question_multi[ptr]->setChecked(true);
				}
			}
			delete ans;
			control_components(round,true);
			button_update[ptr]->setDisabled(true);
			button_cancel[ptr]->setDisabled(true);
			history[ptr]=index;
			
			qmod[ptr]=false;
			fully_updated[ptr]=true;
			if (file_prefix!="")
				this->setWindowTitle(file_prefix+".xgrp - QEditor");
			
		}
	}
	else
	{
		question_num[ptr]->setText("No question selected");
		question_text[ptr]->setPlainText("");
		question_a[ptr]->setText("");
		question_b[ptr]->setText("");
		question_c[ptr]->setText("");
		question_d[ptr]->setText("");
		if (round > 2) question_e[ptr]->setText("");
		question_score[ptr]->setValue(0);
		if (round > 2) question_time[ptr]->setValue(0);
		question_ans_a[ptr]->setDown(true);
		question_ans_b[ptr]->setDown(true);
		question_ans_c[ptr]->setDown(true);
		question_ans_d[ptr]->setDown(true);
		
		control_components(round,false);
	}
}

void QEditor::add_question(int round)
{
	roundmodel[round-1]->addNewQuestion();
	fully_updated[round-1]=false;
	if (file_prefix!="")
		this->setWindowTitle(file_prefix+".xgrp [modified] - QEditor");
}

void QEditor::remove_question(int round)
{
	int index=question_list[round-1]->currentIndex().row();
	roundmodel[round-1]->removeQuestion(index);
	list_focus(round);
	if (roundmodel[round-1]->rowCount()==0)
	{
		control_components(round,false);
		button_update[round-1]->setDisabled(true);
		button_cancel[round-1]->setDisabled(true);
	}
	fully_updated[round-1]=false;
	if (file_prefix!="")
		this->setWindowTitle(file_prefix+".xgrp [modified] - QEditor");
}

void QEditor::changed_general(int round)
{
	button_update[round-1]->setEnabled(true);
	button_cancel[round-1]->setEnabled(true);
	fully_updated[round-1]=false;
	if (!qmod[round-1]) qmod[round-1]=false; 
	if (file_prefix!="")
		this->setWindowTitle(file_prefix+".xgrp [modified] - QEditor");
}

void QEditor::changed_details(int round)
{
	button_update[round-1]->setEnabled(true);
	button_cancel[round-1]->setEnabled(true);
	fully_updated[round-1]=false;
	qmod[round-1]=true;
	if (file_prefix!="")
		this->setWindowTitle(file_prefix+".xgrp [modified] - QEditor");
}

void QEditor::update_question(int round)
{
	int ptr=round-1;
	int index=question_list[ptr]->currentIndex().row();
	if(index!=-1)
	{
		QString question=question_text[ptr]->toPlainText();
		QString a=question_a[ptr]->text();
		QString b=question_b[ptr]->text();
		QString c=question_c[ptr]->text();
		QString d=question_d[ptr]->text();
		QString e;
		if (round > 2) e=question_e[ptr]->text();
		else e="";
		QString anskey="";
		if (question_ans_a[ptr]->isChecked()) 
			anskey.append("1");
		else 
			anskey.append("0");
		if (question_ans_b[ptr]->isChecked()) 
			anskey.append("1");
		else 
			anskey.append("0");
		if (question_ans_c[ptr]->isChecked()) 
			anskey.append("1");
		else 
			anskey.append("0");
		if (question_ans_d[ptr]->isChecked()) 
			anskey.append("1");
		else 
			anskey.append("0");
		anskey.append("0");
		
		QString score=QString::number(question_score[ptr]->value());
		QString time;
		QString type="0";
		
		if (round>2) {
			
			time=QString::number(question_time[ptr]->value());
			if (question_iden[ptr]->isChecked()) 
				type="1";
		}
		else {
			anskey.append("0");
			time="0";
		}
		
		roundmodel[ptr]->updateQuestion(index,question,a,b,c,d,e,anskey,score,time,type);
		
		fully_updated[ptr]=true;
		qmod[ptr]=false;
		
		q_ui->statusBar->showMessage("Question "+QString::number(index+1)+" of round "+QString::number(round)+" has been updated.");
	}
	button_update[ptr]->setDisabled(true);
	button_cancel[ptr]->setDisabled(true);
}

void QEditor::cancel_update(int round)
{
	list_focus(round);
	button_update[round-1]->setDisabled(true);
	button_cancel[round-1]->setDisabled(true);
	
	q_ui->statusBar->showMessage("");
}

void QEditor::move_up(int round)
{
	int ptr=round-1;
	int index=question_list[ptr]->currentIndex().row();
	if (index!=0 && index!=-1)
	{
		roundmodel[ptr]->swapOrder(index,index-1);
		QModelIndex curr=question_list[ptr]->currentIndex();
		QModelIndex child=roundmodel[ptr]->index(curr.row()-1,0);
		question_list[ptr]->setCurrentIndex(child);
	}
	if (file_prefix!="")
	{
		save();
	}
}

void QEditor::move_down(int round)
{
	int ptr=round-1;
	int index=question_list[ptr]->currentIndex().row();
	if (index!=roundmodel[ptr]->rowCount()-1 && index!=-1)
	{
		roundmodel[ptr]->swapOrder(index,index+1);
		QModelIndex curr=question_list[ptr]->currentIndex();
		QModelIndex child=roundmodel[ptr]->index(curr.row()+1,0);
		question_list[ptr]->setCurrentIndex(child);
	}
	if (file_prefix!="")
	{
		save();
	}
}

void QEditor::control_components(int round,bool enable)
{
	int ptr=round-1;
	question_text[ptr]->setReadOnly(!enable);
	question_a[ptr]->setReadOnly(!enable);
	question_b[ptr]->setReadOnly(!enable);
	question_c[ptr]->setReadOnly(!enable);
	question_d[ptr]->setReadOnly(!enable);
	question_ans_a[ptr]->setEnabled(enable);
	question_ans_b[ptr]->setEnabled(enable);
	question_ans_c[ptr]->setEnabled(enable);
	question_ans_d[ptr]->setEnabled(enable);
	question_score[ptr]->setEnabled(enable);
	if (round > 2)
	{
		question_e[ptr]->setReadOnly(!enable);
		question_time[ptr]->setEnabled(enable);
		question_iden[ptr]->setEnabled(enable);
		question_multi[ptr]->setEnabled(enable);
	}
	
}

void QEditor::newfile()
{
	if (file_prefix!="" && (!fully_updated[0] || !fully_updated[1] || !fully_updated[2] || !fully_updated[3]))
	{
		//warning here
		QMessageBox conf;
		conf.setWindowTitle("Notification - QEditor");
		conf.setText("Details have been modified, but have not been saved.");
		conf.setInformativeText("What do you want to do?");
		conf.setIcon(QMessageBox::Question);
		QPushButton *update=conf.addButton(tr("Save"),QMessageBox::ActionRole);
		QPushButton *canpro=conf.addButton(tr("Discard changes"),QMessageBox::ActionRole);
		QPushButton *goback=conf.addButton(tr("Cancel"),QMessageBox::ActionRole);
		conf.setDefaultButton(update);
		conf.exec();
		if (conf.clickedButton()==update)
			save();
		else if (conf.clickedButton()==goback)
			return;
	}
	
	for (int ctr=0;ctr<CHOICE;ctr++)
	{
		roundmodel[ctr]->clear();
		fully_updated[ctr]=true;
		qmod[ctr]=false;
		duration[ctr]->setValue(0);
		welcome[ctr]->setPlainText("");
	}
	file_prefix="";
	this->setWindowTitle("untitled - QEditor");
}

void QEditor::import()
{
	QFileDialog load_dlg;
	load_dlg.setAcceptMode(QFileDialog::AcceptOpen);
	load_dlg.setFileMode(QFileDialog::ExistingFiles);
	load_dlg.setFilter(tr("Group XML files (*.xgrp)"));
	load_dlg.exec();
	
	int rec[4];
	for (int ctr=0;ctr<CHOICE;ctr++)
	{
		rec[ctr]=roundmodel[ctr]->rowCount();
	}
	
	file_prefix=load_dlg.selectedFiles().join("");
	file_prefix.replace(QString(".xgrp"),QString(""));
	
	if (file_prefix=="")
		return;
	for (int ctr=0;ctr<CHOICE;ctr++)
	{
		StageData sd;
		QFile fs(file_prefix+QString::number(ctr+1)+".xml");
		if(fs.open(QIODevice::ReadOnly))
		{
			QString stage_xml=fs.readAll();
			try {
				xml_util.readStageData(stage_xml,sd);
			} catch (exception &e) {
				//QString error=e.what();
				QMessageBox notice;
				notice.setWindowTitle("Ooops... - QEditor ^_^;");
				notice.setText("i guess something went wrong while reading "+
						file_prefix+QString::number(ctr+1)+".xml");
				notice.setDetailedText(e.what());
				notice.exec();
				continue;
			}
		}
		
		roundmodel[ctr]->feedData(sd);
		
		if (roundmodel[ctr]->rowCount()!=rec[ctr]) 
			fully_updated[ctr]=false;
	}
}

int QEditor::showUnsavedWarning()
{
	QMessageBox conf;
	conf.setWindowTitle("Notification - QEditor");
	conf.setText("Details have been modified, but have not been saved.");
	conf.setInformativeText("What do you want to do?");
	conf.setIcon(QMessageBox::Question);
	QPushButton *update=conf.addButton(tr("Save"),QMessageBox::ActionRole);
	QPushButton *discard=conf.addButton(tr("Discard changes"),QMessageBox::ActionRole);
	QPushButton *abort=conf.addButton(tr("Cancel"),QMessageBox::ActionRole);
	conf.setDefaultButton(update);
	conf.exec();
	if (conf.clickedButton()==update)
	{
		int resp=save();
		if (resp==0)
			return 0;
		else
			return 2;
	}
	else if (conf.clickedButton()==discard)
		return 1;
	else
		return 2;
}

void QEditor::load()
{
	if (file_prefix!="" && (!fully_updated[0] || !fully_updated[1] || !fully_updated[2] || !fully_updated[3]))
	{
		int response = showUnsavedWarning();
		if (response == 2)
			return;
	}
	
	for (int ctr=0;ctr<CHOICE;ctr++)
	{
		roundmodel[ctr]->clear();
	}
	
	QFileDialog load_dlg;
	load_dlg.setAcceptMode(QFileDialog::AcceptOpen);
	load_dlg.setFileMode(QFileDialog::ExistingFiles);
	load_dlg.setFilter(tr("Group XML files (*.xgrp)"));
	load_dlg.exec();
	
	file_prefix=load_dlg.selectedFiles().join("");
	file_prefix.replace(QString(".xgrp"),QString(""));
	if (file_prefix=="")
		return;
	for (int ctr=0;ctr<CHOICE;ctr++)
	{
		StageData sd;
		QFile fs(file_prefix+QString::number(ctr+1)+".xml");
		if(fs.open(QIODevice::ReadOnly))
		{
			QString stage_xml=fs.readAll();
			try {
				xml_util.readStageData(stage_xml,sd);
			} catch (exception &e) {
				//QString error=e.what();
				QMessageBox notice;
				notice.setWindowTitle("Ooops... - QEditor ^_^;");
				notice.setText("i guess something went wrong while reading "+
						file_prefix+QString::number(ctr+1)+".xml");
				notice.setDetailedText(e.what());
				notice.exec();
				continue;
			}
		}
		
		welcome[ctr]->setPlainText(sd.welcome_msg);
		duration[ctr]->setValue(sd.contest_time);
		roundmodel[ctr]->feedData(sd);
		
		this->setWindowTitle(file_prefix+".xgrp - QEditor");
		
		fully_updated[ctr]=true;
	}
}

int QEditor::save()
{
	q_ui->statusBar->showMessage("");
	if (file_prefix=="")
	{
		QFileDialog save_dlg;
		save_dlg.setAcceptMode(QFileDialog::AcceptSave);
		save_dlg.setFileMode(QFileDialog::AnyFile);
		save_dlg.setFilter(tr("Group XML files (*.xgrp)"));
		if (!save_dlg.exec())
			file_prefix="";
		else
		{
			file_prefix=save_dlg.selectedFiles().join("");
			file_prefix.replace(QString(".xgrp"),QString(""));
		}
		if (file_prefix=="")
		{
			q_ui->statusBar->showMessage("File save aborted");
			return 2;
		}
		QFile grp(file_prefix+".xgrp");
		if (!grp.open(QIODevice::ReadWrite))
			return -1;
		QTextStream out(&grp);
		out << "This file holds no data, this just groups xml files to open/edit them easier! ^_^";
	}
	if (file_prefix!="")
	{
		
		for (int roundctr=0;roundctr<CHOICE;roundctr++)
		{
			if (!fully_updated[roundctr])
			{
				q_ui->tabWidget->setCurrentIndex(roundctr);
				QMessageBox conf;
				conf.setWindowTitle("Notification - QEditor");
				conf.setText("Details of round "+QString::number(roundctr+1)+" has been modified, but have not been updated");
				conf.setInformativeText("What do you want to do?");
				conf.setIcon(QMessageBox::Question);
				QPushButton *update=conf.addButton(tr("Update then save"),QMessageBox::ActionRole);
				QPushButton *canpro=conf.addButton(tr("Cancel then save"),QMessageBox::ActionRole);
				QPushButton *goback=conf.addButton(tr("Don't Save"),QMessageBox::ActionRole);
				conf.setDefaultButton(update);
				conf.exec();
				    
				if (conf.clickedButton()==update)
					update_question(roundctr+1);
				else if (conf.clickedButton()==canpro)
					cancel_update(roundctr+1);
				else {
					QMessageBox notice;
					notice.setWindowTitle("Notice - QEditor");
					notice.setText("Changes on round "+QString::number(roundctr+1)+" won't be updated.");
					notice.exec();
					continue;
				}
			}
			//QuestionData rounddata;
			//AnswerData ansdata;
			StageData sd;
			
			QString stage_xml;
			
			int question_cnt=roundmodel[roundctr]->rowCount();
			
			sd.welcome_msg=welcome[roundctr]->toPlainText();
			sd.contest_time=duration[roundctr]->value();
			
			for (int qctr=0;qctr<question_cnt;qctr++)
			{
				Question temp;
				roundmodel[roundctr]->getFullQuestion(qctr,&temp);
				sd.questions.push_back(temp);
				//rounddata.questions.push_back(temp);
			}
			
			//preprocess verification
			
			QFile file_q(file_prefix+QString::number(roundctr+1)+".xml");
			if (!file_q.open(QIODevice::WriteOnly))
				return -1;
			else
			{
				QTextStream out(&file_q);
				xml_util.writeStageData(sd,stage_xml);
				out << stage_xml;
			}
			
			fully_updated[roundctr]=true;
		}
		q_ui->statusBar->showMessage("File saved at "+file_prefix);
		
		if (fully_updated[0] && fully_updated[1] && fully_updated[2] && fully_updated[3])
		{
			this->setWindowTitle(file_prefix+".xgrp - QEditor");
			return 0;
		}
		else
			return 1;
	}
	return 2;
}

void QEditor::saveAs()
{
	QString temp=file_prefix;
	file_prefix="";
	save();
	if (file_prefix=="")
	{
	      file_prefix=temp;
	}
}

void QEditor::exit()
{
	this->close();
}

void QEditor::closeEvent(QCloseEvent *event)
{
	if (roundmodel[0]->rowCount()==0 && roundmodel[1]->rowCount()==0 &&
	    roundmodel[2]->rowCount()==0 && roundmodel[3]->rowCount()==0 &&
	    fully_updated[0] && fully_updated[1] && fully_updated[2] && fully_updated[3])
		event->accept();
	else if (file_prefix=="")
	{
		int response = showUnsavedWarning();
		if (response == 0 || response == 1)
			event->accept();
		else
			event->ignore();
	}
	else if (file_prefix!="" && (!fully_updated[0] || !fully_updated[1] || !fully_updated[2] || !fully_updated[3]))
	{
		int response = showUnsavedWarning();
		if (response == 0 || response == 1)
			event->accept();
		else
			event->ignore();
	}
	else
	{
		event->accept();
	}
}

bool QEditor::eventFilter(QObject* o, QEvent* e)
{
	if (e->type() == QEvent::KeyRelease) {
		QKeyEvent* key= (QKeyEvent*)e;
		if (key->key() == Qt::Key_Enter || key->key() == Qt::Key_Return)
		{
			//cout << question_text[0]->lineWrapMode() << endl;
			for(int ctr=0;ctr<CHOICE;ctr++)
			{
				QString temp=question_text[ctr]->toPlainText();
				temp.replace("\n","");
				temp.replace("\r","");
				cout << temp.toStdString() << endl;
				question_text[ctr]->setPlainText(temp);
			}
			int ctr=q_ui->tabWidget->currentIndex();
			if (button_update[ctr]->isEnabled())
				update_question(ctr+1);
		}
		else if (key->key() == Qt::Key_Escape)
		{
			int ctr=q_ui->tabWidget->currentIndex();
			if (button_cancel[ctr]->isEnabled())
				cancel_update(ctr+1);
		}
		else
		{
			return QMainWindow::eventFilter(o, e);
		}
	}
	return QMainWindow::eventFilter(o, e);
}


int main(int argc, char *argv[])
{
	QApplication app (argc, argv);
	QEditor q_App;
	q_App.show();
	return app.exec();
}
