// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <iostream>
#include <utility>

#include "boost/ut.hpp"
#include "tests/config.h"
#include "tests/utils.h"

using namespace leka;
using namespace std::chrono;
namespace ut = boost::ut;

auto main() -> int
{
	ut::cfg<ut::override> = {.filter = "*", .dry_run = false};

	utils_start(TARGET);

	[[maybe_unused]] const auto result = ut::cfg<>.run({.report_errors = true});

	utils_end();

	return static_cast<int>(result);
}
