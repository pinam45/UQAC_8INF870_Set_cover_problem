//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include "logger.hpp"
#include "Problem.hpp"
#include "greedy.hpp"
#include "descent.hpp"
#include "exhaustive.hpp"

#include <iostream>
#include <dynamic_bitset.hpp>
#include <vector>
#include <list>
#include <Solution.hpp>
#include <random>

namespace
{
	constexpr const char* PROBLEM_FILE_PATH = "resources/scp41.txt";
}

int main()
{
	if(!init_logger())
	{
		return EXIT_FAILURE;
	}
	LOGGER->info("SCPSolver start");
	{
		scp::Problem problem;
		if(!scp::read_problem(PROBLEM_FILE_PATH, problem))
		{
			LOGGER->error("Failed to read problem");
			return EXIT_FAILURE;
		}
		LOGGER->info("Problem: {}", problem);

		// check whether the problem can be solved (aka: all elements can be covered using the provided subsets).
		dynamic_bitset<> cover_check(problem.full_set.size());
		for(const auto& subset_points: problem.subsets_points)
		{
			cover_check |= subset_points;
		}

		if(!cover_check.all())
		{
			LOGGER->error(
			  "Unsolvable problem (some elements cannot be covered using provided subsets).");
			return EXIT_FAILURE;
		}

		scp::Solution greedy_solution = scp::greedy::solve(problem);
		LOGGER->info("Greedy solution: {}", greedy_solution);

		long seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine g(seed);
		scp::Solution improved_solution =
		  scp::descent::improve_by_annealing(greedy_solution, problem, g, 2000, 30.0, 1);
		LOGGER->info("Annealed solution: {}", improved_solution);

		scp::Solution optimal_solution = scp::exhaustive::solve(problem);
		LOGGER->info("Optimal solution: {}", optimal_solution);
	}
	LOGGER->info("SCPSolver end");
	return EXIT_SUCCESS;
}
