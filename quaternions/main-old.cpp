#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// SHADERS
const GLchar *vertexSource = R"glsl(
#version 150 core

in vec3 position;
in vec3 color;

out vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


void main()
{
    Color = color;
    gl_Position = proj * view * vec4(position, 1.0);
}
)glsl";

const GLchar *fragmentSource = R"glsl(
 #version 150 core

in vec3 Color;

out vec4 outColor;

void main()
{
    outColor = vec4(Color, 1.0);
}
)glsl";

GLFWwindow *windowInit();
GLint shaderCompilation();

int main() {
  glfwInit();

  // CALL WINDOW INIT BEFORE BUFFER DECLARATION BECAUSE OF GLEWINIT
  GLFWwindow *window = windowInit();

  // TRIANGLE DATA
  GLfloat vertices[] = {
      // +z

      -.5, -.5, .5, 1.0, 0.0, 0.0, // VERT 1 RED
      .5, .5, .5, 0.0, 1.0, 0.0,   // VERT 2 GREEN
      .5, -.5, .5, 1.0, 1.0, 1.0,  // VERT 3 WHITE
      -.5, -.5, .5, 0.0, 0.0, 1.0, // VERT 4 BLUE

      // -z
      -.5, .5, -.5, 1.0, 1.0, 0.0,  // VERT 5 YELLOW
      .5, .5, -.5, 1.0, 0.0, 1.0,   // VERT 6 MAGENTA
      -.5, -.5, -.5, 0.0, 1.0, 1.0, // VERT 7 CYAN
      .5, -.5, -.5, 0.0, 0.0, 0.0   // VERT 8 BLACK

  };

  GLuint elements[] = {
      0, 1, 2, // TRI 1
      2, 3, 0, // TRI 2
      1, 4, 7, // TRI 3
      7, 2, 1, // TRI 4
      4, 5, 6, // TRI 5
      6, 7, 4, // TRI 6
      2, 7, 6, // TRI 7
      6, 3, 2, // TRI 8
      0, 5, 4, // TRI 9
      4, 1, 0, // TRI 10
      3, 6, 5, // TRI 11
      5, 0, 3  // TRI 12

  };

  // VBO
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // VAO
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // EBO
  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements,
               GL_STATIC_DRAW);

  // SHADER COMPILATION AND LINKING //
  GLint shaderProgram = shaderCompilation();
  glUseProgram(shaderProgram);

  // SPECIFY LAYOUT OF THE VERTEX DATA //
  // position //
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
  // color//
  GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
  glEnableVertexAttribArray(colAttrib);
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  // UNIFORM FRAG COLOR //
  GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
  glUniform3f(uniColor, 1.0f, 1.0f, 0.0f);

  // MODEL
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  GLint uniTrans = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));

  // VIEW AND PROJECTION MATRICES // maybe split to a new function??
  glm::mat4 view =
      glm::lookAt(glm::vec3(1.2f, 1.2f, 1.2f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f));

  GLint uniView = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
  glEnable(GL_DEPTH_TEST);

  glm::mat4 proj =
      glm::perspective(glm::radians(45.0f), 800.f / 800.0f, 1.0f, 10.0f);

  GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
  glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

  // MAIN LOOP //
  while (!glfwWindowShouldClose(window)) {

    // draw a triangle frm 3 verts
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    // swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GL_TRUE);

      glDeleteProgram(shaderProgram);
      glDeleteBuffers(1, &vbo);
      glDeleteVertexArrays(1, &vao);
      glfwTerminate();
    }
  }
}

GLFWwindow *windowInit() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow *window = glfwCreateWindow(800, 800, "OpenGL", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  glewInit();

  return window;
}

GLint shaderCompilation() {
  // VERTEX SHADER//
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);
  // vertex debug//
  GLint statusVertex;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &statusVertex);
  char bufferVertex[512];
  glGetShaderInfoLog(vertexShader, 512, NULL, bufferVertex);
  if (statusVertex == GL_TRUE) {
    printf("VERTEX SHADER COMPILED\n");
  } else {
    printf("VERTEX SHADER FAILED TO COMPILE");
  }

  // FRAGMENT SHADER//
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  // fragment debug//
  GLint statusFragment;
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &statusFragment);
  char bufferFragment[512];
  glGetShaderInfoLog(fragmentShader, 512, NULL, bufferFragment);
  if (statusFragment == GL_TRUE) {
    printf("FRAGMENT SHADER COMPILED\n");
  } else {
    printf("FRAGMENT SHADER FAILED TO COMPILE");
  }

  // SHADER LINKING//
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);

  return shaderProgram;
}
