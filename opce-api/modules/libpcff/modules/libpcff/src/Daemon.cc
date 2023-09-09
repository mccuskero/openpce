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
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include <pcff/Daemon.h>

using namespace pcff;

const char* Daemon::TempRunningDir = "/tmp";
const char* Daemon::DefaultLockFile = "default-Daemon.lock";
const char* Daemon::DefaultLogFile = "default-Daemon.log";

Daemon::Daemon() {
    initMembers();   
}

Daemon::~Daemon() {
    if ( runningDir != NULL ) delete runningDir;
    if ( lockFile != NULL ) delete lockFile;
}

void Daemon::initMembers() {
    runningDir = NULL;
    lockFile = NULL;
}

void Daemon::init() {
    setRunningDir( Daemon::TempRunningDir );
    setLockFilename( Daemon::DefaultLockFile );
}


void Daemon::setRunningDir( const char* dir ) {
    runningDir = new char[strlen(dir)+1];
    strcpy( runningDir, dir );
}

void Daemon::setLockFilename( const char *name ) {
    lockFile = new char[strlen(name) + 1];
    strcpy( lockFile, name );
}

void Daemon::daemonize() {
    int i,lfp;
    char str[10];

    if(getppid()==1) 
        return; /* already a daemon */
    
    i=fork();
	
    if (i<0) 
       exit(1); /* fork error */
    if (i>0) 
       exit(0); /* parent exits */
	
    /* child (daemon) continues */
    setsid(); /* obtain a new process group */
    for (i=getdtablesize();i>=0;--i) 
       close(i); /* close all descriptors */
    i=open("/dev/null",O_RDWR); 
    dup(i); 
    dup(i); /* handle standart I/O */
    
    umask(027); /* set newly created file permissions */
    chdir( runningDir ); /* change running directory */
    lfp=open( lockFile , O_RDWR|O_CREAT, 0640);
    if (lfp<0) 
           exit(1); /* can not open */
    if (lockf(lfp,F_TLOCK,0)<0) 
           exit(0); /* can not lock */
           
    /* first instance continues */
    sprintf(str,"%d\n",getpid());
           
    write(lfp,str,strlen(str)); /* record pid to lockfile */
           
    signal(SIGCHLD,SIG_IGN); /* ignore child */
    signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
           
    //signal(SIGHUP,signal_handler); /* catch hangup signal */
    //signal(SIGTERM,signal_handler); /* catch kill signal */
}
