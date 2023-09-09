#ifndef HONEYTRAPSTRUCTS_H_
#define HONEYTRAPSTRUCTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>


#define MDA5SUM_SIZE   33
#define SHA512SUM_SIZE 129
#define SNAPLEN		   1413
#define USERLEN  	   32
#define PSWDLEN  	   32
#define FILENAMELEN    256
#define DLTYPELEN      16

typedef struct {
	char			md5sum[MDA5SUM_SIZE];	/* md5 checksum */
	char			sha512sum[SHA512SUM_SIZE];	/* sha512 checksum */
	u_int32_t		data_capture_size;		/* length of attack string */
	u_int32_t		data_size;		/* length of attack string */
	u_char			data[SNAPLEN];	/* attack string */	
} Payload;

typedef struct  {
	u_int32_t		l_addr;		/* local ip address */
	u_int32_t		r_addr;		/* remote ip address */
	u_int16_t		l_port;		/* local (tcp/udp) port */
	u_int16_t		r_port;		/* remote (tcp/udp) port */
	u_char			protocol;	/* IP protocol id (tcp/udp) */
	Payload         sPayload;
} Connection;

typedef struct {
	char			downloadType[DLTYPELEN]; /* (FTP, TFTP, VNC, ...) */
	u_int32_t		r_addr;		            /* remote IP address */
	u_int16_t		r_port;		            /* remote port */
	u_int16_t		protocol;	            /* protocol as in IP header */
	u_int16_t        userLen;                /* user length */
	u_int16_t        userCaptureLen;         /* user length */
	char			user[USERLEN];		    /* username for download connection */
	u_int16_t        passwordLen;                /* user length */
	u_int16_t        passwordCaptureLen;         /* user length */	
	char			password[PSWDLEN];		    /* user's password */
	u_int16_t        filenameLen;                /* user length */
	u_int16_t        filenameCaptureLen;         /* user length */	
        char		filename[FILENAMELEN];	/* filename of download */
	Payload         sPayload;
} Download;

typedef struct  {
	u_char			virtual_flag;	/* flag for marking virtual attacks */
	time_t			start_time;	/* time of attack start */
	time_t			end_time;	/* time of attack end */
	Connection		sAttackConn;		/* attack connection */
	Connection		sProxyConn;		/* proxy/mirror connection */
	u_char			op_mode;	/* mode of operation (none, ignore, normal, proxy, mirror) */
	u_int16_t		dl_count;	/* number of downloads */
	Download    	sDownload;	/* array of download structs */
} AttackPacket;

#endif /*HONEYTRAPSTRUCTS_H_*/
