////////////////////////////////////////////////////////////////////////////////
//
//  File          : raid_client.c
//  Description   : This is the client side of the RAID communication protocol.
//
//  Author        : Charles Penunia
//  Last Modified : 18 November 2015
//

// Include Files
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <stdint.h>

// Project Include Files
#include <raid_network.h>
#include <cmpsc311_log.h>
#include <cmpsc311_util.h>
#include <raid_bus.h>

// Defines
#define FAILURE_STATUS 		0x100000000
#define SHIFT_FOR_REQUEST 	56
#define SHIFT_FOR_BLOCKS 	48
#define STRUCTURE_FOR_BLOCKS	0xFF

// Global data
unsigned char *raid_network_address = NULL; // Address of CRUD server
unsigned short raid_network_port = 0; // Port of CRUD server
int sckt;
struct sockaddr_in v4;

//
// Functions

////////////////////////////////////////////////////////////////////////////////
//
// Function     : client_raid_bus_request
// Description  : This the client operation that sends a request to the RAID
//                server.   It will:
//
//                1) if INIT make a connection to the server
//                2) send any request to the server, returning results
//                3) if CLOSE, will close the connection
//
// Inputs       : op - the request opcode for the command
//                buf - the block to be read/written from (READ/WRITE)
// Outputs      : the response structure encoded as needed

RAIDOpCode client_raid_bus_request(RAIDOpCode op, void *buf) {

	// Declares local variables
	RAIDOpCode opNet;
	int requestType, blks;
	uint64_t bufLen, bufLenNet;

	// Extracts the opcode
	requestType = op >> SHIFT_FOR_REQUEST;
	blks = (op >> SHIFT_FOR_BLOCKS) & STRUCTURE_FOR_BLOCKS;

	// Assigns the buffer length
	bufLen = blks * RAID_BLOCK_SIZE;

	// Connects to a RAID server
	if (requestType == RAID_INIT) {
		// Initializes port and family
		v4.sin_port = htons(RAID_DEFAULT_PORT);
		v4.sin_family = AF_INET;

		// Gets the address
		if (inet_aton(RAID_DEFAULT_IP, &(v4.sin_addr)) == 0) {
			logMessage(LOG_ERROR_LEVEL, "Getting address failed");
			return op | FAILURE_STATUS;
		}

		// Creates a socket
		sckt = socket(AF_INET, SOCK_STREAM, 0);
		if (sckt == -1) {
			logMessage(LOG_ERROR_LEVEL, "Creating socket failed");
			return op | FAILURE_STATUS;
		}

		// Connects to a RAID server
		if (connect(sckt, (const struct sockaddr *) &v4, sizeof(struct sockaddr)) == -1) {
			logMessage(LOG_ERROR_LEVEL, "Connecting to server failed");
			return op | FAILURE_STATUS;
		}

		// Reassigns the buffer length
		bufLen = 0;
	}
	
	// Converts opcode and buffer length into network byte order
	opNet = htonll64(op);
	bufLenNet = htonll64(bufLen);

	// Sends the opcode, buffer length, and buffer for any RAID command
	if (write(sckt, &opNet, (size_t) 8) != (ssize_t) 8) {
		logMessage(LOG_ERROR_LEVEL, "Writing opcode failed");
		return op | FAILURE_STATUS;
	}

	if (write(sckt, &bufLenNet, (size_t) 8) != (ssize_t) 8) {
		logMessage(LOG_ERROR_LEVEL, "Writing buffer length failed");
		return op | FAILURE_STATUS;
	}

	if (write(sckt, buf, (size_t) bufLen) != (ssize_t) bufLen) {
		logMessage(LOG_ERROR_LEVEL, "Writing buffer failed");
		return op | FAILURE_STATUS;
	}

	// Reads the opcode, buffer length, and buffer for any RAID command
	if (read(sckt, &opNet, (size_t) 8) != (ssize_t) 8) {
		logMessage(LOG_ERROR_LEVEL, "Reading opcode failed.");
		return op | FAILURE_STATUS;
	}

	if (read(sckt, &bufLenNet, (size_t) 8) != (ssize_t) 8) {
		logMessage(LOG_ERROR_LEVEL, "Reading buffer length failed.");
		return op | FAILURE_STATUS;
	}

	if (read(sckt, buf, (size_t) bufLen) != (ssize_t) bufLen) {
		logMessage(LOG_ERROR_LEVEL, "Reading buffer failed.");
		return op | FAILURE_STATUS;
	}

	// Converts data
	op = ntohll64(opNet);
	bufLen = ntohll64(bufLenNet);

	// Disconnects from the RAID server
	if (requestType == RAID_CLOSE) {
		// Closes the socket
		close(sckt);
		sckt = -1;
	}

    	return op;
}

