//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include "benchmark.hpp"
#include "permutations.hpp"
#include "logger.hpp"
#include "Solution.hpp"
#include "exhaustive.hpp"
#include "bnb.hpp"
#include "greedy.hpp"

#include <dynamic_bitset.hpp>

void scp::benchmark::benchmark_increment()
{
	LOGGER->info("Start increment benchmark");
	LOGGER->info("(bitset size, time in seconds)");
	size_t bitset_size = 0;
	do
	{
		const auto timer_start = std::chrono::system_clock::now();
		dynamic_bitset<> bitset(bitset_size);
		do
		{
			increment(bitset);
		} while(!bitset.none());
		const auto timer_end = std::chrono::system_clock::now();
		const std::chrono::duration<double> elapsed_seconds = timer_end - timer_start;
		LOGGER->info("({}, {})", bitset_size, elapsed_seconds.count());
	} while(++bitset_size);
	LOGGER->info("End increment benchmark");
}

void scp::benchmark::benchmark_generate_permutations()
{
	LOGGER->info("Start generate_permutations benchmark");
	LOGGER->info("(bitset size, time in seconds)");
	for(size_t bitset_size = 0; bitset_size < 28; ++bitset_size)
	{
		const auto timer_start = std::chrono::system_clock::now();
		std::vector<std::vector<dynamic_bitset<>>> permutations =
		  generate_permutations(bitset_size);
		const auto timer_end = std::chrono::system_clock::now();
		const std::chrono::duration<double> elapsed_seconds = timer_end - timer_start;
		LOGGER->info("({}, {})", bitset_size, elapsed_seconds.count());
	}
	LOGGER->info("End generate_permutations benchmark");
}

void scp::benchmark::benchmark_PermutationsGenerator()
{
	LOGGER->info("Start increment benchmark");
	LOGGER->info("(bitset size, time in seconds)");
	size_t bitset_size = 0;
	do
	{
		const auto timer_start = std::chrono::system_clock::now();
		for(size_t bits_on = 0; bits_on <= bitset_size; ++bits_on)
		{
			PermutationsGenerator perm(bitset_size, bits_on);
			while(!perm.finished())
			{
				perm.next();
			}
		}
		const auto timer_end = std::chrono::system_clock::now();
		const std::chrono::duration<double> elapsed_seconds = timer_end - timer_start;
		LOGGER->info("({}, {})", bitset_size, elapsed_seconds.count());
	} while(++bitset_size);
	LOGGER->info("End increment benchmark");
}

std::vector<scp::Problem> scp::benchmark::generate_problems(size_t points_number,
                                                            size_t last_problem_subsets_number,
                                                            std::default_random_engine& generator)
{
	assert(last_problem_subsets_number > 2);
	std::vector<scp::Problem> problems;
	problems.reserve(last_problem_subsets_number + 1);
	for(size_t subsets_number = 2; subsets_number <= last_problem_subsets_number; ++subsets_number)
	{
		problems.push_back(generate_problem(points_number, subsets_number, generator));
	}
	return problems;
}

bool scp::benchmark::save_problems(const std::vector<Problem>& problems,
                                   std::string_view files_names_prefix,
                                   std::string_view files_names_suffix,
                                   bool override_file)
{
	std::string file_name_prefix(files_names_prefix);
	for(const Problem& problem: problems)
	{
		std::string file_name(files_names_prefix);
		file_name += std::to_string(problem.subsets_points.size());
		file_name += files_names_suffix;
		if(!write_problem(problem, file_name, override_file))
		{
			return false;
		}
	}
	return true;
}

std::vector<scp::Problem> scp::benchmark::load_problems(std::string_view files_names_prefix,
                                                        std::string_view files_names_suffix)
{
	std::vector<Problem> problems;
	size_t subsets_number = 2;
	do
	{
		std::string file_name(files_names_prefix);
		file_name += std::to_string(subsets_number);
		file_name += files_names_suffix;

		Problem problem;
		if(!read_problem(file_name, problem))
		{
			break;
		}
		problems.push_back(problem);
		assert(problem.subsets_points.size() == subsets_number);

		++subsets_number;
	} while(true);

	return problems;
}

void scp::benchmark::benchmark_exhaustive_solve_ram(const std::vector<scp::Problem>& problems)
{
	LOGGER->info("Start exhaustive solve ram benchmark");
	LOGGER->info("(subsets number, time in seconds)");
	for(const Problem& problem: problems)
	{
		if(problem.subsets_points.size() > 27)
		{
			LOGGER->info("({}, -)", problem.subsets_points.size());
			continue;
		}
		const auto timer_start = std::chrono::system_clock::now();
		Solution solution = scp::exhaustive::solve_ram(problem);
		const auto timer_end = std::chrono::system_clock::now();
		const std::chrono::duration<double> elapsed_seconds = timer_end - timer_start;
		LOGGER->info("({}, {})", problem.subsets_points.size(), elapsed_seconds.count());
	}
	LOGGER->info("End exhaustive solve ram benchmark");
}

