//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#ifndef SCPSOLVER_BNB_HPP
#define SCPSOLVER_BNB_HPP

#include "Solution.hpp"
#include "Problem.hpp"

namespace scp::bnb
{
	Solution solve(const Problem& problem);
}

#endif //SCPSOLVER_BNB_HPP
