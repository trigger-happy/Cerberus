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

using namespace std;
//TODO: reading/saving xml files, change detection to determine if save should be called when prog quits

QEditor::QEditor(QWidget *parent) : QMainWindow(parent), q_ui(new Ui::q_editor)
{
	q_ui->setupUi(this);
	QFile f ("../resources/stage1_q.xml");
	f.open ( QIODevice::ReadOnly );
	QString xml = f.readAll();
	
	//q_ui->textarea_welcome->setPlainText(xml);
	
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
	sigToDetailUpdate=new QSignalMapper(this);
	sigToUp=new QSignalMapper(this);
	sigToDown=new QSignalMapper(this);
	sigToChoices=new QSignalMapper(this);
	
	connect(sigToList,SIGNAL(mapped(int)),this,SLOT(list_focus(int)));
	connect(sigToAdd,SIGNAL(mapped(int)),this,SLOT(add_question(int)));
	connect(sigToRemove,SIGNAL(mapped(int)),this,SLOT(remove_question(int)));
	connect(sigToUpdate,SIGNAL(mapped(int)),this,SLOT(update_question(int)));
	connect(sigToCancel,SIGNAL(mapped(int)),this,SLOT(cancel_update(int)));
	connect(sigToDetailUpdate,SIGNAL(mapped(int)),this,SLOT(changed_details(int)));
	connect(sigToUp,SIGNAL(mapped(int)),this,SLOT(move_up(int)));
	connect(sigToDown,SIGNAL(mapped(int)),this,SLOT(move_down(int)));
	connect(sigToChoices,SIGNAL(mapped(int)),this,SLOT(disableDuplicates(int)));
	
	for (int ctr=0;ctr<4;ctr++)
	{
		sigToList->setMapping(question_list[ctr],ctr+1);
		sigToAdd->setMapping(button_add[ctr],ctr+1);
		sigToRemove->setMapping(button_remove[ctr],ctr+1);
		sigToDetailUpdate->setMapping(welcome[ctr],ctr+1);
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
		sigToDetailUpdate->setMapping(duration[ctr],0);
		sigToUp->setMapping(button_up[ctr],ctr+1);
		sigToDown->setMapping(button_down[ctr],ctr+1);
		sigToChoices->setMapping(question_ans_a[ctr],ctr+1);
		sigToChoices->setMapping(question_ans_b[ctr],ctr+1);
		sigToChoices->setMapping(question_ans_c[ctr],ctr+1);
		sigToChoices->setMapping(question_ans_d[ctr],ctr+1);
		sigToUpdate->setMapping(button_update[ctr],ctr+1);
		sigToCancel->setMapping(button_cancel[ctr],ctr+1);

		
		connect(question_list[ctr],SIGNAL(activated(QModelIndex)),sigToList,SLOT(map()));
		connect(button_add[ctr],SIGNAL(clicked(bool)),sigToAdd,SLOT(map()));
		connect(button_remove[ctr],SIGNAL(clicked(bool)),sigToRemove,SLOT(map()));
		connect(welcome[ctr],SIGNAL(textChanged()),sigToDetailUpdate,SLOT(map()));
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
		connect(duration[ctr],SIGNAL(valueChanged(int)),sigToDetailUpdate,SLOT(map()));
		connect(button_up[ctr],SIGNAL(clicked(bool)),sigToUp,SLOT(map()));
		connect(button_down[ctr],SIGNAL(clicked(bool)),sigToDown,SLOT(map()));
		connect(question_ans_a[ctr],SIGNAL(toggled(bool)),sigToChoices,SLOT(map()));
		connect(question_ans_b[ctr],SIGNAL(toggled(bool)),sigToChoices,SLOT(map()));
		connect(question_ans_c[ctr],SIGNAL(toggled(bool)),sigToChoices,SLOT(map()));
		connect(question_ans_d[ctr],SIGNAL(toggled(bool)),sigToChoices,SLOT(map()));
		connect(button_update[ctr],SIGNAL(clicked(bool)),sigToUpdate,SLOT(map()));
		connect(button_cancel[ctr],SIGNAL(clicked(bool)),sigToCancel,SLOT(map()));
		
		
		if (ctr>1)
		{
			sigToDetailUpdate->setMapping(question_e[ctr],ctr+1);
			sigToDetailUpdate->setMapping(question_ans_e[ctr],ctr+1);
			sigToDetailUpdate->setMapping(question_time[ctr],ctr+1);
			sigToChoices->setMapping(question_ans_e[ctr],ctr+1);
			
			connect(question_e[ctr],SIGNAL(textEdited(QString)),sigToDetailUpdate,SLOT(map()));
			connect(question_ans_e[ctr],SIGNAL(toggled(bool)),sigToDetailUpdate,SLOT(map()));
			connect(question_time[ctr],SIGNAL(valueChanged(int)),sigToDetailUpdate,SLOT(map()));
			connect(question_ans_e[ctr],SIGNAL(toggled(bool)),sigToChoices,SLOT(map()));
		}
		
		welcome[ctr]->setTabChangesFocus(true);
		question_list[ctr]->setModel(roundmodel[ctr]);
		question_list[ctr]->setEditTriggers(QAbstractItemView::NoEditTriggers);
		question_text[ctr]->setTabChangesFocus(true);
		
		fully_updated[ctr]=true;
		control_components(ctr+1,false);
		button_update[ctr]->setEnabled(false);
		button_cancel[ctr]->setEnabled(false);
	}
	
	
	//for menu/keyboard commands
	q_ui->act_save_xml->setShortcut(QKeySequence::Save);
	q_ui->act_save_as->setShortcut(QKeySequence::SaveAs);
	q_ui->act_load_xml->setShortcut(QKeySequence::Open);
	q_ui->act_exit->setShortcut(QKeySequence::Close);
	q_ui->act_save_xml->setStatusTip("Saves questions to xml files");
	q_ui->act_save_as->setStatusTip("Saves questions to some other xml files");
	q_ui->act_load_xml->setStatusTip("Load xml files containing the questions.");
	q_ui->act_exit->setStatusTip("Closes the program");
	
	connect(q_ui->act_save_xml,SIGNAL(triggered(bool)),this,SLOT(save()));
	connect(q_ui->act_save_as,SIGNAL(triggered(bool)),this,SLOT(saveAs()));
	connect(q_ui->act_load_xml,SIGNAL(triggered(bool)),this,SLOT(load()));
	connect(q_ui->act_exit,SIGNAL(triggered(bool)),this,SLOT(exit()));
	
	changed=false;
}

