//
// Copyright (c) 2019 Maxime Pinard and Benoît Cortier
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include <dynamic_bitset.hpp>

#include "crossover.hpp"
#include "greedy.hpp"

scp::Solution scp::crossover::solve_subproblem_from(const scp::Solution& sol1,
                                                    const scp::Solution& sol2)
{
	return greedy::weighted_solve(sol1.problem, sol1.selected_subsets | sol2.selected_subsets);
}

scp::Solution scp::crossover::random_merge(const scp::Solution& sol1,
                                           const scp::Solution& sol2,
                                           std::default_random_engine& generator)
{
	dynamic_bitset<> selectable_subsets = sol1.selected_subsets | sol2.selected_subsets;
	size_t selectable_count = selectable_subsets.count();

	scp::Solution sol(sol1.problem);

	while(!sol.cover_all_points)
	{
		size_t selected_subset =
		  std::uniform_int_distribution<size_t>(0, selectable_count - 1)(generator);
		--selectable_count;

		size_t idx = selectable_subsets.find_first();
		for(size_t i = 0; i < selected_subset; ++i)
		{
			idx = selectable_subsets.find_next(idx);
		}
		sol.selected_subsets.set(idx);
		selectable_subsets.reset(idx);

		sol.compute_cover();
	}

	sol.compute_cost();
	return sol;
}

scp::Solution scp::crossover::fat_merge(const scp::Solution& sol1, const scp::Solution& sol2)
{
	scp::Solution sol(sol1.problem);
	sol.selected_subsets = sol1.selected_subsets | sol2.selected_subsets;
	sol.compute_cover();
	sol.compute_cost();
	return sol;
}