/***************************************************************************
                          addconstraintstudentssetmaxbuildingchangesperweekform.h  -  description
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

#ifndef ADDCONSTRAINTSTUDENTSSETMAXBUILDINGCHANGESPERWEEKFORM_H
#define ADDCONSTRAINTSTUDENTSSETMAXBUILDINGCHANGESPERWEEKFORM_H

#include "ui_addconstraintstudentssetmaxbuildingchangesperweekform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class AddConstraintStudentsSetMaxBuildingChangesPerWeekForm : public QDialog, Ui::AddConstraintStudentsSetMaxBuildingChangesPerWeekForm_template  {
	Q_OBJECT
public:
	AddConstraintStudentsSetMaxBuildingChangesPerWeekForm(QWidget* parent);
	~AddConstraintStudentsSetMaxBuildingChangesPerWeekForm();

	void updateStudentsSetComboBox();

public slots:
	void addCurrentConstraint();
};

#endif
