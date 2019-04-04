//
// Copyright (c) 2019 Maxime Pinard and Benoît Cortier
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#ifndef SCPSOLVER_EXHAUSTIVE_HPP
#define SCPSOLVER_EXHAUSTIVE_HPP

#include "Problem.hpp"
#include "Solution.hpp"

namespace scp::exhaustive
{
	Solution solve_ram(const Problem& problem);

	Solution solve_cpu(const Problem& problem);

	Solution solve_counter(const Problem& problem);
} // namespace scp::exhaustive

#endif //SCPSOLVER_EXHAUSTIVE_HPP
