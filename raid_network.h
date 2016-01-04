#ifndef RAID_NETWORK_INCLUDED
#define RAID_NETWORK_INCLUDED

////////////////////////////////////////////////////////////////////////////////
//
//  File          : raid_network.h
//  Description   : This is the network definitions for  the RAID simulator.
//
//  Author        : Patrick McDaniel
//  Last Modified : Tue Oct 27 07:41:39 PDT 2015
//

// Include Files

// Project Include Files
#include <raid_bus.h>

// Defines
#define RAID_DEFAULT_IP "127.0.0.1"
#define RAID_DEFAULT_PORT 19878

// Address information
extern unsigned char *raid_network_address;  // Address of RAID server
extern unsigned short raid_network_port;     // Port of RAID server

//
// Functional Prototypes

RAIDOpCode client_raid_bus_request(RAIDOpCode op, void *buf);
    // This is the implementation of the client operation (raid_client.c)

#endif
