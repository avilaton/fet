/***************************************************************************
                          modifyconstraintactivitytagpreferredroomform.cpp  -  description
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

#include "modifyconstraintactivitytagpreferredroomform.h"

ModifyConstraintActivityTagPreferredRoomForm::ModifyConstraintActivityTagPreferredRoomForm(QWidget* parent, ConstraintActivityTagPreferredRoom* ctr): QDialog(parent)
{
	setupUi(this);

	okPushButton->setDefault(true);

	connect(okPushButton, &QPushButton::clicked, this, &ModifyConstraintActivityTagPreferredRoomForm::ok);
	connect(cancelPushButton, &QPushButton::clicked, this, &ModifyConstraintActivityTagPreferredRoomForm::cancel);

	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);

	QSize tmp4=activityTagsComboBox->minimumSizeHint();
	Q_UNUSED(tmp4);
	
	QSize tmp5=roomsComboBox->minimumSizeHint();
	Q_UNUSED(tmp5);
	
	this->_ctr=ctr;
	
	weightLineEdit->setText(CustomFETString::number(ctr->weightPercentage));

	updateActivityTagsComboBox();
	updateRoomsComboBox();
}

ModifyConstraintActivityTagPreferredRoomForm::~ModifyConstraintActivityTagPreferredRoomForm()
{
	saveFETDialogGeometry(this);
}

void ModifyConstraintActivityTagPreferredRoomForm::updateActivityTagsComboBox()
{
	int i=0, j=-1;
	activityTagsComboBox->clear();
	for(int k=0; k<gt.rules.activityTagsList.size(); k++){
		ActivityTag* sb=gt.rules.activityTagsList[k];
		activityTagsComboBox->addItem(sb->name);
		if(sb->name==this->_ctr->activityTagName)
			j=i;
		i++;
	}
	assert(j>=0);
	activityTagsComboBox->setCurrentIndex(j);
}

void ModifyConstraintActivityTagPreferredRoomForm::updateRoomsComboBox()
{
	int i=0, j=-1;
	roomsComboBox->clear();
	for(int k=0; k<gt.rules.roomsList.size(); k++){
		Room* rm=gt.rules.roomsList[k];
		roomsComboBox->addItem(rm->name);
		if(rm->name==this->_ctr->roomName)
			j=i;
		i++;
	}
	assert(j>=0);
	roomsComboBox->setCurrentIndex(j);
}

void ModifyConstraintActivityTagPreferredRoomForm::cancel()
{
	this->close();
}

void ModifyConstraintActivityTagPreferredRoomForm::ok()
{
	double weight;
	QString tmp=weightLineEdit->text();
	weight_sscanf(tmp, "%lf", &weight);
	if(weight<0.0 || weight>100){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight"));
		return;
	}

	int i=activityTagsComboBox->currentIndex();
	if(i<0 || activityTagsComboBox->count()<=0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid activity tag"));
		return;
	}

	QString activityTag=activityTagsComboBox->currentText();

	i=roomsComboBox->currentIndex();
	if(i<0 || roomsComboBox->count()<=0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid room"));
		return;
	}
	QString room=roomsComboBox->currentText();

	QString oldcs=this->_ctr->getDetailedDescription(gt.rules);

	this->_ctr->weightPercentage=weight;
	this->_ctr->roomName=room;
	this->_ctr->activityTagName=activityTag;

	QString newcs=this->_ctr->getDetailedDescription(gt.rules);
	gt.rules.addUndoPoint(tr("Modified the constraint:\n\n%1\ninto\n\n%2").arg(oldcs).arg(newcs));

	gt.rules.internalStructureComputed=false;
	setRulesModifiedAndOtherThings(&gt.rules);
	
	this->close();
}
