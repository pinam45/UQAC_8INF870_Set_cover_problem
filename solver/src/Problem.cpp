//
// Copyright (c) 2019 Maxime Pinard
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#include "Problem.hpp"
#include "logger.hpp"
#include "ostream_config_guard.hpp"

#include <fstream>
#include <iomanip>
#include <chrono>

std::ostream& scp::operator<<(std::ostream& os, const scp::Problem& problem)
{
	os << "scp::Problem{\n";
	os << "\tpoints number = " << problem.full_set.size() << ",\n";
	os << "\tsubsets number = " << problem.subsets_points.size() << ",\n";
	os << "\tsubsets points = {\n";
	ostream_config_guard guard(os, std::setfill(' '));
	const int counter_width =
	  static_cast<int>(std::to_string(problem.subsets_points.size() - 1).size());
	const int cost_width =
	  static_cast<int>(std::to_string(*std::max_element(std::cbegin(problem.subsets_costs),
	                                                    std::cend(problem.subsets_costs)))
	                     .size());
	for(size_t i = 0; i < problem.subsets_points.size(); ++i)
	{
		os << "\t\t (" << std::setw(counter_width) << i << ") [" << std::setw(cost_width)
		   << problem.subsets_costs[i] << "] " << problem.subsets_points[i] << "\n";
	}
	os << "\t},\n";
	os << "}\n";
	return os;
}

bool scp::read_problem(const std::filesystem::path& path, scp::Problem& problem_out)
{
	const auto start = std::chrono::system_clock::now();

	std::error_code error;
	if(!std::filesystem::exists(path, error))
	{
		if(error)
		{
			SPDLOG_LOGGER_DEBUG(LOGGER, "std::filesystem::exists failed: {}", error.message());
			LOGGER->warn("Check if file/folder exist failed for {}", path);
		}
		else
		{
			LOGGER->warn("Tried to read problem from non-existing file/folder {}", path);
		}
		return false;
	}

	if(!std::filesystem::is_regular_file(path, error))
	{
		if(error)
		{
			SPDLOG_LOGGER_DEBUG(
			  LOGGER, "std::filesystem::is_regular_file failed: {}", error.message());
			LOGGER->warn("Check if path is a regular file failed for: {}", path);
		}
		else
		{
			LOGGER->warn("Tried to read problem from non-file {}", path);
		}
		return false;
	}

	std::ifstream problem_stream(path);
	if(!problem_stream)
	{
		SPDLOG_LOGGER_DEBUG(LOGGER, "std::ifstream constructor failed");
		LOGGER->warn("Failed to read file {}", path);
		return false;
	}

	LOGGER->info("Started to read problem from file {}", path);
	Problem problem;

	// Read points number
	size_t points_number = 0;
	if(!problem_stream.good())
	{
		LOGGER->warn("Invalid file format");
		return false;
	}
	problem_stream >> points_number;
	problem.full_set.resize(points_number, true);

	// Read subsets number
	size_t subsets_number = 0;
	if(!problem_stream.good())
	{
		LOGGER->warn("Invalid file format");
		return false;
	}
	problem_stream >> subsets_number;

	// Read subsets costs
	problem.subsets_costs.resize(subsets_number);
	for(size_t i = 0; i < subsets_number; ++i)
	{
		if(!problem_stream.good())
		{
			LOGGER->warn("Invalid file format");
			return false;
		}
		problem_stream >> problem.subsets_costs[i];
	}

	// Read subsets covering points
	problem.subsets_points.resize(subsets_number);
	for(size_t i = 0; i < subsets_number; ++i)
	{
		problem.subsets_points[i].resize(points_number);
	}
	for(size_t i_point = 0; i_point < points_number; ++i_point)
	{
		size_t subsets_covering_point = 0;
		if(!problem_stream.good())
		{
			LOGGER->warn("Invalid file format");
			return false;
		}
		problem_stream >> subsets_covering_point;
		assert(subsets_covering_point <= subsets_number);
		for(size_t i_subset = 0; i_subset < subsets_covering_point; ++i_subset)
		{
			size_t subset_number = 0;
			if(!problem_stream.good())
			{
				LOGGER->warn("Invalid file format");
				return false;
			}
			problem_stream >> subset_number;
			assert(subset_number > 0);
			--subset_number; // numbered from 1 in the file
			assert(subset_number < subsets_number);
			problem.subsets_points[subset_number][i_point] = true;
		}
	}

	// Success
	problem_out = std::move(problem);

	const auto end = std::chrono::system_clock::now();
	const std::chrono::duration<double> elapsed_seconds = end - start;
	LOGGER->info("successfully read problem with {} points and {} subsets in {}s",
	             points_number,
	             subsets_number,
	             elapsed_seconds.count());

	return true;
}

