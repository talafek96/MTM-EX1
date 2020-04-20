CC=gcc
OBJS=election.o map.o electionTestsExample.o
EXEC=election
DEBUG_FLAG=
COMP_FLAG=-std=c99 -Wall -Werror -pedantic-errors

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@
election.o : election.c election.h map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
map.o : mtm_map/map.c mtm_map/map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
electionTestsExample.o : tests/electionTestsExample.c election.h mtm_map/map.h \
test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
clean :
	rm -f $(OBJS) $(EXEC)