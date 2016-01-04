#
# CMPSC311 - F15 Assignment #2
# Makefile - makefile for the assignment
#

# Locations
CMPSC311_LIBDIR=/home/mcdaniel/dev/libcmpsc311/
                    
# Make environment
INCLUDES=-I. -I$(CMPSC311_LIBDIR)
CC=gcc
CFLAGS=-I. -c -g -Wall $(INCLUDES)
LINKARGS=-g
LIBS=-lm -lcmpsc311 -L. -L$(CMPSC311_LIBDIR) -lgcrypt -lpthread -lcurl
                    
# Suffix rules
.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS)  -o $@ $<
	
# Files
TARGETS=    tagline_client

CLIENT_OBJECT_FILES=	tagline_sim.o \
				        tagline_driver.o \
				        raid_cache.o \
                        raid_client.o 
				
# Productions
all : $(TARGETS)

tagline_client: $(CLIENT_OBJECT_FILES)
	$(CC) $(LINKARGS) $(CLIENT_OBJECT_FILES) -o $@ $(LIBS)

clean : 
	rm -f $(TARGETS) $(CLIENT_OBJECT_FILES)
	
