/*****************************************************************************

 @(#) $RCSfile$ $Name$($Revision$) $Date$

 -----------------------------------------------------------------------------

 Copyright (c) 2001-2007  OpenSS7 Corporation <http://www.openss7.com/>
 Copyright (c) 1997-2000  Brian F. G. Bidulock <bidulock@openss7.org>

 All Rights Reserved.

 This program is free software: you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation, version 3 of the license.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 details.

 You should have received a copy of the GNU General Public License along with
 this program.  If not, see <http://www.gnu.org/licenses/>, or write to the
 Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

 -----------------------------------------------------------------------------

 U.S. GOVERNMENT RESTRICTED RIGHTS.  If you are licensing this Software on
 behalf of the U.S. Government ("Government"), the following provisions apply
 to you.  If the Software is supplied by the Department of Defense ("DoD"), it
 is classified as "Commercial Computer Software" under paragraph 252.227-7014
 of the DoD Supplement to the Federal Acquisition Regulations ("DFARS") (or any
 successor regulations) and the Government is acquiring only the license rights
 granted herein (the license rights customarily provided to non-Government
 users).  If the Software is supplied to any unit or agency of the Government
 other than DoD, it is classified as "Restricted Computer Software" and the
 Government's rights in the Software are defined in paragraph 52.227-19 of the
 Federal Acquisition Regulations ("FAR") (or any successor regulations) or, in
 the cases of NASA, in paragraph 18.52.227-86 of the NASA Supplement to the FAR
 (or any successor regulations).

 -----------------------------------------------------------------------------

 Commercial licensing and support of this software is available from OpenSS7
 Corporation at a fee.  See http://www.openss7.com/

 -----------------------------------------------------------------------------

 Last Modified $Date$ by $Author$

 -----------------------------------------------------------------------------

 $Log$
 *****************************************************************************/

#ident "@(#) $RCSfile$ $Name$($Revision$) $Date$"

static char const ident[] = "$RCSfile$ $Name$($Revision$) $Date$";

/* ftampass.c - FPM: encode/decode access passwords */

#ifndef	lint
static char *rcsid =
    "Header: /xtel/isode/isode/ftam/RCS/ftampass.c,v 9.0 1992/06/16 12:14:55 isode Rel";
#endif

/* 
 * Header: /xtel/isode/isode/ftam/RCS/ftampass.c,v 9.0 1992/06/16 12:14:55 isode Rel
 *
 *
 * Log: ftampass.c,v
 * Revision 9.0  1992/06/16  12:14:55  isode
 * Release 8.0
 *
 */

/*
 *				  NOTICE
 *
 *    Acquisition, use, and distribution of this module and related
 *    materials are subject to the restrictions of a license agreement.
 *    Consult the Preface in the User's Manual for the full terms of
 *    this agreement.
 *
 */

/* LINTLIBRARY */

#include <stdio.h>
#include "fpkt.h"

/*  */

struct type_FTAM_Access__Passwords *
pass2fpm(fsb, fp, fti)
	register struct ftamblk *fsb;
	register struct FTAMpasswords *fp;
	struct FTAMindication *fti;
{
	register struct type_FTAM_Access__Passwords *fpm;

	if ((fpm = (struct type_FTAM_Access__Passwords *)
	     calloc(1, sizeof *fpm)) == NULL) {
	      no_mem:;
		(void) ftamlose(fti, FS_GEN(fsb), 1, NULLCP, "out of memory");
		if (fpm)
			free_FTAM_Access__Passwords(fpm);
		return NULL;
	}
#define	dopass(s,t,u) \
{ \
    if ((fpm -> s = (struct type_FTAM_Password *) \
			    calloc (1, sizeof *fpm -> s)) \
	    == NULL) \
	goto no_mem; \
    fpm -> s -> offset = type_FTAM_Password_binary; \
    if ((fpm -> s -> un.binary = str2qb (fp -> t, fp -> u, 1)) == NULL) \
	goto no_mem; \
}

	dopass(read__password, fp_read, fp_readlen);
	dopass(insert__password, fp_insert, fp_insertlen);
	dopass(replace__password, fp_replace, fp_replacelen);
	dopass(extend__password, fp_extend, fp_extendlen);
	dopass(erase__password, fp_erase, fp_eraselen);
	dopass(read__attribute__password, fp_readattr, fp_readattrlen);
	dopass(change__attribute__password, fp_chngattr, fp_chngattrlen);
	dopass(delete__password, fp_delete, fp_deletelen);

#undef	dopass

	return fpm;
}

/*  */

int
fpm2pass(fsb, fpm, fp, fti)
	register struct ftamblk *fsb;
	register struct type_FTAM_Access__Passwords *fpm;
	register struct FTAMpasswords *fp;
	struct FTAMindication *fti;
{
	register struct qbuf *qb;

	bzero((char *) fp, sizeof *fp);

/* both choices of this structure are qbuf's, so... */
#define	dopass(s,t,u) \
{ \
    qb = fpm -> s -> un.graphic; \
 \
    if ((fp -> t = qb2str (qb)) == NULL) \
	goto no_mem; \
    fp -> u = qb -> qb_len; \
}

	dopass(read__password, fp_read, fp_readlen);
	dopass(insert__password, fp_insert, fp_insertlen);
	dopass(replace__password, fp_replace, fp_replacelen);
	dopass(extend__password, fp_extend, fp_extendlen);
	dopass(erase__password, fp_erase, fp_eraselen);
	dopass(read__attribute__password, fp_readattr, fp_readattrlen);
	dopass(change__attribute__password, fp_chngattr, fp_chngattrlen);
	dopass(delete__password, fp_delete, fp_deletelen);

#undef	dopass

	return OK;

      no_mem:;
	FPFREE(fp);
	return ftamlose(fti, FS_GEN(fsb), 1, NULLCP, "out of memory");
}