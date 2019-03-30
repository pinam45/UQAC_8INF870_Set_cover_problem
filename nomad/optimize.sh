#!/bin/bash
export LD_LIBRARY_PATH=.

run_problem() {
	echo "================> $1 <================"
	SCP_PROBLEM_PATH=../solver/resources/OR-Library/$1.txt ./nomad $2
}

run_problem "scpa1" $1
run_problem "scpb1" $1
run_problem "scpc1" $1
run_problem "scpd1" $1
run_problem "scp41" $1
run_problem "scp51" $1
run_problem "scp61" $1
