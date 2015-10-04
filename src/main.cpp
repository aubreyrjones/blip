#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <maximilian.h>
#include "WavReader.h"

int main(int argc, char **argv) {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(1920, 1080, "blip", nullptr, nullptr);
	if (!window){
		return 0;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
		return -1;
	}


	WavReader wav(argv[0]);
	if (!wav.initialize()){
		std::cout << "Cannot read wave file." << std::endl;
		return -1;
	}

	while (!glfwWindowShouldClose(window)){
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
