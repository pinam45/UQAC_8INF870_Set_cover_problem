//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//

#include <limits>

#include "greedy.hpp"
#include "logger.hpp"

scp::Solution scp::greedy::unweighted_solve(const scp::Problem& problem)
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
			LOGGER->error("The problem has no solution");
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
	const std::chrono::duration<double> elapsed_seconds = end - start;
	LOGGER->info("found unweighted greedy solution with {} subsets cost of {} in {}s",
	             solution.selected_subsets.count(),
	             solution.cost,
	             elapsed_seconds.count());

	return solution;
}

scp::Solution scp::greedy::weighted_solve(const scp::Problem& problem)
{
	const auto start = std::chrono::system_clock::now();

	Solution solution(problem);
	while(!solution.cover_all_points)
	{
		size_t best_subset_number = solution.selected_subsets.size(); //invalid initial value
		dynamic_bitset<> covered_points_with_best_subset(problem.full_set.size());
		double best_ratio = 0;
		for(size_t i = 0; i < solution.selected_subsets.size(); ++i)
		{
			if(solution.selected_subsets[i])
			{
				// already selected
				continue;
			}

			const dynamic_bitset<> new_all_covered_points =
			  solution.covered_points | problem.subsets_points[i];
			// compute points that the new subset would add (not already covered).
			const dynamic_bitset<> new_covered_points_by_subset =
			  ~solution.covered_points & problem.subsets_points[i];
			const size_t new_covered_points_number = new_covered_points_by_subset.count();
			const double ratio = static_cast<double>(new_covered_points_number)
			                     / static_cast<double>(problem.subsets_costs[i]);
			if(ratio > best_ratio)
			{
				best_subset_number = i;
				covered_points_with_best_subset = new_all_covered_points;
				best_ratio = ratio;
			}
		}

		// all subset already included or no subset add covered points
		if(best_subset_number == solution.selected_subsets.size())
		{
			LOGGER->error("The problem has no solution");
			return solution;
		}

		// update solution (faster)
		solution.selected_subsets[best_subset_number].set();
		solution.covered_points = covered_points_with_best_subset;
		solution.cover_all_points = solution.covered_points.all();
		solution.cost += problem.subsets_costs[best_subset_number];

		// update solution (slower)
		//solution.compute_cover();
		//solution.compute_cost();
	}

	const auto end = std::chrono::system_clock::now();
	const std::chrono::duration<double> elapsed_seconds = end - start;
	SPDLOG_LOGGER_DEBUG(LOGGER,
	                    "found weighted greedy solution with {} subsets and cost of {} in {}s",
	                    solution.selected_subsets.count(),
	                    solution.cost,
	                    elapsed_seconds.count());

	return solution;
}

scp::Solution scp::greedy::weighted_solve(const scp::Problem& problem,
                                          const dynamic_bitset<>& allowed_subsets)
{
	assert(allowed_subsets.size() == problem.subsets_costs.size());
	const auto start = std::chrono::system_clock::now();

	Solution solution(problem);
	while(!solution.cover_all_points)
	{
		size_t best_subset_number = solution.selected_subsets.size(); //invalid initial value
		dynamic_bitset<> covered_points_with_best_subset(problem.full_set.size());
		double best_ratio = 0;
		for(size_t i = 0; i < solution.selected_subsets.size(); ++i)
		{
			if(!allowed_subsets[i])
			{
				// not allowed
				continue;
			}

			if(solution.selected_subsets[i])
			{
				// already selected
				continue;
			}

			const dynamic_bitset<> new_all_covered_points =
			  solution.covered_points | problem.subsets_points[i];
			// compute points that the new subset would add (not already covered).
			const dynamic_bitset<> new_covered_points_by_subset =
			  ~solution.covered_points & problem.subsets_points[i];
			const size_t new_covered_points_number = new_covered_points_by_subset.count();
			const double ratio = static_cast<double>(new_covered_points_number)
			                     / static_cast<double>(problem.subsets_costs[i]);
			if(ratio > best_ratio)
			{
				best_subset_number = i;
				covered_points_with_best_subset = new_all_covered_points;
				best_ratio = ratio;
			}
		}

		// all subset already included or no subset add covered points
		if(best_subset_number == solution.selected_subsets.size())
		{
			LOGGER->error("The problem has no solution");
			return solution;
		}

		// update solution (faster)
		solution.selected_subsets[best_subset_number].set();
		solution.covered_points = covered_points_with_best_subset;
		solution.cover_all_points = solution.covered_points.all();
		solution.cost += problem.subsets_costs[best_subset_number];

		// update solution (slower)
		//solution.compute_cover();
		//solution.compute_cost();
	}

	const auto end = std::chrono::system_clock::now();
	const std::chrono::duration<double> elapsed_seconds = end - start;
	SPDLOG_LOGGER_DEBUG(
	  LOGGER,
	  "found weighted greedy solution with {} subsets of {} allowed subsets and a cost of {} in {}s",
	  solution.selected_subsets.count(),
	  allowed_subsets.count(),
	  solution.cost,
	  elapsed_seconds.count());

	return solution;
}
