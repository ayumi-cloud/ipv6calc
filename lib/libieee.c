/*
 * Project    : ipv6calc
 * File       : libieee.c
 * Version    : $Id: libieee.c,v 1.7 2005/09/15 12:32:21 peter Exp $
 * Copyright  : 2002-2004 by Peter Bieringer <pb (at) bieringer.de>
 *
 * Information:
 *  Function libary for IEEE information
 */

#include <stdio.h>
#include <string.h>

#include "libipv6calc.h"
#include "libipv6calcdebug.h"
#include "libieee.h"
#include "libmac.h"
#include "../databases/ieee-oui/dbieee_oui.h"
#include "../databases/ieee-iab/dbieee_iab.h"

/*
 * Get vendor string
 * in:  o1,o2,o3: first octets
 * mod: resultstring
 * out: 0=found, 1=not found
 */
#define DEBUG_function_name "libieee/get_vendor_string"
int libieee_get_vendor_string(char *resultstring, const ipv6calc_macaddr *macaddrp) {
	int retval = 1;
	int i;
	uint32_t idval, subidval;

	if (ipv6calc_debug != 0) {
		fprintf(stderr, "%s: called\n", DEBUG_function_name);
	};

	/* catch special ones */
	if ((macaddrp->addr[0] == 0xfc && macaddrp->addr[1] == 0xfc)) {
		/* Linux special OUI for ISDN-NET or PLIP interfaces */
		snprintf(resultstring, NI_MAXHOST - 1, "Linux ISDN-NET/PLIP");
		return (0);
	};
	
	if ( (macaddrp->addr[0] & 0x01) != 0 ) {
		/* Multicast */
		return (1);
	};

	if ( (macaddrp->addr[0] & 0x02) != 0 ) {
		/* Not global scope */
		return (1);
	};

	idval = (macaddrp->addr[0] << 16) | (macaddrp->addr[1] << 8) | macaddrp->addr[2];
	subidval = (macaddrp->addr[3] << 16) | (macaddrp->addr[4] << 8) | macaddrp->addr[5];

	/* run through IAB list */
	for (i = 0; i < (int) (sizeof(libieee_iab) / sizeof(libieee_iab[0])); i++) {
		if (libieee_iab[i].id == idval) {
			/* major id match */
			if (libieee_iab[i].subid_begin <= subidval && libieee_iab[i].subid_end >= subidval) {
				snprintf(resultstring, NI_MAXHOST - 1, "%s", libieee_iab[i].string_owner);
				return (0);
			};
		};
	};

	/* run through OUI list */
	for (i = 0; i < (int) (sizeof(libieee_oui) / sizeof(libieee_oui[0])); i++) {
		if (libieee_oui[i].id == idval) {
			/* match */
			snprintf(resultstring, NI_MAXHOST - 1, "%s", libieee_oui[i].string_owner);
			return (0);
		};
	};

	/* not found */
   	retval = 1;	
	return (retval);
};
#undef DEBUG_function_name

/*
 * Get short vendor string
 * in:  o1,o2,o3: first octets
 * mod: resultstring
 * out: 0=found, 1=not found
 */
#define DEBUG_function_name "libieee/get_short_vendor_string"
int libieee_get_short_vendor_string(char *resultstring, const ipv6calc_macaddr *macaddrp) {
	int retval = 1;
	int i;
	uint32_t idval, subidval;

	if (ipv6calc_debug != 0) {
		fprintf(stderr, "%s: called\n", DEBUG_function_name);
	};

	/* catch special ones */
	if ((macaddrp->addr[0] == 0xfc && macaddrp->addr[1] == 0xfc)) {
		/* Linux special OUI for ISDN-NET or PLIP interfaces */
		snprintf(resultstring, NI_MAXHOST - 1, "Linux-ISDN-NET+PLIP");
		return (0);
	};
	
	if ( (macaddrp->addr[0] & 0x01) != 0 ) {
		/* Multicast */
		return (1);
	};

	if ( (macaddrp->addr[0] & 0x02) != 0 ) {
		/* Not global scope */
		return (1);
	};

	idval = (macaddrp->addr[0] << 16) | (macaddrp->addr[1] << 8) | macaddrp->addr[2];
	subidval = (macaddrp->addr[3] << 16) | (macaddrp->addr[4] << 8) | macaddrp->addr[5];

	/* run through IAB list */
	for (i = 0; i < (int) (sizeof(libieee_iab) / sizeof(libieee_iab[0])); i++) {
		if (libieee_iab[i].id == idval) {
			/* major id match */
			if (libieee_iab[i].subid_begin <= subidval && libieee_iab[i].subid_end >= subidval) {
				snprintf(resultstring, NI_MAXHOST - 1, "%s", libieee_iab[i].shortstring_owner);
				return (0);
			};
		};
	};

	/* run through OUI list */
	for (i = 0; i < (int) (sizeof(libieee_oui) / sizeof(libieee_oui[0])); i++) {
		if (libieee_oui[i].id == idval) {
			/* match */
			snprintf(resultstring, NI_MAXHOST - 1, "%s", libieee_oui[i].shortstring_owner);
			return (0);
		};
	};

	/* not found */
   	retval = 1;	
	return (retval);
};
#undef DEBUG_function_name
