/*
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 * Copyright (c) 2022, Saveliy Pototskiy (SavaLione) (savalione.com)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/**
 * @file
 * @brief Lyssa application
 * @author Saveliy Pototskiy (SavaLione)
 * @date 16 Sep 2022
 */
#include "core/lyssa.h"

// clang-format off
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <opencv2/core.hpp>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>
// clang-format on

#include "core/variables.h"

#include <cstdlib>
#include <future>
#include <iostream>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

void place_text_info(cv::Mat &frame);
void draw_rectangle(cv::Mat &frame);
void draw_rectangle(cv::Mat &frame, cv::Point const &top_left, cv::Point const &bottom_right);
void cv_operations();
void gui();

static void glfw_error_callback(int error, const char *description)
{
	// fprintf(stderr, "Glfw Error %d: %s\n", error, description);
	spdlog::error("Glfw Error {}: {}", error, description);
}

void place_text_info(cv::Mat &frame)
{
	/* Variables initialization */
	variables &variables_instance = variables::instance();

	// std::string label = "Author: Pototskiy Saveliy (FOM-210510)";
	std::string label = "Variable: ";

	label += std::to_string(variables_instance.test_variable);

	cv::putText(frame, label, cv::Point(0, 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));
}

void draw_rectangle(cv::Mat &frame)
{
	// Top Left Corner
	cv::Point p1(30, 30);

	// Bottom Right Corner
	cv::Point p2(255, 255);

	int thickness = 2;

	// Drawing the Rectangle
	cv::rectangle(frame, p1, p2, cv::Scalar(255, 255, 255), thickness, cv::LINE_8);
}

void draw_rectangle(cv::Mat &frame, cv::Point const &top_left, cv::Point const &bottom_right)
{
	int thickness = 2;
	cv::rectangle(frame, top_left, bottom_right, cv::Scalar(255, 255, 255), thickness, cv::LINE_8);
}

