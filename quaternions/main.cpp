#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <cmath>
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

uniform mat4 quatRot;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    Color = color;
    gl_Position = proj * view * quatRot* vec4(position, 1.0);
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

glm::vec4 quaternionMultiplication(glm::vec4 quaternionA,
                                   glm::vec4 quaternionB);
glm::vec4 eulerToQuaternion(glm::vec4 angleAxis);
glm::mat4 quaternionToMatrix(glm::vec4 quaternion);

int main() {
  auto t_start = std::chrono::high_resolution_clock::now();
  glfwInit();

  // CALL WINDOW INIT BEFORE BUFFER DECLARATION BECAUSE OF GLEWINIT
  GLFWwindow *window = windowInit();

  // TRIANGLE DATA
  GLfloat vertices[] = {
      // +z
      -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // VERT // SIDE 1 // RED
      0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // VERT
      0.5f,  0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, // VERT
      0.5f,  0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, // VERT
      -0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, // VERT
      -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // VERT

      -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 0.0f, // VERT // SIDE 2 // YELLOW
      0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 0.0f, // VERT
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, // VERT
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, // VERT
      -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 0.0f, // VERT
      -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 0.0f, // VERT

      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // VERT // SIDE 3 // GREEN
      -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, // VERT
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // VERT
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // VERT
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, // VERT
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // VERT

      0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // VERT // SIDE 4 // CYAN
      0.5f,  0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, // VERT
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // VERT
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // VERT
      0.5f,  -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, // VERT
      0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // VERT

      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // VERT // SIDE 5 // YELLOW
      0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // VERT
      0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 0.0f, // VERT
      0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 0.0f, // VERT
      -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 0.0f, // VERT
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, // VERT

      -0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, 1.0f, // VERT 1 // SIDE 6 // MAGENTA
      0.5f,  0.5f,  -0.5f, 1.0f, 0.0f, 1.0f, // VERT 2
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // VERT 1
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // VERT 2
      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, 1.0f, // VERT 1
      -0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, 1.0f  // VERT 2
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

  ///////////////////////////////////
  // SPECIFY LAYOUT OF VERTEX DATA //
  ///////////////////////////////////

  // position //
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
  // color//
  GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
  glEnableVertexAttribArray(colAttrib);
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  //////////////
  // UNIFORMS //
  //////////////

  // frag color //
  GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
  glUniform3f(uniColor, 1.0f, 1.0f, 0.0f);

  // VIEW AND PROJECTION MATRICES // maybe split to a new function??
  glm::mat4 view =
      glm::lookAt(glm::vec3(1.2f, 1.2f, 1.2f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f));

  GLint uniView = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
  glEnable(GL_DEPTH_TEST);

  glm::mat4 proj =
      glm::perspective(glm::radians(90.0f), 800.f / 800.0f, 1.0f, 10.0f);

  GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
  glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

  GLint uniRot = glGetUniformLocation(shaderProgram, "quatRot");

  // MAIN LOOP //
  while (!glfwWindowShouldClose(window)) {

    // draw a triangle frm 3 verts
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    auto t_now = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration_cast<std::chrono::duration<float>>(
                     t_now - t_start)
                     .count();
    /////////////////////////
    // QUATERNION ROTATION //
    /////////////////////////

    // Euler Axis Angle //
    // glm::vec4 axisAngleA(glm::radians(100.0) * time, 5, 8, 60);
    // glm::vec4 axisAngleB(glm::radians(45.0) * time, 60, 1, -32);

    // Sans time //
    glm::vec4 axisAngleA(glm::radians(100.0), 5, 8, 60);
    glm::vec4 axisAngleB(glm::radians(45.0), 60, 1, -32);

    // Euler Angles to Quaternion//
    glm::vec4 quaternionA = eulerToQuaternion(axisAngleA);
    glm::vec4 quaternionB = eulerToQuaternion(axisAngleB);

    // Quaternion to Rotation Matrix //
    glm::mat4 quatRot =
        quaternionToMatrix(quaternionMultiplication(quaternionA, quaternionB));
    glUniformMatrix4fv(uniRot, 1, GL_FALSE, glm::value_ptr(quatRot));
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

glm::vec4 quaternionMultiplication(glm::vec4 quaternionA,
                                   glm::vec4 quaternionB) {

  glm::vec4 quaternionC;

  quaternionC[0] =
      (quaternionA[0] * quaternionB[0]) - (quaternionA[1] * quaternionB[1]) -
      (quaternionA[2] * quaternionB[2]) - (quaternionA[3] * quaternionB[3]);

  quaternionC[1] =
      (quaternionA[0] * quaternionB[1]) + (quaternionA[1] * quaternionB[0]) +
      (quaternionA[2] * quaternionB[3]) - (quaternionA[3] * quaternionB[2]);

  quaternionC[2] =
      (quaternionA[0] * quaternionB[2]) - (quaternionA[1] * quaternionB[3]) +
      (quaternionA[2] * quaternionB[0]) + (quaternionA[3] * quaternionB[1]);

  quaternionC[3] =
      (quaternionA[0] * quaternionB[3]) + (quaternionA[1] * quaternionB[2]) -
      (quaternionA[2] * quaternionB[1]) + (quaternionA[3] * quaternionB[0]);
  return quaternionC;
}

glm::vec4 eulerToQuaternion(glm::vec4 axisAngle) {
  // q₀ = cos(theta/2)
  // q₁ = normalizedx * sin(theta/2)
  // q₂ = normalizedy * sin(theta/2)
  // q₃ = normalizedz * sin(theta/2)

  // float min = std::min({axisAngle[1], axisAngle[2], axisAngle[3]});
  // float max = std::max({axisAngle[1], axisAngle[2], axisAngle[3]});

  float magnitude =
      std::sqrt(axisAngle[1] * axisAngle[1] + axisAngle[2] * axisAngle[2] +
                axisAngle[3] * axisAngle[3]);

  float normalizedX = (axisAngle[1] / magnitude);
  float normalizedY = (axisAngle[2] / magnitude);
  float normalizedZ = (axisAngle[3] / magnitude);

  float q0 = cos(axisAngle[0] / 2);
  float q1 = normalizedX * sin(axisAngle[0] / 2);
  float q2 = normalizedY * sin(axisAngle[0] / 2);
  float q3 = normalizedZ * sin(axisAngle[0] / 2);

  glm::vec4 returnQuaternion = glm::vec4(q0, q1, q2, q3);
  return returnQuaternion;
}

glm::mat4 quaternionToMatrix(glm::vec4 quaternion) {

  // q = w + xi + yk + zk
  // or
  // q = q₀ + q₁i + q₂j + q₃k
  //
  // w = scaler value that represents an angle of rotation
  // x, y, z = correspond to an axis of rotation.
  float q0 = quaternion[0];
  float q1 = quaternion[1];
  float q2 = quaternion[2];
  float q3 = quaternion[3];

  float r00 = (2 * q0 * q0) + (2 * q1 * q1) - 1;
  float r01 = (2 * q1 * q2) - (2 * q0 * q3);
  float r02 = (2 * q1 * q3) + (2 * q0 * q2);

  float r10 = (2 * q1 * q2) + (2 * q0 * q3);
  float r11 = (2 * q0 * q0) + (2 * q2 * q2) - 1;
  float r12 = (2 * q2 * q3) - (2 * q0 * q1);

  float r20 = (2 * q1 * q3) - (2 * q0 * q2);
  float r21 = (2 * q2 * q3) + (2 * q0 * q1);
  float r22 = (2 * q0 * q0) + (q3 * q3 * 2) - 1;

  glm::vec4 Row0 = glm::vec4(r00, r01, r02, 0); // Row 0
  glm::vec4 Row1 = glm::vec4(r10, r11, r12, 0); // Row 1
  glm::vec4 Row2 = glm::vec4(r20, r21, r22, 0); // Row 2
  glm::vec4 Row3 = glm::vec4(0, 0, 0, 1);       // Row 3
  glm::mat4 quaternionRotationMatrix = glm::mat4(Row0, Row1, Row2, Row3);

  return quaternionRotationMatrix;
}
