/***************************************************************************
                          constraintstudentsmaxhourscontinuouslyform.h  -  description
                             -------------------
    begin                : July 19, 2007
    copyright            : (C) 2007 by Liviu Lalescu
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

#ifndef CONSTRAINTSTUDENTSMAXHOURSCONTINUOUSLYFORM_H
#define CONSTRAINTSTUDENTSMAXHOURSCONTINUOUSLYFORM_H

#include "ui_constraintstudentsmaxhourscontinuouslyform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class ConstraintStudentsMaxHoursContinuouslyForm : public QDialog, Ui::ConstraintStudentsMaxHoursContinuouslyForm_template  {
	Q_OBJECT
public:
	TimeConstraintsList visibleConstraintsList;

	ConstraintStudentsMaxHoursContinuouslyForm(QWidget* parent);
	~ConstraintStudentsMaxHoursContinuouslyForm();

	bool filterOk(TimeConstraint* ctr);

public slots:
	void constraintChanged(int index);
	void addConstraint();
	void modifyConstraint();
	void removeConstraint();

	void filterChanged();
};

#endif
