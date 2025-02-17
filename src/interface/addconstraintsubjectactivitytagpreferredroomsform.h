/***************************************************************************
                          addconstraintsubjectactivitytagpreferredroomsform.h  -  description
                             -------------------
    begin                : Aug 18, 2007
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

#ifndef ADDCONSTRAINTSUBJECTACTIVITYTAGPREFERREDROOMSFORM_H
#define ADDCONSTRAINTSUBJECTACTIVITYTAGPREFERREDROOMSFORM_H

#include "ui_addconstraintsubjectactivitytagpreferredroomsform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

class AddConstraintSubjectActivityTagPreferredRoomsForm : public QDialog, Ui::AddConstraintSubjectActivityTagPreferredRoomsForm_template  {
	Q_OBJECT
public:
	AddConstraintSubjectActivityTagPreferredRoomsForm(QWidget* parent);
	~AddConstraintSubjectActivityTagPreferredRoomsForm();

	void updateRoomsListWidget();

public slots:
	void addRoom();
	void removeRoom();

	void addConstraint();
	
	void clear();
};

#endif
