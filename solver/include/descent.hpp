//
// Copyright (c) 2019 Maxime Pinard and Benoît Cortier
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#ifndef SCPSOLVER_DESCENT_HPP
#define SCPSOLVER_DESCENT_HPP

#include <random>

#include "Problem.hpp"
#include "Solution.hpp"

namespace scp::descent
{
	struct Config
	{
		size_t iterations_number = 10000;
		double initial_temperature = 50;
		double final_temperature = 1;
	};
	std::ostream& operator<<(std::ostream& os, const Config& conf);

	Solution improve_by_annealing(const Solution& initial_solution,
	                              std::default_random_engine& generator,
	                              const Config& conf = Config{});
} // namespace scp::descent

#endif //SCPSOLVER_DESCENT_HPP
