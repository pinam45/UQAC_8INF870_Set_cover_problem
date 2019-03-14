//
// Created by Benoit Cortier on 3/12/19.
//

#ifndef SCPSOLVER_NEIGHBOURS_HPP
#define SCPSOLVER_NEIGHBOURS_HPP

#include <random>

#include "Solution.hpp"

namespace scp::neighbouring
{
	/// Make sure the returned solution is valid.
	/// A valid initial solution should be provided, otherwise… BOOM (potential infinite loop).
	/// Select the bit to be flipped uniformly.
	size_t uniform_flip_bit_safe(Solution& solution, std::default_random_engine& generator);

	/// May return an invalid solution.
	/// Select the bit to be flipped uniformly.
	size_t uniform_flip_bit_unsafe(Solution& solution, std::default_random_engine& generator);

	/// Make sure the returned solution is valid.
	/// Select either a one or a zero to flip with a probability of 1/2.
	size_t flip_bit_safe(Solution& solution, std::default_random_engine& generator);

	/// Make sure the returned solution is valid.
	size_t flip_a_one_to_zero_safe(Solution& solution, std::default_random_engine& generator);

	/// Make sure the returned solution is valid.
	size_t flip_a_zero_to_one_safe(Solution& solution, std::default_random_engine& generator);

	/// May return an invalid solution.
	/// Select either a one or a zero to flip with a probability of 1/2.
	size_t flip_bit_unsafe(Solution& solution, std::default_random_engine& generator);

	/// May return an invalid solution.
	size_t flip_a_one_to_zero_unsafe(Solution& solution, std::default_random_engine& generator);

	/// May return an invalid solution.
	size_t flip_a_zero_to_one_unsafe(Solution& solution, std::default_random_engine& generator);
} // namespace scp::neighbouring

#endif //SCPSOLVER_NEIGHBOURS_HPP
