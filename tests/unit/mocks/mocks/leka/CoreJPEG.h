// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/JPEG.hpp"

namespace leka::mock {

class CoreJPEG : public interface::JPEGBase
{
  public:
	MOCK_METHOD(void, initialize, (), (override));
	MOCK_METHOD(JPEG_HandleTypeDef, getHandle, (), (override));
	MOCK_METHOD(JPEG_HandleTypeDef *, getHandlePointer, (), (override));
	MOCK_METHOD(void, decodeImage, (interface::File * file), (override));
	MOCK_METHOD(HAL_StatusTypeDef, decodeImageWithPolling, (), (override));
	MOCK_METHOD(JPEGImageProperties, getImageProperties, (), (override));
	MOCK_METHOD(void, onErrorCallback, (JPEG_HandleTypeDef * hjpeg), (override));
	MOCK_METHOD(void, onInfoReadyCallback, (JPEG_HandleTypeDef * hjpeg, JPEG_ConfTypeDef *info), (override));
	MOCK_METHOD(void, onDataAvailableCallback, (JPEG_HandleTypeDef * hjpeg, uint32_t size), (override));
	MOCK_METHOD(void, onDataReadyCallback, (JPEG_HandleTypeDef * hjpeg, uint8_t *output_buffer, uint32_t size),
				(override));
	MOCK_METHOD(void, onDecodeCompleteCallback, (JPEG_HandleTypeDef * hjpeg), (override));
};

}	// namespace leka::mock
