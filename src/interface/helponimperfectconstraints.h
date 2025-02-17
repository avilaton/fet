/***************************************************************************
                          helponimperfectconstraints.h  -  description
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

#ifndef HELPONIMPERFECTCONSTRAINTS_H
#define HELPONIMPERFECTCONSTRAINTS_H

#include <QApplication>

class HelpOnImperfectConstraints{
	Q_DECLARE_TR_FUNCTIONS(HelpOnImperfectConstraints)
	
public:
	static void help(QWidget* parent, bool activityTagMinHoursDaily=false);
};

#endif
