/***************************************************************************
                          addconstraintstudentsmaxgapsperdayform.h  -  description
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

#ifndef ADDCONSTRAINTSTUDENTSMAXGAPSPERDAYFORM_H
#define ADDCONSTRAINTSTUDENTSMAXGAPSPERDAYFORM_H

#include "ui_addconstraintstudentsmaxgapsperdayform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class AddConstraintStudentsMaxGapsPerDayForm : public QDialog, Ui::AddConstraintStudentsMaxGapsPerDayForm_template  {
	Q_OBJECT
public:
	AddConstraintStudentsMaxGapsPerDayForm(QWidget* parent);
	~AddConstraintStudentsMaxGapsPerDayForm();

public slots:
	void addCurrentConstraint();
};

#endif
