/***************************************************************************
                          addconstraintstudentsmaxroomchangesperrealdayform.h  -  description
                             -------------------
    begin                : 2021
    copyright            : (C) 2021 by Liviu Lalescu
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

#ifndef ADDCONSTRAINTSTUDENTSMAXROOMCHANGESPERREALDAYFORM_H
#define ADDCONSTRAINTSTUDENTSMAXROOMCHANGESPERREALDAYFORM_H

#include "ui_addconstraintstudentsmaxroomchangesperrealdayform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class AddConstraintStudentsMaxRoomChangesPerRealDayForm : public QDialog, Ui::AddConstraintStudentsMaxRoomChangesPerRealDayForm_template  {
	Q_OBJECT
public:
	AddConstraintStudentsMaxRoomChangesPerRealDayForm(QWidget* parent);
	~AddConstraintStudentsMaxRoomChangesPerRealDayForm();

public slots:
	void addCurrentConstraint();
};

#endif
