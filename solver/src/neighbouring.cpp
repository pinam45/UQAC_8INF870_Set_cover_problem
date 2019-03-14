//
// Created by Benoit Cortier on 3/12/19.
//

#include "neighbouring.hpp"

namespace
{
	template<typename Func>
	size_t helper_call_until_valid(scp::Solution& solution,
	                               std::default_random_engine& generator,
	                               Func functor)
	{
		size_t flipped_bit = functor(solution, generator);
		solution.compute_cover();
		while(!solution.cover_all_points)
		{
			solution.selected_subsets.flip(flipped_bit);
			flipped_bit = functor(solution, generator);
			solution.compute_cover();
		}

		return flipped_bit;
	}
} // namespace

size_t scp::neighbouring::uniform_flip_bit_safe(Solution& solution,
                                                std::default_random_engine& generator)
{
	assert(solution.cover_all_points);
	return helper_call_until_valid(solution, generator, uniform_flip_bit_unsafe);
}

size_t scp::neighbouring::uniform_flip_bit_unsafe(Solution& solution,
                                                  std::default_random_engine& generator)
{
	std::uniform_int_distribution<size_t> dist(0, solution.problem.subsets_points.size() - 1);
	const size_t pos = dist(generator);
	solution.selected_subsets.flip(pos);
	return pos;
}

size_t scp::neighbouring::flip_bit_safe(Solution& solution, std::default_random_engine& generator)
{
	assert(solution.cover_all_points);
	return helper_call_until_valid(solution, generator, flip_bit_unsafe);
}

size_t scp::neighbouring::flip_a_one_to_zero_safe(Solution& solution,
                                                  std::default_random_engine& generator)
{
	assert(solution.cover_all_points);
	assert(!solution.selected_subsets.empty());
	return helper_call_until_valid(solution, generator, flip_a_one_to_zero_unsafe);
}

size_t scp::neighbouring::flip_a_zero_to_one_safe(Solution& solution,
                                                  std::default_random_engine& generator)
{
	assert(solution.cover_all_points);
	assert(!solution.selected_subsets.all());
	return helper_call_until_valid(solution, generator, flip_a_zero_to_one_unsafe);
}

size_t scp::neighbouring::flip_bit_unsafe(Solution& solution, std::default_random_engine& generator)
{
	std::bernoulli_distribution dist;
	if(dist(generator))
	{
		return flip_a_one_to_zero_unsafe(solution, generator);
	}
	else
	{
		return flip_a_zero_to_one_unsafe(solution, generator);
	}
}

size_t scp::neighbouring::flip_a_one_to_zero_unsafe(Solution& solution,
                                                    std::default_random_engine& generator)
{
	assert(!solution.selected_subsets.none());
	const size_t count = solution.selected_subsets.count();
	std::uniform_int_distribution<size_t> dist(0, count - 1);
	size_t next_bit = solution.selected_subsets.find_first();
	for(size_t i = dist(generator); i > 0; --i)
	{
		next_bit = solution.selected_subsets.find_next(next_bit);
	}
	solution.selected_subsets.flip(next_bit);
	return next_bit;
}

size_t scp::neighbouring::flip_a_zero_to_one_unsafe(Solution& solution,
                                                    std::default_random_engine& generator)
{
	assert(!solution.selected_subsets.all());
	// might be optimized without the two flips
	solution.selected_subsets.flip();
	const size_t flipped_bit = flip_a_one_to_zero_unsafe(solution, generator);
	solution.selected_subsets.flip();
	return flipped_bit;
}
