#include "Window.h"


using namespace std;

Window* window;

Window::Window(char* window_name, float _width, float _height)
{
	name = window_name;
	width = _width;
	height = _height;
}


Window::~Window()
{
	glfwTerminate();
}

/*
void cleanup() {
	for (int i = 0; i < VAOS.size(); ++i)
		free(VAOS[i]);
}
*/

int Window::init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, name, NULL, NULL);



	if (window == NULL)
	{
		cout << "ERROR: GLFW Window creation failed!" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR: GLAD Initialization failed!" << std::endl;
		return -1;
	}

	//bunch of call back functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);


	glEnable(GL_DEPTH_TEST); //z buffer
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //remove cursor




	glViewport(0, 0, width, height);
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (spheres[0]->roughness < 0.99f) spheres[0]->roughness += 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (spheres[0]->roughness > 0.05f) spheres[0]->roughness -= 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		if (spheres[0]->metallic > 0.01f) spheres[0]->metallic -= 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		if (spheres[0]->metallic < 0.99f) spheres[0]->metallic += 0.01;
	}

	if (cam != NULL) {
		float cameraSpeed = cam->camProtoSpeed * cam->deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cam->cameraPos += cameraSpeed * cam->cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cam->cameraPos -= cameraSpeed * cam->cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cam->cameraPos -= normalize(cross(cam->cameraFront, cam->cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cam->cameraPos += normalize(cross(cam->cameraFront, cam->cameraUp)) * cameraSpeed;
	}
	else cout << "cam == NULL!" << endl;
}


void Window::loop() {
	drawerinit();

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		logic();
		draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	cleanup();
}