/***************************************************************************
                          addconstrainttwoactivitiesorderedform.h  -  description
                             -------------------
    begin                : Aug 21, 2007
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

#ifndef ADDCONSTRAINTTWOACTIVITIESORDEREDFORM_H
#define ADDCONSTRAINTTWOACTIVITIESORDEREDFORM_H

#include "ui_addconstrainttwoactivitiesorderedform_template.h"
#include "timetable_defs.h"
#include "timetable.h"
#include "fet.h"

#include <QList>

class AddConstraintTwoActivitiesOrderedForm : public QDialog, Ui::AddConstraintTwoActivitiesOrderedForm_template  {
	Q_OBJECT
public:
	AddConstraintTwoActivitiesOrderedForm(QWidget* parent);
	~AddConstraintTwoActivitiesOrderedForm();

	bool filterOk(Activity* a);

public slots:
	void addCurrentConstraint();
	
	void swap();
	
	void filterChanged();
	
private:
	//the id's of the activities listed in the first and second activities combo
	QList<int> firstActivitiesList;
	QList<int> secondActivitiesList;
};

#endif
