/***************************************************************************
                          constraintteachersmingapsbetweenorderedpairofactivitytagsperrealdayform.cpp  -  description
                             -------------------
    begin                : 2024
    copyright            : (C) 2024 by Liviu Lalescu
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

#include "constraintteachersmingapsbetweenorderedpairofactivitytagsperrealdayform.h"
#include "addconstraintteachersmingapsbetweenorderedpairofactivitytagsperrealdayform.h"
#include "modifyconstraintteachersmingapsbetweenorderedpairofactivitytagsperrealdayform.h"

#include <QListWidget>
#include <QScrollBar>
#include <QAbstractItemView>

ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm::ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm(QWidget* parent): QDialog(parent)
{
	setupUi(this);

	currentConstraintTextEdit->setReadOnly(true);
	
	modifyConstraintPushButton->setDefault(true);

	constraintsListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(constraintsListWidget, &QListWidget::currentRowChanged, this, &ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm::constraintChanged);
	connect(addConstraintPushButton, &QPushButton::clicked, this, &ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm::addConstraint);
	connect(closePushButton,  &QPushButton::clicked, this, &ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm::close);
	connect(removeConstraintPushButton, &QPushButton::clicked, this, &ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm::removeConstraint);
	connect(modifyConstraintPushButton, &QPushButton::clicked, this, &ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm::modifyConstraint);
	connect(constraintsListWidget, &QListWidget::itemDoubleClicked, this, &ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm::modifyConstraint);

	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);

	QSize tmp4=firstActivityTagComboBox->minimumSizeHint();
	Q_UNUSED(tmp4);
	QSize tmp5=secondActivityTagComboBox->minimumSizeHint();
	Q_UNUSED(tmp5);
	
	firstActivityTagComboBox->clear();
	firstActivityTagComboBox->addItem("");
	for(ActivityTag* at : std::as_const(gt.rules.activityTagsList))
		firstActivityTagComboBox->addItem(at->name);

	secondActivityTagComboBox->clear();
	secondActivityTagComboBox->addItem("");
	for(ActivityTag* at : std::as_const(gt.rules.activityTagsList))
		secondActivityTagComboBox->addItem(at->name);

	this->filterChanged();

	connect(firstActivityTagComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm::filterChanged);
	connect(secondActivityTagComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm::filterChanged);
}

ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm::~ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm()
{
	saveFETDialogGeometry(this);
}

bool ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm::filterOk(TimeConstraint* ctr)
{
	if(ctr->type==CONSTRAINT_TEACHERS_MIN_GAPS_BETWEEN_ORDERED_PAIR_OF_ACTIVITY_TAGS_PER_REAL_DAY){
		ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDay* c=(ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDay*) ctr;
		return (c->firstActivityTag==firstActivityTagComboBox->currentText() || firstActivityTagComboBox->currentText()=="")
		 &&
		 (c->secondActivityTag==secondActivityTagComboBox->currentText() || secondActivityTagComboBox->currentText()=="");
	}
	else
		return false;
}

void ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm::filterChanged()
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
		this->constraintChanged(-1);
}

void ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm::constraintChanged(int index)
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

void ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm::addConstraint()
{
	AddConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm form(this);
	setParentAndOtherThings(&form, this);
	form.exec();

	filterChanged();
	
	constraintsListWidget->setCurrentRow(constraintsListWidget->count()-1);
}

void ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm::modifyConstraint()
{
	int valv=constraintsListWidget->verticalScrollBar()->value();
	int valh=constraintsListWidget->horizontalScrollBar()->value();

	int i=constraintsListWidget->currentRow();
	if(i<0){
		QMessageBox::information(this, tr("FET information"), tr("Invalid selected constraint"));
		return;
	}
	TimeConstraint* ctr=this->visibleConstraintsList.at(i);

	ModifyConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm form(this, (ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDay*)ctr);
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

void ConstraintTeachersMinGapsBetweenOrderedPairOfActivityTagsPerRealDayForm::removeConstraint()
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
