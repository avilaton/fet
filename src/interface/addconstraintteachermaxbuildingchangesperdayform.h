/***************************************************************************
                          addconstraintteachermaxbuildingchangesperdayform.h  -  description
                             -------------------
    begin                : Feb 10, 2005
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

#ifndef ADDCONSTRAINTTEACHERMAXBUILDINGCHANGESPERDAYFORM_H
#define ADDCONSTRAINTTEACHERMAXBUILDINGCHANGESPERDAYFORM_H

#include "ui_addconstraintteachermaxbuildingchangesperdayform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class AddConstraintTeacherMaxBuildingChangesPerDayForm : public QDialog, Ui::AddConstraintTeacherMaxBuildingChangesPerDayForm_template  {
	Q_OBJECT
public:
	AddConstraintTeacherMaxBuildingChangesPerDayForm(QWidget* parent);
	~AddConstraintTeacherMaxBuildingChangesPerDayForm();

	void updateTeachersComboBox();

public slots:
	void addCurrentConstraint();
	void addCurrentConstraints();
};

#endif
