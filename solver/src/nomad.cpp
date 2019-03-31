//
// Created by Benoît Cortier on 3/29/19.
//

#include <iostream>
#include <fstream>
#include <cstdlib>

#include "descent.hpp"
#include "nomad.hpp"
#include "genetic.hpp"
#include "ckeck.hpp"
#include "greedy.hpp"

namespace
{
	const char* get_problem_path()
	{
		if(const char* env_problem = std::getenv("SCP_PROBLEM_PATH"))
		{
			return env_problem;
		}
		else
		{
			return "resources/OR-Library/scpa1.txt";
		}
	}

	int init(int argc, char** argv, scp::Problem& problem, double* x, size_t dim)
	{
		if(argc < 2)
		{
			std::cerr << "failure: wrong arguments count\n";
			return EXIT_FAILURE;
		}

		if(!scp::read_problem(get_problem_path(), problem))
		{
			std::cerr << "failure: couldn't read problem\n";
			return EXIT_FAILURE;
		}
		if(!scp::check::has_solution(problem))
		{
			std::cerr << "failure: problem has no solution\n";
			return EXIT_FAILURE;
		}

		std::ifstream in{argv[1]};
		for(size_t i = 0; i < dim; ++i)
		{
			in >> x[i];
		}
		if(in.fail())
		{
			std::cerr << "failure: couldn't read input parameters\n";
			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}
} // namespace

int nomad::run_genetic(int argc, char** argv)
{
	constexpr size_t dim = 6;
	double x[dim];
	scp::Problem problem;
	if(init(argc, argv, problem, x, dim) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	scp::genetic::Config conf{};
	conf.population_size = static_cast<size_t>(x[0]);
	conf.execution_duration = std::chrono::seconds(60);
	conf.replacement_ratio = x[1];
	conf.mutation_probability = x[2];
	conf.local_search_probability = x[3];
	conf.descent_config.iterations_number = static_cast<size_t>(x[4]);
	conf.descent_config.initial_temperature = x[5];
	conf.descent_config.final_temperature = 1;
	std::cout << scp::genetic::solve(problem, conf).cost;

	return EXIT_SUCCESS;
}

int nomad::run_annealing(int argc, char** argv)
{
	constexpr size_t dim = 2;
	double x[dim];
	scp::Problem problem;
	if(init(argc, argv, problem, x, dim) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	std::default_random_engine g(std::random_device{}());

	scp::descent::Config conf{};
	conf.iterations_number = 50000000;
	conf.initial_temperature = x[0];
	conf.final_temperature = x[1];
	if(conf.initial_temperature < conf.final_temperature)
	{
		std::cerr << "failure: initial temperature < final temperature\n";
		return EXIT_FAILURE;
	}
	std::cout << scp::descent::improve_by_annealing(
	               scp::generate_random_solution(problem, g), g, conf)
	               .cost;

	return EXIT_SUCCESS;
}
