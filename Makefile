CC      = gcc
PROF    = -O3 -ggdb3
NOCRYPT = -lcrypt
C_FLAGS = -Wall -Wshadow -Wcast-qual -Wcast-align $(PROF) 
L_FLAGS =  $(PROF) $(NOCRYPT)

O_FILES = act_comm.o act_enter.o act_info.o act_move.o act_obj.o act_wiz.o \
          alias.o ban.o comm.o const.o db.o db2.o effects.o fight.o flags.o \
	  handler.o healer.o interp.o note.o lookup.o magic.o magic2.o \
	  music.o quest.o recycle.o remort.o save.o scan.o skills.o special.o tables.o \
	  update.o olc.o olc_act.o olc_save.o bit.o mem.o string.o webwho.o

magrathea: $(O_FILES)
	rm -f magrathea
	$(CC) $(L_FLAGS) -o magrathea $(O_FILES)

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<
