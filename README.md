# tagline-driver
Penn State Intro to Systems Programming Solo Project

# Description
This project implements a tagline device driver by utilizing the RAID software abstraction.
It features a randomized tagline block allocation strategy, a linear tagline to RAID block map using memory allocation,
a disk recovery method, a LRU cache, and a client-side networking RAID function. For more information on the RAID commands and the RAID network protocol, search for the tables within the following links:

- http://www.cse.psu.edu/~pdm12/cmpsc311-f15/cmpsc311-assign2.html
- http://www.cse.psu.edu/~pdm12/cmpsc311-f15/cmpsc311-assign3.html
- http://www.cse.psu.edu/~pdm12/cmpsc311-f15/cmpsc311-assign4.html

# Usage
Since the project uses a loopback to simulate the network, open two terminal tabs or windows at the project directory.
On one tab/window, do the following commands: 

    % make
    % ./tagline_server -v

After that, go to the other tab/window and run the client by typing: 

    % ./tagline_client -v workload-linear.dat
OR

    % ./tagline_client -v workload-refloc.dat

# Files
My work is in the following files: 
- raid_cache.c
- raid_client.c
- tagline_driver.c
