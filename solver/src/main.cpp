//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include "logger.hpp"
#include "Problem.hpp"
#include "Solution.hpp"
#include "greedy.hpp"
#include "descent.hpp"
#include "exhaustive.hpp"
#include "crossover.hpp"
#include "genetic.hpp"
#include "ckeck.hpp"

#include <dynamic_bitset.hpp>

#include <iostream>
#include <vector>
#include <list>
#include <random>

namespace
{
	constexpr const char* PROBLEM_FILE_PATH = "resources/scpa2.txt";
}

int main()
{
	if(!init_logger())
	{
		return EXIT_FAILURE;
	}
	LOGGER->info("SCPSolver start");
	{
		std::default_random_engine g(std::random_device{}());

		// read problem
		scp::Problem problem;
		if(!scp::read_problem(PROBLEM_FILE_PATH, problem))
		{
			LOGGER->error("Failed to read problem");
			return EXIT_FAILURE;
		}

		// generate problem
		/*scp::Problem problem = scp::generate_problem(50, 25, g, 1, 10, 1, 50);
		if(!scp::write_problem(problem, "last_problem.txt", true))
		{
			LOGGER->error("Failed to write problem");
			return EXIT_FAILURE;
		}
		LOGGER->info("Problem: {}", problem);*/

		// check whether the problem can be solved (aka: all elements can be covered using the provided subsets).
		dynamic_bitset<> cover_check(problem.full_set.size());
		for(const auto& subset_points: problem.subsets_points)
		{
			cover_check |= subset_points;
		}

		if(!scp::check::has_solution(problem))
		{
			LOGGER->error(
			  "Unsolvable problem (some elements cannot be covered using provided subsets).");
			return EXIT_FAILURE;
		}

		// solve
		scp::Solution unweighted_greedy_solution = scp::greedy::unweighted_solve(problem);
		LOGGER->info("Unweighted greedy solution: {}", unweighted_greedy_solution);

		scp::Solution weighted_greedy_solution = scp::greedy::weighted_solve(problem);
		LOGGER->info("Weighted greedy solution: {}", weighted_greedy_solution);

		scp::Solution annealed_solution =
		  scp::descent::improve_by_annealing(weighted_greedy_solution, g, {200000, 50.0, 1});
		LOGGER->info("Annealed solution: {}", annealed_solution);

		if(problem.subsets_points.size() < 23)
		{ // to prevent memory'splosion
			scp::Solution optimal_solution = scp::exhaustive::solve(problem);
			LOGGER->info("Optimal solution: {}", optimal_solution);
		}

		LOGGER->info("Optimized solution: {}", scp::genetic::solve(problem));
	}
	LOGGER->info("SCPSolver end");
	return EXIT_SUCCESS;
}
