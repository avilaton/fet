/***************************************************************************
                          modifyconstraintteacherroomnotavailabletimesform.cpp  -  description
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

#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QBrush>
#include <QColor>
#include <QPalette>

#include "modifyconstraintteacherroomnotavailabletimesform.h"

#define YES		(QString("X"))
#define NO		(QString(" "))

ModifyConstraintTeacherRoomNotAvailableTimesForm::ModifyConstraintTeacherRoomNotAvailableTimesForm(QWidget* parent, ConstraintTeacherRoomNotAvailableTimes* ctr): QDialog(parent)
{
	setupUi(this);

	okPushButton->setDefault(true);

	connect(okPushButton, &QPushButton::clicked, this, &ModifyConstraintTeacherRoomNotAvailableTimesForm::ok);
	connect(cancelPushButton, &QPushButton::clicked, this, &ModifyConstraintTeacherRoomNotAvailableTimesForm::cancel);
	connect(notAllowedTimesTable, &QTableWidget::itemClicked, this, &ModifyConstraintTeacherRoomNotAvailableTimesForm::itemClicked);
	connect(setAllAvailablePushButton, &QPushButton::clicked, this, &ModifyConstraintTeacherRoomNotAvailableTimesForm::setAllAvailable);
	connect(setAllNotAvailablePushButton, &QPushButton::clicked, this, &ModifyConstraintTeacherRoomNotAvailableTimesForm::setAllNotAvailable);

	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);

	QSize tmp5=roomsComboBox->minimumSizeHint();
	Q_UNUSED(tmp5);
	
	QSize tmp6=teachersComboBox->minimumSizeHint();
	Q_UNUSED(tmp6);
	
	this->_ctr=ctr;
	
	weightLineEdit->setText(CustomFETString::number(ctr->weightPercentage));

	updateTeachersComboBox();
	updateRoomsComboBox();

	notAllowedTimesTable->setRowCount(gt.rules.nHoursPerDay);
	notAllowedTimesTable->setColumnCount(gt.rules.nDaysPerWeek);

	for(int j=0; j<gt.rules.nDaysPerWeek; j++){
		QTableWidgetItem* item=new QTableWidgetItem(gt.rules.daysOfTheWeek[j]);
		notAllowedTimesTable->setHorizontalHeaderItem(j, item);
	}
	for(int i=0; i<gt.rules.nHoursPerDay; i++){
		QTableWidgetItem* item=new QTableWidgetItem(gt.rules.hoursOfTheDay[i]);
		notAllowedTimesTable->setVerticalHeaderItem(i, item);
	}

	Matrix2D<bool> currentMatrix;
	currentMatrix.resize(gt.rules.nHoursPerDay, gt.rules.nDaysPerWeek);
	
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++)
			currentMatrix[i][j]=false;
	assert(ctr->days.count()==ctr->hours.count());
	for(int k=0; k<ctr->days.count(); k++){
		if(ctr->hours.at(k)==-1 || ctr->days.at(k)==-1)
			assert(0);
		int i=ctr->hours.at(k);
		int j=ctr->days.at(k);
		if(i>=0 && i<gt.rules.nHoursPerDay && j>=0 && j<gt.rules.nDaysPerWeek)
			currentMatrix[i][j]=true;
	}

	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++){
			QTableWidgetItem* item= new QTableWidgetItem();
			item->setTextAlignment(Qt::AlignCenter);
			item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
			if(SHOW_TOOLTIPS_FOR_CONSTRAINTS_WITH_TABLES)
				item->setToolTip(gt.rules.daysOfTheWeek[j]+QString("\n")+gt.rules.hoursOfTheDay[i]);
			notAllowedTimesTable->setItem(i, j, item);

			if(!currentMatrix[i][j])
				item->setText(NO);
			else
				item->setText(YES);
				
			colorItem(item);
		}
		
	notAllowedTimesTable->resizeRowsToContents();

	connect(notAllowedTimesTable->horizontalHeader(), &QHeaderView::sectionClicked, this, &ModifyConstraintTeacherRoomNotAvailableTimesForm::horizontalHeaderClicked);
	connect(notAllowedTimesTable->verticalHeader(), &QHeaderView::sectionClicked, this, &ModifyConstraintTeacherRoomNotAvailableTimesForm::verticalHeaderClicked);

	notAllowedTimesTable->setSelectionMode(QAbstractItemView::NoSelection);

	setStretchAvailabilityTableNicely(notAllowedTimesTable);

	connect(notAllowedTimesTable, &QTableWidget::cellEntered, this, &ModifyConstraintTeacherRoomNotAvailableTimesForm::cellEntered);
	notAllowedTimesTable->setMouseTracking(true);
}

ModifyConstraintTeacherRoomNotAvailableTimesForm::~ModifyConstraintTeacherRoomNotAvailableTimesForm()
{
	saveFETDialogGeometry(this);
}

void ModifyConstraintTeacherRoomNotAvailableTimesForm::colorItem(QTableWidgetItem* item)
{
	if(USE_GUI_COLORS){
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
		if(item->text()==NO)
			item->setBackground(QBrush(QColorConstants::DarkGreen));
		else
			item->setBackground(QBrush(QColorConstants::DarkRed));
		item->setForeground(QBrush(QColorConstants::LightGray));
#else
		if(item->text()==NO)
			item->setBackground(QBrush(Qt::darkGreen));
		else
			item->setBackground(QBrush(Qt::darkRed));
		item->setForeground(QBrush(Qt::lightGray));
#endif
	}
}

void ModifyConstraintTeacherRoomNotAvailableTimesForm::horizontalHeaderClicked(int col)
{
	highlightOnHorizontalHeaderClicked(notAllowedTimesTable, col);

	if(col>=0 && col<gt.rules.nDaysPerWeek){
		QString s=notAllowedTimesTable->item(0, col)->text();
		if(s==YES)
			s=NO;
		else{
			assert(s==NO);
			s=YES;
		}

		for(int row=0; row<gt.rules.nHoursPerDay; row++){
			notAllowedTimesTable->item(row, col)->setText(s);
			colorItem(notAllowedTimesTable->item(row,col));
		}
	}
}

void ModifyConstraintTeacherRoomNotAvailableTimesForm::verticalHeaderClicked(int row)
{
	highlightOnVerticalHeaderClicked(notAllowedTimesTable, row);

	if(row>=0 && row<gt.rules.nHoursPerDay){
		QString s=notAllowedTimesTable->item(row, 0)->text();
		if(s==YES)
			s=NO;
		else{
			assert(s==NO);
			s=YES;
		}
	
		for(int col=0; col<gt.rules.nDaysPerWeek; col++){
			notAllowedTimesTable->item(row, col)->setText(s);
			colorItem(notAllowedTimesTable->item(row,col));
		}
	}
}

void ModifyConstraintTeacherRoomNotAvailableTimesForm::cellEntered(int row, int col)
{
	highlightOnCellEntered(notAllowedTimesTable, row, col);
}

void ModifyConstraintTeacherRoomNotAvailableTimesForm::setAllAvailable()
{
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++){
			notAllowedTimesTable->item(i, j)->setText(NO);
			colorItem(notAllowedTimesTable->item(i,j));
		}
}

void ModifyConstraintTeacherRoomNotAvailableTimesForm::setAllNotAvailable()
{
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++){
			notAllowedTimesTable->item(i, j)->setText(YES);
			colorItem(notAllowedTimesTable->item(i,j));
		}
}

void ModifyConstraintTeacherRoomNotAvailableTimesForm::updateTeachersComboBox()
{
	int i=0, j=-1;
	teachersComboBox->clear();
	for(int k=0; k<gt.rules.teachersList.size(); k++){
		Teacher* teacher=gt.rules.teachersList[k];
		teachersComboBox->addItem(teacher->name);
		if(teacher->name==this->_ctr->teacherName)
			j=i;
		i++;
	}
	assert(j>=0);
	teachersComboBox->setCurrentIndex(j);
}

void ModifyConstraintTeacherRoomNotAvailableTimesForm::updateRoomsComboBox()
{
	int i=0, j=-1;
	roomsComboBox->clear();
	for(int k=0; k<gt.rules.roomsList.size(); k++){
		Room* room=gt.rules.roomsList[k];
		//roomsComboBox->addItem(room->getDescription());
		roomsComboBox->addItem(room->name);
		if(room->name==this->_ctr->room)
			j=i;
		i++;
	}
	assert(j>=0);
	roomsComboBox->setCurrentIndex(j);
}

void ModifyConstraintTeacherRoomNotAvailableTimesForm::itemClicked(QTableWidgetItem* item)
{
	QString s=item->text();
	if(s==YES)
		s=NO;
	else{
		assert(s==NO);
		s=YES;
	}
	item->setText(s);
	colorItem(item);
}

void ModifyConstraintTeacherRoomNotAvailableTimesForm::cancel()
{
	this->close();
}

void ModifyConstraintTeacherRoomNotAvailableTimesForm::ok()
{
	double weight;
	QString tmp=weightLineEdit->text();
	weight_sscanf(tmp, "%lf", &weight);
	if(weight<0.0 || weight>100){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight"));
		return;
	}

	int j=teachersComboBox->currentIndex();
	if(j<0 || teachersComboBox->count()<=0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid teacher"));
		return;
	}
	
	Teacher* teacher=gt.rules.teachersList.at(j);

	int i=roomsComboBox->currentIndex();
	if(i<0 || roomsComboBox->count()<=0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid room"));
		return;
	}
	
	Room* room=gt.rules.roomsList.at(i);

	QString oldcs=this->_ctr->getDetailedDescription(gt.rules);

	this->_ctr->weightPercentage=weight;
	this->_ctr->teacherName=teacher->name;
	this->_ctr->room=room->name;

	QList<int> days;
	QList<int> hours;
	for(int j=0; j<gt.rules.nDaysPerWeek; j++)
		for(int i=0; i<gt.rules.nHoursPerDay; i++)
			if(notAllowedTimesTable->item(i, j)->text()==YES){
				days.append(j);
				hours.append(i);
			}

	this->_ctr->days=days;
	this->_ctr->hours=hours;

	QString newcs=this->_ctr->getDetailedDescription(gt.rules);
	gt.rules.addUndoPoint(tr("Modified the constraint:\n\n%1\ninto\n\n%2").arg(oldcs).arg(newcs));

	gt.rules.internalStructureComputed=false;
	setRulesModifiedAndOtherThings(&gt.rules);
	
	this->close();
}
