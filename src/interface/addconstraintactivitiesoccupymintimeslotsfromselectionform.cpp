/***************************************************************************
                          addconstraintactivitiesoccupymintimeslotsfromselectionform.cpp  -  description
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

#include <Qt>

#include <QMessageBox>

#include "longtextmessagebox.h"

#include "addconstraintactivitiesoccupymintimeslotsfromselectionform.h"
#include "timeconstraint.h"

#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QListWidget>
#include <QAbstractItemView>
#include <QScrollBar>

#include <QBrush>
#include <QColor>
#include <QPalette>

#define YES	(QString("X"))
#define NO	(QString(" "))

AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm(QWidget* parent): QDialog(parent)
{
	setupUi(this);

	addConstraintPushButton->setDefault(true);
	
	allActivitiesListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	selectedActivitiesListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	
	connect(addConstraintPushButton, &QPushButton::clicked, this, &AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::addCurrentConstraint);
	connect(closePushButton, &QPushButton::clicked, this, &AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::close);
	connect(selectedTimesTable, &QTableWidget::itemClicked, this, &AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::itemClicked);
	connect(setAllUnselectedPushButton, &QPushButton::clicked, this, &AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::setAllUnselected);
	connect(setAllSelectedPushButton, &QPushButton::clicked, this, &AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::setAllSelected);
	connect(allActivitiesListWidget, &QListWidget::itemDoubleClicked, this, &AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::addActivity);
	connect(addAllActivitiesPushButton, &QPushButton::clicked, this, &AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::addAllActivities);
	connect(selectedActivitiesListWidget, &QListWidget::itemDoubleClicked, this, &AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::removeActivity);
	connect(clearPushButton, &QPushButton::clicked, this, &AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::clear);
	
	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);
	
	tabWidget->setCurrentIndex(0);

	minOccupiedSpinBox->setMinimum(0);
	minOccupiedSpinBox->setMaximum(gt.rules.nDaysPerWeek*gt.rules.nHoursPerDay);
	minOccupiedSpinBox->setValue(0);

	selectedTimesTable->setRowCount(gt.rules.nHoursPerDay);
	selectedTimesTable->setColumnCount(gt.rules.nDaysPerWeek);

	for(int j=0; j<gt.rules.nDaysPerWeek; j++){
		QTableWidgetItem* item=new QTableWidgetItem(gt.rules.daysOfTheWeek[j]);
		selectedTimesTable->setHorizontalHeaderItem(j, item);
	}
	for(int i=0; i<gt.rules.nHoursPerDay; i++){
		QTableWidgetItem* item=new QTableWidgetItem(gt.rules.hoursOfTheDay[i]);
		selectedTimesTable->setVerticalHeaderItem(i, item);
	}

	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++){
			QTableWidgetItem* item=new QTableWidgetItem(NO);
			item->setTextAlignment(Qt::AlignCenter);
			item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			colorItem(item);
			if(SHOW_TOOLTIPS_FOR_CONSTRAINTS_WITH_TABLES)
				item->setToolTip(gt.rules.daysOfTheWeek[j]+QString("\n")+gt.rules.hoursOfTheDay[i]);
			selectedTimesTable->setItem(i, j, item);
		}
		
	selectedTimesTable->resizeRowsToContents();
	//selectedTimesTable->resizeColumnsToContents();

	connect(selectedTimesTable->horizontalHeader(), &QHeaderView::sectionClicked, this, &AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::horizontalHeaderClicked);
	connect(selectedTimesTable->verticalHeader(), &QHeaderView::sectionClicked, this, &AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::verticalHeaderClicked);

	selectedTimesTable->setSelectionMode(QAbstractItemView::NoSelection);
	
	setStretchAvailabilityTableNicely(selectedTimesTable);
	
	connect(selectedTimesTable, &QTableWidget::cellEntered, this, &AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::cellEntered);
	selectedTimesTable->setMouseTracking(true);

	//activities
	QSize tmp1=teachersComboBox->minimumSizeHint();
	Q_UNUSED(tmp1);
	QSize tmp2=studentsComboBox->minimumSizeHint();
	Q_UNUSED(tmp2);
	QSize tmp3=subjectsComboBox->minimumSizeHint();
	Q_UNUSED(tmp3);
	QSize tmp4=activityTagsComboBox->minimumSizeHint();
	Q_UNUSED(tmp4);

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

	selectedActivitiesListWidget->clear();
	selectedActivitiesList.clear();

	filterChanged();

	connect(teachersComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::filterChanged);
	connect(studentsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::filterChanged);
	connect(subjectsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::filterChanged);
	connect(activityTagsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::filterChanged);
}

AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::~AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm()
{
	saveFETDialogGeometry(this);
}

void AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::colorItem(QTableWidgetItem* item)
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

void AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::horizontalHeaderClicked(int col)
{
	highlightOnHorizontalHeaderClicked(selectedTimesTable, col);

	if(col>=0 && col<gt.rules.nDaysPerWeek){
		QString s=selectedTimesTable->item(0, col)->text();
		if(s==YES)
			s=NO;
		else{
			assert(s==NO);
			s=YES;
		}

		for(int row=0; row<gt.rules.nHoursPerDay; row++){
			selectedTimesTable->item(row, col)->setText(s);
			colorItem(selectedTimesTable->item(row,col));
		}
	}
}

void AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::verticalHeaderClicked(int row)
{
	highlightOnVerticalHeaderClicked(selectedTimesTable, row);

	if(row>=0 && row<gt.rules.nHoursPerDay){
		QString s=selectedTimesTable->item(row, 0)->text();
		if(s==YES)
			s=NO;
		else{
			assert(s==NO);
			s=YES;
		}
	
		for(int col=0; col<gt.rules.nDaysPerWeek; col++){
			selectedTimesTable->item(row, col)->setText(s);
			colorItem(selectedTimesTable->item(row,col));
		}
	}
}

void AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::cellEntered(int row, int col)
{
	highlightOnCellEntered(selectedTimesTable, row, col);
}

void AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::setAllUnselected()
{
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++){
			selectedTimesTable->item(i, j)->setText(NO);
			colorItem(selectedTimesTable->item(i,j));
		}
}

void AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::setAllSelected()
{
	for(int i=0; i<gt.rules.nHoursPerDay; i++)
		for(int j=0; j<gt.rules.nDaysPerWeek; j++){
			selectedTimesTable->item(i, j)->setText(YES);
			colorItem(selectedTimesTable->item(i,j));
		}
}

void AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::itemClicked(QTableWidgetItem* item)
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

void AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::addCurrentConstraint()
{
	TimeConstraint *ctr=nullptr;

	double weight;
	QString tmp=weightLineEdit->text();
	weight_sscanf(tmp, "%lf", &weight);
	if(weight<100.0 || weight>100.0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight (percentage). It has to be 100"));
		return;
	}

	QList<int> days;
	QList<int> hours;
	for(int j=0; j<gt.rules.nDaysPerWeek; j++)
		for(int i=0; i<gt.rules.nHoursPerDay; i++)
			if(selectedTimesTable->item(i, j)->text()==YES){
				days.append(j);
				hours.append(i);
			}
			
	int minOccupiedSlots=minOccupiedSpinBox->value();
	
	if(minOccupiedSlots==0){
		QMessageBox::warning(this, tr("FET information"), tr("You specified min occupied time slots to be 0. This results in "
		"a useless constraint. Please correct."));
		return;
	}
	
	if(this->selectedActivitiesList.count()==0){
		QMessageBox::warning(this, tr("FET information"),
		 tr("Empty list of activities"));
		return;
	}
	//we allow even only one activity
	/*if(this->selectedActivitiesList.count()==1){
		QMessageBox::warning(this, tr("FET information"),
		 tr("Only one selected activity"));
		return;
	}*/

	ctr=new ConstraintActivitiesOccupyMinTimeSlotsFromSelection(weight, selectedActivitiesList, days, hours, minOccupiedSlots);

	bool tmp2=gt.rules.addTimeConstraint(ctr);
	if(tmp2){
		LongTextMessageBox::information(this, tr("FET information"),
			tr("Constraint added:")+"\n\n"+ctr->getDetailedDescription(gt.rules));

		gt.rules.addUndoPoint(tr("Added the constraint:\n\n%1").arg(ctr->getDetailedDescription(gt.rules)));
	}
	else{
		QMessageBox::warning(this, tr("FET information"),
			tr("Constraint NOT added - please report error"));
		delete ctr;
	}
}

