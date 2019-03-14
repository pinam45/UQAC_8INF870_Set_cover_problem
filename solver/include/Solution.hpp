//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#ifndef SCPSOLVER_SOLUTION_HPP
#define SCPSOLVER_SOLUTION_HPP

#include "Problem.hpp"

#include <dynamic_bitset.hpp>
#include <spdlog/fmt/ostr.h> // for operator<<

#include <filesystem>
#include <ostream>

namespace scp
{
	struct Solution final
	{
		static constexpr size_t invalid_cost = 0;

		const Problem& problem;
		dynamic_bitset<> selected_subsets;
		dynamic_bitset<> covered_points;
		bool cover_all_points;
		size_t cost;

		explicit Solution(const Problem& problem) noexcept;
		Solution(const Solution& other) = default;
		Solution(Solution&& other) noexcept = default;
		Solution& operator=(const Solution& other);
		Solution& operator=(Solution&& other) noexcept;

		void compute_cover() noexcept;
		void compute_cost() noexcept;
	};
	std::ostream& operator<<(std::ostream& os, const Solution& solution);
} // namespace scp

#endif //SCPSOLVER_SOLUTION_HPP
