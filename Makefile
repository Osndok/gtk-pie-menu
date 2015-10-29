GTKCFLAGS = $(shell gtk-config --cflags) -DDEBUG -DHAVE_SHAPE_EXT
GTKLDFLAGS = $(shell gtk-config --libs)

CFLAGS = -g -Wall $(GTKCFLAGS)
LDFLAGS = -g $(GTKLDFLAGS)

CC = gcc

all: gtk-taste-pie

gtk-taste-pie: gtk-taste-pie.o gdkaddons.o gtkpiemenu.o gtkaddons.o
	$(CC) -o gtk-taste-pie gtk-taste-pie.o gdkaddons.o gtkpiemenu.o gtkaddons.o $(LDFLAGS)

gtk-taste-pie.o: gtk-taste-pie.c gdkaddons.h gtkpiemenu.h
gtk-test-addon.o: gtk-test-addon.c gtkaddons.h

gdkaddons.o: gdkaddons.h gdkaddons.c
gtkpiemenu.o: gdkaddons.h gtkpiemenu.h gtkpiemenu.c gtkaddons.h
gtkaddons.o: gtkaddons.c gtkaddons.h

clean:
	-rm -f *~ *.o gtk-taste-pie gtk-test-addon TAGS

tags: TAGS


TAGS:
	etags *.[ch]
