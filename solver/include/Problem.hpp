//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#ifndef SCPSOLVER_PROBLEM_HPP
#define SCPSOLVER_PROBLEM_HPP

#include <dynamic_bitset.hpp>

#include <filesystem>
#include <ostream>

namespace scp
{
	struct Problem final
	{
		dynamic_bitset<> full_set; // all bits to 1
		std::vector<dynamic_bitset<>> subsets_points;
		std::vector<size_t> subsets_costs;

		Problem() = default;
		Problem(const Problem&) = default;
		Problem(Problem&&) noexcept = default;
		Problem& operator=(const Problem&) = default;
		Problem& operator=(Problem&&) noexcept = default;
	};
} // namespace scp

#endif //SCPSOLVER_PROBLEM_HPP
