/***************************************************************************
                          modifyconstraintstudentssetafternoonintervalmaxdaysperweekform.cpp  -  description
                             -------------------
    begin                : 2020
    copyright            : (C) 2020 by Liviu Lalescu
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

#include "modifyconstraintstudentssetafternoonintervalmaxdaysperweekform.h"
#include "timeconstraint.h"

ModifyConstraintStudentsSetAfternoonIntervalMaxDaysPerWeekForm::ModifyConstraintStudentsSetAfternoonIntervalMaxDaysPerWeekForm(QWidget* parent, ConstraintStudentsSetAfternoonIntervalMaxDaysPerWeek* ctr): QDialog(parent)
{
	setupUi(this);

	okPushButton->setDefault(true);

	connect(okPushButton, &QPushButton::clicked, this, &ModifyConstraintStudentsSetAfternoonIntervalMaxDaysPerWeekForm::ok);
	connect(cancelPushButton, &QPushButton::clicked, this, &ModifyConstraintStudentsSetAfternoonIntervalMaxDaysPerWeekForm::cancel);

	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);

	QSize tmp2=studentsComboBox->minimumSizeHint();
	Q_UNUSED(tmp2);
	
	QSize tmp5=startHourComboBox->minimumSizeHint();
	Q_UNUSED(tmp5);
	QSize tmp6=endHourComboBox->minimumSizeHint();
	Q_UNUSED(tmp6);
	
	this->_ctr=ctr;
	
	weightLineEdit->setText(CustomFETString::number(ctr->weightPercentage));
	
	updateMaxDaysSpinBox();
	updateStudentsComboBox(parent);
	
	maxDaysSpinBox->setValue(ctr->maxDaysPerWeek);
	
	for(int i=0; i<gt.rules.nHoursPerDay; i++){
		startHourComboBox->addItem(gt.rules.hoursOfTheDay[i]);
	}
	startHourComboBox->setCurrentIndex(ctr->startHour);

	for(int i=0; i<gt.rules.nHoursPerDay; i++){
		endHourComboBox->addItem(gt.rules.hoursOfTheDay[i]);
	}
	endHourComboBox->addItem(tr("End of day"));
	endHourComboBox->setCurrentIndex(ctr->endHour);
}

ModifyConstraintStudentsSetAfternoonIntervalMaxDaysPerWeekForm::~ModifyConstraintStudentsSetAfternoonIntervalMaxDaysPerWeekForm()
{
	saveFETDialogGeometry(this);
}

void ModifyConstraintStudentsSetAfternoonIntervalMaxDaysPerWeekForm::updateStudentsComboBox(QWidget* parent){
	int j=populateStudentsComboBox(studentsComboBox, this->_ctr->students);
	if(j<0)
		showWarningForInvisibleSubgroupConstraint(parent, this->_ctr->students);
	else
		assert(j>=0);
	studentsComboBox->setCurrentIndex(j);
}

void ModifyConstraintStudentsSetAfternoonIntervalMaxDaysPerWeekForm::updateMaxDaysSpinBox(){
	maxDaysSpinBox->setMinimum(0);
	maxDaysSpinBox->setMaximum(gt.rules.nDaysPerWeek/2);
}

void ModifyConstraintStudentsSetAfternoonIntervalMaxDaysPerWeekForm::ok()
{
	if(studentsComboBox->currentIndex()<0){
		showWarningCannotModifyConstraintInvisibleSubgroupConstraint(this, this->_ctr->students);
		return;
	}

	double weight;
	QString tmp=weightLineEdit->text();
	weight_sscanf(tmp, "%lf", &weight);
	if(weight<0.0 || weight>100.0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight (percentage)"));
		return;
	}
	if(weight!=100.0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight (percentage) - it has to be 100%"));
		return;
	}

	int max_days=maxDaysSpinBox->value();

	QString students_name=studentsComboBox->currentText();
	StudentsSet* s=gt.rules.searchStudentsSet(students_name);
	if(s==nullptr){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid students set"));
		return;
	}
	
	int startHour=startHourComboBox->currentIndex();
	int endHour=endHourComboBox->currentIndex();
	if(startHour<0 || startHour>=gt.rules.nHoursPerDay){
		QMessageBox::warning(this, tr("FET information"),
		 tr("Start hour invalid"));
		return;
	}
	if(endHour<0 || endHour>gt.rules.nHoursPerDay){
		QMessageBox::warning(this, tr("FET information"),
		 tr("End hour invalid"));
		return;
	}
	if(endHour<=startHour){
		QMessageBox::warning(this, tr("FET information"),
		 tr("Start hour cannot be greater or equal than end hour"));
		return;
	}

	QString oldcs=this->_ctr->getDetailedDescription(gt.rules);

	this->_ctr->weightPercentage=weight;
	this->_ctr->maxDaysPerWeek=max_days;
	this->_ctr->students=students_name;

	this->_ctr->startHour=startHour;
	this->_ctr->endHour=endHour;

	QString newcs=this->_ctr->getDetailedDescription(gt.rules);
	gt.rules.addUndoPoint(tr("Modified the constraint:\n\n%1\ninto\n\n%2").arg(oldcs).arg(newcs));

	gt.rules.internalStructureComputed=false;
	setRulesModifiedAndOtherThings(&gt.rules);
	
	this->close();
}

void ModifyConstraintStudentsSetAfternoonIntervalMaxDaysPerWeekForm::cancel()
{
	this->close();
}
