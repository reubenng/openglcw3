//============================================================================
// Name        : cw3
// Author      : rdcn1g14
// Description : COMP3214 Coursework 3
/*
References: 
Anton's OpenGL 4 Tutorials
*/
//============================================================================
#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <assert.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include "stb_image.h"
#include "gl_utils.h"
#include "objloader.h"
#include "shader.h"
#include "texture.h"

#define CUBEVERT "shaders/cubevert.glsl"
#define CUBEFRAG "shaders/cubefrag.glsl"

using namespace glm;

int width = 640;
int height = 640;
GLFWwindow* window = NULL;

// for camera
glm::vec3 camPos (-6.0f, 1.5f, -2.0f);
glm::mat4 T = glm::translate (glm::mat4(1.0f), camPos);
glm::mat4 R = glm::mat4(1.0f);
glm::mat4 view = R * T;
glm::mat4 view2;
glm::mat4 proj = glm::perspective(45.0f, 5.0f / 4.0f, 0.1f, 100.0f);

glm::mat4 model = glm::translate(glm::mat4(1.0f) ,glm::vec3(0.0f ,0.0f ,0.0f));
glm::mat4 model2 = glm::translate(glm::mat4(1.0f) ,glm::vec3(-5.0f ,0.0f ,0.0f));
glm::mat4 MVP = proj * view * model;

float ONE_DEG_IN_RAD=0.0174;
float aspect = (float)width / (float)height;
float cspeed = 0.0f;
float headspeed = 150.0f;
float heading = 0.0f;

glm::quat q = glm::quat(0.7f, 0.0f, -0.7f, 0.0f); 
glm::quat q2 = glm::angleAxis(0.0f, glm::vec3 (0.0f, 1.0f, 0.0f));
glm::vec4 forward (0.0f, 0.0f, -1.0f, 0.0f);
glm::vec4 right (1.0f, 0.0f, 0.0f, 0.0f);
glm::vec4 up (0.0f, 1.0f, 0.0f, 0.0f);

