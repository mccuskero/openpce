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

#ifndef TimeStamp_h
#define TimeStamp_h

#include <stdlib.h>
#include <time.h>

#include <string.h>
#include <string>

namespace pcff
{

struct XTimeStamp
{
   short _year;
   char _month;
   char _day;
   char _hour;
   char _minute;
   char _second;
   short _millisec;

   XTimeStamp(short y = 0,
              char m = 0,
              char d = 0,
              char h = 0,
              char n = 0,
              char s = 0,
              short milli = 0)
   : _year(y), _month(m), _day(d), _hour(h), _minute(n), _second(s), _millisec(milli)
   {
   }

   XTimeStamp(const XTimeStamp & src)
   : _year(src._year),
   _month(src._month),
   _day(src._day),
   _hour(src._hour),
   _minute(src._minute),
   _second(src._second),
   _millisec(src._millisec)
   {
   }

   const XTimeStamp & operator=(const XTimeStamp & src)
   {
      _year = src._year;
      _month = src._month;
      _day = src._day;
      _hour = src._hour;
      _minute = src._minute;
      _second = src._second;
      _millisec = src._millisec;

      return *this;
   }

   void set(short y = 0,
            char m = 0,
            char d = 0,
            char h = 0,
            char n = 0,
            char s = 0,
            short milli = 0)
   {
      _year = y;
      _month = m;
      _day = d;
      _hour = h;
      _minute = n;
      _second = s;
      _millisec = milli;
   }
};

//
// TimeStamp class
//  short year, char month, char day, char hour, char minute, char second, short millisecond )
//  -/+32767     1/12         1/31      0/23          0/59         0/59       0/999
//
// If expressed as double: integer part is representing days since 1/1/0 and decimals are
//   hours/minutes/seconds: 0.25 = 6 hours, 0.50 = 12 hours, ...; 1 sec = 0.000011574

class TimeStamp
{
public:

   typedef enum
   {
      UseGMT,
      UseLocalTime,
      SetToNow,
      DoNotSetToNow
   } Constants;
public:
   static const double decToSec; // = 0.000011574
   static const double decToHour; // = 0.041666667

   TimeStamp(int setToNow = DoNotSetToNow,
             int localOrGMT = UseLocalTime); // default constructor, sets it to now
   TimeStamp(long long t); // int64 time
   TimeStamp(long &t_sec, long &t_usec);
   TimeStamp(const tm& t); // from the tm structure
   TimeStamp(const time_t& t); // from the old time_t
   TimeStamp(short y, char m, char d, char h = 0, char n = 0, char s = 0, short milli = 0);
   TimeStamp(char h, char n, char s, short milli = 0); // today's date plus the given time
   TimeStamp(const char* strTimeStamp, const char* strFormat); // from a format string
   TimeStamp(const TimeStamp& src);

   TimeStamp(const XTimeStamp& src) : _dt(src)
   {
   }
   TimeStamp(double dateTime);
   const TimeStamp & operator=(const TimeStamp& src);

   const TimeStamp & operator=(const XTimeStamp& src)
   {
      _dt = src;
      return *this;
   }
   const TimeStamp & operator=(double dateTime);

   virtual ~TimeStamp()
   {
   }

public:

   short year() const
   {
      return _dt._year;
   } //  -32768 to +32767

   char month() const
   {
      return _dt._month;
   } //  1/12  NOTE: if the month is 0 the date is empty (was not set)

   char day() const
   {
      return _dt._day;
   } //  1/31

   char hour() const
   {
      return _dt._hour;
   } //  0/23

   char minute() const
   {
      return _dt._minute;
   } //  0/59

   char second() const
   {
      return _dt._second;
   } //  0/59

   short millisec() const
   {
      return _dt._millisec;
   } //  0/999

   bool isEmpty() const
   {
      return _dt._month == 0;
   }

   char hourPM() const
   {
      return (_dt._hour > 11 ? _dt._hour - 12 : _dt._hour);
   }
   short dayOfYear() const;
   char dayOfWeek(int firstDayOfWeek = 1) const; // Sunday = 1, Monday = 2, ...
   char weekOfYear() const; // Week 1 includes January 1

public:
   bool set(short y, char m, char d, char h = 0, char n = 0, char s = 0, short milli = 0);
   bool set(char h, char n, char s = 0, short milli = 0);
   bool set(long &t_sec, long &t_usec);
   bool set(const tm& t);
   bool set(const time_t& t);

