
SRC_H=obj.h scn.h
SRC_C=obj.c scn.c main.c
OBJS=obj.o scn.o main.o
TARGET=test

#CFLAGS=-pedantic -Wall -ansi

INDENT_OPT=-npro -bad -bap  -cdb -sca -bli0 -bls --line-length80 -nbbo -ut -ts4 -sc -npsl -i4 -ncs -npcs -nprs -nss -nip -nlp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@
    
.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

indent: 
	indent $(INDENT_OPT) $(SRC_C) $(SRC_H)