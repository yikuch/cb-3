#!/usr/bin/make
.SUFFIXES:
.PHONY: all run pack clean

PCK = lab-3.zip
LEX = flex
TAR = minako

CSRC = minako-syntax.c
LSRC = minako-lexic.l
OBJ = $(LSRC:%.l=%.o) $(CSRC:%.c=%.o)
DEP = $(OBJ:%.o=%.d)
-include $(DEP)

CFLAGS = -std=c11 -Wall -pedantic -MMD -MP -DLEXDEBUG
LFLAGS = -t

%.c: %.l
	$(LEX) $(LFLAGS) $< > $@
%.o: %.c
	$(CC) $(CFLAGS) -c $<

$(TAR): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

all: $(TAR)

run: all
	./$(TAR) beispiel.c-1

pack:
	zip -vj $(PCK) $(CSRC)

clean:
	$(RM) $(RMFILES) $(TAR) $(OBJ) $(DEP) $(PCK) $(LSRC:%.l=%.c)
