//
// Copyright (c) 2019 Maxime Pinard and Benoît Cortier
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
#include "benchmark.hpp"
#include "bnb.hpp"
#include "nomad.hpp"

#include <dynamic_bitset.hpp>

#include <iostream>
#include <vector>
#include <list>
#include <random>

namespace
{
	constexpr bool GENERATE_PROBLEM = false;
	constexpr bool BENCHMARK = true;
	constexpr bool NOMAD_GENETIC = false;
	constexpr bool NOMAD_ANNEALING = false;

	constexpr const char* BENCHMARK_PROBLEMS_FILES_PREFIX =
	  "resources/benchmark/benchmarck_problem_";
	constexpr const char* PROBLEM_FILE_PATH = "resources/OR-Library/scpa2.txt";
	//constexpr const char* PROBLEM_FILE_PATH = "last_problem.txt";

	bool generate_benchmark_problems()
	{
		std::default_random_engine g(std::random_device{}());
		return scp::benchmark::save_problems(scp::benchmark::generate_problems(200, 100, g),
		                                     "benchmarck_problem_");
	}

	void launch_benchmark()
	{
		// generate benchmark problems
		/*if(!::generate_benchmark_problems())
		{
			LOGGER->error("generate_benchmark_problems failed");
		}*/

		// benchmark permutations generators
		//benchmark_increment();
		//benchmark_generate_permutations();
		//benchmark_PermutationsGenerator();

		// load benchmark problems
		std::vector<scp::Problem> problems =
		  scp::benchmark::load_problems(BENCHMARK_PROBLEMS_FILES_PREFIX);

		// call benchmarks
		//scp::benchmark::benchmark_exhaustive_solve_counter(problems);
		scp::benchmark::benchmark_bnb_solve(problems);
		//scp::benchmark::benchmark_unweighted_greedy(problems);
		//scp::benchmark::benchmark_weighted_greedy(problems);
	}
} // namespace

int main(int argc, char** argv)
{
	if constexpr(NOMAD_GENETIC)
	{
		return nomad::run_genetic(argc, argv);
	}
	else if constexpr(NOMAD_ANNEALING)
	{
		return nomad::run_annealing(argc, argv);
	}

	if(!init_logger())
	{
		return EXIT_FAILURE;
	}
	LOGGER->info("SCPSolver start");
	{
		if constexpr(BENCHMARK)
		{
			launch_benchmark();
			return EXIT_SUCCESS;
		}

		std::default_random_engine g(std::random_device{}());

		// read problem
		scp::Problem problem;
		if constexpr(GENERATE_PROBLEM)
		{
			problem = scp::generate_problem(50, 25, g);

			// save problem
			if(!scp::write_problem(problem, "last_problem.txt", true))
			{
				LOGGER->error("Failed to write problem");
				return EXIT_FAILURE;
			}
		}
		else
		{
			if(!scp::read_problem(PROBLEM_FILE_PATH, problem))
			{
				LOGGER->error("Failed to read problem");
				return EXIT_FAILURE;
			}
		}

		// print problem
		LOGGER->info("Problem: {}", problem);

		// check if the problem have a solution
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
		  scp::descent::improve_by_annealing(scp::generate_random_solution(problem, g), g);
		LOGGER->info("Annealed solution: {}", annealed_solution);

		if(problem.subsets_points.size() < 23)
		{ // to prevent memory'splosion
			scp::Solution optimal_solution = scp::exhaustive::solve_ram(problem);
			LOGGER->info("Optimal solution: {}", optimal_solution);
		}
		else if(problem.subsets_points.size() < 26)
		{ // to prevent endless wait
			scp::Solution optimal_solution = scp::exhaustive::solve_cpu(problem);
			LOGGER->info("Optimal solution: {}", optimal_solution);
		}
		else if(problem.subsets_points.size() < 30)
		{
			scp::Solution optimal_solution = scp::bnb::solve(problem);
			LOGGER->info("Branch and bound solution: {}", optimal_solution);
		}

		scp::genetic::Config conf{};
		conf.population_size = 75;
		conf.execution_duration = std::chrono::seconds(60);
		conf.replacement_ratio = 0.75;
		conf.mutation_probability = 0.1;
		conf.local_search_probability = 0.3;
		conf.descent_config.iterations_number = 4500;
		conf.descent_config.initial_temperature = 30;
		conf.descent_config.final_temperature = 1;
		LOGGER->info("Optimized solution: {}", scp::genetic::solve(problem, conf));
	}
	LOGGER->info("SCPSolver end");
	return EXIT_SUCCESS;
}