int main () {
	restart_gl_log ();
	start_gl (); // initialise and opengl window

	GLuint cube = create_programme_from_files (CUBEVERT, CUBEFRAG);
	int cubeV = glGetUniformLocation (cube, "V");
	int cubeP = glGetUniformLocation (cube, "P");

	glUseProgram (cube); // use program
	glUniformMatrix4fv (cubeV, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv (cubeP, 1, GL_FALSE, &proj[0][0]);

	// setup for rendering
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao); // generate vao

	GLuint program = LoadShaders( "shaders/main.vert", "shaders/main.frag" );
	GLuint Matrix = glGetUniformLocation(program, "MVP");

	GLuint Tex = loadDDS("images/stone.DDS");
	GLuint Tex1 = loadDDS("images/body.DDS");
	GLuint Tex2 = loadDDS("images/body_sp.DDS");
	GLuint Tex3 = loadDDS("images/grass.DDS");
	GLuint Tex4 = loadDDS("images/99body.DDS");
	GLuint TexID  = glGetUniformLocation(program, "myTextureSampler");

	std::vector<glm::vec3> vert, vert1, vert2, vert3, vert4;
	std::vector<glm::vec2> uv, uv2, uv3, uv1, uv4;
	std::vector<glm::vec3> norm;

	bool res = loadOBJ("images/untitled.obj", vert, uv, norm);
	bool res1 = loadOBJ("images/PGZ-95.obj", vert1, uv1, norm);
	bool res2 = loadOBJ("images/Cavalera Light Tank.obj", vert2, uv2, norm);
	bool res3 = loadOBJ("images/ground.obj", vert3, uv3, norm);
	bool res4 = loadOBJ("images/type-99.obj", vert4, uv4, norm);
	
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, (vert.size()+vert1.size()+vert2.size()+vert3.size()+vert4.size()) * sizeof(glm::vec3), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vert.size() * sizeof(glm::vec3), &vert[0]);
	glBufferSubData(GL_ARRAY_BUFFER, vert.size() * sizeof(glm::vec3), vert1.size() * sizeof(glm::vec3), &vert1[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (vert.size()+vert1.size()) * sizeof(glm::vec3), vert2.size() * sizeof(glm::vec3), &vert2[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (vert.size()+vert1.size()+vert2.size()) * sizeof(glm::vec3), vert3.size() * sizeof(glm::vec3), &vert3[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (vert.size()+vert1.size()+vert2.size()+vert3.size()) * sizeof(glm::vec3), vert4.size() * sizeof(glm::vec3), &vert4[0]);
	
	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, (uv.size()+uv1.size()+uv2.size()+uv3.size()+uv4.size()) * sizeof(glm::vec2), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, uv.size() * sizeof(glm::vec2), &uv[0]);
	glBufferSubData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), uv1.size() * sizeof(glm::vec2), &uv1[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (uv.size()+uv1.size()) * sizeof(glm::vec2), uv2.size() * sizeof(glm::vec2), &uv2[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (uv.size()+uv1.size()+uv2.size()) * sizeof(glm::vec2), uv3.size() * sizeof(glm::vec2), &uv3[0]);
	glBufferSubData(GL_ARRAY_BUFFER, (uv.size()+uv1.size()+uv2.size()+uv3.size()) * sizeof(glm::vec2), uv4.size() * sizeof(glm::vec2), &uv4[0]);
	
	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LESS);
	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);
	glFrontFace (GL_CCW);
	glClearColor (.0, .0, .0, 1.0); // black
	glViewport (0, 0, width, height);
	bool tour = false;
	bool fix = false;
	float i = 0;
	int theta = 0;
	float angle = 0;

	R = glm::toMat4(q); // for animation
	T = glm::translate (glm::mat4(1.0f), glm::vec3 (camPos));
	view = inverse (R) * inverse (T);
	MVP = proj* view * model;

	while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS 
		   && glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 ){
		
		bool moved = false;
		bool reset = false;
		bool screencap = false;
		
		glm::vec3 move (0.0, 0.0, 0.0);
		glm::vec3 moveCar (0.0, 0.0, 0.0);
		float camyaw = 0.0f;
		float yaw = 0.0f;

		static double previous = glfwGetTime ();
		double current = glfwGetTime ();
		double elapsed = current - previous;
		previous = current;
		_update_fps_counter (window);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

		glUseProgram(program);
		glUniformMatrix4fv(Matrix, 1, GL_FALSE, &MVP[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Tex);
		glUniform1i(TexID, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, vert.size() ); // first

		glActiveTexture(GL_TEXTURE0); // tank
		glBindTexture(GL_TEXTURE_2D, Tex2);
		glUniform1i(TexID, 0);
		glDrawArrays(GL_TRIANGLES, vert.size() + vert1.size(), vert2.size() );

		glActiveTexture(GL_TEXTURE0); // grass
		glBindTexture(GL_TEXTURE_2D, Tex3);
		glUniform1i(TexID, 0);
		glDrawArrays(GL_TRIANGLES, vert.size() + vert1.size()+vert2.size(), vert3.size() );

		glActiveTexture(GL_TEXTURE0); // tank
		glBindTexture(GL_TEXTURE_2D, Tex4);
		glUniform1i(TexID, 0);

		theta += 1; // for tank movement
		if(theta == 360) theta = 0;
		quat q_yaw2 = glm::angleAxis((ONE_DEG_IN_RAD), vec3(0.0, 1.0, 0.0));
		q2 = q_yaw2 * q2;
		glm::mat4 R1 = glm::toMat4(q2);	
		glm::mat4 view2 = inverse(R) * inverse(T);
		glm::mat4 model2 = glm::translate(glm::mat4(1.0f), 
			glm::vec3(30.0f + 8.0*cos(theta*ONE_DEG_IN_RAD), 0.0f, 0.0f+8.0*sin(theta*ONE_DEG_IN_RAD)));
		glm::mat4 model2r = model2 * inverse(R1);
		glm::mat4 MVP2 = proj* view2 * model2r;
		glUniformMatrix4fv(Matrix, 1, GL_FALSE, &MVP2[0][0]);
		glDrawArrays(GL_TRIANGLES, vert.size() + vert1.size()+vert2.size()+vert3.size(), vert4.size() );

		glActiveTexture(GL_TEXTURE0); // another moving tank
		glBindTexture(GL_TEXTURE_2D, Tex1);
		glUniform1i(TexID, 0);

		glm::mat4 R2 = glm::toMat4(q2);	
		glm::mat4 view3 = inverse(R) * inverse(T);
		glm::mat4 model3 = glm::translate(glm::mat4(1.0f), 
			glm::vec3(0.0f + 8.0*cos(theta*ONE_DEG_IN_RAD), 0.0f, -30.0f+8.0*sin(theta*ONE_DEG_IN_RAD)));
		glm::mat4 model3r = model3 * inverse(R2);
		glm::mat4 MVP3 = proj * view3 * model3r;
		glUniformMatrix4fv(Matrix, 1, GL_FALSE, &MVP3[0][0]);
		glDrawArrays(GL_TRIANGLES, vert.size(), vert1.size() );

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glfwSwapBuffers(window); // display to window
		glfwPollEvents(); // update non-graphic input

		if (glfwGetKey (window, GLFW_KEY_M)) { // default position
			tour = false;
			reset = true;
		}
		if (glfwGetKey (window, GLFW_KEY_P)) { // screenshot position
			screencap = true;
			tour = false;
		}
		if (glfwGetKey (window, GLFW_KEY_T)) {
			q = glm::quat(-0.04f, 0.0f, -1.0f, 0.0f);
			camPos = glm::vec3 (4.0f, 12.0f, -65.0f);
			tour = true;
		}
		if (glfwGetKey (window, GLFW_KEY_UP)) {
			cspeed += elapsed*10;
			if(cspeed > 100) cspeed = 100;
			move[2] -= cspeed * elapsed;
			moved = true;
		}
		if (glfwGetKey (window, GLFW_KEY_DOWN)) {
			cspeed -= elapsed*10;
			if(cspeed < 0) cspeed = 0;
			move[2] -= cspeed * elapsed;
			moved = true;
		}
		if (glfwGetKey (window, GLFW_KEY_LEFT)) {
			camyaw += headspeed * elapsed;
			moved = true;
		    quat q_yaw = glm::angleAxis((ONE_DEG_IN_RAD*camyaw), vec3(up[0], up[1], up[2]));
			q = q_yaw * q;
		}
		if (glfwGetKey (window, GLFW_KEY_RIGHT)) {
			camyaw -= headspeed * elapsed;
			moved = true;
			quat q_yaw = glm::angleAxis((ONE_DEG_IN_RAD*camyaw), vec3(up[0], up[1], up[2]));
			q = q_yaw * q;
		}
		if (moved) {
			heading += camyaw;
			
			R = glm::toMat4(q);
			forward = R * glm::vec4 (0.0, 0.0, -1.0, 0.0);
			right = R * glm::vec4 (1.0, 0.0, 0.0, 0.0);
			up = R * glm::vec4 (0.0, 1.0, 0.0, 0.0);
			camPos = camPos + glm::vec3 (forward) * -move[2];
			camPos = camPos + glm::vec3 (up) * move[1];
			camPos = camPos + glm::vec3 (right) * move[0];
			
			T = glm::translate (glm::mat4(1.0f), camPos);
			view = inverse (R) * inverse (T);
			MVP = proj* view * model;
			view[3][0] = 0;
			view[3][1] = 0;
			view[3][2] = 0;

			glUseProgram (cube);
			glUniformMatrix4fv (cubeV, 1, GL_FALSE, &view[0][0]);		
		}

		if(tour)
		{	
			cspeed = 15.0f;
			move[0] -= cspeed * elapsed;

			camyaw -= headspeed/10 * elapsed;
			quat q_yaw = glm::angleAxis((ONE_DEG_IN_RAD*camyaw), vec3(up[0], up[1], up[2]));
			q = q_yaw * q;
			R = glm::toMat4 (q);
			forward = R * glm::vec4 (0.0, 0.0, -1.0, 0.0);
			right = R * glm::vec4 (1.0, 0.0, 0.0, 0.0);
			up = R * glm::vec4 (0.0, 1.0, 0.0, 0.0);
			camPos = camPos + glm::vec3 (forward) * -move[2];
			camPos = camPos + glm::vec3 (up) * move[1];
			camPos = camPos + glm::vec3 (right) * move[0];
			
			T = glm::translate (glm::mat4(1.0f), camPos);
			view = inverse (R) * inverse (T);
			MVP = proj* view * model;

			view[3][0] = 0;
			view[3][1] = 0;
			view[3][2] = 0;

			glUseProgram (cube);
			glUniformMatrix4fv (cubeV, 1, GL_FALSE, &view[0][0]);	
		}
		if(reset){ // default position
			q = glm::quat(0.7f, 0.0f, -0.7f, 0.0f);
			R = glm::toMat4 (q);
			forward = R * glm::vec4 (0.0, 0.0, -1.0, 0.0);
			right = R * glm::vec4 (1.0, 0.0, 0.0, 0.0);
			up = R * glm::vec4 (0.0, 1.0, 0.0, 0.0);
			camPos = glm::vec3 (-6.0f, 1.5f, -2.0f);
			T = glm::translate (glm::mat4(1.0f), camPos);
			view = inverse (R) * inverse (T);
			MVP = proj* view * model;

			view[3][0] = 0;
			view[3][1] = 0;
			view[3][2] = 0;

			glUseProgram (cube);
			glUniformMatrix4fv (cubeV, 1, GL_FALSE, &view[0][0]);	

			screencap = false;
		}
		if(screencap) { // screencap position
			q = glm::quat(-0.26f, 0.07f, -0.93f, -0.25f);
			R = glm::toMat4 (q);
			forward = R * glm::vec4 (0.0, 0.0, -1.0, 0.0);
			right = R * glm::vec4 (1.0, 0.0, 0.0, 0.0);
			up = R * glm::vec4 (0.0, 1.0, 0.0, 0.0);
			camPos = glm::vec3 (36.5f, 29.0f, -48.2f);

			T = glm::translate (glm::mat4(1.0f), camPos);
			view = inverse (R) * inverse (T);
			MVP = proj* view * model;

			view[3][0] = 0;
			view[3][1] = 0;
			view[3][2] = 0;

			glUseProgram (cube);
			glUniformMatrix4fv (cubeV, 1, GL_FALSE, &view[0][0]);	
			screencap = false;
		}
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}