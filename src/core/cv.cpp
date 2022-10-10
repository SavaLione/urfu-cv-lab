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
 * @brief OpenCV labs
 * @author Saveliy Pototskiy (SavaLione)
 * @date 10 Oct 2022
 */
#include "core/cv.h"

#include "core/variables.h"

#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>

void place_text_info(cv::Mat &frame);
void draw_rectangle(cv::Mat &frame, cv::Rect const &rectangle);

void place_text_info(cv::Mat &frame)
{
	std::string label = "Pototskiy Saveliy (FOM-210510)";

	cv::putText(frame, label, cv::Point(0, 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));
}

void draw_rectangle(cv::Mat &frame, cv::Rect const &rectangle)
{
	int thickness = 2;

	// Drawing the Rectangle
	cv::rectangle(frame, rectangle, cv::Scalar(255, 255, 255), thickness, cv::LINE_8);
}

void cv_first_lab()
{
	/* Variables initialization */
	variables &variables_instance = variables::instance();

	/* Default video capture device */
	cv::VideoCapture video_capture_device(0);

	if(video_capture_device.isOpened() == false)
	{
		spdlog::error("Cannot open the video camera.");
		variables_instance.exit = true;
	}

	/* Print width and height of the capture device frame */
	spdlog::info(
		"Resolution of the capture device frame: {}x{}",
		video_capture_device.get(cv::CAP_PROP_FRAME_WIDTH),
		video_capture_device.get(cv::CAP_PROP_FRAME_HEIGHT));

	std::string window_name = "OpenCV colour object detection";

	cv::namedWindow(window_name);

	while(!variables_instance.exit)
	{
		cv::Mat frame;

		// std::string path = "C://cvtemp\\green1.png";
		// cv::Mat frame	 = cv::imread(path);

		if(!video_capture_device.read(frame))
		{
			spdlog::error("Can't read the frame from a camera.");
			variables_instance.exit = true;
		}

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

		cv::Mat drawing = cv::Mat::zeros(frame.size(), CV_8UC3);
		for(std::size_t i = 0; i < contours.size(); i++)
		{
            if(contours[i].size() >= variables_instance.contours_min_size)
			{
				int thickness = 2;
				cv::Rect box  = cv::boundingRect(contours[i]);
				draw_rectangle(frame, box);
			}
		}

		place_text_info(frame);

		cv::imshow("Thresholded Image", img_thresholded);
		cv::imshow(window_name, frame);
		// cv::imshow("drawing", drawing);

		if(cv::waitKey(10) == 27)
		{
			spdlog::info("Esc key is pressed by user.");
			spdlog::info("Stoppig the video.");
			variables_instance.exit = true;
		}
	}
}