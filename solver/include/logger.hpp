//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#ifndef SCPSOLVER_LOGGER_HPP
#define SCPSOLVER_LOGGER_HPP

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <filesystem>

extern const std::shared_ptr<spdlog::logger> LOGGER;
extern const std::shared_ptr<spdlog::logger> NULL_LOGGER;

bool init_logger();

inline std::ostream& operator<<(std::ostream& os, const std::filesystem::path& p)
{
	return os << '\"' << p.generic_string() << '\"';
}

#endif //SCPSOLVER_LOGGER_HPP
