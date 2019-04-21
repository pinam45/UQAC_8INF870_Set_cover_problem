# 8INF870 - Set Cover Problem

## Information

This repository contains everything produced for our project of algorithm (lecture 8INF870, UQAC) on the Set Cover Problem (SCP).

There are three folders:

- `solver`: contains all the code of our SCP solver.
- `nomad`: [NOMAD](https://www.gerad.ca/nomad/) configurations and scripts to optimize parameters of our approached methods.
- `documents`: report and presentation LaTeX sources (french).

## Build the solver

Don't forget to pull the submodules with the repository using ``--recursive``:

    git clone --recursive https://github.com/pinam45/UQAC_8INF870_Set_cover_problem.git

or if you have already cloned the repository:

    git submodule init
    git submodule update

To build the project you will need a C++17 compliant compiler. The project uses CMake to build, see [Running CMake](https://cmake.org/runningcmake/) for more information.

On linux, a common CMake use is:

	$ mkdir cmake-build
	$ cd cmake-build
	$ cmake ..
	$ make


On Windows, there is batch files available to configure a Visual Studio project in the ``ide`` folder.

## License

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

This work is licensed under the [MIT License](http://opensource.org/licenses/MIT):

Copyright &copy; 2019 [Maxime Pinard](https://github.com/pinam45) & [Benoît Cortier](https://github.com/CBenoit)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
