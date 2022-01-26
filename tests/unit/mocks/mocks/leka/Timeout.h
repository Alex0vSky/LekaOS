// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_MOCK_DRIVERS_TIMEOUT_H_
#define _LEKA_OS_MOCK_DRIVERS_TIMEOUT_H_

#include "gmock/gmock.h"
#include "interface/drivers/Timeout.h"

namespace leka::mock {

class Timeout : public interface::Timeout
{
  public:
	MOCK_METHOD(void, onTimeout, (callback_t const &), (override));

	MOCK_METHOD(void, start, (std::chrono::microseconds), (override));
	MOCK_METHOD(void, stop, (), (override));
};

}	// namespace leka::mock

#endif	 // _LEKA_OS_MOCK_DRIVERS_TIMEOUT_H_
