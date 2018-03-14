#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <iostream>
#include "storage.h"


class Generator
{
public:
	Generator();
	bool setN(int);
	bool setP(double);
	bool setAlpha(double);
	bool setR(double);

	int getN();
	double getP();
	double getAlpha();
	double getR();

	int get_error(bool val = false);

	int calc_constraints();
	int calc_incompatables();
	int calc_domain();
	bool unique_touple(int, int);
	bool unique_incompatable(int, int, int);
	bool create();
	void print();
	std::vector<_constraint_touple> return_csp();
	std::vector<_variable> return_var();
	

private:

	
	int n;
	double p;
	double alpha;
	double r;
	int number_variables;
	int number_constraints;
	int number_incompatables;
	int domain;
	int error;
	

	std::vector<_constraint_touple> constraint_touples;
	std::vector<_variable> variables;
};

#endif