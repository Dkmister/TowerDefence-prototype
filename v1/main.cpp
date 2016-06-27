#include "stdafx.h"

#include "DemoScene.h"
#include "HeightScene.h"
#include "GameScene.h"

const GLint g_width = 900;
const GLint g_height = 600;

static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
}

static void windowSizeCallback(GLFWwindow* window, int width, int height) {
   glViewport(0, 0, width, height);
}

static GLFWwindow* glInit(GLint width, GLint height) {
   GLFWwindow* window;

   if (!glfwInit())
      exit(EXIT_FAILURE);

   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

   glfwWindowHint(GLFW_SAMPLES, 4);

   window = glfwCreateWindow(width, height, "LAB 06 - 2016 - dev", NULL, NULL);
   if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
   }

   glfwMakeContextCurrent(window);
   glfwSwapInterval(1);

   glfwSetKeyCallback(window, glfwKeyCallback);

   glfwSetWindowSizeCallback(window, windowSizeCallback);

   if (gl3wInit() || !gl3wIsSupported(3, 0)) {
      fprintf(stderr, "failed to initialize OpenGL\n");
      return nullptr;
   }

   printf("OpenGL %s, GLSL %s\n\n", glGetString(GL_VERSION),
      glGetString(GL_SHADING_LANGUAGE_VERSION));

   return window;
}

int main(int argc, char* argv[]) {
   GLFWwindow* window = glInit(g_width, g_height);

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

   // Enable depth test
   glEnable(GL_DEPTH_TEST);

   // Cull triangles which normal is not towards the camera
   glEnable(GL_CULL_FACE);

   //glEnable(GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// DemoScene scene;
	//HeightScene scene;
   GameScene scene;

   while (!glfwWindowShouldClose(window)) {
      scene.render(window);
      
      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glfwDestroyWindow(window);

   glfwTerminate();
   exit(EXIT_SUCCESS);
}