QEditor::~QEditor()
{
	delete sigToAdd;
	delete sigToRemove;
	delete sigToList;
	delete sigToUpdate;
	delete sigToCancel;
	delete sigToDetailUpdate;
	delete sigToUp;
	delete sigToDown;
	
	delete q_ui;
	for (int ctr=0;ctr<4;ctr++)
	{
		delete roundmodel[ctr];
	}
}

void QEditor::list_focus(int round)
{
	int ptr=round-1;
	//control_components(ptr,true);
	int index=question_list[ptr]->currentIndex().row();
	
	if (index!=-1)
	{
		bool change=true;
		if (button_update[ptr]->isEnabled() && index!=history[ptr])
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
			control_components(round,true);
			button_update[ptr]->setDisabled(true);
			button_cancel[ptr]->setDisabled(true);
			history[ptr]=index;
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
		if (round > 2) question_ans_e[ptr]->setChecked(false);
		control_components(round,false);
	}
}

void QEditor::add_question(int round)
{
	roundmodel[round-1]->addNewQuestion();
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
}

void QEditor::changed_details(int round)
{
	button_update[round-1]->setEnabled(true);
	button_cancel[round-1]->setEnabled(true);
	//fully_updated[round]
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
		//q_ui->statusBar->setStatusTip("Question "+QString::number(index+1)+" of round "+QString::number(round)+" has been updated.");
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
		question_ans_e[ptr]->setEnabled(enable);
		question_time[ptr]->setEnabled(enable);
	}
	
}

void QEditor::disableDuplicates(int round)
{
	if(round>2)
	{
		bool activated=false;
		int ptr=round-1;
		if (question_ans_a[ptr]->isChecked() || question_ans_b[ptr]->isChecked()
		    || question_ans_c[ptr]->isChecked() || question_ans_d[ptr]->isChecked())
			activated=true;
		question_ans_e[ptr]->setDisabled(activated);
		bool specify=question_ans_e[ptr]->isChecked();
		question_ans_a[ptr]->setDisabled(specify);
		question_ans_b[ptr]->setDisabled(specify);
		question_ans_c[ptr]->setDisabled(specify);
		question_ans_d[ptr]->setDisabled(specify);
	}
}

void QEditor::load()
{
	if (file_prefix!="")
	{
		//warning here
	}
	else
	{
		QFileDialog load_dlg;
		load_dlg.setAcceptMode(QFileDialog::AcceptOpen);
		load_dlg.setFileMode(QFileDialog::ExistingFiles);
		load_dlg.setFilter(tr("Group XML files (*.xgrp)"));
		load_dlg.exec();
		
		file_prefix=load_dlg.selectedFiles().join("");
		file_prefix.replace(QString(".xgrp"),QString(""));
		if (file_prefix=="")
			return;
		for (int ctr=0;ctr<1;ctr++)
		{
			QFile fq(file_prefix+QString::number(ctr+1)+"_q.xml");
			fq.open(QIODevice::ReadOnly);
			QString xml_q=fq.readAll();
			QuestionData qd;
			xml_util.readQuestionData(ctr+1,xml_q,qd);
			
			QFile fa(file_prefix+QString::number(ctr+1)+"_a.xml");
			fa.open(QIODevice::ReadOnly);
			QString xml_a=fa.readAll();
			AnswerData ad;
			xml_util.readAnswerData(ctr+1,xml_a,ad);
			
			roundmodel[ctr]->feedData(qd,ad);
		}
	}
}

