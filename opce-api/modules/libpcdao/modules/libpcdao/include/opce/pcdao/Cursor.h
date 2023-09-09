/**
 * Copyright (c) 1994-2009 Owen McCusker <mccuskero@ct.metrocast.com>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * All rights reserved.
 */

/*
 * Cursors are created from a transtraction.
 *
 * Cursors are created from the Database class.
 *
 * They are used to manage
 * large rows returned from queries. The cursor is implemented in the
 * database, and is created via a SQL statement.
 *
 * ""DECLARE myCursor CURSOR FOR SELECT SELECT * FROM mytable ORDER BY field"
 * "MOVE " + to_string(3) + " IN myCursor"
 *
 */

#ifndef _CURSOR_H
#define	_CURSOR_H


namespace pcdao
{

class Cursor
{
public:
    Cursor();
    Cursor(const Cursor& orig);
    virtual ~Cursor();
private:

};

};

#endif	/* _CURSOR_H */

