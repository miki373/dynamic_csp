// MIRZA OVCINA
// 200272124
// March of 2018 for CS 421
// CPS solver using backtrack, backtrack + fla, backtrack + pla
// ACKNOWLEDGMENTS
// Backtrack, Revise function, FLA and PLA based on "Russell, S. and Norvig, P. (2009). Artificial intelligence. Upper Saddle River, N.J.: Prentice Hall.", class notes.
// Cited algorithms may contain alterations, addition and/or removal of code based on my understanding and compatability with other functions found in this program. 

// NOTE * please forgive any spelling errors in names of variables and function. 

#include "generator.h"
#include "solver.h"
#include <iostream>
#include <cmath>
#include <time.h>
#include <algorithm>


using namespace std;



void create_additional_vars(vector<_constraint_touple> constreaints);
void probability();

int main()
{
	Generator generate;
	Generator generate_additional;
	Solver solve;
	vector<_constraint_touple> constreaints;
	vector<_constraint_touple> constreaints_additional;
	vector<_variable> var;
	vector<int> assignments;
	// User input
	int n, choice, ac;
	double p, alpha, r;
	// Timing
	clock_t s_time, e_time, t_time;
	

	

	do{
		cout << "Enter n: "; cin >> n;
	} while (!generate.setN(n));

	do
	{
		cout << "Enter p: "; cin >> p;
	} while (!generate.setP(p));

	do{
		cout << "Enter alpha: "; cin >> alpha;
	} while (!generate.setAlpha(alpha));

	do{
		cout << "Enter r: "; cin >> r;
	} while (!generate.setR(r));

	cout << "Generating CSP...\n";
	generate.create();	// generate constraints and variables
	constreaints = generate.return_csp();	// return constraints
	var = generate.return_var();	// return variables
	generate.print();	// print vars and constraints


	
	

	// aditional constraints //
	
	generate_additional.setR(r);
	generate_additional.setP(p);
	generate_additional.setN(n);
	generate_additional.setAlpha(alpha);
	


	do{
		cout << "\nAC Arch consistancy yes [1] no [0]\nChoice: "; cin >> ac;
	} while (ac < 0 || ac >1);

	if (ac)
	{
		int ac_resoult = solve.ac(var, constreaints);
		if (!ac_resoult)
		{
			cout << "\nAC has found at least one variable with empty domain - problem is unsolvable!!\n\n";
			return 0;
		}
	}


	do{
		cout << "Choose algorithm\n[1] BT Standart Backtracking\n[2] FC Forward Checking\n[3] FLA Full Look Ahead\nChoice: "; cin >> choice;
	} while (choice < 1 || choice > 3);



	// Create additional, give srand time to react
	generate_additional.create();
	cout << "These are secondary constraints\n";
	constreaints_additional = generate_additional.return_csp();
	generate_additional.print();

	// PUSH ADDITIONAL CONSTRAINTS
	// SET PROBABILITY

	solve.push_constraints(constreaints_additional);
	solve.set_probability(50);

	// END 



	// start timer
	s_time = clock();
	// backtrack run
	// chice
	// 1 = no look ahead
	// 2 = partial look ahead
	// 3 = full lookahead
	solve.backtrack(var, constreaints, choice);
	// end times
	e_time = clock();
	// time difference
	t_time = (e_time - s_time) / CLOCKS_PER_SEC;

	if (solve.is_solved())
	{
		assignments = solve.return_assign();
		cout << "\nSolution:\n";
		for (unsigned int i = 0; i < assignments.size(); i++)
		{
			cout << "x" << i << " " << assignments[i] << endl;
		}
	}
	else
	{
		cout << "No solution...\n";
	}

	cout << "Search took: " << t_time << " seconds.\n" << endl;

	
	
	

	// end

	//// EXEC NEW VARS ////
	//create_additional_vars(constreaints);
	//// EXEC NEW VARS ////
	return 0;
}




void create_additional_vars(vector<_constraint_touple> constreaints)
{

	// CREATE TWO VARIABLES .....
	_variable var1;
	_variable var2;

	// assign
	var1.var = 0;
	var2.var = 1;


	// create domain for both vars
	for (int i = 0; i < 5; i++)
	{
		var1.domain.push_back(i);
		var2.domain.push_back(i);
	}
	



	// CREATE 5 CONSTREAINTS BETWEEN VARIABLES
	//1
	_constraint_values values1;
	values1.x = 1;
	values1.y = 2;
	
	//2
	_constraint_values values2;
	values2.x = 0;
	values2.y = 2;
	
	//3
	_constraint_values values3;
	values3.x = 3;
	values3.y = 0;

	//4
	_constraint_values values4;
	values4.x = 0;
	values4.y = 1;

	//5
	_constraint_values values5;
	values5.x = 1;
	values5.y = 1;


	



	// CREATE SINGLE CONSTRAINT W/ THOSE VARS AND CONSTRAINTS 
	_constraint_touple tuple;

	// x
	tuple.x = var1;

	// y
	tuple.y = var2;

	// constraints
	tuple.constraints.push_back(values1);
	tuple.constraints.push_back(values2);
	tuple.constraints.push_back(values3);
	tuple.constraints.push_back(values4);
	tuple.constraints.push_back(values5);




	// being print >>>>>>>>


	printf("(%d,%d) <", tuple.x.var, tuple.y.var);
	for (unsigned int i = 0; i < tuple.constraints.size(); i++)
	{
		printf("(%d,%d) ", tuple.constraints[i].x, tuple.constraints[i].x);
	}
	printf(">\n");


	cout << "AFTER NEW \n";
	constreaints.push_back(tuple);

	for (unsigned int i = 0; i < constreaints.size(); i++)
	{
		cout << constreaints[i].x.var << constreaints[i].y.var << ": ";

		for (unsigned int j = 0; j < constreaints[i].constraints.size(); j++)
		{
			cout << "(" << constreaints[i].constraints[j].x << constreaints[i].constraints[j].y << ") ";
		}
		cout << endl;
	}



}


