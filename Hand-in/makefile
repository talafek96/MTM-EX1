CC=gcc
OBJS=election.o map.o electionTestsExample.o
EXEC=election
DEBUG_FLAG=
COMP_FLAG=-std=c99 -Wall -Werror -pedantic-errors -DNDEBUG

$(EXEC) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@
election.o : election.c election.h mtm_map/map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
map.o : mtm_map/map.c mtm_map/map.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) mtm_map/$*.c
electionTestsExample.o : tests/electionTestsExample.c election.h mtm_map/map.h \
test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) tests/$*.c
clean :
	rm -f $(OBJS) $(EXEC)