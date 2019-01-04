SRC=./src
BUILD=./build
LIB=./lib/lib
INCLUDE=./lib/include

LIBS=-lm -pthread -X11
ARGS=-03 -Wall
OBJECTS=$(BUILD)/
TARGET=./test
LIBTARGET=libsgx.a
CC=gcc
CLEAN=rm -f
COPY=cp -R

$(TARGET): $(OBJECTS)



lib: $(OBJECTS)
	$(COPY) $(SRC)/
	ar rcs $(LIB)/$(LIBTARGET) $(OBJECTS)

clean:
	$(CLEAN) $(OBJECTS)

cleanall:
	$(CLEAN) $(OBJECTS) $(TARGET)
