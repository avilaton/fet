/***************************************************************************
                          addconstraintactivitypreferredroomsform.cpp  -  description
                             -------------------
    begin                : March 28, 2005
    copyright            : (C) 2005 by Liviu Lalescu
    email                : Please see https://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find there the email address)
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Affero General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include <QMessageBox>

#include "longtextmessagebox.h"

#include "addconstraintactivitypreferredroomsform.h"

#include <QListWidget>
#include <QAbstractItemView>

AddConstraintActivityPreferredRoomsForm::AddConstraintActivityPreferredRoomsForm(QWidget* parent): QDialog(parent)
{
	setupUi(this);

	addConstraintPushButton->setDefault(true);
	
	roomsListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	selectedRoomsListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(closePushButton, &QPushButton::clicked, this, &AddConstraintActivityPreferredRoomsForm::close);
	connect(addConstraintPushButton, &QPushButton::clicked, this, &AddConstraintActivityPreferredRoomsForm::addConstraint);
	connect(roomsListWidget, &QListWidget::itemDoubleClicked, this, &AddConstraintActivityPreferredRoomsForm::addRoom);
	connect(selectedRoomsListWidget, &QListWidget::itemDoubleClicked, this, &AddConstraintActivityPreferredRoomsForm::removeRoom);
	connect(clearPushButton, &QPushButton::clicked, this, &AddConstraintActivityPreferredRoomsForm::clear);

	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);
	
	QSize tmp1=teachersComboBox->minimumSizeHint();
	Q_UNUSED(tmp1);
	QSize tmp2=studentsComboBox->minimumSizeHint();
	Q_UNUSED(tmp2);
	QSize tmp3=subjectsComboBox->minimumSizeHint();
	Q_UNUSED(tmp3);
	QSize tmp4=activityTagsComboBox->minimumSizeHint();
	Q_UNUSED(tmp4);
	
	QSize tmp5=activitiesComboBox->minimumSizeHint();
	Q_UNUSED(tmp5);

	activitiesComboBox->setMaximumWidth(maxRecommendedWidth(this));
	
	teachersComboBox->addItem("");
	for(int i=0; i<gt.rules.teachersList.size(); i++){
		Teacher* tch=gt.rules.teachersList[i];
		teachersComboBox->addItem(tch->name);
	}
	teachersComboBox->setCurrentIndex(0);

	subjectsComboBox->addItem("");
	for(int i=0; i<gt.rules.subjectsList.size(); i++){
		Subject* sb=gt.rules.subjectsList[i];
		subjectsComboBox->addItem(sb->name);
	}
	subjectsComboBox->setCurrentIndex(0);

	activityTagsComboBox->addItem("");
	for(int i=0; i<gt.rules.activityTagsList.size(); i++){
		ActivityTag* st=gt.rules.activityTagsList[i];
		activityTagsComboBox->addItem(st->name);
	}
	activityTagsComboBox->setCurrentIndex(0);

	populateStudentsComboBox(studentsComboBox, QString(""), true);
	studentsComboBox->setCurrentIndex(0);
	
	filterChanged();

	updateRoomsListWidget();

	connect(teachersComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &AddConstraintActivityPreferredRoomsForm::filterChanged);
	connect(studentsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &AddConstraintActivityPreferredRoomsForm::filterChanged);
	connect(subjectsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &AddConstraintActivityPreferredRoomsForm::filterChanged);
	connect(activityTagsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &AddConstraintActivityPreferredRoomsForm::filterChanged);
}

AddConstraintActivityPreferredRoomsForm::~AddConstraintActivityPreferredRoomsForm()
{
	saveFETDialogGeometry(this);
}

bool AddConstraintActivityPreferredRoomsForm::filterOk(Activity* act)
{
	QString tn=teachersComboBox->currentText();
	QString stn=studentsComboBox->currentText();
	QString sbn=subjectsComboBox->currentText();
	QString atn=activityTagsComboBox->currentText();
	int ok=true;

	//teacher
	if(tn!=""){
		bool ok2=false;
		for(QStringList::const_iterator it=act->teachersNames.constBegin(); it!=act->teachersNames.constEnd(); it++)
			if(*it == tn){
				ok2=true;
				break;
			}
		if(!ok2)
			ok=false;
	}

	//subject
	if(sbn!="" && sbn!=act->subjectName)
		ok=false;
		
	//activity tag
	if(atn!="" && !act->activityTagsNames.contains(atn))
		ok=false;
		
	//students
	if(stn!=""){
		bool ok2=false;
		for(QStringList::const_iterator it=act->studentsNames.constBegin(); it!=act->studentsNames.constEnd(); it++)
			if(*it == stn){
				ok2=true;
				break;
			}
		if(!ok2)
			ok=false;
	}
	
	return ok;
}

void AddConstraintActivityPreferredRoomsForm::filterChanged(){
	activitiesComboBox->clear();
	activitiesList.clear();
	
	for(int i=0; i<gt.rules.activitiesList.size(); i++){
		Activity* act=gt.rules.activitiesList[i];
		
		if(filterOk(act)){
			activitiesComboBox->addItem(act->getDescription(gt.rules));
			this->activitiesList.append(act->id);
		}
	}
}

void AddConstraintActivityPreferredRoomsForm::updateRoomsListWidget()
{
	roomsListWidget->clear();
	selectedRoomsListWidget->clear();

	for(int i=0; i<gt.rules.roomsList.size(); i++){
		Room* rm= gt.rules.roomsList[i];
		roomsListWidget->addItem(rm->name);
	}
}

void AddConstraintActivityPreferredRoomsForm::addConstraint()
{
	SpaceConstraint *ctr=nullptr;

	double weight;
	QString tmp=weightLineEdit->text();
	weight_sscanf(tmp, "%lf", &weight);
	if(weight<0.0 || weight>100){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight (percentage)"));
		return;
	}

	if(selectedRoomsListWidget->count()==0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Empty list of selected rooms"));
		return;
	}
	/*if(selectedRoomsListWidget->count()==1){
		QMessageBox::warning(this, tr("FET information"),
			tr("Only one selected room - please use constraint activity preferred room if you want a single room"));
		return;
	}*/

	int id;
	int tmp2=activitiesComboBox->currentIndex();
	if(tmp2<0 || tmp2>=gt.rules.activitiesList.size() || tmp2>=activitiesList.size()){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid activity"));
		return;
	}
	else
		id=activitiesList.at(tmp2);
	
	QStringList roomsList;
	for(int i=0; i<selectedRoomsListWidget->count(); i++)
		roomsList.append(selectedRoomsListWidget->item(i)->text());
	
	ctr=new ConstraintActivityPreferredRooms(weight, id, roomsList);
	bool tmp3=gt.rules.addSpaceConstraint(ctr);
	
	if(tmp3){
		QString s=tr("Constraint added:");
		s+="\n\n";
		s+=ctr->getDetailedDescription(gt.rules);
		LongTextMessageBox::information(this, tr("FET information"), s);

		gt.rules.addUndoPoint(tr("Added the constraint:\n\n%1").arg(ctr->getDetailedDescription(gt.rules)));
	}
	else{
		QMessageBox::warning(this, tr("FET information"),
			tr("Constraint NOT added - please report error"));
		delete ctr;
	}
}

