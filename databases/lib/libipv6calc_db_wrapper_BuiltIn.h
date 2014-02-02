/*
 * Project    : ipv6calc
 * File       : databases/lib/libipv6calc_db_wrapper_BuiltIn.h
 * Version    : $Id: libipv6calc_db_wrapper_BuiltIn.h,v 1.4 2014/02/02 09:20:49 ds6peter Exp $
 * Copyright  : 2013-2013 by Peter Bieringer <pb (at) bieringer.de>
 *
 * Information:
 *  Header file for libipv6calc_db_wrapper_BuiltIn.c
 */

#include "ipv6calctypes.h"
#include "libmac.h"

#ifndef _libipv6calc_db_wrapper_BuiltIn_h

#define _libipv6calc_db_wrapper_BuiltIn_h 1

#define SUPPORT_BUILTIN  // default

// features
extern uint32_t wrapper_features_BuiltIn;

/* ASN->Registry assignment structure */
typedef struct {
	const uint32_t asn_start;
	const uint32_t asn_stop;
	const uint8_t  registry;
} s_asn_assignment;


/* CC->Registry assignment structure */
typedef struct {
	const char    cc[3];
	const uint8_t registry;
} s_cc_reg_assignment;

typedef struct {
	const uint8_t  registry;
} s_cc_index_reg_assignment;

#endif


extern int  libipv6calc_db_wrapper_BuiltIn_wrapper_init(void);
extern int  libipv6calc_db_wrapper_BuiltIn_wrapper_cleanup(void);
extern void libipv6calc_db_wrapper_BuiltIn_wrapper_info(char* string, const size_t size);
extern void libipv6calc_db_wrapper_BuiltIn_wrapper_print_db_info(const int level_verbose, const char *prefix_string);

extern int libipv6calc_db_wrapper_BuiltIn_registry_num_by_as_num32(const uint32_t as_num32);
extern int libipv6calc_db_wrapper_BuiltIn_registry_num_by_cc_index(const uint16_t cc_index);

extern int libipv6calc_db_wrapper_BuiltIn_ieee_vendor_string_by_macaddr(char *resultstring, const ipv6calc_macaddr *macaddrp);
extern int libipv6calc_db_wrapper_BuiltIn_ieee_vendor_string_short_by_macaddr(char *resultstring, const ipv6calc_macaddr *macaddrp); 
