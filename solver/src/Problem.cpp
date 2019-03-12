//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include "Problem.hpp"
#include "ostream_config_guard.hpp"

#include <iomanip>

std::ostream& scp::operator<<(std::ostream& os, const scp::Problem& problem)
{
	os << "problem{\n";
	os << "\tpoints number = " << problem.full_set.size() << ",\n";
	os << "\tsubsets number = " << problem.subsets_points.size() << ",\n";
	os << "\tsubsets points = {\n";
	ostream_config_guard guard(os, std::setfill(' '));
	const int width = static_cast<int>(std::to_string(problem.subsets_points.size() - 1).size());
	for(size_t i = 0; i < problem.subsets_points.size(); ++i)
	{
		os << "\t\t (" << std::setw(width) << i << ") " << problem.subsets_points[i] << "\n";
	}
	os << "\t},\n";
	os << "}\n";
	return os;
}
