// Prevent circular inclusion
#ifndef STORAGE_H
#define STORAGE_H

// This file contains storage variables needed by generator and solver

#include <vector>
#define UNASSIGNED -1

// This container holds x, y values of cingle constraint
struct _constraint_values
{
	int x;
	int y;
};

// This container is a single variable
// Var holds variable number, note this is not assignment value
// Assignment is assignment that will be given by the solver, -1 or UNASSIGNED is its initial value
// Domain is an array of integers variable is allowed to be assigned. This array will be generated by the generator. 
struct _variable
{
	int var;
	int assignment;
	std::vector<int> domain;
};

struct _deleted_domains
{
	int var;
	std::vector<int> domain;
};

// This container holds two variables (x and y) and constraints between them
// Constraints are stored inside an array 
struct _constraint_touple
{
	_variable x;
	_variable y;
	std::vector<_constraint_values> constraints;
	std::vector<_deleted_domains> deleted_domain;
};



#endif