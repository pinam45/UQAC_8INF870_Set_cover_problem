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

std::ostream& scp::operator<<(std::ostream& os, const scp::Problem& problem)
{
	os << "problem{\n";
	os << "\tpoints number = " << problem.full_set.size() << ",\n";
	os << "\tsubsets number = " << problem.subsets_points.size() << ",\n";
	os << "\tsubsets points = {\n";
	ostream_config_guard guard(os, std::setfill(' '));
	const int width = static_cast<int>(std::to_string(problem.subsets_points.size() - 1).size());
	for(size_t i = 0; i < problem.subsets_points.size(); ++i)
	{
		os << "\t\t (" << std::setw(width) << i << ") " << problem.subsets_points[i] << "\n";
	}
	os << "\t},\n";
	os << "}\n";
	return os;
}

bool scp::read_problem(const std::filesystem::path& path, scp::Problem& problem_out)
{
	std::error_code error;
	if(!std::filesystem::exists(path, error))
	{
		if(error)
		{
			SPDLOG_DEBUG(LOGGER, "std::filesystem::exists failed: {}", error.message());
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
			SPDLOG_DEBUG(LOGGER, "std::filesystem::is_regular_file failed: {}", error.message());
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
		SPDLOG_DEBUG(LOGGER, "std::ifstream constructor failed");
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
	LOGGER->info(
	  "successfully red problem with {} point and {} subsets", points_number, subsets_number);
	return true;
}
