//
// Copyright (c) 2019 Maxime Pinard and Benoît Cortier
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#ifndef SCPSOLVER_CROSSOVER_HPP
#define SCPSOLVER_CROSSOVER_HPP

#include <random>

#include "Solution.hpp"

namespace scp::crossover
{
	/// A crossover operation that solve the subproblem obtained
	/// by the union of the used subsets set of both solution.
	Solution solve_subproblem_from(const Solution& sol1, const Solution& sol2);

	// Keep randomly selected subsets from both parents.
	Solution random_merge(const scp::Solution& sol1,
	                      const scp::Solution& sol2,
	                      std::default_random_engine& generator);

	/// Keep all selected subsets from both parents.
	Solution fat_merge(const Solution& sol1, const Solution& sol2);
} // namespace scp::crossover

#endif //SCPSOLVER_CROSSOVER_HPP
