/***************************************************************************
                          constraintstudentsmaxroomchangesperdayform.cpp  -  description
                             -------------------
    begin                : 2019
    copyright            : (C) 2019 by Liviu Lalescu
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

#include "constraintstudentsmaxroomchangesperdayform.h"
#include "addconstraintstudentsmaxroomchangesperdayform.h"
#include "modifyconstraintstudentsmaxroomchangesperdayform.h"

#include <QListWidget>
#include <QScrollBar>
#include <QAbstractItemView>

ConstraintStudentsMaxRoomChangesPerDayForm::ConstraintStudentsMaxRoomChangesPerDayForm(QWidget* parent): QDialog(parent)
{
	setupUi(this);

	currentConstraintTextEdit->setReadOnly(true);
	
	modifyConstraintPushButton->setDefault(true);

	constraintsListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(constraintsListWidget, &QListWidget::currentRowChanged, this, &ConstraintStudentsMaxRoomChangesPerDayForm::constraintChanged);
	connect(addConstraintPushButton, &QPushButton::clicked, this, &ConstraintStudentsMaxRoomChangesPerDayForm::addConstraint);
	connect(closePushButton, &QPushButton::clicked, this, &ConstraintStudentsMaxRoomChangesPerDayForm::close);
	connect(removeConstraintPushButton, &QPushButton::clicked, this, &ConstraintStudentsMaxRoomChangesPerDayForm::removeConstraint);
	connect(modifyConstraintPushButton, &QPushButton::clicked, this, &ConstraintStudentsMaxRoomChangesPerDayForm::modifyConstraint);
	connect(constraintsListWidget, &QListWidget::itemDoubleClicked, this, &ConstraintStudentsMaxRoomChangesPerDayForm::modifyConstraint);

	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);
	
	this->filterChanged();
}

ConstraintStudentsMaxRoomChangesPerDayForm::~ConstraintStudentsMaxRoomChangesPerDayForm()
{
	saveFETDialogGeometry(this);
}

bool ConstraintStudentsMaxRoomChangesPerDayForm::filterOk(SpaceConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_STUDENTS_MAX_ROOM_CHANGES_PER_DAY){
		return true;
	}
	else
		return false;
}

void ConstraintStudentsMaxRoomChangesPerDayForm::filterChanged()
{
	this->visibleConstraintsList.clear();
	constraintsListWidget->clear();
	for(int i=0; i<gt.rules.spaceConstraintsList.size(); i++){
		SpaceConstraint* ctr=gt.rules.spaceConstraintsList[i];
		if(filterOk(ctr)){
			visibleConstraintsList.append(ctr);
			constraintsListWidget->addItem(ctr->getDescription(gt.rules));
		}
	}
	
	if(constraintsListWidget->count()>0)
		constraintsListWidget->setCurrentRow(0);
	else
		this->constraintChanged(-1);
}

void ConstraintStudentsMaxRoomChangesPerDayForm::constraintChanged(int index)
{
	if(index<0){
		currentConstraintTextEdit->setPlainText("");
		return;
	}
	assert(index<this->visibleConstraintsList.size());
	SpaceConstraint* ctr=this->visibleConstraintsList.at(index);
	assert(ctr!=nullptr);
	currentConstraintTextEdit->setPlainText(ctr->getDetailedDescription(gt.rules));
}

void ConstraintStudentsMaxRoomChangesPerDayForm::addConstraint()
{
	AddConstraintStudentsMaxRoomChangesPerDayForm form(this);
	setParentAndOtherThings(&form, this);
	form.exec();

	filterChanged();
	
	constraintsListWidget->setCurrentRow(constraintsListWidget->count()-1);
}

void ConstraintStudentsMaxRoomChangesPerDayForm::modifyConstraint()
{
	int valv=constraintsListWidget->verticalScrollBar()->value();
	int valh=constraintsListWidget->horizontalScrollBar()->value();

	int i=constraintsListWidget->currentRow();
	if(i<0){
		QMessageBox::information(this, tr("FET information"), tr("Invalid selected constraint"));
		return;
	}
	SpaceConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraintStudentsMaxRoomChangesPerDayForm form(this, (ConstraintStudentsMaxRoomChangesPerDay*)ctr);
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

void ConstraintStudentsMaxRoomChangesPerDayForm::removeConstraint()
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
