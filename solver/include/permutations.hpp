//
// Copyright (c) 2019 Maxime Pinard and Benoît Cortier
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#ifndef SCPSOLVER_PERMUTATIONS_HPP
#define SCPSOLVER_PERMUTATIONS_HPP

#include <memory>

#include <dynamic_bitset.hpp>

std::vector<std::vector<dynamic_bitset<>>> generate_permutations(size_t bitsets_size);

class PermutationsGenerator
{
public:
	PermutationsGenerator(size_t bitset_size, size_t ones_number) noexcept;

	size_t bitset_size() const noexcept;
	size_t ones_number() const noexcept;

	void reset() noexcept;
	void reset(size_t ones_number) noexcept;

	const dynamic_bitset<>& next() noexcept;

	bool finished() noexcept;

private:
	bool m_finished;
	bool m_bit_to_add;
	const size_t m_bitset_size;
	size_t m_ones_number;
	dynamic_bitset<> m_current;
	std::unique_ptr<PermutationsGenerator> m_lower_permutation;
};

void increment(dynamic_bitset<>& bitset);

#endif //SCPSOLVER_PERMUTATIONS_HPP
