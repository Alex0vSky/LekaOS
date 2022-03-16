// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LoadingGreen.h"

namespace leka::led::animation {

void LoadingGreen::start()
{
	turnLedBlack();
}

void LoadingGreen::stop()
{
	turnLedBlack();
	_step  = 0;
	_stage = 0;
}

void LoadingGreen::run()
{
	switch (_stage) {
		case 0:
			stage0();
			break;
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
			stage5();
			break;
		case 6:
			stage6();
			break;
		case 7:
			stage7();
			break;
		case 8:
			stage8();
			break;
		case 9:
			stage9();
			break;
		case 10:
			stage10();
			break;
		case 11:
			stage11();
			break;
		default:
			break;
	}
	_belt.show();
}

void LoadingGreen::stage0()
{
	increaseBrightness();
}

void LoadingGreen::stage1()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingGreen::stage2()
{
	increaseBrightness();
}

void LoadingGreen::stage3()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingGreen::stage4()
{
	increaseBrightness();
}

void LoadingGreen::stage5()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingGreen::stage6()
{
	increaseBrightness();
}

void LoadingGreen::stage7()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingGreen::stage8()
{
	increaseBrightness();
}

void LoadingGreen::stage9()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingGreen::stage10()
{
	increaseBrightness();
}

void LoadingGreen::stage11()
{
	decreaseBrightness(0.F);
}

void LoadingGreen::increaseBrightness()
{
	static constexpr auto kMaxInputValue = uint8_t {10};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_green, pos);
		_belt.setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void LoadingGreen::decreaseBrightness(float treshold)
{
	static constexpr auto kMaxInputValue = uint8_t {10};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_green, pos);
		_belt.setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void LoadingGreen::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
