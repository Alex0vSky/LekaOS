// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// +  Stage 0  +  Stage 1    +   Stage 2    +   3   + 4  +5 + 6 + 7  +
// |           |             |              |       |    |  |   |    |
// |           |             |              |       |    |  |   |    |
// |           |             |\             |       |    || |  ||    |  --- White
// |           |            /| \            |       |    |\ |  ||    |
// |           |           / |  \           |       |   /| || / ||   |
// |           |          /  |   \          |       |   || || | ||   |
// |           |         /   |    \         |       |   || \| | |\   |
// |           |        /    |     \        |       |   ||  |/  | |  |
// |           |       /     |      \       |       |  / |  ||  | |  |
// |           |      /      |       \      |       |  | |  |   | \  |
// |           |     /       |        \     |       |  | |  |   |  | |
// |           |    /        |         \    |       | /  |  |   |  \ |
// |           |   /         |          \   |       | |  |  |   |   ||
// |           |  /          |           \  |       | |  |  |   |   ||
// |           | /           |            \ |       | |  |  |   |   \|
// |           |/            |             \|       |/   |  |   |    |
// |-----------|             |              |-------|    |  |   |    | --- Black
// |           |             |              |       |    |  |   |    |

#pragma once

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LEDAnimation.h"

namespace leka::led::animation {

class Fly : public interface::LEDAnimation
{
  public:
	explicit Fly(interface::LED &ears, interface::LED &belt) : _ears(ears), _belt(belt) {};

	void start() final;
	void run() final;
	void stop() final;

  private:
	interface::LED &_ears;
	interface::LED &_belt;
	uint8_t _step  = 0;
	uint8_t _stage = 0;

	[[nodiscard]] auto mapStep(uint8_t step, uint8_t val) const -> float;

	void stage0();
	void stage1();
	void stage2();
	void stage3();
	void stage4();
	void stage5();
	void stage6();
	void stage7();

	void turnLedBlack();
};

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
