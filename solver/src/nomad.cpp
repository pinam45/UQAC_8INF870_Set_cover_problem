//
// Created by Benoît Cortier on 3/29/19.
//

#include <iostream>
#include <fstream>

#include "descent.hpp"
#include "nomad.hpp"
#include "genetic.hpp"
#include "ckeck.hpp"
#include "greedy.hpp"

constexpr const char* PROBLEM_FILE_PATH = "../solver/resources/OR-Library/scpa1.txt";

int nomad::run_genetic(int argc, char** argv) {
	if (argc < 2) {
		return EXIT_FAILURE;
	}

	scp::Problem problem;
	if (!scp::read_problem(PROBLEM_FILE_PATH, problem)) {
		return EXIT_FAILURE;
	}
	if(!scp::check::has_solution(problem)) {
		return EXIT_FAILURE;
	}

	double x[6];
	std::ifstream in{argv[1]};
	for (double& i : x) {
		in >> i;
	}
	if (in.fail()) {
		return EXIT_FAILURE;
	}

	scp::genetic::Config conf{};
	conf.population_size = static_cast<size_t>(x[0]);
	conf.execution_duration = std::chrono::seconds(30);
	conf.replacement_ratio = x[1];
	conf.mutation_probability = x[2];
	conf.local_search_probability = x[3];
	conf.descent_config.iterations_number = static_cast<size_t>(x[4]);
	conf.descent_config.initial_temperature = x[5];
	conf.descent_config.final_temperature = 1;
	std::cout << scp::genetic::solve(problem, conf).cost;

	return EXIT_SUCCESS;
}

int nomad::run_annealing(int argc, char** argv) {
	if (argc < 2) {
		return EXIT_FAILURE;
	}

	scp::Problem problem;
	if (!scp::read_problem(PROBLEM_FILE_PATH, problem)) {
		return EXIT_FAILURE;
	}
	if(!scp::check::has_solution(problem)) {
		return EXIT_FAILURE;
	}

	double x[2];
	std::ifstream in{argv[1]};
	for (double& i : x) {
		in >> i;
	}
	if (in.fail()) {
		return EXIT_FAILURE;
	}

	std::default_random_engine g(std::random_device{}());

	scp::descent::Config conf{};
	conf.iterations_number = 500000;
	conf.initial_temperature = static_cast<size_t>(x[0]);
	conf.final_temperature = static_cast<size_t>(x[1]);
	if (conf.initial_temperature < conf.final_temperature) {
		return EXIT_FAILURE;
	}
	std::cout << scp::descent::improve_by_annealing(scp::greedy::weighted_solve(problem), g, conf).cost;

	return EXIT_SUCCESS;
}
