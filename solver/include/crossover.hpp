//
// Created by Benoit Cortier on 3/14/19.
//

#ifndef SCPSOLVER_CROSSOVER_HPP
#define SCPSOLVER_CROSSOVER_HPP

#include "Solution.hpp"

namespace scp::crossover
{
	/// A crossover operation that solve the subproblem obtained
	/// by the union of the used subsets set of both solution.
	Solution solve_subproblem_from(const Solution& sol1, const Solution& sol2);
} // namespace scp::crossover

#endif //SCPSOLVER_CROSSOVER_HPP
