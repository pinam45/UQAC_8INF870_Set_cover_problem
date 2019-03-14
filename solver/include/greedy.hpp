//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#ifndef SCPSOLVER_GREEDY_HPP
#define SCPSOLVER_GREEDY_HPP

#include "Problem.hpp"
#include "Solution.hpp"

namespace scp::greedy
{
	Solution unweighted_solve(const Problem& problem);
	Solution weighted_solve(const Problem& problem);
	Solution weighted_solve(const Problem& problem, const dynamic_bitset<>& allowed_subsets);
} // namespace scp::greedy

#endif //SCPSOLVER_GREEDY_HPP
