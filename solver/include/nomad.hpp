//
// Copyright (c) 2019 Maxime Pinard and Benoît Cortier
//
// Distributed under the MIT license
// See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT
//
#ifndef SCPSOLVER_NOMAD_H
#define SCPSOLVER_NOMAD_H

namespace nomad
{
	int run_genetic(int argc, char** argv);
	int run_annealing(int argc, char** argv);
} // namespace nomad

#endif //SCPSOLVER_NOMAD_H
