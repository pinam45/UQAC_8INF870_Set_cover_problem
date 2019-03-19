//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include "Solution.hpp"
#include "neighbouring.hpp"

#include <ostream_config_guard.hpp>

scp::Solution::Solution(const scp::Problem& problem_) noexcept
  : problem(problem_)
  , selected_subsets(problem_.subsets_points.size())
  , covered_points(problem_.full_set.size())
  , cover_all_points(false)
  , cost(invalid_cost)
{
}

scp::Solution& scp::Solution::operator=(const scp::Solution& other)
{
	assert(&problem == &other.problem);
	selected_subsets = other.selected_subsets;
	covered_points = other.covered_points;
	cover_all_points = other.cover_all_points;
	cost = other.cost;
	return *this;
}

scp::Solution& scp::Solution::operator=(scp::Solution&& other) noexcept
{
	assert(&problem == &other.problem);
	selected_subsets = std::move(other.selected_subsets);
	covered_points = std::move(other.covered_points);
	cover_all_points = other.cover_all_points;
	cost = other.cost;
	return *this;
}

void scp::Solution::compute_cover() noexcept
{
	assert(selected_subsets.size() == problem.subsets_points.size());
	assert(covered_points.size() == problem.full_set.size());

	covered_points.reset();
	size_t selected_subset = selected_subsets.find_first();
	while(selected_subset != dynamic_bitset<>::npos)
	{
		covered_points |= problem.subsets_points[selected_subset];
		selected_subset = selected_subsets.find_next(selected_subset);
	}
	cover_all_points = covered_points.all();
}

void scp::Solution::compute_cost() noexcept
{
	assert(selected_subsets.size() == problem.subsets_points.size());

	cost = 0;
	size_t selected_subset = selected_subsets.find_first();
	while(selected_subset != dynamic_bitset<>::npos)
	{
		cost += problem.subsets_costs[selected_subset];
		selected_subset = selected_subsets.find_next(selected_subset);
	}
}

std::ostream& scp::operator<<(std::ostream& os, const scp::Solution& solution)
{
	assert(solution.selected_subsets.size() == solution.problem.subsets_points.size());
	assert(solution.covered_points.size() == solution.problem.full_set.size());

	os << "scp::Solution{\n";
	os << "\tselected subsets number = " << solution.selected_subsets.count() << ",\n";
	os << "\tselected subsets = ";
	size_t selected_subset = solution.selected_subsets.find_first();
	while(selected_subset != dynamic_bitset<>::npos)
	{
		os << selected_subset << ", ";
		selected_subset = solution.selected_subsets.find_next(selected_subset);
	}
	os << "\n";
	if(!solution.cover_all_points)
	{
		os << "\tcovered points = " << solution.covered_points << ",\n";
	}
	ostream_config_guard guard(os, std::boolalpha);
	os << "\tcover all points = " << solution.cover_all_points << ",\n";
	os << "\tcost = " << solution.cost << ",\n";
	os << "}\n";
	return os;
}

scp::Solution scp::generate_random_solution(const Problem& problem, std::default_random_engine& generator)
{
	scp::Solution solution(problem);
	while(!solution.cover_all_points)
	{
		scp::neighbouring::flip_a_zero_to_one_unsafe(solution, generator);
		solution.compute_cover();
	}

	return solution;
}
