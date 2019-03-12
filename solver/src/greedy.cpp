//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include "greedy.hpp"
#include "logger.hpp"

scp::Solution scp::greedy::solve(const scp::Problem& problem)
{
	const auto start = std::chrono::system_clock::now();

	Solution solution(problem);
	while(!solution.cover_all_points)
	{
		size_t max_subset_number = solution.selected_subsets.size(); //invalid initial value
		dynamic_bitset<> covered_points_with_max_subset(problem.full_set.size());
		size_t covered_points_number_with_max_subset = solution.covered_points.count();
		for(size_t i = 0; i < solution.selected_subsets.size(); ++i)
		{
			if(solution.selected_subsets[i])
			{
				// already selected
				continue;
			}

			dynamic_bitset<> new_covered_points =
			  solution.covered_points | problem.subsets_points[i];
			const size_t new_covered_points_number = new_covered_points.count();
			if(new_covered_points_number > covered_points_number_with_max_subset)
			{
				max_subset_number = i;
				covered_points_with_max_subset = new_covered_points;
				covered_points_number_with_max_subset = new_covered_points_number;
			}
		}

		// all subset already included or no subset add covered points
		if(max_subset_number == solution.selected_subsets.size())
		{
			LOGGER->error("The problem have no solution");
			return solution;
		}

		// update solution (faster)
		solution.selected_subsets[max_subset_number].set();
		solution.covered_points = covered_points_with_max_subset;
		solution.cover_all_points = solution.covered_points.all();
		solution.cost += problem.subsets_costs[max_subset_number];

		// update solution (slower)
		//solution.compute_cover();
		//solution.compute_cost();
	}

	const auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	LOGGER->info("found greedy solution with {} subsets in {}s",
	             solution.selected_subsets.count(),
	             elapsed_seconds.count());

	return solution;
}
