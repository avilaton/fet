/***************************************************************************
                          addconstraintteacherhomeroomform.cpp  -  description
                             -------------------
    begin                : 8 Apr 2005
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

#include "addconstraintteacherhomeroomform.h"

AddConstraintTeacherHomeRoomForm::AddConstraintTeacherHomeRoomForm(QWidget* parent): QDialog(parent)
{
	setupUi(this);

	addConstraintPushButton->setDefault(true);

	connect(closePushButton, &QPushButton::clicked, this, &AddConstraintTeacherHomeRoomForm::close);
	connect(addConstraintPushButton, &QPushButton::clicked, this, &AddConstraintTeacherHomeRoomForm::addConstraint);

	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);

	QSize tmp1=teachersComboBox->minimumSizeHint();
	Q_UNUSED(tmp1);
	
	QSize tmp5=roomsComboBox->minimumSizeHint();
	Q_UNUSED(tmp5);
	
	updateTeachersComboBox();
	updateRoomsComboBox();
}

AddConstraintTeacherHomeRoomForm::~AddConstraintTeacherHomeRoomForm()
{
	saveFETDialogGeometry(this);
}

void AddConstraintTeacherHomeRoomForm::updateTeachersComboBox(){
	teachersComboBox->clear();
	for(Teacher* tch : std::as_const(gt.rules.teachersList))
		teachersComboBox->addItem(tch->name);
}

void AddConstraintTeacherHomeRoomForm::updateRoomsComboBox()
{
	roomsComboBox->clear();
	for(int i=0; i<gt.rules.roomsList.size(); i++){
		Room* rm=gt.rules.roomsList[i];
		roomsComboBox->addItem(rm->name);
	}
}

void AddConstraintTeacherHomeRoomForm::addConstraint()
{
	SpaceConstraint *ctr=nullptr;

	double weight;
	QString tmp=weightLineEdit->text();
	weight_sscanf(tmp, "%lf", &weight);
	if(weight<0.0 || weight>100){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight"));
		return;
	}

	QString teacher=teachersComboBox->currentText();
	assert(gt.rules.searchTeacher(teacher)>=0);

	int i=roomsComboBox->currentIndex();
	if(i<0 || roomsComboBox->count()<=0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid room"));
		return;
	}
	QString room=roomsComboBox->currentText();

	ctr=new ConstraintTeacherHomeRoom(weight, teacher, room);

	bool tmp2=gt.rules.addSpaceConstraint(ctr);
	if(tmp2){
		QString s=tr("Constraint added:");
		s+="\n\n";
		s+=ctr->getDetailedDescription(gt.rules);
		LongTextMessageBox::information(this, tr("FET information"), s);

		gt.rules.addUndoPoint(tr("Added the constraint:\n\n%1").arg(ctr->getDetailedDescription(gt.rules)));
	}
	else{
		QMessageBox::warning(this, tr("FET information"),
			tr("Constraint NOT added - error?"));
		delete ctr;
	}
}
