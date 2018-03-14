#ifndef SOLVER_H
#define SOLVER_H

#define NONE 1
#define FC 2
#define MAC 3
#include "storage.h"
#include <vector>
#include <iostream>
#include <queue>

#include <time.h>	// for determening 
#include <stdlib.h>	// how often to inject new constraint



class Solver
{
public:
	Solver();
	int unassigned(std::vector<_variable>, std::vector<_constraint_touple>);
	bool is_valid(int value, _variable, std::vector<_constraint_touple>);
	bool copy_to_assign(std::vector<_variable>);
	std::vector<int> return_assign();
	bool backtrack(std::vector<_variable>, std::vector<_constraint_touple>&, int);
	bool is_solved();
	bool is_consistant(int, int, std::vector<_variable>, std::vector<_constraint_touple>);
	int get_assignment(std::vector<_variable>, int);
	bool ac(std::vector<_variable>&, std::vector<_constraint_touple>);
	bool revise(_constraint_touple, _variable&, _variable&);
	int variable_position(std::vector<_variable>, int);
	bool pla(int, std::vector<_variable>&, std::vector<_constraint_touple>);
	bool fla(int, std::vector<_variable>&, std::vector<_constraint_touple>);

	
	
	
	// EXPERIMENTAL
	void add_constraints(std::vector<_constraint_touple>&, std::vector<_variable>&);
	void push_constraints(std::vector<_constraint_touple>);
	void set_probability(int);

private:
	std::vector<int> assign;
	bool copied;
	bool solved;
	bool w_ac;
	bool w_mac;


	int turn;


	std::queue<_constraint_touple> new_constraints;
	int _probability;
};
#endif