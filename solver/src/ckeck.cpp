//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include "ckeck.hpp"

bool scp::check::has_solution(const scp::Problem& problem)
{
	dynamic_bitset<> cover(problem.full_set.size());
	for(const auto& subset_points: problem.subsets_points)
	{
		cover |= subset_points;
	}

	return cover.all();
}
