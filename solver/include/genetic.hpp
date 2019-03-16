//
// Created by Benoit Cortier on 3/14/19.
//

#ifndef SCPSOLVER_GENETIC_HPP
#define SCPSOLVER_GENETIC_HPP

#include "Problem.hpp"
#include "Solution.hpp"

namespace scp::genetic
{
	struct Config
	{
		size_t population_size = 100;
		size_t iteration_number = 50;
		double replacement_ratio = 0.8;
	};
	std::ostream& operator<<(std::ostream& os, const Config& conf);

	Solution solve(const Problem& problem, const Config& conf = Config{});
} // namespace scp::genetic

#endif //SCPSOLVER_GENETIC_HPP
