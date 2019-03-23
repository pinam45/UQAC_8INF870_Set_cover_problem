//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include "benchmark.hpp"
#include "permutations.hpp"
#include "logger.hpp"

#include <dynamic_bitset.hpp>

void benchmark_increment()
{
	LOGGER->info("Start increment benchmark");
	LOGGER->info("(bitset size, time in seconds)");
	size_t bitset_size = 0;
	do
	{
		const auto timer_start = std::chrono::system_clock::now();
		dynamic_bitset<> bitset(bitset_size);
		do
		{
			increment(bitset);
		} while(!bitset.none());
		const auto timer_end = std::chrono::system_clock::now();
		const std::chrono::duration<double> elapsed_seconds = timer_end - timer_start;
		LOGGER->info("({}, {})", bitset_size, elapsed_seconds.count());
	} while(++bitset_size);
	LOGGER->info("End increment benchmark");
}

void benchmark_generate_permutations()
{
	LOGGER->info("Start generate_permutations benchmark");
	LOGGER->info("(bitset size, time in seconds)");
	for(size_t bitset_size = 0; bitset_size < 28; ++bitset_size)
	{
		const auto timer_start = std::chrono::system_clock::now();
		std::vector<std::vector<dynamic_bitset<>>> permutations =
		  generate_permutations(bitset_size);
		const auto timer_end = std::chrono::system_clock::now();
		const std::chrono::duration<double> elapsed_seconds = timer_end - timer_start;
		LOGGER->info("({}, {})", bitset_size, elapsed_seconds.count());
	}
	LOGGER->info("End generate_permutations benchmark");
}

void benchmark_PermutationsGenerator()
{
	LOGGER->info("Start increment benchmark");
	LOGGER->info("(bitset size, time in seconds)");
	size_t bitset_size = 0;
	do
	{
		const auto timer_start = std::chrono::system_clock::now();
		for(size_t bits_on = 0; bits_on <= bitset_size; ++bits_on)
		{
			PermutationsGenerator perm(bitset_size, bits_on);
			while(!perm.finished())
			{
				perm.next();
			}
		}
		const auto timer_end = std::chrono::system_clock::now();
		const std::chrono::duration<double> elapsed_seconds = timer_end - timer_start;
		LOGGER->info("({}, {})", bitset_size, elapsed_seconds.count());
	} while(++bitset_size);
	LOGGER->info("End increment benchmark");
}
