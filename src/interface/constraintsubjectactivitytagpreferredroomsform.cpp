/***************************************************************************
                          constraintsubjectactivitytagpreferredroomsform.cpp  -  description
                             -------------------
    begin                : 18 Aug 2007
    copyright            : (C) 2007 by Liviu Lalescu
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

#include "constraintsubjectactivitytagpreferredroomsform.h"
#include "addconstraintsubjectactivitytagpreferredroomsform.h"
#include "modifyconstraintsubjectactivitytagpreferredroomsform.h"

#include <QListWidget>
#include <QScrollBar>
#include <QAbstractItemView>

ConstraintSubjectActivityTagPreferredRoomsForm::ConstraintSubjectActivityTagPreferredRoomsForm(QWidget* parent): QDialog(parent)
{
	setupUi(this);

	currentConstraintTextEdit->setReadOnly(true);
	
	modifyConstraintPushButton->setDefault(true);

	constraintsListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(addConstraintPushButton, &QPushButton::clicked, this, &ConstraintSubjectActivityTagPreferredRoomsForm::addConstraint);
	connect(removeConstraintPushButton, &QPushButton::clicked, this, &ConstraintSubjectActivityTagPreferredRoomsForm::removeConstraint);
	connect(closePushButton, &QPushButton::clicked, this, &ConstraintSubjectActivityTagPreferredRoomsForm::close);
	connect(constraintsListWidget, &QListWidget::currentRowChanged, this, &ConstraintSubjectActivityTagPreferredRoomsForm::constraintChanged);
	connect(modifyConstraintPushButton, &QPushButton::clicked, this, &ConstraintSubjectActivityTagPreferredRoomsForm::modifyConstraint);
	connect(constraintsListWidget, &QListWidget::itemDoubleClicked, this, &ConstraintSubjectActivityTagPreferredRoomsForm::modifyConstraint);

	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);
	
	QSize tmp3=subjectsComboBox->minimumSizeHint();
	Q_UNUSED(tmp3);
	QSize tmp4=activityTagsComboBox->minimumSizeHint();
	Q_UNUSED(tmp4);
	
	QSize tmp5=roomsComboBox->minimumSizeHint();
	Q_UNUSED(tmp5);
	
	roomsComboBox->addItem("");
	for(int i=0; i<gt.rules.roomsList.size(); i++){
		Room* rm=gt.rules.roomsList[i];
		roomsComboBox->addItem(rm->name);
	}

	subjectsComboBox->addItem("");
	for(int i=0; i<gt.rules.subjectsList.size(); i++){
		Subject* sb=gt.rules.subjectsList[i];
		subjectsComboBox->addItem(sb->name);
	}

	activityTagsComboBox->addItem("");
	for(int i=0; i<gt.rules.activityTagsList.size(); i++){
		ActivityTag* sb=gt.rules.activityTagsList[i];
		activityTagsComboBox->addItem(sb->name);
	}

	this->filterChanged();

	connect(roomsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &ConstraintSubjectActivityTagPreferredRoomsForm::filterChanged);
	connect(subjectsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &ConstraintSubjectActivityTagPreferredRoomsForm::filterChanged);
	connect(activityTagsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &ConstraintSubjectActivityTagPreferredRoomsForm::filterChanged);
}

ConstraintSubjectActivityTagPreferredRoomsForm::~ConstraintSubjectActivityTagPreferredRoomsForm()
{
	saveFETDialogGeometry(this);
}

void ConstraintSubjectActivityTagPreferredRoomsForm::filterChanged()
{
	this->visibleConstraintsList.clear();
	constraintsListWidget->clear();
	for(int i=0; i<gt.rules.spaceConstraintsList.size(); i++){
		SpaceConstraint* ctr=gt.rules.spaceConstraintsList[i];
		if(filterOk(ctr)){
			QString s;
			s=ctr->getDescription(gt.rules);
			visibleConstraintsList.append(ctr);
			constraintsListWidget->addItem(s);
		}
	}

	if(constraintsListWidget->count()>0)
		constraintsListWidget->setCurrentRow(0);
	else
		constraintsListWidget->setCurrentRow(-1);
}

bool ConstraintSubjectActivityTagPreferredRoomsForm::filterOk(SpaceConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_SUBJECT_ACTIVITY_TAG_PREFERRED_ROOMS){
		ConstraintSubjectActivityTagPreferredRooms* c=(ConstraintSubjectActivityTagPreferredRooms*)ctr;
		return (c->roomsNames.contains(roomsComboBox->currentText()) || roomsComboBox->currentText()=="") &&
		 (c->subjectName==subjectsComboBox->currentText() || subjectsComboBox->currentText()=="") &&
		 (c->activityTagName==activityTagsComboBox->currentText() || activityTagsComboBox->currentText()=="");
	}
	else
		return false;
}

void ConstraintSubjectActivityTagPreferredRoomsForm::constraintChanged(int index)
{
	if(index<0){
		currentConstraintTextEdit->setPlainText("");
		return;
	}
	QString s;
	assert(index<this->visibleConstraintsList.size());
	SpaceConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=nullptr);
	s=ctr->getDetailedDescription(gt.rules);
	currentConstraintTextEdit->setPlainText(s);
}

void ConstraintSubjectActivityTagPreferredRoomsForm::addConstraint()
{
	AddConstraintSubjectActivityTagPreferredRoomsForm form(this);
	setParentAndOtherThings(&form, this);
	form.exec();

	this->filterChanged();
	
	constraintsListWidget->setCurrentRow(constraintsListWidget->count()-1);
}

void ConstraintSubjectActivityTagPreferredRoomsForm::modifyConstraint()
{
	int valv=constraintsListWidget->verticalScrollBar()->value();
	int valh=constraintsListWidget->horizontalScrollBar()->value();

	int i=constraintsListWidget->currentRow();
	if(i<0){
		QMessageBox::information(this, tr("FET information"), tr("Invalid selected constraint"));
		return;
	}
	SpaceConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraintSubjectActivityTagPreferredRoomsForm form(this, (ConstraintSubjectActivityTagPreferredRooms*)ctr);
	setParentAndOtherThings(&form, this);
	form.exec();

	this->filterChanged();
	
	constraintsListWidget->verticalScrollBar()->setValue(valv);
	constraintsListWidget->horizontalScrollBar()->setValue(valh);

	if(i>=constraintsListWidget->count())
		i=constraintsListWidget->count()-1;

	if(i>=0)
		constraintsListWidget->setCurrentRow(i);
	else
		this->constraintChanged(-1);
}

void ConstraintSubjectActivityTagPreferredRoomsForm::removeConstraint()
{
	int i=constraintsListWidget->currentRow();
	if(i<0){
		QMessageBox::information(this, tr("FET information"), tr("Invalid selected constraint"));
		return;
	}
	SpaceConstraint* ctr=this->visibleConstraintsList.at(i);
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

		gt.rules.removeSpaceConstraint(ctr);

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
