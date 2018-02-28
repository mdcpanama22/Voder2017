#makefile for VoderEngine-core
#Febuary 27, 2018
#
#Overview:
#    target [all]: 
#        will compile and link the whole project
#
#    target [buffers]: 
#        will compile the buffers subdirectory into object files
#
#    target [graphics]: 
#        will compile the graphics subdirectory into object files
#
#    target [logging]:
#        will compile the logging subdirectory into object files
#
#    target [math]: 
#        will compile the math subdirectory into object files
#
#    target [clean]: 
#        will clean the whole project removing all .o files and executables
#

#Compiler to use
CC=g++ 

#Compiler Flags
CFLAGS=-fpermissive

#Linker Flags
LDFLAGS=-lpthread -lasound -lm -lglfw -lGLEW -lGLU -lGL

#Output File Name
OUTPUT=VoderEngine

#Source Files in Buffer Subdirectory
BUFFER_FILES=src/buffers/IBO.cpp src/buffers/VAO.cpp src/buffers/VBO.cpp 

#Source Files in Graphics Subdirectory
GRAPHICS_FILES=src/graphics/Camera.cpp src/graphics/Shader.cpp src/graphics/Window.cpp

#Source Files in Logging Subdirectory
#LOGGING_FILES=src/logging/Logging.cpp

#Source Files in Math Subdirectory
MATH_FILES=src/math/mat4.cpp src/math/vec2.cpp src/math/vec3.cpp src/math/vec4.cpp

#Source Files in Src Directory
SRC_FILES=src/main.cpp

#All Source File Names
SOURCES=$(BUFFER_FILES) $(GRAPHICS_FILES) $(MATH_FILES) $(SRC_FILES)


all: 
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $(OUTPUT)

