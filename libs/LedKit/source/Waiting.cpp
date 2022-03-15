// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Waiting.h"

#include "MathUtils.h"

namespace leka::led::animation {

void Waiting::start()
{
	turnLedBlack();
}

void Waiting::stop()
{
	turnLedBlack();
	_step  = 0;
	_stage = 1;
}

void Waiting::run()
{
	switch (_stage) {
		case 1:
			stage1();
			break;
		case 2:
			stage2();
			break;
		case 3:
			stage3();
			break;
		case 4:
			stage4();
			break;
		case 5:
			_stage = 1;
			break;
		default:
			break;
	}
	_belt.show();
}

auto Waiting::mapStep(uint8_t step) const -> float
{
	static constexpr auto kMaxInputValue = uint8_t {40};
	return utils::math::map(step, uint8_t {0}, kMaxInputValue, 0.F, 1.F);
}

void Waiting::stage1()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void Waiting::stage2()
{
	static constexpr auto tresholdDown = 0.3F;
	if (auto pos = mapStep(_step); pos >= tresholdDown) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void Waiting::stage3()
{
	if (auto pos = mapStep(_step); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void Waiting::stage4()
{
	if (auto pos = mapStep(_step); pos != 0.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void Waiting::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
