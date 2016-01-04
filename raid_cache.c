////////////////////////////////////////////////////////////////////////////////
//
//  File           : raid_cache.c
//  Description    : This is the implementation of the cache for the TAGLINE
//                   driver.
//
//  Author         : Charles Penunia
//  Last Modified  : 10 November 2015
//

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Project includes
#include <cmpsc311_log.h>
#include <cmpsc311_util.h>
#include <raid_cache.h>

// Structure for an entry in the cache
typedef struct {
	time_t clkTick;
	RAIDDiskID disk;
	RAIDBlockID blockID;
	int *buffer;
} CacheEntry;

// Global variables

CacheEntry **cache, *leastRecent;
int initialized, maxSize;

// Fuction prototype
void calc_least_recent();

//
// TAGLINE Cache interface

////////////////////////////////////////////////////////////////////////////////
//
// Function     : init_raid_cache
// Description  : Initialize the cache and note maximum blocks
//
// Inputs       : max_items - the maximum number of items your cache can hold
// Outputs      : 0 if successful, -1 if failure

int init_raid_cache(uint32_t max_items) {

	// Initializes cache
	cache = (CacheEntry **) calloc(TAGLINE_CACHE_SIZE, sizeof(CacheEntry));
	
	if (cache == NULL) {
		logMessage(LOG_ERROR_LEVEL, "Memory is not allocated successfully");
		return (-1);
	}

	// Initializes cache information
	initialized = 0;
	maxSize = max_items * 1024;
	
	// Return successfully
	return(0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : close_raid_cache
// Description  : Clear all of the contents of the cache, cleanup
//
// Inputs       : none
// Outputs      : o if successful, -1 if failure

int close_raid_cache(void) {

	// Declares variables
	int i;

	// Frees buffer in cache entrires and
	// the cache entries themselves
	i = 0;
	while (i < initialized) {
		free(cache[i]->buffer);
		cache[i]->buffer = NULL;
		free(cache[i]);
		cache[i] = NULL;
		i++;
	}
	
	// Frees the cache
	free(cache);
	cache = NULL;

	// Return successfully
	return(0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : put_raid_cache
// Description  : Put an object into the block cache
//
// Inputs       : dsk - this is the disk number of the block to cache
//                blk - this is the block number of the block to cache
//                buf - the buffer to insert into the cache
// Outputs      : 0 if successful, -1 if failure

int put_raid_cache(RAIDDiskID dsk, RAIDBlockID blk, void *buf)  {

	// Declares variables
	int i;
	CacheEntry *temp;
	struct timeval tval;

	// Updates the contents of an existing cache entry,
	// if applicable
	i = 0;
	while (i < initialized) {
		temp = cache[i];
		if (temp->disk == dsk && temp->blockID == blk) {
			memcpy(temp->buffer, buf, maxSize);
			// Returns successfully
			return (0);
		}
		i++;
	}

	if (initialized == TAGLINE_CACHE_SIZE){
		// Overwrites the entry of the least recently used entry
		// (capacity miss)
		gettimeofday(&tval, NULL);
		leastRecent->clkTick = tval.tv_sec;
		leastRecent->disk = dsk;
		leastRecent->blockID = blk;
		memcpy(leastRecent->buffer, buf, maxSize);

		// Redetermines the least recently used cache entry
		calc_least_recent();
	}
	else {
		// Creates a new cache entry
		// (cold miss)
		temp = (CacheEntry *) malloc(sizeof(CacheEntry));

		if (temp == NULL) {
			logMessage(LOG_ERROR_LEVEL, "Memory is not allocated successfully");
			return (-1);
		}

		// Creates space for the cache data
		temp->buffer = (int *) malloc(maxSize);

		if (temp->buffer == NULL) {
			logMessage(LOG_ERROR_LEVEL, "Memory is not allocated successfully");
			return (-1);
		}

		// Enters information into cache entry
		gettimeofday(&tval, NULL);
		temp->clkTick = tval.tv_sec;
		temp->disk = dsk;
		temp->blockID = blk;
		memcpy(temp->buffer, buf, maxSize);

		// Adds cache entry to cache
		cache[initialized] = temp;

		// Initializes least recently used cache to the very first
		// entry in the cache
		if (initialized == 0) leastRecent = cache[0];

		// Increment initialized statistic
		initialized++;
	}

	// Return successfully
	return(0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : get_raid_cache
// Description  : Get an object from the cache (and return it)
//
// Inputs       : dsk - this is the disk number of the block to find
//                blk - this is the block number of the block to find
// Outputs      : pointer to cached object or NULL if not found

void * get_raid_cache(RAIDDiskID dsk, RAIDBlockID blk) {

	// Declares variables
	int i;
	CacheEntry *temp;
	struct timeval tval;

	// Examines the cache if the entry exists
	i = 0;
	while (i < initialized) {
		temp = cache[i];
		if (temp->disk == dsk && temp->blockID == blk) {
			// Updates the clock tick for the cache entry
			gettimeofday(&tval, NULL);
			temp->clkTick = tval.tv_sec;

			// Redetermines the least recently used cache entry
			// if the previous one was accessed again
			if (leastRecent == temp) calc_least_recent();

			// Return the address to cached data
			return (temp->buffer);
		}
		i++;
	}

	// Return NULL
	return(NULL);
}


////////////////////////////////////////////////////////////////////////////////
//
// Function     : calc_least_recent
// Description  : Determines the least recently accessed entry
//

void calc_least_recent() {

	// Declares varables
	CacheEntry *temp;
	int i = 1;
	
	// Searches for the least recently used cache entry
	leastRecent = cache[0];
	while (i < initialized) {
		temp = cache[i];
		if (temp->clkTick > leastRecent->clkTick) leastRecent = temp;
		i++;
	}
}
