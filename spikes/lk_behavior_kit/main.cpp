// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "BehaviorKit.h"
#include "CoreDMA2D.hpp"
#include "CoreDSI.hpp"
#include "CoreFont.hpp"
#include "CoreGraphics.hpp"
#include "CoreJPEG.hpp"
#include "CoreJPEGModeDMA.hpp"
#include "CoreJPEGModePolling.hpp"
#include "CoreLCD.hpp"
#include "CoreLCDDriverOTM8009A.hpp"
#include "CoreLED.h"
#include "CoreLL.h"
#include "CoreLTDC.hpp"
#include "CoreMotor.h"
#include "CorePwm.h"
#include "CoreSDRAM.hpp"
#include "CoreSPI.h"
#include "CoreSTM32Hal.h"
#include "CoreVideo.hpp"
#include "EventLoopKit.h"
#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "LedKit.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"

using namespace leka;
using namespace std::chrono;

//
// MARK: - Global definitions
//

namespace {

namespace sd {

	namespace internal {

		auto bd = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
		auto fs = FATFileSystem {"fs"};

		constexpr auto default_frequency = uint64_t {25'000'000};

	}	// namespace internal

	void init()
	{
		internal::bd.init();
		internal::bd.frequency(internal::default_frequency);
		internal::fs.mount(&internal::bd);
	}

}	// namespace sd

namespace leds {

	namespace internal {

		namespace ears {

			auto spi			= CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};
			constexpr auto size = 2;

		}	// namespace ears

		namespace belt {

			auto spi			= CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
			constexpr auto size = 20;

		}	// namespace belt

		namespace animations {

			auto event_loop = EventLoopKit {};

		}	// namespace animations

	}	// namespace internal

	auto ears = CoreLED<internal::ears::size> {internal::ears::spi};
	auto belt = CoreLED<internal::belt::size> {internal::belt::spi};

	auto kit = LedKit {internal::animations::event_loop, ears, belt};

	void turnOff()
	{
		ears.setColor(RGB::black);
		belt.setColor(RGB::black);
		ears.show();
		belt.show();
	}

}	// namespace leds

namespace motors {

	namespace left {

		namespace internal {

			auto dir_1 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
			auto dir_2 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
			auto speed = CorePwm {MOTOR_LEFT_PWM};

		}	// namespace internal

		auto motor = CoreMotor {internal::dir_1, internal::dir_2, internal::speed};

	}	// namespace left

	namespace right {

		namespace internal {

			auto dir_1 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_1};
			auto dir_2 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_2};
			auto speed = CorePwm {MOTOR_RIGHT_PWM};

		}	// namespace internal

		auto motor = CoreMotor {internal::dir_1, internal::dir_2, internal::speed};

	}	// namespace right

	void turnOff()
	{
		left::motor.stop();
		right::motor.stop();
	}

}	// namespace motors

namespace display {

	namespace internal {

		auto event_loop = EventLoopKit {};

		auto corell		   = CoreLL {};
		auto pixel		   = CGPixel {corell};
		auto hal		   = CoreSTM32Hal {};
		auto coresdram	   = CoreSDRAM {hal};
		auto coredma2d	   = CoreDMA2D {hal};
		auto coredsi	   = CoreDSI {hal};
		auto coreltdc	   = CoreLTDC {hal};
		auto coregraphics  = CoreGraphics {coredma2d};
		auto corefont	   = CoreFont {pixel};
		auto coreotm	   = CoreLCDDriverOTM8009A {coredsi, PinName::SCREEN_BACKLIGHT_PWM};
		auto corelcd	   = CoreLCD {coreotm};
		auto _corejpegmode = CoreJPEGModeDMA {hal};
		auto corejpeg	   = CoreJPEG {hal, _corejpegmode};

		extern "C" auto corevideo =
			CoreVideo {hal, coresdram, coredma2d, coredsi, coreltdc, corelcd, coregraphics, corefont, corejpeg};

	}	// namespace internal

	auto videokit = VideoKit {internal::event_loop, internal::corevideo};

}	// namespace display

auto behaviorkit = BehaviorKit {display::videokit, leds::kit, motors::left::motor, motors::right::motor};
auto hello		 = HelloWorld {};

}	// namespace

auto main() -> int
{
	logger::init();

	leds::turnOff();
	motors::turnOff();

	leds::kit.init();
	sd::init();
	display::internal::corelcd.turnOn();
	display::videokit.initializeScreen();

	log_info("Hello, World!\n\n");

	hello.start();

	while (true) {
		behaviorkit.launching();
		rtos::ThisThread::sleep_for(10s);
		behaviorkit.stop();
		rtos::ThisThread::sleep_for(3s);

		behaviorkit.sleeping();
		rtos::ThisThread::sleep_for(10s);
		behaviorkit.stop();
		rtos::ThisThread::sleep_for(3s);
	}
}
