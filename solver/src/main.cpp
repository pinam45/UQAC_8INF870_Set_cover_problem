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

#include <iostream>
#include <dynamic_bitset.hpp>
#include <vector>
#include <list>
#include <Solution.hpp>

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

	scp::Problem problem;
	if(!scp::read_problem(PROBLEM_FILE_PATH, problem))
	{
		LOGGER->error("Failed to read problem");
		return EXIT_FAILURE;
	}
	std::cout << problem;

	dynamic_bitset<> cover_check(problem.full_set.size());
	for(const auto& subset_points: problem.subsets_points)
	{
		cover_check |= subset_points;
	}
	assert(cover_check == problem.full_set);

	scp::Solution solution = scp::greedy::solve(problem);
	std::cout << solution;

	return EXIT_SUCCESS;
}