void AddConstraintActivityPreferredRoomsForm::addRoom()
{
	if(roomsListWidget->currentRow()<0)
		return;
	QString rmName=roomsListWidget->currentItem()->text();
	assert(rmName!="");
	int i;
	//duplicate?
	for(i=0; i<selectedRoomsListWidget->count(); i++)
		if(rmName==selectedRoomsListWidget->item(i)->text())
			break;
	if(i<selectedRoomsListWidget->count())
		return;
	selectedRoomsListWidget->addItem(rmName);
	selectedRoomsListWidget->setCurrentRow(selectedRoomsListWidget->count()-1);
}

void AddConstraintActivityPreferredRoomsForm::removeRoom()
{
	if(selectedRoomsListWidget->currentRow()<0 || selectedRoomsListWidget->count()<=0)
		return;
	int tmp=selectedRoomsListWidget->currentRow();
	
	selectedRoomsListWidget->setCurrentRow(-1);
	QListWidgetItem* item=selectedRoomsListWidget->takeItem(tmp);
	delete item;
	if(tmp<selectedRoomsListWidget->count())
		selectedRoomsListWidget->setCurrentRow(tmp);
	else
		selectedRoomsListWidget->setCurrentRow(selectedRoomsListWidget->count()-1);
}

void AddConstraintActivityPreferredRoomsForm::clear()
{
	selectedRoomsListWidget->clear();
}
