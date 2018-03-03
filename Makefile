### Voder2017 Makefile
### Creates:
###   voder_ex        - simple example of sounds and filters
###   voder_keyboard  - rudimentary voder
###   voder1          - redesigned voder

PROGRAMS = voder_ex voder_key voder_key2 voder_fall
RM = /bin/rm
SRC_PATH = src/
OBJECT_PATH = Release
vpath %.o $(OBJECT_PATH)

INCLUDE = src/
INCLUDE2 = src/voder/include
ifeq ($(strip $(INCLUDE)), )
	INCLUDE = include
endif
vpath %.h $(INCLUDE)

# === Setup ===

# Compiler
CC       = g++
# STK Defines
DEFS     = -DHAVE_GETTIMEOFDAY -D__LINUX_ALSA__
DEFS    += -D__LITTLE_ENDIAN__
# STK Flags
CFLAGS   = -O3 -Wall
CFLAGS  += -I$(INCLUDE) -I$(INCLUDE2)
# Featherfall Flags
FF_CFLAGS = -std=c++11 #-fpermissive
# Basic Voder example libraries
LIBRARY1  = -lpthread -lasound -lm
# Full Voder libraries
LIBRARY2  = -lpthread -lasound -lm -lglfw -lGLEW -lGLU -lGL

REALTIME = yes
ifeq ($(REALTIME),yes)

endif

# STK files
%.o : $(SRC_PATH)/voder/%.cpp $(OBJECT_PATH)/.placeholder
	$(CC) $(CFLAGS) $(DEFS) -c $(<) -o $(OBJECT_PATH)/$@

# === Featherfall files ===

# Buffer files
%.o : $(SRC_PATH)/buffers/%.cpp $(OBJECT_PATH)/.placeholder
	$(CC) $(CFLAGS) $(FF_CFLAGS) -c $(<) -o $(OBJECT_PATH)/$@

# Math files
%.o : $(SRC_PATH)/math/%.cpp $(OBJECT_PATH)/.placeholder
	$(CC) $(CLFAGS) $(FF_CFLAGS) -c $(<) -o $(OBJECT_PATH)/$@

all : $(PROGRAMS)

$(OBJECT_PATH)/.placeholder:
	mkdir -vp $(OBJECT_PATH)
	touch $(OBJECT_PATH)/.placeholder

clean : 
	$(RM) -f $(OBJECT_PATH)/*.o
	$(RM) -f $(PROGRAMS) *.out
	$(RM) -fR *~ *.dSYM

distclean: clean
	$(RM) Makefile

### === BASIC VODER EXAMPLE PROGRAMS ===

VODER_OBJS = Stk.o BlitSaw.o Noise.o RtAudio.o FormSwep.o

VODER_FILES = $(OBJECT_PATH)/Stk.o $(OBJECT_PATH)/BlitSaw.o $(OBJECT_PATH)/Noise.o $(OBJECT_PATH)/RtAudio.o $(OBJECT_PATH)/FormSwep.o

voder_ex: Voder_example.cpp $(VODER_OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(DEFS) -o example.out Voder_example.cpp $(VODER_FILES)  $(LIBRARY1)

voder_key: Voder_keyboard.cpp Old_Window.o $(VODER_OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(DEFS) -o keyboard_example.out Voder_keyboard.cpp $(VODER_FILES) $(OBJECT_PATH)/Old_Window.o  $(LIBRARY2)

voder_key2: Voder_kb2.cpp Old_Window.o $(VODER_OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(DEFS) -o voder1.out Voder_kb2.cpp $(VODER_FILES) $(OBJECT_PATH)/Old_Window.o  $(LIBRARY2)


### === Featherfall Voder Program ===

FF_OBJS  = IBO.o VAO.o VBO.o mat4.o vec2.o vec3.o vec4.o

BUFFER_FILES = $(OBJECT_PATH)/IBO.o $(OBJECT_PATH)/VAO.o $(OBJECT_PATH)/VBO.o
MATH_FILES = $(OBJECT_PATH)/mat4.o $(OBJECT_PATH)/vec2.o $(OBJECT_PATH)/vec3.o $(OBJECT_PATH)/vec4.o
#GRAPHICS_FILES = $(OBJECT_PATH)/Camera.o  $(OBJECT_PATH)/Shader.o $(OBJECT_PATH)/Window.o

FF_FILES = $(BUFFER_FILES) $(MATH_FILES)




GRAPHICS_FILES = src/graphics/Camera.cpp src/graphics/Shader.cpp src/graphics/Window.cpp

SRC_FILES = src/main.cpp

FF_SOURCES = $(GRAPHICS_FILES) $(SRC_FILES)


voder_fall: $(FF_SOURCES) $(FF_OBJS) $(VODER_OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(FF_CFLAGS) $(DEFS) -o VoderEngine.out $(FF_SOURCES) $(VODER_FILES) $(FF_FILES) $(LIBRARY2)

