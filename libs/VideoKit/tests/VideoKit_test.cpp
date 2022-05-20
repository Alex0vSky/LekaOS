// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "VideoKit.h"

#include "gtest/gtest.h"
#include "mocks/leka/CoreLCD.h"
#include "mocks/leka/CoreVideo.h"
#include "mocks/leka/EventFlags.h"

using namespace leka;

class VideoKitTest : public ::testing::Test
{
  protected:
	VideoKitTest() {}

	void SetUp() override
	{
		strcpy(temp_file_path, "/tmp/XXXXXX");
		mkstemp(temp_file_path);
	}
	// void TearDown() override {}

	char temp_file_path[L_tmpnam];	 // NOLINT

	mock::EventFlags mock_event_flags {};
	mock::CoreLCD mock_lcd {};
	mock::CoreVideo mock_corevideo {};
	VideoKit video_kit {mock_event_flags, mock_lcd, mock_corevideo};
};

TEST_F(VideoKitTest, initialization)
{
	EXPECT_NE(&video_kit, nullptr);
}

TEST_F(VideoKitTest, initializeScreen)
{
	EXPECT_CALL(mock_corevideo, initialize);
	EXPECT_CALL(mock_corevideo, setBrightness);
	EXPECT_CALL(mock_corevideo, clearScreen);

	video_kit.initializeScreen();
}

TEST_F(VideoKitTest, displayImage)
{
	EXPECT_CALL(mock_event_flags, set(VideoKit::flags::STOP_VIDEO_FLAG));
	EXPECT_CALL(mock_corevideo, displayImage);

	video_kit.displayImage(temp_file_path);
}

TEST_F(VideoKitTest, displayImageFileDoesNotExist)
{
	video_kit.displayImage("/unexisting/path");
}

TEST_F(VideoKitTest, displayImageSamePathTwice)
{
	EXPECT_CALL(mock_event_flags, set(VideoKit::flags::STOP_VIDEO_FLAG));
	EXPECT_CALL(mock_corevideo, displayImage).Times(1);

	video_kit.displayImage(temp_file_path);

	EXPECT_CALL(mock_corevideo, displayImage).Times(0);

	video_kit.displayImage(temp_file_path);
}

TEST_F(VideoKitTest, playVideoInALoopFalse)
{
	EXPECT_CALL(mock_event_flags, set(VideoKit::flags::STOP_VIDEO_FLAG));
	EXPECT_CALL(mock_event_flags, set(VideoKit::flags::START_VIDEO_FLAG));

	auto play_in_a_loop = false;
	video_kit.playVideo(temp_file_path, play_in_a_loop);
}

TEST_F(VideoKitTest, playVideoInALoopTrue)
{
	EXPECT_CALL(mock_event_flags, set(VideoKit::flags::STOP_VIDEO_FLAG));
	EXPECT_CALL(mock_event_flags, set(VideoKit::flags::START_VIDEO_FLAG));

	auto play_in_a_loop = true;
	video_kit.playVideo(temp_file_path, play_in_a_loop);
}

TEST_F(VideoKitTest, playVideoInALoopFalseFileDoesNotExist)
{
	auto play_in_a_loop = false;
	video_kit.playVideo("some_video", play_in_a_loop);
}

TEST_F(VideoKitTest, playVideoInALoopTrueFileDoesNotExist)
{
	auto play_in_a_loop = true;
	video_kit.playVideo("some_video", play_in_a_loop);
}

TEST_F(VideoKitTest, stopVideo)
{
	EXPECT_CALL(mock_event_flags, set(VideoKit::flags::STOP_VIDEO_FLAG));

	video_kit.stopVideo();
}
