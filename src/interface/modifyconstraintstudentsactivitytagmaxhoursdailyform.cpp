/***************************************************************************
                          modifyconstraintstudentsactivitytagmaxhoursdailyform.cpp  -  description
                             -------------------
    begin                : 2009
    copyright            : (C) 2009 by Liviu Lalescu
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

#include "modifyconstraintstudentsactivitytagmaxhoursdailyform.h"
#include "timeconstraint.h"

ModifyConstraintStudentsActivityTagMaxHoursDailyForm::ModifyConstraintStudentsActivityTagMaxHoursDailyForm(QWidget* parent, ConstraintStudentsActivityTagMaxHoursDaily* ctr): QDialog(parent)
{
	setupUi(this);

	okPushButton->setDefault(true);

	connect(okPushButton, &QPushButton::clicked, this, &ModifyConstraintStudentsActivityTagMaxHoursDailyForm::ok);
	connect(cancelPushButton, &QPushButton::clicked, this, &ModifyConstraintStudentsActivityTagMaxHoursDailyForm::cancel);

	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);

	QSize tmp4=activityTagsComboBox->minimumSizeHint();
	Q_UNUSED(tmp4);
	
	this->_ctr=ctr;
	
	weightLineEdit->setText(CustomFETString::number(ctr->weightPercentage));
	
	updateActivityTagsComboBox();

	maxHoursSpinBox->setMinimum(1);
	maxHoursSpinBox->setMaximum(gt.rules.nHoursPerDay);
	maxHoursSpinBox->setValue(ctr->maxHoursDaily);
}

ModifyConstraintStudentsActivityTagMaxHoursDailyForm::~ModifyConstraintStudentsActivityTagMaxHoursDailyForm()
{
	saveFETDialogGeometry(this);
}

void ModifyConstraintStudentsActivityTagMaxHoursDailyForm::updateActivityTagsComboBox()
{
	activityTagsComboBox->clear();
	int j=-1;
	for(int i=0; i<gt.rules.activityTagsList.count(); i++){
		ActivityTag* at=gt.rules.activityTagsList.at(i);
		activityTagsComboBox->addItem(at->name);
		if(at->name==this->_ctr->activityTagName)
			j=i;
	}
	assert(j>=0);
	activityTagsComboBox->setCurrentIndex(j);
}

void ModifyConstraintStudentsActivityTagMaxHoursDailyForm::ok()
{
	double weight;
	QString tmp=weightLineEdit->text();
	weight_sscanf(tmp, "%lf", &weight);
	if(weight<0.0 || weight>100.0){
		QMessageBox::warning(this, tr("FET warning"),
			tr("Invalid weight (percentage)"));
		return;
	}

	QString activityTagName=activityTagsComboBox->currentText();
	int ati=gt.rules.searchActivityTag(activityTagName);
	if(ati<0){
		QMessageBox::warning(this, tr("FET warning"), tr("Invalid activity tag"));
		return;
	}

	QString oldcs=this->_ctr->getDetailedDescription(gt.rules);

	this->_ctr->weightPercentage=weight;
	this->_ctr->activityTagName=activityTagName;
	this->_ctr->maxHoursDaily=maxHoursSpinBox->value();

	QString newcs=this->_ctr->getDetailedDescription(gt.rules);
	gt.rules.addUndoPoint(tr("Modified the constraint:\n\n%1\ninto\n\n%2").arg(oldcs).arg(newcs));

	gt.rules.internalStructureComputed=false;
	setRulesModifiedAndOtherThings(&gt.rules);
	
	this->close();
}

void ModifyConstraintStudentsActivityTagMaxHoursDailyForm::cancel()
{
	this->close();
}
