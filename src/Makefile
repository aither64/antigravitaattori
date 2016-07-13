CC = g++
CFLAGS = -O2 -D_REENTRANT -I/usr/include/SDL2 -Itinyxml -DTIXML_USE_STL \
		 -DDATADIR="\"$(DATADIR)\""
LDFLAGS = -lpng -lalut -lopenal -lGL -lGLU -lSDL2

SOURCES = background.cpp \
	craft.cpp \
	font.cpp \
	game.cpp \
	level.cpp \
	m3dmaterial.cpp \
	m3dmesh.cpp \
	m3dtexture.cpp \
	main.cpp \
	menu.cpp \
	player.cpp \
	ring.cpp \
	terrain.cpp \
	vector2.cpp

OBJECTS = $(subst .cpp,.o,$(SOURCES))

all: $(OBJECTS) tinyxml
	$(CC) -o antigrav $(OBJECTS) tinyxml/libtinyxml.a $(LDFLAGS)

$(OBJECTS): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

tinyxml:
	$(MAKE) -C tinyxml

clean:
	rm -f *.o
	$(MAKE) -C tinyxml clean

.PHONY: all clean tinyxml