void QEditor::save()
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
			return;
		}
		QFile grp(file_prefix+".xgrp");
		if (!grp.open(QIODevice::ReadWrite))
			return;
		QTextStream out(&grp);
		out << "This file holds no data, this just groups xml files to open/edit them easier! ^_^";
	}
	if (file_prefix!="")
	{
		
		for (int roundctr=0;roundctr<4;roundctr++)
		{
			QuestionData rounddata;
			AnswerData ansdata;
			QString xml_q;
			QString xml_a;
			int question_cnt=roundmodel[roundctr]->rowCount();
			
			rounddata.welcome_msg=welcome[roundctr]->toPlainText();
			rounddata.contest_time=duration[roundctr]->value();
			
			for (int qctr=0;qctr<question_cnt;qctr++)
			{
				Question temp;
				roundmodel[roundctr]->getFullQuestion(qctr,&temp);
				rounddata.questions.push_back(temp);
				
				bool* cheat=new bool[5];
				roundmodel[roundctr]->getAnskey(qctr,cheat);
				for (int cctr=0;cctr<5;cctr++)
				{
					if (cheat[cctr])
					{
						QString temp=QString::number(cctr+1);
						if (roundctr>1)
							temp+=" "+roundmodel[roundctr]->getE(qctr);
						ansdata.insert(pair<int,QString>(qctr+1,temp));
					}
				}
				delete cheat;
			}
			
			
			//uncomment this part if its completed;
			
			QFile file_q(file_prefix+QString::number(roundctr+1)+"_q.xml");
			if (!file_q.open(QIODevice::WriteOnly))
				return;
			else
			{
				QTextStream out(&file_q);
				xml_util.writeQuestionData(roundctr+1,rounddata,xml_q);
				//out << "xml doc " << QString::number(roundctr+1); //replace w/ xml_q
				out << xml_q;
			}
			
			QFile file_a(file_prefix+QString::number(roundctr+1)+"_a.xml");
			if (!file_a.open(QIODevice::WriteOnly))
				return;
			else
			{
				QTextStream out(&file_a);
				xml_util.writeAnswerData(roundctr+1,ansdata,xml_a);
				//out << "xml doc " << QString::number(roundctr+1); //replace w/ xml_a
				out << xml_a;
			}
		}
		q_ui->statusBar->showMessage("File saved at "+file_prefix);
	}
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
	if (file_prefix=="")
	{
		cout << "A"<< endl;
		QMessageBox conf;
		conf.setWindowTitle("Notification - QEditor");
		conf.setText("Questions have not been saved yet");
		conf.setInformativeText("What do you want to do?");
		conf.setIcon(QMessageBox::Question);
		QPushButton *comsave=conf.addButton(tr("Save as..."),QMessageBox::ActionRole);
		QPushButton *comdiscard=conf.addButton(tr("Discard"),QMessageBox::ActionRole);
		QPushButton *comback=conf.addButton(tr("Cancel"),QMessageBox::ActionRole);
		conf.setDefaultButton(comsave);
		conf.exec();
		if (conf.clickedButton() == comsave)
		{
			save();
			if (file_prefix=="")
				event->ignore();
			else 
				event->accept();
		}
		else if (conf.clickedButton() == comdiscard)
		{
			event->accept();
		}
		else
		{
			event->ignore();
		}
	}
	else if (file_prefix!="" && changed)
	{
		cout << "B" << endl;
		QMessageBox conf;
		conf.setWindowTitle("Notification - QEditor");
		conf.setText("Details of question has been modified, but have not been saved");
		conf.setInformativeText("What do you want to do?");
		conf.setIcon(QMessageBox::Question);
		QPushButton *comsave=conf.addButton(tr("Save"),QMessageBox::ActionRole);
		QPushButton *comdiscard=conf.addButton(tr("Discard"),QMessageBox::ActionRole);
		QPushButton *comback=conf.addButton(tr("Cancel"),QMessageBox::ActionRole);
		conf.setDefaultButton(comsave);
		conf.exec();
		if (conf.clickedButton() == comsave)
		{
			save();
			event->accept();
		}
		else if (conf.clickedButton() == comdiscard)
		{
			event->accept();
		}
		else
		{
			event->ignore();
		}
		
	}
	else
	{
		cout << "c"<< endl;
		event->accept();
	}
}

int main(int argc, char *argv[])
{
	QApplication app (argc, argv);
	QEditor q_App;
	q_App.show();
	return app.exec();
}
