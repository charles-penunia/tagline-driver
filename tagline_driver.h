#ifndef TAGLINE_DRIVER_INCLUDED
#define TAGLINE_DRIVER_INCLUDED

////////////////////////////////////////////////////////////////////////////////
//
//  File           : tagline_driver.h
//  Description    : This is the header file for the driver interface between
//                   the OS and the low-level driver.
//
//  Author         : Patrick McDaniel
//  Created        : Mon Jun 15 07:51:29 EDT 2015
//

// Includes
#include "raid_bus.h"

// Project Includes
#define MAX_TAGLINE_BLOCK_NUMBER  256
#define TAGLINE_BLOCK_SIZE        RAID_BLOCK_SIZE
#define RAID_DISKS                9
#define RAID_DISKBLOCKS           4096

// Type definitions
typedef uint16_t TagLineNumber;
typedef uint32_t TagLineBlockNumber;

//
// Interface functions

int tagline_driver_init(uint32_t maxlines);
	// Initialize the driver with a number of maximum lines to process

int tagline_read(TagLineNumber tag, TagLineBlockNumber bnum, uint8_t blks, char *buf);
	// Read a number of blocks from the tagline driver

int tagline_write(TagLineNumber tag, TagLineBlockNumber bnum, uint8_t blks, char *buf);
	// Write a number of blocks from the tagline driver

int tagline_close(void);
	// Close the tagline interface

int raid_disk_signal(void);
	// A disk has failed which needs to be recovered

RAIDOpCode create_raid_request(uint64_t requestType, uint64_t numBlocks, uint64_t diskNum,
	uint64_t unused, uint64_t status, uint64_t blockID, void *buf);
	// Constructs a request structure and makes a request.

void extract_raid_response (RAIDOpCode resp, int arr[], int arrLen);
	// Extracts an opcode and stores it in an array

int insertEntry (TagLineNumber tagNum, TagLineBlockNumber tagBlockNum, uint8_t blks, char *buf);
	// Creates a new entry with the given opcodes, provided that the tag
	// number and block number combination does not already exist

void* getTagEntry (TagLineNumber tagNum, TagLineBlockNumber tagBlockNum);
	// Extracts the address of the entry with the tagline number and tagline
	// block number combination, if it exists

void* getRAIDEntry (RAIDDiskID diskNum, RAIDBlockID blockIDNum);
	// Extracts the address of the entry with the RAID disk number and block ID
	// combination, if it exists

#endif /* RAID_DRIVER_INCLUDED */
