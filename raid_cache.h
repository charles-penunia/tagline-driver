#ifndef RAID_CACHE_INCLUDED
#define RAID_CACHE_INCLUDED

////////////////////////////////////////////////////////////////////////////////
//
//  File           : raid_cache.h
//  Description    : This is the header file for the implementation of the
//                   block cache for the TAGLINE driver.
//
//  Author         : Patrick McDaniel
//  Last Modified  : Fri Oct  9 17:14:45 PDT 2015
//

// Includes
#include <tagline_driver.h>

// Defines
#define TAGLINE_CACHE_SIZE 1024

///
// Cache Interfaces

int init_raid_cache(uint32_t max_blocks);
	// Initialize the cache and note maximum blocks

int close_raid_cache(void);
	// Clear all of the contents of the cache, cleanup

int put_raid_cache(RAIDDiskID dsk, RAIDBlockID blk, void *buf);
	// Put an object into the object cache, evicting other items as necessary

void * get_raid_cache(RAIDDiskID dsk, RAIDBlockID blk);
	// Get an object from the cache (and return it)

void calc_least_recent(void);
	// Determines the least recently accessed entry

#endif
