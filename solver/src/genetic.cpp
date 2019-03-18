//
// Created by Benoit Cortier on 3/14/19.
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

	scp::Solution& select_by_rank(std::vector<scp::Solution>& population,
	                              std::default_random_engine& generator)
	{
		assert(!population.empty());
		const size_t rank_sum = (population.size() * (population.size() + 1)) / 2;

		std::uniform_int_distribution<size_t> dist(0, rank_sum);
		const size_t rnd = dist(generator);

		size_t accumulated_rank = 0;
		for(size_t i = 0; i < population.size(); ++i)
		{
			accumulated_rank += (i + 1);
			if(accumulated_rank >= rnd)
			{
				return population[i];
			}
		}

		assert(false); //impossible
		return population[population.size() - 1];
	}
} // namespace

std::ostream& scp::genetic::operator<<(std::ostream& os, const scp::genetic::Config& conf)
{
	os << "scp::genetic::Config{\n";
	os << "\tpopulation size = " << conf.population_size << ",\n";
	os << "\tnumber of iterations = " << conf.iteration_number << ",\n";
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
	for(size_t gen = 0; gen < conf.iteration_number; ++gen)
	{
		const auto generation_start = std::chrono::system_clock::now();
		// sort by decreasing cost to perform rank based selection
		std::sort(population.begin(), population.end(), [](const Solution& a, const Solution& b) {
			return b.cost < a.cost;
		});

		// create offsprings by crossover + local search
		for(size_t i = 0; i < replacements_per_iteration; ++i)
		{
			Solution& parent1 = select_by_rank(population, generator);
			Solution& parent2 = select_by_rank(population, generator);

			// crossover
			Solution offspring = crossover::random_merge(parent1, parent2, generator);

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
		LOGGER->info("generation {}: best cost = {} elapsed time = {}s",
		             gen,
		             best.cost,
		             generation_elapsed_seconds.count());
	}

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
