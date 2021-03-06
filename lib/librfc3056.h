/*
 * Project    : ipv6calc
 * File       : librfc3056.h
 * Version    : $Id$
 * Copyright  : 2002-2014 by Peter Bieringer <pb (at) bieringer.de>
 *
 * Information:
 *  Header file for librfc3056.c
 */ 

#include "libipv6addr.h"
#include "libipv4addr.h"

/* prototypes */
int librfc3056_ipv4addr_to_ipv6to4addr(ipv6calc_ipv6addr *ipv6addrp, const ipv6calc_ipv4addr *ipv4addrp);
int librfc3056_ipv6addr_to_ipv4addr(ipv6calc_ipv4addr *ipv4addrp, const ipv6calc_ipv6addr *ipv6addrp, char* resultstring, const size_t resultstring_length);
