/***************************************************************************
                          addconstraintteacherhomeroomform.h  -  description
                             -------------------
    begin                : April 8, 2005
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

#ifndef ADDCONSTRAINTTEACHERHOMEROOMFORM_H
#define ADDCONSTRAINTTEACHERHOMEROOMFORM_H

#include "ui_addconstraintteacherhomeroomform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class AddConstraintTeacherHomeRoomForm : public QDialog, Ui::AddConstraintTeacherHomeRoomForm_template  {
	Q_OBJECT
public:
	AddConstraintTeacherHomeRoomForm(QWidget* parent);
	~AddConstraintTeacherHomeRoomForm();

	void updateTeachersComboBox();
	void updateRoomsComboBox();

public slots:
	void addConstraint();
};

#endif
