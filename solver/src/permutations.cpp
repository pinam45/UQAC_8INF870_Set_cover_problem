//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include "permutations.hpp"

std::vector<std::vector<dynamic_bitset<>>> generate_permutations(size_t bitsets_size)
{
	// permutations[k] : list of bitset permutations with k bit to 1
	std::vector<std::vector<dynamic_bitset<>>> permutations;
	permutations.reserve(bitsets_size);
	permutations.emplace_back();
	permutations[0].emplace_back();

	for(size_t bitset_size = 0; bitset_size < bitsets_size; ++bitset_size)
	{
		// generate bitsets of size (bitset_size + 1)
		// reverse order

		// special last case (all bits to 1)
		permutations.emplace_back();
		permutations[permutations.size() - 1] = permutations[permutations.size() - 2];
		for(dynamic_bitset<>& permutation: permutations[permutations.size() - 1])
		{
			permutation.push_back(true);
		}

		// general case
		for(size_t i = (permutations.size() - 2); i > 0; --i)
		{
			const size_t old_size = permutations[i].size();
			permutations[i].reserve(old_size + permutations[i - 1].size());
			std::copy(std::cbegin(permutations[i - 1]),
			          std::cend(permutations[i - 1]),
			          std::back_inserter(permutations[i]));
			for(size_t j = 0; j < old_size; ++j)
			{
				permutations[i][j].push_back(false);
			}
			for(size_t j = old_size; j < permutations[i].size(); ++j)
			{
				permutations[i][j].push_back(true);
			}
		}

		// special first case (all bits to 0)
		for(dynamic_bitset<>& permutation: permutations[0])
		{
			permutation.push_back(false);
		}
	}

	return permutations;
}
