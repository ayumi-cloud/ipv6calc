/*
 * Project    : ipv6calc
 * File       : databases/lib/libipv6calc_db_wrapper_DBIP.h
 * Version    : $Id$
 * Copyright  : 2014-2019 by Peter Bieringer <pb (at) bieringer.de>
 *
 * Information:
 *  Header file for libipv6calc_db_wrapper_DBIP.c
 *
 *  !!! Support of BerkeleyDB files will be dropped in 3.0.0 !!!
 */

#include "ipv6calctypes.h"

#ifndef _libipv6calc_db_wrapper_DBIP_h

#define _libipv6calc_db_wrapper_DBIP_h 1

// database types
#define DBIP_DB_IPV4_COUNTRY				1
#define DBIP_DB_IPV6_COUNTRY				2
#define DBIP_DB_IPV4_CITY				3
#define DBIP_DB_IPV6_CITY				4
#define DBIP_DB_IPV4_ISP				5	// commercial
#define DBIP_DB_IPV6_ISP				6	// commercial
#define DBIP_DB_IPV4_LOCATION				7	// commercial
#define DBIP_DB_IPV6_LOCATION				8	// commercial
#define DBIP_DB_IPV4_FULL				9	// commercial
#define DBIP_DB_IPV6_FULL				10	// commercial
#define DBIP_DB_MAX					DBIP_DB_IPV6_FULL

#define IPV6CALC_DB_DBIP_INTERNAL_FREE		0x00000001
#define IPV6CALC_DB_DBIP_INTERNAL_COMM		0x00000002


/*@unused@*/ static const db_file_desc2 libipv6calc_db_wrapper_DBIP_db_file_desc[] = {
	// country
	{ DBIP_DB_IPV4_COUNTRY
		, "ipv6calc-dbip-ipv4-country.db"
		, "IPv4 CountryCode (FREE)"
		, IPV6CALC_DB_IPV4_TO_CC | IPV6CALC_DB_DBIP_IPV4
		, IPV6CALC_DB_DBIP_INTERNAL_FREE
	},
	{ DBIP_DB_IPV6_COUNTRY
		, "ipv6calc-dbip-ipv6-country.db"
		, "IPv6 CountryCode (FREE)"
		, IPV6CALC_DB_IPV6_TO_CC | IPV6CALC_DB_DBIP_IPV6
		, IPV6CALC_DB_DBIP_INTERNAL_FREE
	},

	// city
	{ DBIP_DB_IPV4_CITY
		, "ipv6calc-dbip-ipv4-city.db"
		, "IPv4 City/Region/CountryCode (FREE)"
		, IPV6CALC_DB_IPV4_TO_CITY | IPV6CALC_DB_IPV4_TO_REGION | IPV6CALC_DB_IPV4_TO_CC | IPV6CALC_DB_DBIP_IPV4
		, IPV6CALC_DB_DBIP_INTERNAL_FREE
	},
	{ DBIP_DB_IPV6_CITY
		, "ipv6calc-dbip-ipv6-city.db"
		, "IPv6 City/Region/CountryCode (FREE)"
		, IPV6CALC_DB_IPV6_TO_CITY | IPV6CALC_DB_IPV6_TO_REGION | IPV6CALC_DB_IPV6_TO_CC | IPV6CALC_DB_DBIP_IPV6
		, IPV6CALC_DB_DBIP_INTERNAL_FREE
	},

	// isp
	{ DBIP_DB_IPV4_ISP
		, "ipv6calc-dbip-ipv4-isp.db"
		, "IPv4 ISP (COMM)", IPV6CALC_DB_IPV4_TO_CC | IPV6CALC_DB_DBIP_IPV4
		, IPV6CALC_DB_DBIP_INTERNAL_COMM
	},
	{ DBIP_DB_IPV6_ISP
		, "ipv6calc-dbip-ipv6-isp.db"
		, "IPv6 ISP (COMM)", IPV6CALC_DB_IPV6_TO_CC | IPV6CALC_DB_DBIP_IPV6
		, IPV6CALC_DB_DBIP_INTERNAL_COMM
	},

	// location
	{ DBIP_DB_IPV4_LOCATION
		, "ipv6calc-dbip-ipv4-location.db"
		, "IPv4 Location (COMM)"
		, IPV6CALC_DB_IPV4_TO_CITY | IPV6CALC_DB_IPV4_TO_REGION | IPV6CALC_DB_IPV4_TO_CC | IPV6CALC_DB_DBIP_IPV4 | IPV6CALC_DB_IPV4_TO_COUNTRY
		, IPV6CALC_DB_DBIP_INTERNAL_COMM
	},
	{ DBIP_DB_IPV6_LOCATION
		, "ipv6calc-dbip-ipv6-location.db"
		, "IPv6 Location (COMM)"
		, IPV6CALC_DB_IPV6_TO_CITY | IPV6CALC_DB_IPV6_TO_REGION | IPV6CALC_DB_IPV6_TO_CC | IPV6CALC_DB_DBIP_IPV6 | IPV6CALC_DB_IPV6_TO_COUNTRY
		, IPV6CALC_DB_DBIP_INTERNAL_COMM
	},

	// full
	{ DBIP_DB_IPV4_FULL
		, "ipv6calc-dbip-ipv4-full.db"
		, "IPv4 Location/ISP (COMM)"
		, IPV6CALC_DB_IPV4_TO_CITY | IPV6CALC_DB_IPV4_TO_REGION | IPV6CALC_DB_IPV4_TO_CC | IPV6CALC_DB_DBIP_IPV4 | IPV6CALC_DB_IPV4_TO_COUNTRY
		, IPV6CALC_DB_DBIP_INTERNAL_COMM
	},
	{ DBIP_DB_IPV6_FULL
		, "ipv6calc-dbip-ipv6-full.db"
		, "IPv6 Location/ISP (COMM)"
		, IPV6CALC_DB_IPV6_TO_CITY | IPV6CALC_DB_IPV6_TO_REGION | IPV6CALC_DB_IPV6_TO_CC | IPV6CALC_DB_DBIP_IPV6 | IPV6CALC_DB_IPV6_TO_COUNTRY
		, IPV6CALC_DB_DBIP_INTERNAL_COMM
	},
};

