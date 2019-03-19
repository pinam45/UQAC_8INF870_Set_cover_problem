//
// Created by Benoit Cortier on 3/14/19.
//

#ifndef SCPSOLVER_GENETIC_HPP
#define SCPSOLVER_GENETIC_HPP

#include "Problem.hpp"
#include "Solution.hpp"
#include "descent.hpp"

namespace scp::genetic
{
	struct Config
	{
		size_t population_size = 100;
		std::chrono::duration<double> execution_duration = std::chrono::seconds(30);
		double replacement_ratio = 0.8;
		double mutation_probability = 0.1;
		double local_search_probability = 0.3;
		scp::descent::Config descent_config{};
	};
	std::ostream& operator<<(std::ostream& os, const Config& conf);

	Solution solve(const Problem& problem, const Config& conf = Config{});
} // namespace scp::genetic

#endif //SCPSOLVER_GENETIC_HPP
