/***************************************************************************
                          addconstraintmaxtermsbetweenactivitiesform.cpp  -  description
                             -------------------
    begin                : 2022
    copyright            : (C) 2022 by Liviu Lalescu
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

#include <QList>

#include <QMessageBox>

#include "longtextmessagebox.h"

#include "addconstraintmaxtermsbetweenactivitiesform.h"

#include <QListWidget>
#include <QAbstractItemView>
#include <QScrollBar>

AddConstraintMaxTermsBetweenActivitiesForm::AddConstraintMaxTermsBetweenActivitiesForm(QWidget* parent): QDialog(parent)
{
	setupUi(this);

	addConstraintPushButton->setDefault(true);
	
	allActivitiesListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	selectedActivitiesListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(closePushButton, &QPushButton::clicked, this, &AddConstraintMaxTermsBetweenActivitiesForm::close);
	connect(addConstraintPushButton, &QPushButton::clicked, this, &AddConstraintMaxTermsBetweenActivitiesForm::addConstraint);
	connect(allActivitiesListWidget, &QListWidget::itemDoubleClicked, this, &AddConstraintMaxTermsBetweenActivitiesForm::addActivity);
	connect(addAllActivitiesPushButton, &QPushButton::clicked, this, &AddConstraintMaxTermsBetweenActivitiesForm::addAllActivities);
	connect(selectedActivitiesListWidget, &QListWidget::itemDoubleClicked, this, &AddConstraintMaxTermsBetweenActivitiesForm::removeActivity);
	connect(clearPushButton, &QPushButton::clicked, this, &AddConstraintMaxTermsBetweenActivitiesForm::clear);

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

	maxTermsSpinBox->setMinimum(0);
	maxTermsSpinBox->setValue(gt.rules.nTerms-1);
	maxTermsSpinBox->setMaximum(gt.rules.nTerms-1);

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
	this->selectedActivitiesList.clear();

	filterChanged();

	connect(teachersComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &AddConstraintMaxTermsBetweenActivitiesForm::filterChanged);
	connect(studentsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &AddConstraintMaxTermsBetweenActivitiesForm::filterChanged);
	connect(subjectsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &AddConstraintMaxTermsBetweenActivitiesForm::filterChanged);
	connect(activityTagsComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &AddConstraintMaxTermsBetweenActivitiesForm::filterChanged);
}

AddConstraintMaxTermsBetweenActivitiesForm::~AddConstraintMaxTermsBetweenActivitiesForm()
{
	saveFETDialogGeometry(this);
}

bool AddConstraintMaxTermsBetweenActivitiesForm::filterOk(Activity* act)
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

void AddConstraintMaxTermsBetweenActivitiesForm::filterChanged()
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

void AddConstraintMaxTermsBetweenActivitiesForm::addConstraint()
{
	TimeConstraint *ctr=nullptr;

	double weight;
	QString tmp=weightLineEdit->text();
	weight_sscanf(tmp, "%lf", &weight);
	if(weight<0.0 || weight>100){
		QMessageBox::warning(this, tr("FET information"),
			tr("Invalid weight (percentage)"));
		return;
	}

	if(this->selectedActivitiesList.count()==0){
		QMessageBox::warning(this, tr("FET information"),
			tr("Empty list of selected activities"));
		return;
	}
	if(this->selectedActivitiesList.count()==1){
		QMessageBox::warning(this, tr("FET information"),
			tr("Only one selected activity"));
		return;
	}

	QList<int> ids;
	QList<int>::const_iterator it;
	for(it=this->selectedActivitiesList.constBegin(); it!=this->selectedActivitiesList.constEnd(); it++)
		ids.append(*it);
	
	ctr=new ConstraintMaxTermsBetweenActivities(weight, this->selectedActivitiesList.count(), ids, maxTermsSpinBox->value());

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
			tr("Constraint NOT added - please report error"));
		delete ctr;
	}
}

void AddConstraintMaxTermsBetweenActivitiesForm::addActivity()
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

void AddConstraintMaxTermsBetweenActivitiesForm::addAllActivities()
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

void AddConstraintMaxTermsBetweenActivitiesForm::removeActivity()
{
	if(selectedActivitiesListWidget->currentRow()<0 || selectedActivitiesListWidget->count()<=0)
		return;
	int tmp=selectedActivitiesListWidget->currentRow();
	
	this->selectedActivitiesList.removeAt(tmp);
	
	selectedActivitiesListWidget->setCurrentRow(-1);
	QListWidgetItem* item=selectedActivitiesListWidget->takeItem(tmp);
	delete item;
	if(tmp<selectedActivitiesListWidget->count())
		selectedActivitiesListWidget->setCurrentRow(tmp);
	else
		selectedActivitiesListWidget->setCurrentRow(selectedActivitiesListWidget->count()-1);
}

void AddConstraintMaxTermsBetweenActivitiesForm::clear()
{
	selectedActivitiesListWidget->clear();
	selectedActivitiesList.clear();
}
