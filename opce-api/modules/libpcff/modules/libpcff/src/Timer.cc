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

#include <stdio.h>
#include <sys/time.h>

#include <pcff/Timer.h>

using namespace pcff;

Timer::Timer()
{
  _startTime = 0;
  _stopTime = 0;
}

//
//
//
void Timer::start() 
{
  timeval curTime;

  gettimeofday(&curTime, NULL);

  _startTime = (double)curTime.tv_sec + \
               (double)curTime.tv_usec/1000000;
}

//
//
//
void Timer::stop() 
{
  timeval curTime;

  gettimeofday(&curTime, NULL);

  _stopTime = (double)curTime.tv_sec + \
              (double)curTime.tv_usec/1000000;
}

//
//
//
double Timer::getTime() 
{
	return (_stopTime - _startTime);
}

//
//
//
double Timer::getElapsedTime() 
{
	timeval curTime;
	double  elapsedTime = 0.0;

	gettimeofday(&curTime, NULL);

	elapsedTime =  (double)curTime.tv_sec + \
	               (double)curTime.tv_usec/1000000;

	return (elapsedTime - _startTime);
}

//
// TODO: not thread safe!!!
//
char* Timer::getElapsedTimeAsString() 
{
	static char sElapsedTime[256];
	timeval curTime;
	double  elapsedTime = 0.0;

	gettimeofday(&curTime, NULL);

	elapsedTime =  (double)curTime.tv_sec + \
	               (double)curTime.tv_usec/1000000;

	sprintf(sElapsedTime, "%.3fs", (elapsedTime - _startTime) );

	return sElapsedTime;
}
