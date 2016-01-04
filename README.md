# tagline-driver
Penn State Intro to Systems Programming Solo Project

This project implements a tagline device driver by utilizing the RAID software abstraction.
It features a randomized tagline block allocation strategy, a linear tagline to RAID block map using memory allocation,
a disk recovery method, a LRU cache, and a client-side networking RAID function.

Since the project uses a loopback to simulate the network, open two terminal tabs or windows at the project directory.
On one tab/window, do the following commands: 

		% make
		% ./tagline_server -v

After that, go to the other tab/window and do ONE of these commands: 
% ./tagline_client -v workload-linear.dat
% ./tagline_client -v workload-refloc.dat

My work is in the following files: raid_cache.c raid_client.c tagline_driver.c
