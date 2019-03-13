//
// Created by Benoit Cortier on 3/12/19.
//

#include <cmath>
#include <memory>

#include "descent.hpp"
#include "neighbouring.hpp"

scp::Solution scp::descent::improve_by_annealing(const Solution& initial_solution,
                                                 const Problem& problem,
                                                 std::default_random_engine& generator,
                                                 size_t iterations_number,
                                                 double initial_temperature,
                                                 double final_temperature)
{
	double initial_to_final_range = final_temperature - initial_temperature;
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	std::unique_ptr<Solution> best = std::make_unique<Solution>(initial_solution);
	Solution solution{initial_solution};

	for(size_t iter = 0; iter < iterations_number; ++iter)
	{
		solution.compute_cost();
		size_t previous_cost = solution.cost;

		size_t flipped_bit = neighbouring::flip_bit_safe(solution, problem, generator);
		solution.compute_cost();

		if(previous_cost < solution.cost)
		{
			double temperature =
			  initial_temperature
			  + (static_cast<double>(iter) / static_cast<double>(iterations_number))
			      * initial_to_final_range;
			size_t cost_delta = solution.cost - previous_cost;
			double probability = std::exp(-static_cast<double>(cost_delta) / temperature);

			if(dist(generator) <= probability)
			{
				// keep the new (worst) solution
			}
			else
			{
				// revert to the old (better) solution
				solution.selected_subsets.flip(flipped_bit);
			}
		}
		else if(solution.cost < best->cost)
		{
			best = std::make_unique<Solution>(solution);
		}
	}

	return std::move(Solution{*best});
}
