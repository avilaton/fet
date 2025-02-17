/***************************************************************************
                          addconstraintstudentsactivitytagmaxhoursdailyform.h  -  description
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

#ifndef ADDCONSTRAINTSTUDENTSACTIVITYTAGMAXHOURSDAILYFORM_H
#define ADDCONSTRAINTSTUDENTSACTIVITYTAGMAXHOURSDAILYFORM_H

#include "ui_addconstraintstudentsactivitytagmaxhoursdailyform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class AddConstraintStudentsActivityTagMaxHoursDailyForm : public QDialog, Ui::AddConstraintStudentsActivityTagMaxHoursDailyForm_template  {
	Q_OBJECT
public:
	AddConstraintStudentsActivityTagMaxHoursDailyForm(QWidget* parent);
	~AddConstraintStudentsActivityTagMaxHoursDailyForm();
	
	void updateActivityTagsComboBox();

public slots:
	void addCurrentConstraint();
};

#endif
