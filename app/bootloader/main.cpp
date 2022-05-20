// Leka - LekaOS
// Copyright 2020 Embedded Planet
// Copyright 2020 ARM Limited
// SPDX-License-Identifier: Apache-2.0
// Original file: https://github.com/mcu-tools/mcuboot/blob/main/boot/mbed/mcuboot_main.cpp

#include "mbed_application.h"
#include "mbedtls/platform.h"

#include "drivers/InterruptIn.h"
#include "rtos/ThisThread.h"

#include "ConfigKit.h"
#include "CoreBattery.h"
#include "CoreFlashIS25LP016D.h"
#include "CoreFlashManagerIS25LP016D.h"
#include "CoreLED.h"
#include "CoreMotor.h"
#include "CorePwm.h"
#include "CoreQSPI.h"
#include "CoreSPI.h"
#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "FirmwareKit.h"
#include "SDBlockDevice.h"
#include "bootutil/bootutil.h"
#include "bootutil/image.h"

using namespace std::chrono;
using namespace leka;

//
// MARK: - Global definitions
//

namespace {

namespace bootloader {

	constexpr auto version = uint8_t {1};

}

namespace os {

	constexpr auto start_address = uint32_t {0x8040000 + 0x1000};	// Start application address + header

}	// namespace os

namespace battery {

	namespace charge {

		auto status_input = mbed::InterruptIn {PinName::BATTERY_CHARGE_STATUS};

	}

	constexpr auto default_hysteresis_offset = uint8_t {5};
	constexpr auto minimum_working_level	 = float {8.0};

	auto cells = CoreBattery {PinName::BATTERY_VOLTAGE, battery::charge::status_input};

}	// namespace battery

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

namespace factory_reset {

	constexpr auto default_limit = uint8_t {10};

	namespace internal {

		constexpr auto factory_reset_counter_path = "/fs/conf/factory_reset_counter";

		auto qspi	 = CoreQSPI();
		auto manager = CoreFlashManagerIS25LP016D(qspi);
		auto flash	 = CoreFlashIS25LP016D(qspi, manager);

	}	// namespace internal

	auto firmwarekit = FirmwareKit(internal::flash);

	void initializeExternalFlash()
	{
		internal::flash.reset();
		internal::qspi.setDataTransmissionFormat();
		internal::qspi.setFrequency(flash::is25lp016d::max_clock_frequency_in_hz);
	}

	auto getCounter() -> uint8_t
	{
		FileManagerKit::File file {internal::factory_reset_counter_path, "r"};

		if (!file.is_open()) {
			return default_limit + 1;
		}

		auto data = std::array<uint8_t, 1> {};
		file.read(data);

		return data.front();
	}

	void setCounter(uint8_t value)
	{
		FileManagerKit::File file {internal::factory_reset_counter_path, "w+"};

		if (!file.is_open()) {
			return;
		}

		auto output = std::to_array<uint8_t>({value});
		file.write(output);
	}

	void incrementCounter()
	{
		auto counter = factory_reset::getCounter();
		counter += 1;
		setCounter(counter);
	}

	void resetCounter()
	{
		setCounter(0);
	}

	void applyFactoryReset()
	{
		auto firmware_version = FirmwareVersion {.major = 1, .minor = 0, .revision = 0};
		firmwarekit.loadUpdate(firmware_version);
		boot_set_pending(1);
	}

}	// namespace factory_reset

namespace config {

	auto bootloader_version = Config {"/fs/conf/bootloader_version", bootloader::version};
	auto battery_hysteresis_offset =
		Config {"/fs/conf/bootloader_battery_hysteresis_offset", battery::default_hysteresis_offset};
	auto factory_reset_limit = Config {"/fs/conf/factory_reset_limit", factory_reset::default_limit};

	auto configkit = ConfigKit {};

	void setBootloaderVersion()
	{
		configkit.write(bootloader_version, bootloader_version.default_value());
	}

	auto batteryHysteresisOffset() -> uint8_t
	{
		return configkit.read(config::battery_hysteresis_offset);
	}

	auto shouldApplyFactoryReset() -> bool
	{
		auto counter = factory_reset::getCounter();
		auto limit	 = configkit.read(config::factory_reset_limit);

		return counter > limit;
	}

}	// namespace config

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

	}	// namespace internal

	auto ears = CoreLED<internal::ears::size> {internal::ears::spi};
	auto belt = CoreLED<internal::belt::size> {internal::belt::spi};

	namespace internal {

		constexpr auto light_blue = RGB {0x00, 0x22, 0x43};

		void blink()
		{
			leds::ears.setColor(light_blue);
			leds::ears.show();
			rtos::ThisThread::sleep_for(100ms);
			leds::ears.setColor(RGB::black);
			leds::ears.show();
		}

	}	// namespace internal

	namespace blink {

		void lowEnergy()
		{
			internal::blink();
			rtos::ThisThread::sleep_for(2s);
		}

		void mediumEnergy()
		{
			internal::blink();
			rtos::ThisThread::sleep_for(100ms);
			internal::blink();
			rtos::ThisThread::sleep_for(2s);
		}

		void highEnergy()
		{
			internal::blink();
			rtos::ThisThread::sleep_for(100ms);
			internal::blink();
			rtos::ThisThread::sleep_for(100ms);
			internal::blink();
			rtos::ThisThread::sleep_for(2s);
		}

	}	// namespace blink

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

}	// namespace

//
// MARK: - main()
//

auto main() -> int
{
	leds::turnOff();
	motors::turnOff();

	sd::init();

	// ? As bootloader, os & sd card can evelove independently, there is no way to know which
	// ? version of the bootloader is being used a priori.
	// ? On first run, we write the bootloader version to a special config file, to allow us to:
	// ?    - share the information with the os
	// ?    - guarantee the persistence of the information in case of sd card is changed
	// ?    - or in case of os update/modification
	config::setBootloaderVersion();

	while (battery::cells.level() < 0 + config::batteryHysteresisOffset()) {
		if (battery::cells.isCharging() && battery::cells.voltage() < battery::minimum_working_level) {
			leds::blink::lowEnergy();
		} else if (battery::cells.isCharging() && battery::cells.voltage() < leka::CoreBattery::Capacity::empty) {
			leds::blink::mediumEnergy();
		} else if (battery::cells.isCharging()) {
			leds::blink::highEnergy();
		}

		rtos::ThisThread::sleep_for(5s);
	}

	if (config::shouldApplyFactoryReset()) {
		factory_reset::initializeExternalFlash();

		factory_reset::applyFactoryReset();
		factory_reset::resetCounter();

	} else {
		factory_reset::incrementCounter();
	}

	auto start_address = os::start_address;

	if (battery::cells.isCharging() && battery::cells.voltage() > CoreBattery::Capacity::quarter) {
		// Initialize mbedtls crypto for use by MCUboot
		mbedtls_platform_context unused_ctx;
		if (auto ret = mbedtls_platform_setup(&unused_ctx); ret != 0) {
			exit(ret);
		}

		// Create handler to get application information
		auto boot_handler = boot_rsp {};
		if (auto ret = boot_go(&boot_handler); ret != FIH_SUCCESS) {
			exit(ret);
		}

		start_address = boot_handler.br_image_off + boot_handler.br_hdr->ih_hdr_size;
	}

	// Run the application in the primary slot
	// Add header size offset to calculate the actual start address of application
	mbed_start_application(start_address);
}
