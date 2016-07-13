CC = g++
CFLAGS = -W -Wall -DTIXML_USE_STL
LDFLAGS =
TARGET = libtinyxml.a

SOURCES = tinyxml.cpp \
	tinyxmlerror.cpp \
	tinyxmlparser.cpp \
	tinystr.cpp

OBJECTS = $(subst .cpp,.o,$(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	ar crs $(TARGET) $(OBJECTS)

$(OBJECTS): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o

.PHONY: all clean
