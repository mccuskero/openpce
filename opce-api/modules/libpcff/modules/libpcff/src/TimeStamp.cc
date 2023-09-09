/**
 * Copyright (c) 1994-2007 Owen McCusker <pcave@myeastern.com>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
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

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>

#include <algorithm>
#include <cctype>      // old <ctype.h>
#include <string>
#include <sstream>

#include <pcff/TimeStamp.h>

using namespace std;
using namespace pcff;

// TODO: toupper and tolower exist
//      std::transform (s.begin(), s.end(), s.begin(), toupper);
//

// the two arrays are used by the string parsing routine, for other languages the locale should be
// used - this task is deffered for now
static string _ma[] = {"JAN",
   "FEB",
   "MAR",
   "APR",
   "MAY",
   "JUN",
   "JUL",
   "AUG",
   "SEP",
   "OCT",
   "NOV",
   "DEC"};

static string _mf[] = {"JANUARY",
   "FEBRUARY",
   "MARCH",
   "APRIL",
   "MAY",
   "JUNE",
   "JULY",
   "AUGUST",
   "SEPTEMBER",
   "OCTOBER",
   "NOVEMBER",
   "DECEMBER"};

const double TimeStamp::decToSec = 0.000011574;
const double TimeStamp::decToHour = 0.041666667;

const char *TimeStamp::StandardFormat = "YYYY-MM-DD HH:NN:SS.III";

//
// empty constructor
//

TimeStamp::TimeStamp(int setToNow, int useGMTOrLocal)
{
   struct tm* t = NULL;
   if (setToNow != TimeStamp::SetToNow) return;
   time_t localTime;
   time(&localTime);
   if (useGMTOrLocal == TimeStamp::UseGMT)
      t = gmtime(&localTime);
   else
      t = localtime(&localTime);
   set(1900 + t->tm_year,
       1 + t->tm_mon,
       t->tm_mday,
       t->tm_hour,
       t->tm_min,
       t->tm_sec);
}

//
//
//

TimeStamp::TimeStamp(long long t)
{
   struct tm ts;
   imaxdiv_t t_div;
   time_t t_sec;

   t_div = imaxdiv(t, 1000);

   t_sec = (time_t) (t_div.quot);

   gmtime_r(&t_sec, &ts);

   set(1900 + ts.tm_year, 1 + ts.tm_mon, ts.tm_mday, ts.tm_hour, ts.tm_min, ts.tm_sec, t_div.rem);
}

TimeStamp::TimeStamp(long &t_sec, long &t_usec)
{
   struct tm ts;
   imaxdiv_t t_div;
   time_t time;

   time = (time_t) (t_sec);

   gmtime_r(&time, &ts);

   set(1900 + ts.tm_year, 1 + ts.tm_mon, ts.tm_mday, ts.tm_hour, ts.tm_min, ts.tm_sec, 0);
}

TimeStamp::TimeStamp(const tm& t)
{
   set(1900 + t.tm_year, 1 + t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, 0);
}

//
//
//

TimeStamp::TimeStamp(const time_t& tt)
{
   struct tm* t = gmtime(&tt);
   set(1900 + t->tm_year, 1 + t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, 0);
}

//
//
//

TimeStamp::TimeStamp(short y, char m, char d, char h, char n, char s, short milli)
{
   set(y, m, d, h, n, s, milli);
}

//
//
//

TimeStamp::TimeStamp(char h, char n, char s, short milli)
{
   set(h, n, s, milli);
}

//
//
//

TimeStamp::TimeStamp(const char* strTimeStamp, const char* strFormat)
{
   set(strTimeStamp, strFormat);
}

//
//
//

TimeStamp::TimeStamp(const TimeStamp& src)
: _dt(src._dt)
{
}

//
//
//

TimeStamp::TimeStamp(double dateTime)
{
   set(dateTime);
}


//
//
//

const TimeStamp& TimeStamp::operator=(const TimeStamp& src)
{
   _dt = src._dt;
   return *this;
}

//
//
//

const TimeStamp& TimeStamp::operator=(double dateTime)
{
   set(dateTime);
   return *this;
}

//
//
//

short TimeStamp::dayOfYear() const
{
   short days = 0;

   // months
   for (int i = 1; i < (int)_dt._month; i++)
      days += lastDofM(_dt._year, i);
   // days
   days += (int)_dt._day - 1;

   // hours, minutes and seconds
   days += (int) (((int)_dt._hour * 3600 + (int)_dt._minute * 60 + (int)_dt._second) * decToSec);
   return days;
}

//
// TODO: need to implement
//

char TimeStamp::dayOfWeek(int firstDayOfWeek) const
{
   return 0;
}

//
//
//

char TimeStamp::weekOfYear() const
{ // Week 1 includes January 1
   TimeStamp dx(_dt._year, 1, 1);

   while (dx.dayOfWeek() != 1) // find the first Sunday of the year
      dx += 1;

   char weekNo = 1;
   while (dx < *this)
   {
      dx += 7;
      weekNo++;
   }
   return weekNo;
}

//
//
//

bool TimeStamp::set(short y, char m, char d, char h, char n, char s, short milli)
{
   empty();
   if (m < 1 || 12 < m) return false;
   if (!isValidDofM(y, m, d)) return false;
   if (h < 0 || 23 < m) return false;
   if (n < 0 || 59 < n) return false;
   if (s < 0 || 59 < s) return false; // ? leap seconds ?
   if (milli < 0 || 999 < milli) return false;
   _dt.set(y, m, d, h, n, s, milli);
   return true;
}

//
//
//

bool TimeStamp::set(char h, char n, char s, short milli)
{
   time_t localTime;
   time(&localTime);
   struct tm* t = gmtime(&localTime);
   return set(1900 + t->tm_year, 1 + t->tm_mon, t->tm_mday, h, n, s, milli);
}

bool TimeStamp::set(long &t_sec, long &t_usec)
{
   struct tm ts;

   gmtime_r(&t_sec, &ts);
   return set(1900 + ts.tm_year, 1 + ts.tm_mon, ts.tm_mday, ts.tm_hour, ts.tm_min, ts.tm_sec, t_usec);
}


//
//
//

bool TimeStamp::set(const tm& t)
{
   return set(1900 + t.tm_year, 1 + t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, 0);
}

//
//
//

bool TimeStamp::set(const time_t& tt)
{
   struct tm* t = gmtime(&tt);
   return set(1900 + t->tm_year, 1 + t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, 0);
}

//
//
//

void TimeStamp::set(double dateTime)
{
   empty();
   if (dateTime < 0) return;

   int days = (int) floor(dateTime);
   days -= 2; // for some weird reason the DATE (double) date format is based on
   _dt._year = 1900;
   _dt._month = 1;
   _dt._day = 1;
   _dt._hour = 0;
   _dt._minute = 0;
   _dt._second = 0;
   while (days >= 365)
   {
      days -= 365;
      _dt._year++;
      if (isLeapYear(_dt._year))
         days--;
   }
   if (days == -1)
   { // if we added one day to many correct
      if (--_dt._day == 0)
      {
         if (--_dt._month == 0)
         {
            _dt._month = 12;
            _dt._year--;
         }
         _dt._day = lastDofM(_dt._year, _dt._month);
      }
      days = 0;
   }
   while (days > 0)
   {
      days--;
      _dt._day++;
      if (_dt._day > lastDofM(_dt._year, _dt._month))
      {
         // it is actually the 1-st day of the next month
         _dt._day = 1;
         if (++_dt._month > 12)
         {
            _dt._month = 1;
            _dt._year++;
         }
      }
   }

   int seconds = (int) ((dateTime - floor(dateTime)) / decToSec);
   _dt._hour = seconds / 3600;
   seconds -= _dt._hour * 3600;
   _dt._minute = seconds / 60;
   seconds -= _dt._minute * 60;
   _dt._second = seconds;
   return;
}

//
// Parse the format string then read the values from the date string
// Valid tokens are:
//    yyyy  four digit year                  1997
//    yy    two  digit year                  97
//    m     month, no leading zero           6
//    mm    month, leading zero              06
//    mmm   month, 3 letter abrev.           Jun (case insensitive)
//    mmmm  month, full name                 June
//    d     day of month, no leading zero    7
//    dd    day of month, leading zero       07
//    h     hour, no leading zero            8
//    hh    hour, leading zero               08
//    nn    minute                           15
//    ss    second                           23
//    iii   millisecond                      120
//    PM    the literal PM indicates the time is given in 12 hour format and the hour is after 12PM
// Anything else is ignored.

bool TimeStamp::set(const char* strTimeStamp, const char* strFrmt)
{
   stringstream ss(stringstream::in | stringstream::out);
   empty();
   if (strTimeStamp == NULL || strFrmt == NULL) return false;
   int dateLen = strlen(strTimeStamp);
   int frmtLen = strlen(strFrmt);
   if (dateLen <= 0 || frmtLen <= 0) return false;

   short year = 0;
   char month = 0;
   char day = 0;
   char hour = 0;
   char minute = 0;
   char second = 0;
   short millisec = 0;

   int datePos = 0;
   int frmtPos = 0;
   bool bData = true;
   bool bPM = false;
   while (bData)
   {
      string v = "";
      string w = "";
      string x = "";
      bData = getToken(strFrmt, frmtPos, v, w);
      if (v.size() > 0) datePos += v.size();
      if (w.size() > 0)
      {
         if (w == "YYYY")
         {
            x += strTimeStamp[ datePos++ ];
            x += strTimeStamp[ datePos++ ];
            x += strTimeStamp[ datePos++ ];
            x += strTimeStamp[ datePos++ ];
            ss.str(x);
            ss >> year;
         }
            // TODO:
            //         else if( w == "YY" ) {
            //   x += strTimeStamp[ datePos++ ];
            //   x += strTimeStamp[ datePos++ ];
            //   ss.str(x);
            //   year = ThisCentury() + (short)x.CLng();
            //}
         else if (w == "M")
         {
            x += strTimeStamp[ datePos++ ];
            ss.str(x);
            ss >> month;
         }
         else if (w == "MM")
         {
            x += strTimeStamp[ datePos++ ];
            x += strTimeStamp[ datePos++ ];
            ss.str(x);
            ss >> month;
         }
         else if (w == "MMM")
         {
            x += strTimeStamp[ datePos++ ];
            x += strTimeStamp[ datePos++ ];
            x += strTimeStamp[ datePos++ ];
            ss.str(x);
            month = monthFromTxt(x.c_str());
         }
         else if (w == "MMMM")
         {
            x = getToken(strTimeStamp, datePos, true);
            ss.str(x);
            month = monthFromTxt(x.c_str());
         }
         else if (w == "D")
         {
            x = getToken(strTimeStamp, datePos, false);
            ss.str(x);
            ss >> day;
         }
         else if (w == "DD")
         {
            x += strTimeStamp[ datePos++ ];
            x += strTimeStamp[ datePos++ ];
            ss.str(x);
            ss >> day;
         }
         else if (w == "H")
         {
            x = getToken(strTimeStamp, datePos, false);
            ss.str(x);
            ss >> hour;
         }
         else if (w == "HH")
         {
            x += strTimeStamp[ datePos++ ];
            x += strTimeStamp[ datePos++ ];
            ss.str(x);
            ss >> hour;
         }
         else if (w == "NN")
         {
            x += strTimeStamp[ datePos++ ];
            x += strTimeStamp[ datePos++ ];
            ss.str(x);
            ss >> minute;
         }
         else if (w == "SS")
         {
            x += strTimeStamp[ datePos++ ];
            x += strTimeStamp[ datePos++ ];
            ss.str(x);
            ss >> second;
         }
         else if (w == "III")
         {
            x = getToken(strTimeStamp, datePos, false);
            ss.str(x);
            ss >> millisec;
         }
         else if (w == "PM")
         {
            x = getToken(strTimeStamp, datePos, true);
            ss.str(x);
            bPM = (x == "PM");
         }
      }
   }
   if (bPM) hour += 12;
   return set(year, month, day, hour, minute, second, millisec);
}


//
//
//

string TimeStamp::get(const char* strFrmt) const
{
   string strTimeStamp = "";
   get(strTimeStamp, strFrmt);
   return strTimeStamp;
}

//
//
//

bool TimeStamp::get(string& strTimeStamp, const char* strFrmt) const
{
   strTimeStamp = "";
   if (strFrmt == NULL || isEmpty()) return false;
   int frmtLen = strlen(strFrmt);
   if (frmtLen <= 0) return false;

   bool bPM = false;
   string fmt = strFrmt;
   transform(fmt.begin(),
             fmt.end(),
             fmt.begin(),
             (int(*)(int))toupper);

   if (fmt.find("PM") != string::npos) bPM = true;

   int frmtPos = 0;
   bool bData = true;
   while (bData)
   {
      string v = "";
      string w = "";
      stringstream z(stringstream::in | stringstream::out);
      stringstream x(stringstream::in | stringstream::out);
      bData = getToken(strFrmt, frmtPos, v, w);
      if (v.size() > 0) strTimeStamp += v;
      if (w.size() > 0)
      {
         if (w == "YYYY")
         {
            x << _dt._year;
         }
            // TODO:
            //         else if( w == "YY" ) {
            //
            //      x <<  _dt._year ;
            //      x << x.Right( 2 );
            //   }
         else if (w == "M")
         {
            x << (short) _dt._month;
         }
         else if (w == "MM")
         {
            z << (short) _dt._month;
            if (z.str().size() == 1)
               x << "0" << z.str();
            else
               x << z.str();
         }
         else if (w == "MMM")
         {
            x << monthToTxt(_dt._month, true);
         }
         else if (w == "MMMM")
         {
            x << monthToTxt(_dt._month, false);
         }
         else if (w == "D")
         {
            x << (short) _dt._day;
         }
         else if (w == "DD")
         {
            z << (short) _dt._day;
            if (z.str().size() == 1)
               x << "0" << z.str();
            else
               x << z.str();
         }
         else if (w == "H")
         {
            x << (short) (bPM ? hour() : _dt._hour);
         }
         else if (w == "HH")
         {
            z << (short) (bPM ? hour() : _dt._hour);
            if (z.str().size() == 1)
               x << "0" << z.str();
            else
               x << z.str();
         }
         else if (w == "NN")
         {
            z << (short) _dt._minute;
            if (z.str().size() == 1)
               x << "0" << z.str();
            else
               x << z.str();
         }
         else if (w == "SS")
         {
            z << (short) _dt._second;
            if (z.str().size() == 1)
               x << "0" << z.str();
            else
               x << z.str();
         }
         else if (w == "III")
         {
            z << _dt._millisec;
            if (z.str().size() == 1)
               x << "00" << z.str();
            else if (z.str().size() == 2)
               x << "0" << z.str();
            else
               x << z.str();
         }
         else if (w == "PM")
         {
            // we already account for this
         }
      }
      strTimeStamp += x.str();
   }
   return true;
}

//
//
//

bool TimeStamp::get(tm& t) const
{
   if (isEmpty()) return false;

   t.tm_year = _dt._year - 1900;
   t.tm_mon = _dt._month - 1;
   t.tm_mday = _dt._day;
   if (_dt._hour > 12)
   {
      t.tm_hour = _dt._hour - 12;
   }
   else
      t.tm_hour = (_dt._hour == 0 ? 12 : _dt._hour);
   t.tm_min = _dt._minute;
   t.tm_sec = _dt._second;
   return true;
}

//
//
//

bool TimeStamp::get(time_t& tt) const
{
   tm t;
   if (!get(t)) return false;
   if ((tt = mktime(&t)) == (time_t) - 1) return false;
   return true;
}

//
//
//

double TimeStamp::get() const
{
   if (isEmpty()) return 0;

   double dt = 2.0;

   // years
   short y = _dt._year - 1900;
   dt += y * 365;
   y = y - (_dt._month > 2 ? 0 : 1);
   while (y > 0)
      if (isLeapYear(y--))
         dt += 1.0;
   // months
   for (int i = 1; i < _dt._month; i++)
      dt += lastDofM(_dt._year, i);
   // days
   dt += _dt._day - 1;

   // hours, minutes and seconds
   dt += (_dt._hour * 3600 + _dt._minute * 60 + _dt._second) * decToSec;
   return dt;
}


//
// static

short TimeStamp::thisCentury()
{
   time_t localTime;
   time(&localTime);
   struct tm* t = gmtime(&localTime);
   short y = 1900 + t->tm_year;
   return y - (y % 100);
}

//
// TODO: need to make toupper in compare, for case less
//

char TimeStamp::monthFromTxt(const char* sMonth)
{
   if (sMonth == NULL) return 0;
   int i = 0;
   for (i = 0; i < 12; i++)
      if (_ma[i] == sMonth) return i + 1;
   for (i = 0; i < 12; i++)
      if (_mf[i] == sMonth) return i + 1;
   return 0;
}

//
// static
//

string TimeStamp::monthToTxt(char month, bool abbrev)
{
   if (month < 1 || 12 < month) return "";
   if (abbrev)
      return _ma[month - 1];
   else
      return _mf[month - 1];
}


//
//
//

bool TimeStamp::operator==(const TimeStamp& x) const
{
   return _dt._year == x._dt._year &&
           _dt._month == x._dt._month &&
           _dt._day == x._dt._day &&
           _dt._hour == x._dt._hour &&
           _dt._minute == x._dt._minute &&
           _dt._second == x._dt._second &&
           _dt._millisec == x._dt._millisec;
}

//
//
//

bool TimeStamp::operator>(const TimeStamp& x) const
{
   if (_dt._year != x._dt._year) return _dt._year > x._dt._year;
   if (_dt._month != x._dt._month) return _dt._month > x._dt._month;
   if (_dt._day != x._dt._day) return _dt._day > x._dt._day;
   if (_dt._hour != x._dt._hour) return _dt._hour > x._dt._hour;
   if (_dt._minute != x._dt._minute) return _dt._minute > x._dt._minute;
   if (_dt._second != x._dt._second) return _dt._second > x._dt._second;
   if (_dt._millisec != x._dt._millisec) return _dt._millisec > x._dt._millisec;
   return false; // they are equal
}

//
//
//

bool TimeStamp::operator<(const TimeStamp& x) const
{
   if (_dt._year != x._dt._year) return _dt._year < x._dt._year;
   if (_dt._month != x._dt._month) return _dt._month < x._dt._month;
   if (_dt._day != x._dt._day) return _dt._day < x._dt._day;
   if (_dt._hour != x._dt._hour) return _dt._hour < x._dt._hour;
   if (_dt._minute != x._dt._minute) return _dt._minute < x._dt._minute;
   if (_dt._second != x._dt._second) return _dt._second < x._dt._second;
   if (_dt._millisec != x._dt._millisec) return _dt._millisec < x._dt._millisec;
   return false; // they are equal
}

//
//
//

const TimeStamp& TimeStamp::operator+=(int days)
{
   if (isEmpty() || days == 0) return *this;
   double dt = get() + days;
   set(dt);
   return *this;
}

//
// 1.50 = 1 day and 12 hours
//

const TimeStamp& TimeStamp::operator+=(double dateTime)
{
   if (dateTime == 0) return *this;
   double dt = get() + dateTime;
   set(dt);
   return *this;
}

//
//
//

const TimeStamp& TimeStamp::operator-=(int days)
{
   if (isEmpty() || days == 0) return *this;
   double dt = get() - days;
   set(dt);
   return *this;
}

//
// 1.50 = 1 day and 12 hours
//

const TimeStamp& TimeStamp::operator-=(double dateTime)
{
   if (dateTime == 0) return *this;
   double dt = get() - dateTime;
   set(dt);
   return *this;
}

//
//
//

bool TimeStamp::getToken(const char* s, int& pos, string& v, string& w) const
{
   v = "";
   w = "";
   if (s == NULL || pos < 0) return false;
   int size = strlen(s);
   if (size <= 0 || size <= pos) return false;

   while (pos < size && !isTokenChar(s[pos]))
      v += s[pos++];
   while (pos < size && isTokenChar(s[pos]))
      w += s[pos++];

   transform(v.begin(),
             v.end(),
             v.begin(),
             (int(*)(int))toupper);
   transform(w.begin(),
             w.end(),
             w.begin(),
             (int(*)(int))toupper);

   return pos < size;
}

//
//
//

string TimeStamp::getToken(const char* s, int& pos, bool bAlpha) const
{
   string v = "";
   if (s == NULL || pos < 0) return v;
   int size = strlen(s);
   if (size <= 0 || size <= pos) return v;

   if (bAlpha)
   {
      while (pos < size && isalpha(s[pos]))
         v += s[pos++];
   }
   else
   {
      while (pos < size && isdigit(s[pos]))
         v += s[pos++];
   }
   return v;
}


//
// Takes 64-bit time value in milliseconds since EPOCH, converts to sec, and usec
//

void TimeStamp::convert64to32(long long t, long &tSec, long &tUsec)
{
   struct tm ts;
   imaxdiv_t t_div;

   t_div = imaxdiv(t, 1000);
   tSec = t_div.quot;
   tUsec = t_div.rem * 1000;
}

