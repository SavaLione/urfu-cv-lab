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

#include <cstdlib>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>

int main()
{
	/* Default video camera object */
	cv::VideoCapture cap(0);

	if(cap.isOpened() == false)
	{
		spdlog::error("Cannot open the video camera.");
		return EXIT_FAILURE;
	}

	/* Get the width of frames of the video */
	double dWidth  = cap.get(cv::CAP_PROP_FRAME_WIDTH);

	/* Get the height of frames of the video */
	double dHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

	spdlog::info("Resolution of the video: {}x{}", dWidth, dHeight);

	std::string window_name = "OpenCV object detection by Saveliy Pototskiy";
	
	cv::namedWindow(window_name);

	while(true)
	{
		cv::Mat frame;

		if(!cap.read(frame))
		{
			spdlog::error("Can't read the frame from a camera.");
			return EXIT_FAILURE;
		}

		imshow(window_name, frame);

		if(cv::waitKey(10) == 27)
		{
			spdlog::info("Esc key is pressed by user.");
			spdlog::info("Stoppig the video.");
			return EXIT_SUCCESS;
		}
	}

	return 0;
}