// features
extern uint32_t wrapper_features_DBIP;

typedef struct
{
        FILE *filehandle;
        uint8_t databasetype;
        uint8_t databasecolumn;
        uint8_t databaseday;
        uint8_t databasemonth;
        uint8_t databaseyear;
        uint32_t databasecount;
        uint32_t databaseaddr;
        uint32_t ipversion;
} DBIP;

#endif

extern int         libipv6calc_db_wrapper_DBIP_wrapper_init(void);
extern int         libipv6calc_db_wrapper_DBIP_wrapper_cleanup(void);
extern void        libipv6calc_db_wrapper_DBIP_wrapper_info(char* string, const size_t size);
extern void        libipv6calc_db_wrapper_DBIP_wrapper_print_db_info(const int level_verbose, const char *prefix_string);
extern char       *libipv6calc_db_wrapper_DBIP_wrapper_db_info_used(void);

extern int         libipv6calc_db_wrapper_DBIP_wrapper_country_code_by_addr(const ipv6calc_ipaddr *ipaddrp, char *country, const size_t country_len);

extern int         libipv6calc_db_wrapper_DBIP_has_features(const uint32_t features);


#ifdef SUPPORT_DBIP

#include <db.h>

extern char dbip_db_dir[NI_MAXHOST];

extern int          libipv6calc_db_wrapper_DBIP_db_avail(const unsigned int type);
extern DB          *libipv6calc_db_wrapper_DBIP_open_type(const unsigned int type_flag, long int *db_recno_max_ptr);

extern const char  *libipv6calc_db_wrapper_DBIP_dbdescription(const unsigned int type);
extern char        *libipv6calc_db_wrapper_DBIP_database_info(const unsigned int type);

extern int dbip_db_comm_to_free_switch_min_delta_months;
extern int dbip_db_only_type;

extern int          libipv6calc_db_wrapper_DBIP_all_by_addr(const ipv6calc_ipaddr *ipaddrp, libipv6calc_db_wrapper_geolocation_record *recordp);

#endif
