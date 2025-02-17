/***************************************************************************
                          constraintactivitiessamestartinghourform.h  -  description
                             -------------------
    begin                : 15 Feb 2005
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

#ifndef CONSTRAINTACTIVITIESSAMESTARTINGHOURFORM_H
#define CONSTRAINTACTIVITIESSAMESTARTINGHOURFORM_H

#include "ui_constraintactivitiessamestartinghourform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class ConstraintActivitiesSameStartingHourForm : public QDialog, Ui::ConstraintActivitiesSameStartingHourForm_template  {
	Q_OBJECT
public:
	TimeConstraintsList visibleConstraintsList;

	ConstraintActivitiesSameStartingHourForm(QWidget* parent);
	~ConstraintActivitiesSameStartingHourForm();
	
	bool filterOk(TimeConstraint* ctr);

public slots:
	void constraintChanged(int index);
	void addConstraint();
	void removeConstraint();
	void modifyConstraint();

	void filterChanged();
	
	void addRemoveMultipleConstraints();
};

#endif