void scp::benchmark::benchmark_exhaustive_solve_cpu(const std::vector<scp::Problem>& problems)
{
	LOGGER->info("Start exhaustive solve cpu benchmark");
	LOGGER->info("(subsets number, time in seconds)");
	for(const Problem& problem: problems)
	{
		const auto timer_start = std::chrono::system_clock::now();
		Solution solution = scp::exhaustive::solve_cpu(problem);
		const auto timer_end = std::chrono::system_clock::now();
		const std::chrono::duration<double> elapsed_seconds = timer_end - timer_start;
		LOGGER->info("({}, {})", problem.subsets_points.size(), elapsed_seconds.count());
	}
	LOGGER->info("End exhaustive solve cpu benchmark");
}

void scp::benchmark::benchmark_exhaustive_solve_counter(const std::vector<scp::Problem>& problems)
{
	LOGGER->info("Start exhaustive solve counter benchmark");
	LOGGER->info("(subsets number, time in seconds)");
	for(const Problem& problem: problems)
	{
		const auto timer_start = std::chrono::system_clock::now();
		Solution solution = scp::exhaustive::solve_counter(problem);
		const auto timer_end = std::chrono::system_clock::now();
		const std::chrono::duration<double> elapsed_seconds = timer_end - timer_start;
		LOGGER->info("({}, {})", problem.subsets_points.size(), elapsed_seconds.count());
	}
	LOGGER->info("End exhaustive solve counter benchmark");
}

void scp::benchmark::benchmark_bnb_solve(const std::vector<scp::Problem>& problems)
{
	LOGGER->info("Start bnb solve benchmark");
	LOGGER->info("optimal solution cost (subsets number, time in seconds)");
	for(const Problem& problem: problems)
	{
		const auto timer_start = std::chrono::system_clock::now();
		Solution solution = scp::bnb::solve(problem);
		const auto timer_end = std::chrono::system_clock::now();
		const std::chrono::duration<double> elapsed_seconds = timer_end - timer_start;
		LOGGER->info(
		  "{} ({}, {})", solution.cost, problem.subsets_points.size(), elapsed_seconds.count());
	}
	LOGGER->info("End bnb solve benchmark");
}

namespace
{
	constexpr std::array<size_t, 101> opt_costs = {
			0, // 0
			0, // 1
			65, // 2
			63,
			165,
			183,
			220,
			224,
			230,
			163,
			287, // 10
			253,
			223,
			345,
			336,
			391,
			356,
			249,
			243,
			264,
			288,
			390,
			300,
			234,
			244,
			274,
			460,
			336,
			315,
			253,
			256, // 30
			327,
			409,
			401,
			334,
			185,
			411,
			310,
			247,
			247,
			358,
			398,
			368,
			507,
			360,
			305,
			263,
			278,
			214,
			270,
			0, // 50
			317,
			185,
			275,
			261,
			302,
			261,
			279,
			0,
			0,
			309, // 60
			129,
			260,
			0,
			290,
			200,
			0,
			195,
			231,
			0,
			225, // 70
			0,
			0,
			141,
			0,
			0,
			0,
			226,
			186,
			166,
			276, // 80
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			219,
			0, // 90
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0 // 100
	};

	template<typename Func>
	void helper_greedy_benchmark(const std::vector<scp::Problem>& problems,
	                             const std::string& greedy_name,
	                             Func&& solve)
	{
		LOGGER->info("Start {} benchmark", greedy_name);
		LOGGER->info("subsets number\taccuracy");
		for(const scp::Problem& problem: problems)
		{
			size_t prob_size = problem.subsets_points.size();
			size_t opt = opt_costs[prob_size];
			if (opt == 0) {
				continue;
			}

			scp::Solution solution = solve(problem);
			solution.compute_cost();
			double accuracy = static_cast<double>(solution.cost - opt) / static_cast<double>(opt);
			LOGGER->info("{}\t{}", prob_size, accuracy);
		}
		LOGGER->info("End {} benchmark", greedy_name);
	}
} // namespace

void scp::benchmark::benchmark_weighted_greedy(const std::vector<scp::Problem>& problems)
{
	helper_greedy_benchmark(problems, "weighted greedy", [](const Problem& p) { return scp::greedy::weighted_solve(p); });
}

void scp::benchmark::benchmark_unweighted_greedy(const std::vector<scp::Problem>& problems)
{
	helper_greedy_benchmark(problems, "unweighted greedy", scp::greedy::unweighted_solve);
}