void cv_operations()
{
	/* Variables initialization */
	variables &variables_instance = variables::instance();

	// /* Default video camera object */
	// cv::VideoCapture cap(0);

	// if(cap.isOpened() == false)
	// {
	// 	spdlog::error("Cannot open the video camera.");
	// 	return EXIT_FAILURE;
	// }

	// /* Get the width of frames of the video */
	// double dWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);

	// /* Get the height of frames of the video */
	// double dHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

	// spdlog::info("Resolution of the video: {}x{}", dWidth, dHeight);

	std::string window_name = "OpenCV object detection by Saveliy Pototskiy";

	cv::namedWindow(window_name);

	while(!variables_instance.exit)
	{
		// cv::Mat frame;

		std::string path = "C://cvtemp\\green1.png";
		cv::Mat frame	 = cv::imread(path);

		// if(!cap.read(frame))
		// {
		// 	spdlog::error("Can't read the frame from a camera.");
		// 	return EXIT_FAILURE;
		// }

		// place_text_info(frame);
		// draw_rectangle(frame);

		// int low_h  = 0;
		// int high_h = 179;
		// int low_s  = 0;
		// int high_s = 255;
		// int low_v  = 0;
		// int high_v = 255;

		// cvCreateTrackbar("LowH", "Control", &low_h, 179);
		// cvCreateTrackbar("HighH", "Control", &high_h, 179);

		// cvCreateTrackbar("LowS", "Control", &low_s, 255);
		// cvCreateTrackbar("HighS", "Control", &high_s, 255);

		// cvCreateTrackbar("LowV", "Control", &low_v, 255);
		// cvCreateTrackbar("HighV", "Control", &high_v, 255);

		// cv::Mat img_HSV;
		// cv::cvtColor(frame, img_HSV, cv::COLOR_BGR2HSV);
		// cv::Mat img_thresholded;
		// cv::inRange(
		// 	img_HSV,
		// 	cv::Scalar(variables_instance.h_low, variables_instance.s_low, variables_instance.v_low),
		// 	cv::Scalar(variables_instance.h_high, variables_instance.s_high, variables_instance.v_high),
		// 	img_thresholded);

		// cv::erode(img_thresholded, img_thresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
		// cv::dilate(img_thresholded, img_thresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

		// cv::dilate(img_thresholded, img_thresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
		// cv::erode(img_thresholded, img_thresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

		// place_text_info(frame);

		/*
			Matrix for HSV scheme frame
    		HSV - Hue Saturation Value
		*/
		cv::Mat frame_HSV;

		cv::Mat img_thresholded;

		/*
			We convert BRG (original frame) to HSV frame
			BGR - is RGB color space
			HSV - Hue Saturation Value
		*/
		cv::cvtColor(frame, frame_HSV, cv::COLOR_BGR2HSV);

		cv::Scalar green_mask_low(variables_instance.h_low, variables_instance.s_low, variables_instance.v_low);
		cv::Scalar green_mask_high(variables_instance.h_high, variables_instance.s_high, variables_instance.v_high);

		cv::inRange(frame_HSV, green_mask_low, green_mask_high, img_thresholded);

		cv::erode(img_thresholded, img_thresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
		cv::dilate(img_thresholded, img_thresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

		std::vector<std::vector<cv::Point>> contours;
		std::vector<cv::Vec4i> hierarchy;

		cv::findContours(img_thresholded, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

		cv::RNG rng(12345);

		cv::Mat drawing = cv::Mat::zeros(frame.size(), CV_8UC3);
		for(std::size_t i = 0; i < contours.size(); i++)
		{
			// cv::Scalar color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
			// cv::drawContours(drawing, contours, (int)i, color, 2, cv::LINE_8, hierarchy, 0);
			// int thickness = 2;
			// cv::rectangle(frame, top_left, bottom_right, cv::Scalar(255, 255, 255), thickness, cv::LINE_8);
			int thickness = 2;
			cv::Rect box = cv::boundingRect(contours[i]);
			cv::rectangle(frame, box, cv::Scalar(255, 255, 255), thickness, cv::LINE_8);
		}

		cv::imshow("Thresholded Image", img_thresholded);
		cv::imshow(window_name, frame);
		// cv::imshow("drawing", drawing);

		if(cv::waitKey(10) == 27)
		{
			spdlog::info("Esc key is pressed by user.");
			spdlog::info("Stoppig the video.");
			// exit(EXIT_SUCCESS);
			variables_instance.exit = true;
		}
	}
}

void gui()
{
	/* Variables initialization */
	variables &variables_instance = variables::instance();

	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if(!glfwInit())
		return;

		// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char *glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char *glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char *glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	GLFWwindow *window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
	if(window == NULL)
		return;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	// Our state
	bool show_demo_window	 = true;
	bool show_another_window = false;
	ImVec4 clear_color		 = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	// while(!glfwWindowShouldClose(window))
	while(!variables_instance.exit)
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			// static float f	   = 0.0f;
			// static int counter = 0;

			ImGui::Begin("Variables");

			// ImGui::Text("This is some useful text.");		   // Display some text (you can use a format strings too)
			// ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
			// ImGui::Checkbox("Another Window", &show_another_window);

			// ImGui::SliderFloat("float", &f, 0.0f, 1.0f);			 // Edit 1 float using a slider from 0.0f to 1.0f
			// ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

			// if(ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
			// 	counter++;
			// ImGui::SameLine();
			// ImGui::Text("counter = %d", counter);

			// ImGui::SliderInt("test_variable", &variables_instance.test_variable, 0, 255);

			ImGui::SliderInt("H low", &variables_instance.h_low, 0, 255);
			ImGui::SliderInt("H high", &variables_instance.h_high, 0, 255);
			ImGui::SliderInt("S low", &variables_instance.s_low, 0, 255);
			ImGui::SliderInt("S high", &variables_instance.s_high, 0, 255);
			ImGui::SliderInt("V low", &variables_instance.v_low, 0, 255);
			ImGui::SliderInt("V high", &variables_instance.v_high, 0, 255);

			ImGui::Separator();
			if(ImGui::Button("Exit"))
			{
				variables_instance.exit = true;
			}

			ImGui::Text(
				"Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

int main()
{
	// gui();
	// cv_operations();

	/* Variables initialization */
	variables &variables_instance = variables::instance();

	auto async_gui = std::async(gui);
	auto async_ocv = std::async(cv_operations);

	return 0;
}