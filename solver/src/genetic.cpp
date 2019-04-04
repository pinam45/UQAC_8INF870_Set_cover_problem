//
// Copyright (c) 2019 Maxime Pinard and Benoît Cortier
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include "genetic.hpp"
#include "greedy.hpp"
#include "neighbouring.hpp"
#include "crossover.hpp"
#include "logger.hpp"

#include <vector>
#include <limits>
#include <random>
#include <algorithm>

namespace
{
	size_t select_by_rank(size_t population_size, std::default_random_engine& generator)
	{
		assert(population_size > 0);
		const size_t rank_sum = (population_size * (population_size + 1)) / 2;

		std::uniform_int_distribution<size_t> dist(0, rank_sum);
		const size_t rnd = dist(generator);

		size_t accumulated_rank = 0;
		for(size_t i = 0; i < population_size; ++i)
		{
			accumulated_rank += (i + 1);
			if(accumulated_rank >= rnd)
			{
				return i;
			}
		}

		assert(false); //impossible
		return population_size;
	}
} // namespace

std::ostream& scp::genetic::operator<<(std::ostream& os, const scp::genetic::Config& conf)
{
	os << "scp::genetic::Config{\n";
	os << "\tpopulation size = " << conf.population_size << ",\n";
	os << "\texecution duration = " << conf.execution_duration.count() << "s"
	   << ",\n";
	os << "\treplacement ratio = " << conf.replacement_ratio << ",\n";
	os << "\tmutation probability = " << conf.mutation_probability << ",\n";
	os << "\tlocal search probability = " << conf.local_search_probability << ",\n";
	os << "\tdescent config = " << conf.descent_config << ",\n";
	os << "}\n";
	return os;
}

scp::Solution scp::genetic::solve(const scp::Problem& problem, const Config& conf)
{
	assert(conf.population_size > 1);
	assert(conf.replacement_ratio <= 1);

	LOGGER->info("Start solving by genetic optimization using {}.", conf);

	std::vector<Solution> population;
	population.reserve(conf.population_size);

	long long seed = std::random_device{}();
	LOGGER->info("Seed = {}", seed);
	std::default_random_engine generator{seed};

	const auto replacements_per_iteration =
	  static_cast<size_t>(static_cast<double>(conf.population_size) * conf.replacement_ratio);

	const auto genetic_start = std::chrono::system_clock::now();

	LOGGER->info("Populate start: generation of random solutions and greedy improvement");
	Solution best = greedy::weighted_solve(
	  problem, generate_random_solution(problem, generator).selected_subsets);
	population.push_back(best);
	while(population.size() < conf.population_size)
	{
		Solution s = generate_random_solution(problem, generator);
		s.compute_cost();
		if(s.cost < best.cost)
		{
			best = s;
		}
		population.push_back(std::move(s));
	}
	const auto populate_end = std::chrono::system_clock::now();
	const std::chrono::duration<double> populate_elapsed_seconds = populate_end - genetic_start;
	LOGGER->info("Populate end: generated {} solutions in {}s",
	             conf.population_size,
	             populate_elapsed_seconds.count());

	// initialize offsprings vector
	std::vector<Solution> offsprings;
	offsprings.resize(replacements_per_iteration, best);

	std::uniform_real_distribution<double> probabilities_dist(0.0, 1.0);
	size_t current_generation = 0;
	do
	{
		const auto generation_start = std::chrono::system_clock::now();
		// sort by decreasing cost to perform rank based selection
		std::sort(population.begin(), population.end(), [](const Solution& a, const Solution& b) {
			return b.cost < a.cost;
		});
		std::vector<std::reference_wrapper<Solution>> population_unique(population.begin(),
		                                                                population.end());
		population_unique.erase(std::unique(population_unique.begin(),
		                                    population_unique.end(),
		                                    [](const Solution& a, const Solution& b) {
			                                    return std::tie(b.cost, b.selected_subsets)
			                                           == std::tie(a.cost, a.selected_subsets);
		                                    }),
		                        population_unique.end());

		// create offsprings by crossover + local search
		for(size_t i = 0; i < replacements_per_iteration; ++i)
		{
			Solution& parent1 =
			  population_unique[select_by_rank(population_unique.size(), generator)];
			Solution& parent2 =
			  population_unique[select_by_rank(population_unique.size(), generator)];

			// crossover
			Solution offspring = crossover::solve_subproblem_from(parent1, parent2);

			// local search
			if(probabilities_dist(generator) < conf.local_search_probability)
			{
				offspring =
				  descent::improve_by_annealing(offspring, generator, conf.descent_config);
			}

			// mutation
			if(probabilities_dist(generator) < conf.mutation_probability)
			{
				scp::neighbouring::flip_bit_safe(offspring, generator);
			}

			// update best
			offspring.compute_cost();
			if(offspring.cost < best.cost)
			{
				best = offspring;
			}
			offsprings[i] = std::move(offspring);
		}

		// replace worst individuals by offsprings
		for(size_t i = 0; i < replacements_per_iteration; ++i)
		{
			population[i] = std::move(offsprings[i]);
		}

		const auto generation_end = std::chrono::system_clock::now();
		const std::chrono::duration<double> generation_elapsed_seconds =
		  generation_end - generation_start;
		LOGGER->info(
		  "generation {}: best cost = {}, unique individuals = {}/{}, elapsed time = {}s",
		  current_generation,
		  best.cost,
		  population_unique.size(),
		  population.size(),
		  generation_elapsed_seconds.count());

		++current_generation;
	} while((std::chrono::system_clock::now() - genetic_start) < conf.execution_duration);

	// get rid of useless subsets using weighted greedy
	best = greedy::weighted_solve(problem, best.selected_subsets);

	const auto genetic_end = std::chrono::system_clock::now();
	const std::chrono::duration<double> genetic_elapsed_seconds = genetic_end - genetic_start;
	LOGGER->info("found solution by genetic optimization with {} subsets and cost of {} in {}s",
	             best.selected_subsets.count(),
	             best.cost,
	             genetic_elapsed_seconds.count());

	return best;
}
