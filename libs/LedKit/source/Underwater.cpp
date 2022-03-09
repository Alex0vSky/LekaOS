// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Underwater.h"

#include "MathUtils.h"

namespace leka::led::animation {

static constexpr auto blue_water = RGB {0x30, 0x2D, 0xAD};

void Underwater::start()
{
	turnLedBlack();
}

void Underwater::stop()
{
	turnLedBlack();
}

void Underwater::run()
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

auto Underwater::mapStep(uint8_t step, uint8_t max_input_value) const -> float
{
	return utils::math::map(step, uint8_t {0}, max_input_value, 0.F, 1.F);
}

void Underwater::stage0()
{
	static constexpr auto kMaxInputValueStageWater = uint8_t {60};
	if (auto pos = mapStep(_step, kMaxInputValueStageWater); pos <= 1.F) {
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Underwater::stage1()
{
	static constexpr auto kTresholdStage1		   = 0.5F / 2;
	static constexpr auto kMaxInputValueStageWater = uint8_t {60};
	if (auto pos = mapStep(_step, kMaxInputValueStageWater); pos <= kTresholdStage1) {
		RGB color = ColorKit::colorGradient(RGB::black, blue_water, pos);
		_belt.setColor(color);
		++_step;
	} else {
		++_stage;
	}
}

void Underwater::stage2()
{
	static constexpr auto kTresholdStage2		   = 0.4F;
	static constexpr auto kMaxInputValueStageWater = uint8_t {60};
	if (auto pos = mapStep(_step, kMaxInputValueStageWater); pos >= kTresholdStage2) {
		RGB color = ColorKit::colorGradient(RGB::black, blue_water, pos);
		_belt.setColor(color);
		--_step;
	} else {
		++_stage;
	}
}

void Underwater::stage3()
{
	static constexpr auto kTresholdStage3		   = 0.85F;
	static constexpr auto kMaxInputValueStageWater = uint8_t {60};
	if (auto pos = mapStep(_step, kMaxInputValueStageWater); pos <= kTresholdStage3) {
		RGB color = ColorKit::colorGradient(RGB::black, blue_water, pos);
		_belt.setColor(color);
		++_step;
	} else {
		++_stage;
	}
}

void Underwater::stage4()
{
	static constexpr auto kTresholdStage4		   = 0.6F;
	static constexpr auto kMaxInputValueStageWater = uint8_t {60};
	if (auto pos = mapStep(_step, kMaxInputValueStageWater); pos >= kTresholdStage4) {
		RGB color = ColorKit::colorGradient(RGB::black, blue_water, pos);
		_belt.setColor(color);
		--_step;
	} else {
		++_stage;
	}
}

void Underwater::stage5()
{
	static constexpr auto kTresholdStage5		   = 0.9F;
	static constexpr auto kMaxInputValueStageWater = uint8_t {60};
	if (auto pos = mapStep(_step, kMaxInputValueStageWater); pos <= kTresholdStage5) {
		RGB color = ColorKit::colorGradient(RGB::black, blue_water, pos);
		_belt.setColor(color);
		++_step;
	} else {
		++_stage;
	}
}

void Underwater::stage6()
{
	static constexpr auto kTresholdStage6		   = 0.8F;
	static constexpr auto kMaxInputValueStageWater = uint8_t {60};
	if (auto pos = mapStep(_step, kMaxInputValueStageWater); pos >= kTresholdStage6) {
		RGB color = ColorKit::colorGradient(RGB::black, blue_water, pos);
		_belt.setColor(color);
		--_step;
	} else {
		++_stage;
	}
}

void Underwater::stage7()
{
	static constexpr auto kMaxInputValueStageWater = uint8_t {60};
	if (auto pos = mapStep(_step, kMaxInputValueStageWater); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, blue_water, pos);
		_belt.setColor(color);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Underwater::stage8()
{
	static constexpr auto kMaxInputValueStageWater = uint8_t {60};
	if (auto pos = mapStep(_step, kMaxInputValueStageWater); pos != 1.F) {
		_belt.setColor(blue_water);
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Underwater::stage9()
{
	static constexpr auto kFishSpeed1			  = uint8_t {5};
	static constexpr auto kFishSpeed2			  = uint8_t {10};
	static constexpr auto kMaxInputValueStageFish = uint8_t {160};
	if (auto pos = mapStep(_step, kMaxInputValueStageFish); pos >= 0.F) {
		showFishAtRight(RGB::pure_blue, position_fish_1, kFishSpeed1, step_fish_1);
	}
	if (auto pos = mapStep(_step, kMaxInputValueStageFish); pos >= 1.F / 4) {
		showFishAtRight(RGB::pure_red, position_fish_3, kFishSpeed2, step_fish_3);
	}
	if (auto pos = mapStep(_step, kMaxInputValueStageFish); pos == 1.F) {
		++_stage;
		_step = 0;
		return;
	}
	_step++;
}

void Underwater::stage10()
{
	static constexpr auto kFishSpeed1			   = uint8_t {5};
	static constexpr auto kFishSpeed2			   = uint8_t {10};
	static constexpr auto kMaxInputValueStageWater = uint8_t {60};
	static constexpr auto kMaxInputValueStageFish  = uint8_t {160};
	if (auto pos = mapStep(_step, kMaxInputValueStageFish); pos >= 0.F) {
		showFishAtLeft(RGB::pure_red, position_fish_3, kFishSpeed1, step_fish_1);
	}
	if (auto pos = mapStep(_step, kMaxInputValueStageFish); pos >= 1.F / 4) {
		showFishAtLeft(RGB::yellow, position_fish_4, kFishSpeed2, step_fish_2);
	}
	if (auto pos = mapStep(_step, kMaxInputValueStageFish); pos == 1.F) {
		++_stage;
		_step = kMaxInputValueStageWater;
		return;
	}
	++_step;
}

void Underwater::stage11()
{
	static constexpr auto kMaxInputValueStageWater = uint8_t {60};
	if (auto pos = mapStep(_step, kMaxInputValueStageWater); pos != 0.F) {
		RGB color = ColorKit::colorGradient(RGB::black, blue_water, pos);
		_belt.setColor(color);
		--_step;
	} else {
		++_stage;
	}
}

void Underwater::showFishAtRight(const RGB &fish_color, int &fish_position, uint8_t fish_speed, uint8_t &step_fish)
{
	if (auto pos = mapStep(step_fish, fish_speed); pos != 1.F) {
		setColorCurrentMinusOne(fish_position - 1, pos, fish_color);
		setColorCurrent(fish_position, pos, fish_color);
		setColorCurrentPlusOne(fish_position + 1, pos, fish_color);
		setColorCurrentPlusTwo(fish_position + 2, pos, fish_color);
		setColorCurrentPlusThree(fish_position + 3, pos, fish_color);
		++step_fish;
	} else {
		step_fish = 0;
		fish_position += 2;
	}
}

void Underwater::showFishAtLeft(const RGB &fish_color, int &fish_position, uint8_t fish_speed, uint8_t &step_fish)
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	if (auto pos = mapStep(step_fish, fish_speed); pos != 1.F) {
		setColorCurrentMinusOne(kNumberOfLedsBelt - (fish_position - 1), pos, fish_color);
		setColorCurrent(kNumberOfLedsBelt - fish_position, pos, fish_color);
		setColorCurrentPlusOne(kNumberOfLedsBelt - (fish_position + 1), pos, fish_color);
		setColorCurrentPlusTwo(kNumberOfLedsBelt - (fish_position + 2), pos, fish_color);
		setColorCurrentPlusThree(kNumberOfLedsBelt - (fish_position + 3), pos, fish_color);
		++step_fish;
	} else {
		step_fish = 0;
		fish_position += 2;
	}
}

void Underwater::setColorCurrentMinusOne(int index, float pos, const RGB &color_movement)
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	if (index >= 0 && index < kNumberOfLedsBelt && pos <= 1.F / 2) {
		RGB color = ColorKit::colorGradient(blue_water, color_movement, 1.F / 2 - pos);
		_belt.setColorAtIndex(index, color);
	}
}

void Underwater::setColorCurrent(int index, float pos, const RGB &color_movement)
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	if (index >= 0 && index < kNumberOfLedsBelt) {
		RGB color = ColorKit::colorGradient(blue_water, color_movement, 1.F - pos);
		_belt.setColorAtIndex(index, color);
	}
}

void Underwater::setColorCurrentPlusOne(int index, float pos, const RGB &color_movement)
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	if (index >= 0 && index < kNumberOfLedsBelt) {
		if (pos <= 1.F / 2) {
			RGB color = ColorKit::colorGradient(blue_water, color_movement, pos + 1.F / 2);
			_belt.setColorAtIndex(index, color);
		} else {
			RGB color = ColorKit::colorGradient(blue_water, color_movement, 1.F + 1.F / 2 - pos);
			_belt.setColorAtIndex(index, color);
		}
	}
}

void Underwater::setColorCurrentPlusTwo(int index, float pos, const RGB &color_movement)
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	if (index >= 0 && index < kNumberOfLedsBelt) {
		RGB color = ColorKit::colorGradient(blue_water, color_movement, pos);
		_belt.setColorAtIndex(index, color);
	}
}

void Underwater::setColorCurrentPlusThree(int index, float pos, const RGB &color_movement)
{
	static constexpr auto kNumberOfLedsBelt = uint8_t {20};
	if (index >= 0 && index < kNumberOfLedsBelt && pos >= 1.F / 2) {
		RGB color = ColorKit::colorGradient(blue_water, color_movement, pos - 1.F / 2);
		_belt.setColorAtIndex(index, color);
	}
}

void Underwater::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
