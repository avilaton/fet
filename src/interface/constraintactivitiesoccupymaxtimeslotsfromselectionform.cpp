/***************************************************************************
                          constraintactivitiesoccupymaxtimeslotsfromselectionform.cpp  -  description
                             -------------------
    begin                : Sept 26, 2011
    copyright            : (C) 2011 by Liviu Lalescu
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

#include "constraintactivitiesoccupymaxtimeslotsfromselectionform.h"
#include "addconstraintactivitiesoccupymaxtimeslotsfromselectionform.h"
#include "modifyconstraintactivitiesoccupymaxtimeslotsfromselectionform.h"

#include <QListWidget>
#include <QScrollBar>
#include <QAbstractItemView>

ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm(QWidget* parent): QDialog(parent)
{
	setupUi(this);

	currentConstraintTextEdit->setReadOnly(true);

	modifyConstraintPushButton->setDefault(true);

	constraintsListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(constraintsListWidget, &QListWidget::currentRowChanged, this, &ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::constraintChanged);
	connect(addConstraintPushButton, &QPushButton::clicked, this, &ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::addConstraint);
	connect(closePushButton, &QPushButton::clicked, this, &ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::close);
	connect(removeConstraintPushButton, &QPushButton::clicked, this, &ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::removeConstraint);
	connect(modifyConstraintPushButton, &QPushButton::clicked, this, &ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::modifyConstraint);
	connect(constraintsListWidget, &QListWidget::itemDoubleClicked, this, &ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::modifyConstraint);

	connect(helpPushButton, &QPushButton::clicked, this, &ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::help);

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
	
/////////////
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
///////////////

	this->filterChanged();

	connect(teachersComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::filterChanged);
	connect(studentsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::filterChanged);
	connect(subjectsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::filterChanged);
	connect(activityTagsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::filterChanged);
}

ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::~ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm()
{
	saveFETDialogGeometry(this);
}

bool ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::filterOk(TimeConstraint* ctr)
{
	if(ctr->type!=CONSTRAINT_ACTIVITIES_OCCUPY_MAX_TIME_SLOTS_FROM_SELECTION)
		return false;

	ConstraintActivitiesOccupyMaxTimeSlotsFromSelection* c=(ConstraintActivitiesOccupyMaxTimeSlotsFromSelection*) ctr;
	
	QString tn=teachersComboBox->currentText();
	QString sbn=subjectsComboBox->currentText();
	QString atn=activityTagsComboBox->currentText();
	QString stn=studentsComboBox->currentText();
	
	if(tn=="" && sbn=="" && atn=="" && stn=="")
		return true;
	
	bool foundTeacher=false, foundStudents=false, foundSubject=false, foundActivityTag=false;
		
	for(int i=0; i<c->activitiesIds.count(); i++){
		int id=c->activitiesIds.at(i);
		/*Activity* act=nullptr;
		for(Activity* a : std::as_const(gt.rules.activitiesList))
			if(a->id==id)
				act=a;*/
		Activity* act=gt.rules.activitiesPointerHash.value(id, nullptr);
		
		if(act!=nullptr){
			//teacher
			if(tn!=""){
				bool ok2=false;
				for(QStringList::const_iterator it=act->teachersNames.constBegin(); it!=act->teachersNames.constEnd(); it++)
					if(*it == tn){
						ok2=true;
						break;
					}
				if(ok2)
					foundTeacher=true;
			}
			else
				foundTeacher=true;

			//subject
			if(sbn!="" && sbn!=act->subjectName)
				;
			else
				foundSubject=true;
		
			//activity tag
			if(atn!="" && !act->activityTagsNames.contains(atn))
				;
			else
				foundActivityTag=true;
		
			//students
			if(stn!=""){
				bool ok2=false;
				for(QStringList::const_iterator it=act->studentsNames.constBegin(); it!=act->studentsNames.constEnd(); it++)
					if(*it == stn){
						ok2=true;
						break;
				}
				if(ok2)
					foundStudents=true;
			}
			else
				foundStudents=true;
		}
	}
	
	if(foundTeacher && foundStudents && foundSubject && foundActivityTag)
		return true;
	else
		return false;
}

void ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::filterChanged()
{
	this->visibleConstraintsList.clear();
	constraintsListWidget->clear();
	for(int i=0; i<gt.rules.timeConstraintsList.size(); i++){
		TimeConstraint* ctr=gt.rules.timeConstraintsList[i];
		if(filterOk(ctr)){
			visibleConstraintsList.append(ctr);
			constraintsListWidget->addItem(ctr->getDescription(gt.rules));
		}
	}
	
	if(constraintsListWidget->count()>0)
		constraintsListWidget->setCurrentRow(0);
	else
		constraintChanged(-1);
}

void ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::constraintChanged(int index)
{
	if(index<0){
		currentConstraintTextEdit->setPlainText("");
		return;
	}
	assert(index<this->visibleConstraintsList.size());
	TimeConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=nullptr);
	currentConstraintTextEdit->setPlainText(ctr->getDetailedDescription(gt.rules));
}

void ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::addConstraint()
{
	AddConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm form(this);
	setParentAndOtherThings(&form, this);
	form.exec();

	filterChanged();
	
	constraintsListWidget->setCurrentRow(constraintsListWidget->count()-1);
}

void ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::modifyConstraint()
{
	int valv=constraintsListWidget->verticalScrollBar()->value();
	int valh=constraintsListWidget->horizontalScrollBar()->value();

	int i=constraintsListWidget->currentRow();
	if(i<0){
		QMessageBox::information(this, tr("FET information"), tr("Invalid selected constraint"));
		return;
	}
	TimeConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm form(this, (ConstraintActivitiesOccupyMaxTimeSlotsFromSelection*)ctr);
	setParentAndOtherThings(&form, this);
	form.exec();

	filterChanged();
	
	constraintsListWidget->verticalScrollBar()->setValue(valv);
	constraintsListWidget->horizontalScrollBar()->setValue(valh);
	
	if(i>=constraintsListWidget->count())
		i=constraintsListWidget->count()-1;
		
	if(i>=0)
		constraintsListWidget->setCurrentRow(i);
	else
		this->constraintChanged(-1);
}

void ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::removeConstraint()
{
	int i=constraintsListWidget->currentRow();
	if(i<0){
		QMessageBox::information(this, tr("FET information"), tr("Invalid selected constraint"));
		return;
	}
	TimeConstraint* ctr=this->visibleConstraintsList.at(i);
	QString s;
	s=tr("Remove constraint?");
	s+="\n\n";
	s+=ctr->getDetailedDescription(gt.rules);
	
	QListWidgetItem* item;

	QString oc;

	switch( LongTextMessageBox::confirmation( this, tr("FET confirmation"),
		s, tr("Yes"), tr("No"), QString(), 0, 1 ) ){
	case 0: // The user clicked the OK button or pressed Enter
		oc=ctr->getDetailedDescription(gt.rules);

		gt.rules.removeTimeConstraint(ctr);

		gt.rules.addUndoPoint(tr("Removed the constraint:\n\n%1").arg(oc));
		
		visibleConstraintsList.removeAt(i);
		constraintsListWidget->setCurrentRow(-1);
		item=constraintsListWidget->takeItem(i);
		delete item;

		break;
	case 1: // The user clicked the Cancel button or pressed Escape
		break;
	}
	
	if(i>=constraintsListWidget->count())
		i=constraintsListWidget->count()-1;
	if(i>=0)
		constraintsListWidget->setCurrentRow(i);
	else
		this->constraintChanged(-1);
}

void ConstraintActivitiesOccupyMaxTimeSlotsFromSelectionForm::help()
{
	QString s=QString("");
	
	s+=tr("IMPORTANT NOTE: Please use this constraint ONLY when strictly necessary, when it is really useful, when it has effect, "
	 "when it is not implied by the other constraints, and in a non-redundant way. Otherwise it may slow down very much the generation.");
	s+=QString("\n\n");
	s+=tr("This constraint type was added on 25 September 2011.");
	s+=QString("\n\n");
	s+=tr("To use this constraint, you need to specify a set of activities, a set of time slots, and a maximum number "
	 "of time slots from those selected which can be occupied by the specified activities.");
	s+=QString("\n\n");
	s+=tr("Uses of this constraint are probably very wide. The main reason it was added was to enable users to specify that a teacher "
	 "should have activities in certain time slots. Example: we have a week with 30 hours. Teacher T has 20 hours of activities, and "
	 "we would like him to have activities Monday and Tuesday on the first two hours. Add a constraint of this type, with the set of activities "
	 "being all the activities of teacher T, selected time slots being the remaining 30-2*2=26 slots of the week, and the maximum number of "
	 "selected time slots which can be occupied by these activities being 20-2*2=16.");
	s+=QString(" ");
	s+=tr("Please note that more recently it was added the constraint of type activities occupy min time slots from selection. This permits to specify "
	 "easier/directly the times when a teacher should have activities.");
	
	LongTextMessageBox::largeInformation(this, tr("FET help"), s);
}