//activities
bool AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::filterOk(Activity* act)
{
	QString tn=teachersComboBox->currentText();
	QString stn=studentsComboBox->currentText();
	QString sbn=subjectsComboBox->currentText();
	QString atn=activityTagsComboBox->currentText();
	int ok=true;

	//teacher
	if(tn!=""){
		bool ok2=false;
		for(QStringList::const_iterator it=act->teachersNames.constBegin(); it!=act->teachersNames.constEnd(); it++)
			if(*it == tn){
				ok2=true;
				break;
			}
		if(!ok2)
			ok=false;
	}

	//subject
	if(sbn!="" && sbn!=act->subjectName)
		ok=false;
		
	//activity tag
	if(atn!="" && !act->activityTagsNames.contains(atn))
		ok=false;
		
	//students
	if(stn!=""){
		bool ok2=false;
		for(QStringList::const_iterator it=act->studentsNames.constBegin(); it!=act->studentsNames.constEnd(); it++)
			if(*it == stn){
				ok2=true;
				break;
			}
		if(!ok2)
			ok=false;
	}
	
	return ok;
}

void AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::filterChanged()
{
	allActivitiesListWidget->clear();
	this->activitiesList.clear();
	
	for(int i=0; i<gt.rules.activitiesList.size(); i++){
		Activity* ac=gt.rules.activitiesList[i];
		if(filterOk(ac)){
			allActivitiesListWidget->addItem(ac->getDescription(gt.rules));
			this->activitiesList.append(ac->id);
		}
	}
	
	int q=allActivitiesListWidget->verticalScrollBar()->minimum();
	allActivitiesListWidget->verticalScrollBar()->setValue(q);
}

void AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::addActivity()
{
	if(allActivitiesListWidget->currentRow()<0)
		return;
	int tmp=allActivitiesListWidget->currentRow();
	int _id=this->activitiesList.at(tmp);
	
	QString actName=allActivitiesListWidget->currentItem()->text();
	assert(actName!="");
	
	//duplicate?
	if(this->selectedActivitiesList.contains(_id))
		return;
	
	selectedActivitiesListWidget->addItem(actName);
	selectedActivitiesListWidget->setCurrentRow(selectedActivitiesListWidget->count()-1);

	this->selectedActivitiesList.append(_id);
}

void AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::addAllActivities()
{
	for(int tmp=0; tmp<allActivitiesListWidget->count(); tmp++){
		int _id=this->activitiesList.at(tmp);
	
		QString actName=allActivitiesListWidget->item(tmp)->text();
		assert(actName!="");
		
		if(this->selectedActivitiesList.contains(_id))
			continue;
		
		selectedActivitiesListWidget->addItem(actName);
		this->selectedActivitiesList.append(_id);
	}
	
	selectedActivitiesListWidget->setCurrentRow(selectedActivitiesListWidget->count()-1);
}

void AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::removeActivity()
{
	if(selectedActivitiesListWidget->currentRow()<0 || selectedActivitiesListWidget->count()<=0)
		return;
	int tmp=selectedActivitiesListWidget->currentRow();
	
	selectedActivitiesList.removeAt(tmp);

	selectedActivitiesListWidget->setCurrentRow(-1);
	QListWidgetItem* item=selectedActivitiesListWidget->takeItem(tmp);
	delete item;
	if(tmp<selectedActivitiesListWidget->count())
		selectedActivitiesListWidget->setCurrentRow(tmp);
	else
		selectedActivitiesListWidget->setCurrentRow(selectedActivitiesListWidget->count()-1);
}

void AddConstraintActivitiesOccupyMinTimeSlotsFromSelectionForm::clear()
{
	selectedActivitiesListWidget->clear();
	selectedActivitiesList.clear();
}
