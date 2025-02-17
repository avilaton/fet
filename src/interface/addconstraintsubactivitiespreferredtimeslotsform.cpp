/***************************************************************************
                          addconstraintsubactivitiespreferredtimeslotsform.cpp  -  description
                             -------------------
    begin                : 2008
    copyright            : (C) 2008 by Liviu Lalescu
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

#include <Qt>

#include <QMessageBox>

#include "longtextmessagebox.h"

#include "addconstraintsubactivitiespreferredtimeslotsform.h"
#include "timeconstraint.h"

#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QBrush>
#include <QColor>
#include <QPalette>

#define YES		(QString(" "))
#define NO		(QString("X"))

AddConstraintSubactivitiesPreferredTimeSlotsForm::AddConstraintSubactivitiesPreferredTimeSlotsForm(QWidget* parent): QDialog(parent)
{
	setupUi(this);

	durationCheckBox->setChecked(false);
	durationSpinBox->setEnabled(false);
	durationSpinBox->setMinimum(1);
	durationSpinBox->setMaximum(gt.rules.nHoursPerDay);
	durationSpinBox->setValue(1);

	addConstraintPushButton->setDefault(true);

	connect(preferredTimesTable, &QTableWidget::itemClicked, this, &AddConstraintSubactivitiesPreferredTimeSlotsForm::itemClicked);
	connect(addConstraintPushButton, &QPushButton::clicked, this, &AddConstraintSubactivitiesPreferredTimeSlotsForm::addConstraint);
	connect(closePushButton, &QPushButton::clicked, this, &AddConstraintSubactivitiesPreferredTimeSlotsForm::close);
	connect(setAllAllowedPushButton, &QPushButton::clicked, this, &AddConstraintSubactivitiesPreferredTimeSlotsForm::setAllSlotsAllowed);
	connect(setAllNotAllowedPushButton, &QPushButton::clicked, this, &AddConstraintSubactivitiesPreferredTimeSlotsForm::setAllSlotsNotAllowed);

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

	updateTeachersComboBox();
	updateStudentsComboBox();
	updateSubjectsComboBox();
	updateActivityTagsComboBox();

	preferredTimesTable->setRowCount(gt.rules.nHoursPerDay);
	preferredTimesTable->setColumnCount(gt.rules.nDaysPerWeek);

	for(int j=0; j<gt.rules.nDaysPerWeek; j++){
		QTableWidgetItem* item=new QTableWidgetItem(gt.rules.daysOfTheWeek[j]);
		preferredTimesTable->setHorizontalHeaderItem(j, item);
	}
	for(int i=0; i<gt.rules.nHoursPerDay; i++){
		QTableWidgetItem* item=new QTableWidgetItem(gt.rules.hoursOfTheDay[i]);
		preferredTimesTable->setVerticalHeaderItem(i, item);
	}

	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++){
			QTableWidgetItem* item=new QTableWidgetItem(YES);
			item->setTextAlignment(Qt::AlignCenter);
			item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
			colorItem(item);
			if(SHOW_TOOLTIPS_FOR_CONSTRAINTS_WITH_TABLES)
				item->setToolTip(gt.rules.daysOfTheWeek[j]+QString("\n")+gt.rules.hoursOfTheDay[i]);
			preferredTimesTable->setItem(i, j, item);
		}
		
	preferredTimesTable->resizeRowsToContents();

	connect(preferredTimesTable->horizontalHeader(), &QHeaderView::sectionClicked, this, &AddConstraintSubactivitiesPreferredTimeSlotsForm::horizontalHeaderClicked);
	connect(preferredTimesTable->verticalHeader(), &QHeaderView::sectionClicked, this, &AddConstraintSubactivitiesPreferredTimeSlotsForm::verticalHeaderClicked);

	preferredTimesTable->setSelectionMode(QAbstractItemView::NoSelection);
	
	componentNumberSpinBox->setMinimum(1);
	componentNumberSpinBox->setMaximum(MAX_SPLIT_OF_AN_ACTIVITY);
	componentNumberSpinBox->setValue(1);

	setStretchAvailabilityTableNicely(preferredTimesTable);

	connect(preferredTimesTable, &QTableWidget::cellEntered, this, &AddConstraintSubactivitiesPreferredTimeSlotsForm::cellEntered);
	preferredTimesTable->setMouseTracking(true);

	connect(durationCheckBox, &QCheckBox::toggled, this, &AddConstraintSubactivitiesPreferredTimeSlotsForm::durationCheckBox_toggled);
	
	durationCheckBox_toggled();
}

AddConstraintSubactivitiesPreferredTimeSlotsForm::~AddConstraintSubactivitiesPreferredTimeSlotsForm()
{
	saveFETDialogGeometry(this);
}

void AddConstraintSubactivitiesPreferredTimeSlotsForm::colorItem(QTableWidgetItem* item)
{
	if(USE_GUI_COLORS){
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
		if(item->text()==YES)
			item->setBackground(QBrush(QColorConstants::DarkGreen));
		else
			item->setBackground(QBrush(QColorConstants::DarkRed));
		item->setForeground(QBrush(QColorConstants::LightGray));
#else
		if(item->text()==YES)
			item->setBackground(QBrush(Qt::darkGreen));
		else
			item->setBackground(QBrush(Qt::darkRed));
		item->setForeground(QBrush(Qt::lightGray));
#endif
	}
}

void AddConstraintSubactivitiesPreferredTimeSlotsForm::horizontalHeaderClicked(int col)
{
	highlightOnHorizontalHeaderClicked(preferredTimesTable, col);

	if(col>=0 && col<gt.rules.nDaysPerWeek){
		QString s=preferredTimesTable->item(0, col)->text();
		if(s==YES)
			s=NO;
		else{
			assert(s==NO);
			s=YES;
		}

		for(int row=0; row<gt.rules.nHoursPerDay; row++){
			preferredTimesTable->item(row, col)->setText(s);
			colorItem(preferredTimesTable->item(row,col));
		}
	}
}

void AddConstraintSubactivitiesPreferredTimeSlotsForm::verticalHeaderClicked(int row)
{
	highlightOnVerticalHeaderClicked(preferredTimesTable, row);

	if(row>=0 && row<gt.rules.nHoursPerDay){
		QString s=preferredTimesTable->item(row, 0)->text();
		if(s==YES)
			s=NO;
		else{
			assert(s==NO);
			s=YES;
		}
	
		for(int col=0; col<gt.rules.nDaysPerWeek; col++){
			preferredTimesTable->item(row, col)->setText(s);
			colorItem(preferredTimesTable->item(row,col));
		}
	}
}

void AddConstraintSubactivitiesPreferredTimeSlotsForm::cellEntered(int row, int col)
{
	highlightOnCellEntered(preferredTimesTable, row, col);
}

void AddConstraintSubactivitiesPreferredTimeSlotsForm::setAllSlotsAllowed()
{
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++){
			preferredTimesTable->item(i, j)->setText(YES);
			colorItem(preferredTimesTable->item(i,j));
		}
}

void AddConstraintSubactivitiesPreferredTimeSlotsForm::setAllSlotsNotAllowed()
{
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++){
			preferredTimesTable->item(i, j)->setText(NO);
			colorItem(preferredTimesTable->item(i,j));
		}
}

void AddConstraintSubactivitiesPreferredTimeSlotsForm::itemClicked(QTableWidgetItem* item)
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

void AddConstraintSubactivitiesPreferredTimeSlotsForm::updateTeachersComboBox(){
	teachersComboBox->clear();
	teachersComboBox->addItem("");
	for(int i=0; i<gt.rules.teachersList.size(); i++){
		Teacher* t=gt.rules.teachersList[i];
		teachersComboBox->addItem(t->name);
	}
}

void AddConstraintSubactivitiesPreferredTimeSlotsForm::updateStudentsComboBox(){
	populateStudentsComboBox(studentsComboBox, QString(""), true);
}

void AddConstraintSubactivitiesPreferredTimeSlotsForm::updateSubjectsComboBox(){
	subjectsComboBox->clear();
	subjectsComboBox->addItem("");
	for(int i=0; i<gt.rules.subjectsList.size(); i++){
		Subject* s=gt.rules.subjectsList[i];
		subjectsComboBox->addItem(s->name);
	}
}

void AddConstraintSubactivitiesPreferredTimeSlotsForm::updateActivityTagsComboBox(){
	activityTagsComboBox->clear();
	activityTagsComboBox->addItem("");
	for(int i=0; i<gt.rules.activityTagsList.size(); i++){
		ActivityTag* s=gt.rules.activityTagsList[i];
		activityTagsComboBox->addItem(s->name);
	}
}

void AddConstraintSubactivitiesPreferredTimeSlotsForm::addConstraint()
{
	TimeConstraint *ctr=nullptr;

	int duration=-1;
	if(durationCheckBox->isChecked()){
		assert(durationSpinBox->isEnabled());
		duration=durationSpinBox->value();
	}

	double weight;
	QString tmp=weightLineEdit->text();
	weight_sscanf(tmp, "%lf", &weight);
	if(weight<0.0 || weight>100.0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight (percentage)"));
		return;
	}

	QString teacher=teachersComboBox->currentText();
	if(teacher!="")
		assert(gt.rules.searchTeacher(teacher)>=0);

	QString students=studentsComboBox->currentText();
	if(students!="")
		assert(gt.rules.searchStudentsSet(students)!=nullptr);

	QString subject=subjectsComboBox->currentText();
	if(subject!="")
		assert(gt.rules.searchSubject(subject)>=0);
		
	QString activityTag=activityTagsComboBox->currentText();
	if(activityTag!="")
		assert(gt.rules.searchActivityTag(activityTag)>=0);
	
	QList<int> days_L;
	QList<int> hours_L;
	int n=0;
	for(int j=0; j<gt.rules.nDaysPerWeek; j++)
		for(int i=0; i<gt.rules.nHoursPerDay; i++)
			if(preferredTimesTable->item(i, j)->text()==YES){
				days_L.append(j);
				hours_L.append(i);
				n++;
			}

	if(n<=0){
		int t=QMessageBox::question(this, tr("FET question"),
		 tr("Warning: 0 slots selected. Are you sure?"),
		 QMessageBox::Yes, QMessageBox::Cancel);
		
		if(t==QMessageBox::Cancel)
				return;
	}

	ctr=new ConstraintSubactivitiesPreferredTimeSlots(weight, componentNumberSpinBox->value(), teacher, students, subject, activityTag, duration, n, days_L, hours_L);

	bool tmp2=gt.rules.addTimeConstraint(ctr);
	if(tmp2){
		QString s=tr("Constraint added:");
		s+="\n\n";
		s+=ctr->getDetailedDescription(gt.rules);
		LongTextMessageBox::information(this, tr("FET information"), s);

		gt.rules.addUndoPoint(tr("Added the constraint:\n\n%1").arg(ctr->getDetailedDescription(gt.rules)));
	}
	else{
		QMessageBox::warning(this, tr("FET information"),
			tr("Constraint NOT added - duplicate?", "'Duplicate' means that the constraint is identical to an existing constraint and cannot be added."));
		delete ctr;
	}
}

void AddConstraintSubactivitiesPreferredTimeSlotsForm::durationCheckBox_toggled()
{
	durationSpinBox->setEnabled(durationCheckBox->isChecked());
}

#undef YES
#undef NO
