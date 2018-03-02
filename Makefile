### Voder2017 Makefile
### Creates:
###   voder_ex        - simple example of sounds and filters
###   voder_keyboard  - rudimentary voder
###   voder1          - redesigned voder

PROGRAMS = voder_ex voder_key voder_key2 voder_fall
RM = /bin/rm
SRC_PATH = src/voder
OBJECT_PATH = Release
vpath %.o $(OBJECT_PATH)

INCLUDE = src/voder/include
ifeq ($(strip $(INCLUDE)), )
	INCLUDE = include
endif
vpath %.h $(INCLUDE)

# Set up for STK compilation
CC       = g++
DEFS     = -DHAVE_GETTIMEOFDAY -D__LINUX_ALSA__
DEFS    += -D__LITTLE_ENDIAN__
CFLAGS   = -O3 -Wall
CFLAGS  += -I$(INCLUDE) -I$(INCLUDE)
LIBRARY1  = -lpthread -lasound -lm
LIBRARY2  = -lpthread -lasound -lm -lglfw -lGLEW -lGLU -lGL

REALTIME = yes
ifeq ($(REALTIME),yes)

endif

%.o : $(SRC_PATH)/%.cpp $(OBJECT_PATH)/.placeholder
	$(CC) $(CFLAGS) $(DEFS) -c $(<) -o $(OBJECT_PATH)/$@

%.o : include/%.cpp $(OBJECT_PATH)/.placeholder
	$(CC) $(CFLAGS) $(DEFS) -c $(<) -o $(OBJECT_PATH)/$@

%.o : $(SRC_PATH)/voder/%.cpp $(OBJECT_PATH)/.placeholder
	$(CC) $(CFLAGS) $(DEFS) -c $(<) -o $(OBJECT_PATH)/$@

all : $(PROGRAMS)

$(OBJECT_PATH)/.placeholder:
	mkdir -vp $(OBJECT_PATH)
	touch $(OBJECT_PATH)/.placeholder

$(OBJECTS) : Stk.h

clean : 
	$(RM) -f $(OBJECT_PATH)/*.o
	$(RM) -f $(PROGRAMS) *.exe
	$(RM) -fR *~ *.dSYM

distclean: clean
	$(RM) Makefile

VODER_FILES = $(OBJECT_PATH)/Stk.o $(OBJECT_PATH)/BlitSaw.o $(OBJECT_PATH)/Noise.o $(OBJECT_PATH)/RtAudio.o $(OBJECT_PATH)/FormSwep.o


# BASIC VODER EXAMPLES

voder_ex: Voder_example.cpp Stk.o BlitSaw.o Noise.o RtAudio.o FormSwep.o
		$(CC) $(LDFLAGS) $(CFLAGS) $(DEFS) -o example Voder_example.cpp $(VODER_FILES)  $(LIBRARY1)

voder_key: Voder_keyboard.cpp Window.o Stk.o BlitSaw.o Noise.o RtAudio.o FormSwep.o
		$(CC) $(LDFLAGS) $(CFLAGS) $(DEFS) -o keyboard_example Voder_keyboard.cpp $(VODER_FILES) $(OBJECT_PATH)/Window.o  $(LIBRARY2)

voder_key2: Voder_kb2.cpp Window.o Stk.o BlitSaw.o Noise.o RtAudio.o FormSwep.o
		$(CC) $(LDFLAGS) $(CFLAGS) $(DEFS) -o voder1 Voder_kb2.cpp $(VODER_FILES) $(OBJECT_PATH)/Window.o  $(LIBRARY2)


# FEATHER FALL ENGINE VODER

#Compiler Flags
FF_CFLAGS = -fpermissive
#Source Files in Buffer Subdirectory
FF_BUFFER_FILES = src/buffers/IBO.cpp src/buffers/VAO.cpp src/buffers/VBO.cpp 
#Source Files in Graphics Subdirectory
FF_GRAPHICS_FILES = src/graphics/Camera.cpp src/graphics/Shader.cpp src/graphics/Window.cpp
#Source Files in Logging Subdirectory
#LOGGING_FILES=src/logging/Logging.cpp
#Source Files in Math Subdirectory
FF_MATH_FILES = src/math/mat4.cpp src/math/vec2.cpp src/math/vec3.cpp src/math/vec4.cpp
#Source Files in Src Directory
FF_SRC_FILES = src/main.cpp
#All Source File Names
FF_SOURCES = $(BUFFER_FILES) $(GRAPHICS_FILES) $(MATH_FILES) $(SRC_FILES)

voder_fall: $(FF_SOURCES) $(VODER_FILES)
		$(CC) $(LDFLAGS) $(CFLAGS) $(DEFS) -o voder1 $(FF_SOURCES) $(VODER_FILES) $(LIBRARY2)
