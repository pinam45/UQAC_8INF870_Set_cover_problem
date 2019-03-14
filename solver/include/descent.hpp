//
// Created by Benoit Cortier on 3/12/19.
//

#ifndef SCPSOLVER_DESCENT_HPP
#define SCPSOLVER_DESCENT_HPP

#include <random>

#include "Problem.hpp"
#include "Solution.hpp"

namespace scp::descent
{
	Solution improve_by_annealing(const Solution& initial_solution,
	                              std::default_random_engine& generator,
	                              size_t iterations_number,
	                              double initial_temperature,
	                              double final_temperature);
}

#endif //SCPSOLVER_DESCENT_HPP
