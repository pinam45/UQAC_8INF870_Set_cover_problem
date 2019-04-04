//
// Copyright (c) 2019 Maxime Pinard and Benoît Cortier
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include "bnb.hpp"
#include "greedy.hpp"
#include "logger.hpp"

#include <chrono>
#include <iostream>

namespace
{
	void bnb(scp::Solution& solution, size_t current_bit, scp::Solution& best_solution);

	void bnb(scp::Solution& solution, size_t current_bit, scp::Solution& best_solution)
	{
		if(current_bit >= solution.selected_subsets.size())
		{
			return;
		}

		// case 0
		if(solution.cost < best_solution.cost)
		{
			bnb(solution, current_bit + 1, best_solution);
		}
		else
		{
			assert(false); // impossible, branch cut at last 1 set
			best_solution = solution;
			// best in this branch
			// adding 1 will increase cost
			// adding 0 will do nothing
			return;
		}

		// case 1
		solution.selected_subsets.set(current_bit);
		solution.compute_cost();
		if(solution.cost < best_solution.cost)
		{
			solution.compute_cover();
			if(solution.cover_all_points)
			{
				best_solution = solution;
				// best in this branch
				// adding 1 will increase cost
				// adding 0 will do nothing
				solution.selected_subsets.reset(current_bit);
				return;
			}
			else
			{
				bnb(solution, current_bit + 1, best_solution);
			}
		}
		solution.selected_subsets.reset(current_bit);
	}

	void bnb_old(scp::Solution& solution, size_t current_bit, scp::Solution& best_solution)
	{
		if(current_bit >= solution.selected_subsets.size())
		{
			return;
		}

		// case 0
		if((!solution.cover_all_points) || (solution.cost > best_solution.cost))
		{
			bnb(solution, current_bit + 1, best_solution);
		}
		else
		{
			assert(false); // impossible, branch cut at last 1 set
			best_solution = solution;
			// best in this branch
			// adding 1 will increase cost
			// adding 0 will do nothing
			return;
		}

		// case 1
		solution.selected_subsets.set(current_bit);
		solution.cost += solution.problem.subsets_costs[current_bit];
		solution.compute_cover();
		if(!solution.cover_all_points)
		{
			bnb(solution, current_bit + 1, best_solution);
		}
		else
		{
			//solution.compute_cost();
			if(solution.cost > best_solution.cost)
			{
				bnb(solution, current_bit + 1, best_solution);
			}
			else
			{
				best_solution = solution;
				// best in this branch
				// adding 1 will increase cost
				// adding 0 will do nothing
				solution.selected_subsets.reset(current_bit);
				solution.cost -= solution.problem.subsets_costs[current_bit];
				return;
			}
		}

		solution.selected_subsets.reset(current_bit);
	}
} // namespace

scp::Solution scp::bnb::solve(const scp::Problem& problem)
{
	const auto start = std::chrono::system_clock::now();
	Solution best_solution = greedy::weighted_solve(problem);

	Solution empty_solution(problem);
	::bnb(empty_solution, 0, best_solution);

	const auto end = std::chrono::system_clock::now();
	const std::chrono::duration<double> elapsed_seconds = end - start;
	SPDLOG_LOGGER_DEBUG(
	  LOGGER,
	  "found optimal solution by branch and bound with {} subsets and a cost of {} in {}s",
	  best_solution.selected_subsets.count(),
	  best_solution.cost,
	  elapsed_seconds.count());
	return best_solution;
}
