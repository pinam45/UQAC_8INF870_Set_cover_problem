//
// Created by Benoit Cortier on 3/14/19.
//
#include "genetic.hpp"
#include "greedy.hpp"
#include "descent.hpp"
#include "neighbouring.hpp"
#include "crossover.hpp"
#include "logger.hpp"

#include <vector>
#include <limits>
#include <random>
#include <algorithm>

namespace
{
	scp::Solution generate_random_solution(const scp::Problem& problem,
	                                       std::default_random_engine& generator)
	{
		scp::Solution solution(problem);
		while(!solution.cover_all_points)
		{
			scp::neighbouring::flip_a_zero_to_one_unsafe(solution, generator);
			solution.compute_cover();
		}

		return solution;
	}

	size_t select_by_rank(size_t population_size, std::default_random_engine& generator)
	{
		const size_t rank_sum = (population_size * (population_size + 1)) / 2;

		std::uniform_int_distribution<size_t> dist(0, rank_sum);
		const size_t rnd = dist(generator);

		size_t accumulated_rank = 1;
		size_t current_rank = 1;
		while(accumulated_rank <= rnd)
		{
			accumulated_rank += ++current_rank;
		}

		return current_rank - 1;
	}
} // namespace

std::ostream& scp::genetic::operator<<(std::ostream& os, const scp::genetic::Config& conf)
{
	os << "scp::genetic::Config{\n";
	os << "\tpopulation size = " << conf.population_size << ",\n";
	os << "\tnumber of iterations = " << conf.iteration_number << ",\n";
	os << "}\n";
	return os;
}

scp::Solution scp::genetic::solve(const scp::Problem& problem, const Config& conf)
{
	assert(conf.population_size > 1);
	assert(conf.iteration_number > 0);
	assert(conf.replacement_ratio <= 1);

	LOGGER->info("Start solving by genetic optimization using {}.", conf);

	std::vector<Solution> population;
	population.reserve(conf.population_size);

	long long seed = std::random_device{}();
	LOGGER->info("Seed = {}", seed);
	std::default_random_engine generator{seed};

	const auto replacements_per_iteration =
	  static_cast<size_t>(static_cast<double>(conf.population_size) * conf.replacement_ratio);

	const auto start = std::chrono::system_clock::now();

	LOGGER->info("Populate...");
	Solution best = greedy::weighted_solve(
	  problem, generate_random_solution(problem, generator).selected_subsets);
	population.push_back(best);
	while(population.size() < conf.population_size)
	{
		Solution s = greedy::weighted_solve(
		  problem, generate_random_solution(problem, generator).selected_subsets);
		if(s.cost < best.cost)
		{
			best = s;
		}
		population.push_back(std::move(s));
	}
	LOGGER->info("Populate OK!");

	// initialize offsprings vector
	std::vector<Solution> offsprings;
	offsprings.resize(replacements_per_iteration, best);

	for(size_t gen = 0; gen < conf.iteration_number; ++gen)
	{
		LOGGER->info("|\tGeneration {}\t|", gen);

		// sort by descending cost to perform rank based selection
		std::sort(population.begin(), population.end(), [](const Solution& a, const Solution& b) {
			return b.cost < a.cost;
		});

		// create offsprings by crossover + local search
		for(size_t i = 0; i < replacements_per_iteration; ++i)
		{
			Solution& parent1 = population[select_by_rank(population.size(), generator)];
			Solution& parent2 = population[select_by_rank(population.size(), generator)];

			Solution offspring = crossover::solve_subproblem_from(parent1, parent2); // crossover
			offsprings[i] = descent::improve_by_annealing(
			  offspring, generator, {2000, 30.0, 1.0}); // local search
			if(offsprings[i].cost < best.cost)
			{
				best = offsprings[i];
			}
		}

		// replace worst individuals by offsprings
		for(size_t i = 0; i < replacements_per_iteration; ++i)
		{
			population[i] = std::move(offsprings[i]);
		}
	}

	// get rid of useless subsets using weighted greedy
	best = greedy::weighted_solve(problem, best.selected_subsets);

	const auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	LOGGER->info("found solution by genetic optimization with {} subsets and cost of {} in {}s",
	             best.selected_subsets.count(),
	             best.cost,
	             elapsed_seconds.count());

	return best;
}