bool scp::write_problem(const Problem& problem,
                        const std::filesystem::path& path,
                        bool override_file)
{
	const auto start = std::chrono::system_clock::now();

	std::error_code error;
	if(std::filesystem::exists(path, error))
	{
		if(error)
		{
			SPDLOG_LOGGER_DEBUG(LOGGER, "std::filesystem::exists failed: {}", error.message());
			LOGGER->warn("Check if file/folder exist failed for {}", path);
		}
		else if(!override_file)
		{
			LOGGER->warn("Tried to write problem to already-existing file/folder {}", path);
			return false;
		}
	}

	std::ofstream problem_stream(path, std::ios::out | std::ios::trunc);
	if(!problem_stream)
	{
		SPDLOG_LOGGER_DEBUG(LOGGER, "std::ofstream constructor failed");
		LOGGER->warn("Failed to write file {}", path);
		return false;
	}

	LOGGER->info("Started to write problem to file {}", path);

	// Write points number
	problem_stream << " " << problem.full_set.size();

	// Write subsets number
	problem_stream << " " << problem.subsets_costs.size() << " \n ";

	// Write subsets costs
	const size_t return_at = 12;
	size_t out_counter = 0;
	for(size_t subset_cost: problem.subsets_costs)
	{
		problem_stream << subset_cost << " ";
		if(++out_counter == return_at)
		{
			problem_stream << "\n ";
			out_counter = 0;
		}
	}
	problem_stream << "\n ";
	out_counter = 0;

	// Write subsets covering points
	for(size_t i_point = 0; i_point < problem.full_set.size(); ++i_point)
	{
		std::vector<size_t> subsets_covering_point;
		for(size_t i_subset = 0; i_subset < problem.subsets_costs.size(); ++i_subset)
		{
			if(problem.subsets_points[i_subset][i_point])
			{
				subsets_covering_point.push_back(i_subset + 1); // numbered from 1 in the file
			}
		}

		problem_stream << subsets_covering_point.size() << " \n ";
		for(size_t subset_number: subsets_covering_point)
		{
			problem_stream << subset_number << " ";
			if(++out_counter == return_at)
			{
				problem_stream << "\n ";
				out_counter = 0;
			}
		}
		if(out_counter != 0)
		{
			problem_stream << "\n ";
			out_counter = 0;
		}
	}

	if(!problem_stream.good())
	{
		LOGGER->warn("Error writing to file");
		return false;
	}

	// Success
	const auto end = std::chrono::system_clock::now();
	const std::chrono::duration<double> elapsed_seconds = end - start;
	LOGGER->info("successfully written problem in {}s", elapsed_seconds.count());

	return true;
}

scp::Problem scp::generate_problem(size_t points_number,
                                   size_t subsets_number,
                                   std::default_random_engine& generator,
                                   size_t min_covering_subsets,
                                   size_t max_covering_subsets,
                                   size_t min_subsets_cost,
                                   size_t max_subsets_cost)
{
	assert(points_number > 0);
	assert(subsets_number > 0);
	assert(min_covering_subsets > 0);
	assert(max_covering_subsets > 0);
	assert(max_covering_subsets <= subsets_number);
	assert(min_subsets_cost > 0);
	assert(max_subsets_cost > 0);

	const auto start = std::chrono::system_clock::now();

	Problem problem;
	problem.full_set.resize(points_number, true);

	std::uniform_int_distribution subsets_costs_dist(min_subsets_cost, max_subsets_cost);
	problem.subsets_costs.reserve(subsets_number);
	for(size_t i = 0; i < subsets_number; ++i)
	{
		problem.subsets_costs.push_back(subsets_costs_dist(generator));
	}

	problem.subsets_points.resize(subsets_number);
	for(dynamic_bitset<>& subset: problem.subsets_points)
	{
		subset.resize(points_number);
	}

	std::uniform_int_distribution covering_subsets_number_dist(min_covering_subsets,
	                                                           max_covering_subsets);
	std::uniform_int_distribution covering_subset_dist(size_t(0), subsets_number - 1);
	for(size_t i_point = 0; i_point < points_number; ++i_point)
	{
		const size_t covering_subsets_number = covering_subsets_number_dist(generator);
		for(size_t i_subset_covering = 0; i_subset_covering < covering_subsets_number;
		    ++i_subset_covering)
		{
			size_t covering_subset = covering_subset_dist(generator);
			while(problem.subsets_points[covering_subset][i_point])
			{
				covering_subset = covering_subset_dist(generator);
			}
			problem.subsets_points[covering_subset][i_point].set();
		}
	}

	const auto end = std::chrono::system_clock::now();
	const std::chrono::duration<double> elapsed_seconds = end - start;
	LOGGER->info("successfully generated problem with {} points and {} subsets in {}s",
	             points_number,
	             subsets_number,
	             elapsed_seconds.count());

	return problem;
}
