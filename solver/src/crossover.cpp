//
// Created by Benoit Cortier on 3/14/19.
//

#include "crossover.hpp"
#include "greedy.hpp"

scp::Solution scp::crossover::solve_subproblem_from(const scp::Solution& sol1,
                                                    const scp::Solution& sol2)
{
	return greedy::weighted_solve(sol1.problem, sol1.selected_subsets | sol2.selected_subsets);
}
