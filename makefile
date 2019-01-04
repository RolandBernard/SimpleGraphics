SRC=./src
MSRC=$(SRC)/math
BUILD=./build
MBUILD=$(BUILD)/math
LIB=./lib/lib
INCLUDE=./lib/include

LIBS=-lm -pthread -X11
ARGS=-03 -Wall
OBJECTS=$(MBUILD)/vec2.o $(MBUILD)/vec3.o $(MBUILD)/vec4.o $(MBUILD)/mat2.o $(MBUILD)/mat3.o $(MBUILD)/mat4.o $(MBUILD)/transform.o\
$(BUILD)/gx.o
TARGET=./test
LIBTARGET=libsgx.a
CC=gcc
CLEAN=rm -f
COPY=cp -R

$(TARGET): $(OBJECTS)


$(BUILD)/gx.o: $(SRC)gx.c $(SRC)/gx.h $(MSRC)/vec2.h $(MSRC)/vec3.h $(MSRC)/vec4.h
	$(CC) $(ARGS) -c -o $(BUILD)/gx.o $(SRC)/gx.c

$(MBUILD)/vec2.o: $(MSRC)/vec2.c $(MSRC)/vec2.h $(MSRC)/scalar.h
	$(CC) $(ARGS) -c -o $(MBUILD)/vec2.o $(MSRC)/vec2.c

$(MBUILD)/vec3.o: $(MSRC)/vec3.c $(MSRC)/vec3.h $(MSRC)/scalar.h
	$(CC) $(ARGS) -c -o $(MBUILD)/vec3.o $(MSRC)/vec3.c

$(MBUILD)/vec4.o: $(MSRC)/vec4.c $(MSRC)/vec4.h $(MSRC)/scalar.h
	$(CC) $(ARGS) -c -o $(MBUILD)/vec4.o $(MSRC)/vec4.c

$(MBUILD)/mat2.o: $(MSRC)/mat2.c $(MSRC)/mat2.h $(MSRC)/vec2.h $(MSRC)/scalar.h
	$(CC) $(ARGS) -c -o $(MBUILD)/mat2.o $(MSRC)/mat2.c

$(MBUILD)/mat3.o: $(MSRC)/mat3.c $(MSRC)/mat3.h $(MSRC)/mat2.h $(MSRC)/vec3.h $(MSRC)/scalar.h
	$(CC) $(ARGS) -c -o $(MBUILD)/mat3.o $(MSRC)/mat3.c

$(MBUILD)/mat4.o: $(MSRC)/mat4.c $(MSRC)/mat4.h $(MSRC)/mat3.h $(MSRC)/vec4.h $(MSRC)/scalar.h
	$(CC) $(ARGS) -c -o $(MBUILD)/mat4.o $(MSRC)/mat4.c

$(MBUILD)/transform.o: $(MSRC)/transform.c $(MSRC)/transform.h $(MSRC)/mat2.h $(MSRC)/mat3.h $(MSRC)/mat4.h $(MSRC)/vec2.h $(MSRC)vec3.h $(MSRC)/scalar.h
	$(CC) $(ARGS) -c -o $(MBUILD)/transform.o $(MSRC)/transform.c

lib: $(OBJECTS)
	$(COPY) $(SRC)/math/{scalar.h,vec2.h,vec3.h,vec4.h,mat2.h,mat3.h,mat4.h,transform.h,math.h} $(INCLUDE)/math/
	$(COPY) $(SRC)/gx.h $(INCLUDE)/
	ar rcs $(LIB)/$(LIBTARGET) $(OBJECTS)

clean:
	$(CLEAN) $(OBJECTS)

cleanall:
	$(CLEAN) $(OBJECTS) $(TARGET)
