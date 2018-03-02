#include "include/FeatherFallEngine.h"
//std includes
#include <iostream>

//defines
#define WINDOW_FULLSCREEN false
#if WINDOW_FULLSCREEN
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#else
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 576
#endif
#define WINDOW_TITLE "FeatherFall Engine"
#define WINDOW_VSYNC true

/*
 *  The FeatherFall Engine is the second iteration of Try-OS Development Studios
 *  in-house Engine, the first being the failed project of the Temperance Engine.
 *  This Project was started on 12/5/2017 and started development to be a 3D
 *  particle system engine. This project will be used as a learning experiance and
 *  as a capstone for those involvedin its making:
 *    -Steven Savold (GitHub: @TryOSdev-Steven)
 *
 */

using namespace core;
using namespace graphics;
using namespace buffers;
using namespace math;


void update(const Window& window, Camera* camera, SoundGens& base);

int main() {

	//Log::init();

	//Log::printText("White for Nominal Message.");
	//Log::printWarning("Yellow for Warning Message.");
	//Log::printError("Red for Error Message.");

	Window window = Window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, WINDOW_FULLSCREEN, WINDOW_VSYNC);

	window.setClearColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));

	
	//========================
	//voder setup


	// Set global sample rate
	Stk::setSampleRate(48000.0);

	// Setup RT Audio
	RtAudio dac;

	// Setup SoundGens and Filters
	SoundGens base;
	set_up(base);

	// Setup stream
	RtAudio::StreamParameters params;
	params.deviceId = dac.getDefaultOutputDevice();
	params.nChannels = 2;
	// check if 64 or 32 bit
	RtAudioFormat format = (sizeof(StkFloat) == 8) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
	unsigned int bufferFrames = RT_BUFFER_SIZE;
	try {
		dac.openStream(&params, NULL, format, (unsigned int)Stk::sampleRate(),
			&bufferFrames, &tick, (void *)&base);
	}
	catch (RtAudioError &error) {
		error.printMessage();
		goto cleanup;
	}

	// Run Stream
	try {
		dac.startStream();
	}
	catch (RtAudioError &error) {
		error.printMessage();
		goto cleanup;
	}

	// Display keys
	chart();


	//========================
	
	
	float verts[] = {
		-0.5f, -0.5f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,//0
		 0.5f, -0.5f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,//1
		 0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,//2
		-0.5f,  0.5f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f //3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	VAO vao = VAO();
	vao.bind();

	VBO vbo = VBO();
	vbo.bind();
	VBO::addBufferData(4 * 7 * sizeof(float), verts, GL_STATIC_DRAW);

	IBO ibo = IBO();
	ibo.bind();
	IBO::addBufferData(6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	VAO::addVertexAtribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);
	VAO::addVertexAtribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(sizeof(float) * 3));
	
	vec4 color(0.2f, 0.4f, 0.8f, 1.0f);
	mat4 proj = mat4::persp(90, 16.0f / 9.0f, 0.0f, 1000.0f);
	mat4 view;
	mat4 rot = mat4::rotation(20.0f, vec3(0.0f, 0.0f, 1.0f));

	Camera camera = Camera(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), 
		vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 1000.0f, 
		WINDOW_WIDTH, WINDOW_HEIGHT);
	

	Shader basic("res/shaders/Basic.shader");
	basic.bind();
	Shader::setUniformVec4(basic.getUniformLocation("col"), color);
	Shader::setUniformMat4(basic.getUniformLocation("rotMat"), rot);

	while (!window.windowShouldClose()) {
		window.clear();
		update(window, &camera, base);

		proj = camera.getProjectionMatrix();
		view = camera.getViewMatrix();

		//std::cout << camera.eye << std::endl;

		Shader::setUniformMat4(basic.getUniformLocation("projMat"), proj);
		Shader::setUniformMat4(basic.getUniformLocation("viewMat"), view);
		Shader::setUniformVec2(basic.getUniformLocation("win_size"), window.getWindowSize());
		Shader::setUniformVec2(basic.getUniformLocation("mousePos"), window.getMousePos());
		
		//TODO: put this in a RENDERER class
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

		window.update();

		
	}

	// Shutdown Stream
	try {
		dac.closeStream();
	}
	catch (RtAudioError &error) {
		error.printMessage();
	}

cleanup:
	clean_up(base);
	return 0;

}

void update(const Window& window, Camera* camera, SoundGens& base) {
	// Check which keys are being pressed
	read_keys(&window, base);

}
