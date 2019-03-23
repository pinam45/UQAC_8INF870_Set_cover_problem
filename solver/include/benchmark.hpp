//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#ifndef SCPSOLVER_BENCHMARK_HPP
#define SCPSOLVER_BENCHMARK_HPP

#include "Problem.hpp"

#include <vector>
#include <string_view>

namespace scp::benchmark
{
	void benchmark_increment();

	void benchmark_generate_permutations();

	void benchmark_PermutationsGenerator();

	std::vector<scp::Problem> generate_problems(size_t points_number,
	                                            size_t last_problem_subsets_number,
	                                            std::default_random_engine& generator);

	bool save_problems(std::vector<Problem> problems,
	                   std::string_view files_names_prefix,
	                   std::string_view files_names_suffix = ".txt",
	                   bool override_file = false);
} // namespace scp::benchmark

#endif //SCPSOLVER_BENCHMARK_HPP