   void set(const XTimeStamp& d)
   {
      _dt = d;
   }
   void set(double dateTime);
   bool set(const char* strTimeStamp, const char* strFormat);

   std::string get(const char* strFormat) const;
   bool get(std::string& strTimeStamp, const char* strFormat) const;
   bool get(tm& t) const;
   bool get(time_t& t) const;
   double get() const;

   void ToXTimeStamp(XTimeStamp& trg) const
   {
      trg = _dt;
   }

   void empty()
   {
      _dt.set();
   }

public:
   static bool isLeapYear(short y);
   static bool isValidDofM(short y, char m, char d);
   static char lastDofM(short y, char m);
   static short thisCentury();
   static char monthFromTxt(const char* sMonth); // "JULY" or "JUL"
   static std::string monthToTxt(char month, bool abbrev);
   static void convert64to32(long long t, long &tsec, long &tusec);

public:
   bool operator==(const TimeStamp& x) const;
   bool operator>(const TimeStamp& x) const;
   bool operator<(const TimeStamp& x) const;

   bool operator!=(const TimeStamp& x) const
   {
      return !operator==(x);
   }

   bool operator>=(const TimeStamp& x) const
   {
      return operator>(x) || operator==(x);
   }

   bool operator<=(const TimeStamp& x) const
   {
      return operator<(x) || operator==(x);
   }

   const TimeStamp & operator+=(int days);
   const TimeStamp & operator+=(double dateTime); // 1.50 = 1 day and 12 hours
   const TimeStamp & operator-=(int days);
   const TimeStamp & operator-=(double dateTime); // 1.50 = 1 day and 12 hours

   const TimeStamp & operator++()
   {
      return operator+=(1);
   } // increment with a day (prefix)

   const TimeStamp & operator--()
   {
      return operator+=(-1);
   } // decrement with a day (prefix)

   const TimeStamp & operator++(int)
   {
      return operator+=(1);
   } // increment with a day (postfix)

   const TimeStamp & operator--(int)
   {
      return operator+=(-1);
   } // decrement with a day (postfix)

public:
   const static char *StandardFormat;

protected:
   XTimeStamp _dt;

   inline static bool isTokenChar(char c);
   std::string getToken(const char* s, int& pos, bool bAlpha) const;
   bool getToken(const char* s, int& pos, std::string& v, std::string& w) const;
};

//
// static

inline bool TimeStamp::isTokenChar(char c)
{
   return (c == 'Y') || (c == 'y') ||
           (c == 'M') || (c == 'm') ||
           (c == 'D') || (c == 'd') ||
           (c == 'H') || (c == 'h') ||
           (c == 'N') || (c == 'n') ||
           (c == 'S') || (c == 's') ||
           (c == 'I') || (c == 'i') ||
           (c == 'P') || (c == 'p');
}

//
// static - LeapYear
// Modulo   4     !100  400   IsLeap?
//          T     T     T     T
//          T     T     F     T
//          T     F     T     T
//          T     F     F     F
//          F     X     X     F

inline bool TimeStamp::isLeapYear(short y)
{
   return ( (y % 4) == 0) && (((y % 100) != 0) || ((y % 400) == 0));
}

//
// static

inline bool TimeStamp::isValidDofM(short y, char m, char d)
{
   switch (m)
   {
   case 1: // JAN
   case 3: // MAR
   case 5: // MAY
   case 7: // JUL
   case 8: // AUG
   case 10: // OCT
   case 12: // DEC
      return ( 1 <= d && d <= 31);

   case 4: // APR
   case 6: // JUN
   case 9: // SEP
   case 11: // NOV
      return ( 1 <= d && d <= 30);

   case 2: // FEB
      return ( 1 <= d && d <= (isLeapYear(y) ? 29 : 28));
   }
   return false;
}

//
// static

inline char TimeStamp::lastDofM(short y, char m)
{
   switch (m)
   {
   case 1: // JAN
   case 3: // MAR
   case 5: // MAY
   case 7: // JUL
   case 8: // AUG
   case 10: // OCT
   case 12: // DEC
      return 31;

   case 4: // APR
   case 6: // JUN
   case 9: // SEP
   case 11: // NOV
      return 30;

   case 2: // FEB
      return ( isLeapYear(y) ? 29 : 28);
   }
   return 0;
}

};

#endif // defined TimeStamp_H
