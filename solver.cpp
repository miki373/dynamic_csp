#include "solver.h"
Solver::Solver()
{
	copied = false;
	solved = false;
	w_ac = false;
	w_mac = false;
	turn = 0;
	// 0 never insert, 100 always insert
	_probability = 0; 
	srand(time(NULL));
 }

// Return most constrained variable (its position in vector) because that
// variable will theoretically fail first and reduce the depth of search
// Using integer to save resources
int Solver::unassigned(std::vector<_variable> vars, std::vector<_constraint_touple> touples)
{
	int pos = -1;
	int pos_num_constrained = -1;
	int temp_pos = -1;
	int num_constrained = -1;
	for (unsigned int i = 0; i < vars.size(); i++)
	{
		if (vars[i].assignment != UNASSIGNED)
		{
			continue;
		}
		else
		{
			temp_pos = i;
		}
		// GET NUMBER OF CONSTRAINT
		num_constrained = 0;
		for (unsigned int k = 0; k < touples.size(); k++)
		{
			if ((vars[i].var == touples[k].x.var) || vars[i].var == touples[k].y.var)
			{
				num_constrained += 1;
			}
		}
		if (pos_num_constrained <= num_constrained)
		{
			pos_num_constrained = num_constrained;
			pos = temp_pos;
		}
	}
	return pos;
}


// copy variable assignments to assignments vector to be returned to user
bool Solver::copy_to_assign(std::vector<_variable> vars)
{
	assign.clear();
	for (unsigned int i = 0; i < vars.size(); i++)
	{
		assign.push_back(vars[i].assignment);
	}
	copied = true;
	return true;
}

// returns assignment
std::vector<int> Solver::return_assign()
{
	return assign;
}




// returns true if CSP is solver, false if no solution
bool Solver::is_solved()
{
	return solved;
}

// checks if assignment is consistant with constraints
bool Solver::is_consistant(int proposed_value,int position, std::vector<_variable> variables, std::vector<_constraint_touple> touple)
{
	int other_value;
	for (unsigned int i = 0; i < touple.size(); i++)
	{
		// check if this variable is in constraints tuple x position
		if (variables[position].var == touple[i].x.var)
		{
			// value of other variable, in this case y
			// if unassigned, y will be -1
			other_value = get_assignment(variables, touple[i].y.var);

			for (unsigned int k = 0; k < touple[i].constraints.size(); k++)
			{
				// this proposed value violates constraints, return false
				if ((proposed_value == touple[i].constraints[k].x) && (other_value == touple[i].constraints[k].y))
				{
					return false;
				}
			}
		}

		if (variables[position].var == touple[i].y.var)
		{
			other_value = get_assignment(variables, touple[i].x.var);

			for (unsigned int k = 0; k < touple[i].constraints.size(); k++)
			{
				if ((proposed_value == touple[i].constraints[k].y) && (other_value == touple[i].constraints[k].x))
				{
					return false;
				}
			}
		}
	}

	return true;
}

// returns assignment of variable if there is any. 
int Solver::get_assignment(std::vector<_variable> vars, int x)
{
	for (unsigned int i = 0; i < vars.size(); i++)
	{
		if (vars[i].var == x)
		{
			return vars[i].assignment;
		}
	}
	return -1;
}

// ac algorithm based on psudocode found in
// credit "Artificial Intellegence: A modern approach"//
// Additional ability to return false if domain of any variable is exhousted
// preventin needless backtrack search... 
bool Solver::ac(std::vector<_variable>& vars, std::vector<_constraint_touple>& constraints)
{
	std::queue<_constraint_touple> contraints_queue;
	_constraint_touple temp;
	int temp_pos_x, temp_pos_y;

	// push constraint in queue
	for (unsigned int i = 0; i < constraints.size(); i++)
	{
		contraints_queue.push(constraints[i]);
		_variable temp = constraints[i].x;
		constraints[i].x = constraints[i].y;
		constraints[i].y = temp;
		for (unsigned int j = 0; j < constraints[i].constraints.size(); j++)
		{
			int temp = constraints[i].constraints[j].x;
			constraints[i].constraints[j].x = constraints[i].constraints[j].y;
			constraints[i].constraints[j].y = temp;
		}
		contraints_queue.push(constraints[i]);
	}

	// begin AC
	while (!contraints_queue.empty())
	{
		temp = contraints_queue.front();// get next time
		contraints_queue.pop();	// remove next item

		temp_pos_x = variable_position(vars, temp.x.var);
		temp_pos_y = variable_position(vars, temp.y.var);

		if (revise(constraints, temp, vars[temp_pos_x], vars[temp_pos_y]))
		{
			if (vars[temp_pos_x].domain.size() == 0)
			{
				return false;
			}
			for (unsigned int i = 0; i < constraints.size(); i++)
			{
				if (((constraints[i].x.var == vars[temp_pos_x].var) || (constraints[i].y.var == vars[temp_pos_x].var)) && (constraints[i].y.var != vars[temp_pos_y].var))
				{
					contraints_queue.push(constraints[i]);
					_variable temp = constraints[i].x;
					constraints[i].x = constraints[i].y;
					constraints[i].y = temp;
					for (unsigned int j = 0; j < constraints[i].constraints.size(); j++)
					{
						int temp = constraints[i].constraints[j].x;
						constraints[i].constraints[j].x = constraints[i].constraints[j].y;
						constraints[i].constraints[j].y = temp;
					}
					contraints_queue.push(constraints[i]);
				}
			}
		}

	}
	return true;
	
}

// Based on ac3 concentrated on current assignmed variable
bool Solver::fla(int var_position, std::vector<_variable>& vars, std::vector<_constraint_touple>& constraints)
{
	std::queue<_constraint_touple> contraints_queue;
	_constraint_touple temp;
	int temp_pos_x, temp_pos_y;
	int target_var_pos = variable_position(vars, var_position);
	_variable target_var = vars[target_var_pos];



	for (unsigned int i = 0; i < constraints.size(); i++)
	{
		if (target_var.var == constraints[i].x.var || target_var.var == constraints[i].y.var)
		{
			contraints_queue.push(constraints[i]);
			_variable temp = constraints[i].x;
			constraints[i].x = constraints[i].y;
			constraints[i].y = temp;
			for (unsigned int j = 0; j < constraints[i].constraints.size(); j++)
			{
				int temp = constraints[i].constraints[j].x;
				constraints[i].constraints[j].x = constraints[i].constraints[j].y;
				constraints[i].constraints[j].y = temp;
			}
			contraints_queue.push(constraints[i]);
		}
	}

	// begin 
	while (!contraints_queue.empty())
	{
		temp = contraints_queue.front();// get next item
		contraints_queue.pop();	// remove from queue

		temp_pos_x = variable_position(vars, temp.x.var);
		temp_pos_y = variable_position(vars, temp.y.var);

		if (revise(constraints, temp, vars[temp_pos_x], vars[temp_pos_y]))
		{
			if (vars[temp_pos_x].domain.size() == 0)
			{
				return false;
			}
			for (unsigned int i = 0; i < constraints.size(); i++)
			{
				if (((constraints[i].x.var == vars[temp_pos_x].var) || (constraints[i].y.var == vars[temp_pos_x].var)) && (constraints[i].y.var != vars[temp_pos_y].var))
				{
					contraints_queue.push(constraints[i]);
					_variable temp = constraints[i].x;
					constraints[i].x = constraints[i].y;
					constraints[i].y = temp;
					for (unsigned int j = 0; j < constraints[i].constraints.size(); j++)
					{
						int temp = constraints[i].constraints[j].x;
						constraints[i].constraints[j].x = constraints[i].constraints[j].y;
						constraints[i].constraints[j].y = temp;
					}
					contraints_queue.push(constraints[i]);
				}
			}
		}

	}
	return true;

}

// Based on ac3 concentrated on current assignmed variable
bool Solver::pla(int var_position, std::vector<_variable>& vars, std::vector<_constraint_touple>& constraints)
{
	std::queue<_constraint_touple> contraints_queue;
	_constraint_touple temp;
	int temp_pos_x, temp_pos_y;
	int target_var_pos = variable_position(vars, var_position);
	_variable target_var = vars[target_var_pos];



	for (unsigned int i = 0; i < constraints.size(); i++)
	{
		if (target_var.var == constraints[i].x.var || target_var.var == constraints[i].y.var)
		{
			contraints_queue.push(constraints[i]);
		}
	}

	while (!contraints_queue.empty())
	{
		temp = contraints_queue.front();// get next item
		contraints_queue.pop();	// remove item

		temp_pos_x = variable_position(vars, temp.x.var);
		temp_pos_y = variable_position(vars, temp.y.var);

		if (revise(constraints, temp, vars[temp_pos_x], vars[temp_pos_y]))
		{
			if (vars[temp_pos_x].domain.size() == 0)
			{
				return false;
			}
			for (unsigned int i = 0; i < constraints.size(); i++)
			{
				if (((constraints[i].x.var == vars[temp_pos_x].var) || (constraints[i].y.var == vars[temp_pos_x].var)) && (constraints[i].y.var != vars[temp_pos_y].var))
				{
					contraints_queue.push(constraints[i]);
				}
			}
		}

	}
	return true;

}

// NOTE * this is my inplementation of revise function psudocode found in "Artificial Intellegence: A modern approach (3rd Edition)"
// Algorithm:
// given xi, this function checks if there exists any value in dj such that
// (xi, dj) satesfies the constraint, if there isn't, remove xi from domain. 
bool Solver::revise(std::vector<_constraint_touple>& constraints, _constraint_touple touple, _variable& xi, _variable& xj)
{
	bool revised = false;
	unsigned int unsatasfied;
	
	for (unsigned int i = 0; i < xi.domain.size(); i++)
	{
		unsatasfied = 0;
		for (unsigned int j = 0; j < xj.domain.size(); j++)
		{	
			for (unsigned int k = 0; k < touple.constraints.size(); k++)
			{
				// check if exists compatable pair in domain
				if ((xi.domain[i] == touple.constraints[k].x) && (xj.domain[j] == touple.constraints[k].y))
				{
					unsatasfied++;
					break;
				}
			}

		}
		if (unsatasfied == xj.domain.size())
		{
			// store domain to be erased 
			set_deleating_constraint(constraints, touple, xi, xi.domain[i]);

			// erase
			xi.domain.erase(xi.domain.begin() + i);
			
			i--;	
			
			revised = true;
			
		}

	}
	return revised;
}

// based on variable value, return variable position in array
int Solver::variable_position(std::vector<_variable> vars, int variable)
{
	for (unsigned int i = 0; i < vars.size(); i++)
	{
		if (vars[i].var == variable)
		{
			return i;
		}
	}

	return -1;
}



void Solver::add_constraints(std::vector<_constraint_touple>& constraints, std::vector<_variable>& vars)
{
	int rnumber = rand() % 100; // number between 0 and 100
	
	if ((rnumber < _probability) && !new_constraints.empty())	// if we want to insert multiple, then change if to while( rnumber < _probavilty )
	{
		_constraint_touple new_constraint = new_constraints.front();
		new_constraints.pop();
		
		int varx = new_constraint.x.var;
		int vary = new_constraint.y.var;

		int varposx = variable_position(vars, varx);
		int varposy = variable_position(vars, vary);

		// IF EITHER OF THE TWO VARIABLES ARE UNASSIGNED, DON'T DO ANYTHING.... 
		if (vars[varposx].assignment != UNASSIGNED && vars[varposy].assignment != UNASSIGNED)
		{
			// ITERATE OVER ALL NEW INCOMPATABLE PAIRS
			for (unsigned int i = 0; i < new_constraint.constraints.size(); i++)
			{
				// IF CURRENT ASSIGNMENT OF VARIABLE MATCHES ASSIGNMENT, UNASSIGN BOTH VARIABLES
				if (vars[varposx].assignment == new_constraint.constraints[i].x && vars[varposy].assignment == new_constraint.constraints[i].y)
				{
					vars[varposx].assignment = UNASSIGNED;
					vars[varposx].assignment = UNASSIGNED;
					// each pair is unique, no need to look further
					std::cout << "REMOVED ASSIGMENT\n";
					break;
				}
			}
		}
		constraints.push_back(new_constraint);

	}
	


	/*
	if (++turn == 4)
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
		values3.x = 0;
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

		int varx = tuple.x.var;
		int vary = tuple.y.var;
		
		int varposx = variable_position(vars, varx);
		int varposy = variable_position(vars, vary);

		// IF EITHER OF THE TWO VARIABLES ARE UNASSIGNED, DON'T DO ANYTHING.... 
		if (vars[varposx].assignment != UNASSIGNED && vars[varposy].assignment != UNASSIGNED)
		{
			// ITERATE OVER ALL NEW INCOMPATABLE PAIRS
			for (unsigned int i = 0; i < tuple.constraints.size(); i++)
			{
				// IF CURRENT ASSIGNMENT OF VARIABLE MATCHES ASSIGNMENT, UNASSIGN BOTH VARIABLES
				if (vars[varposx].assignment == tuple.constraints[i].x && vars[varposy].assignment == tuple.constraints[i].y)
				{
					vars[varposx].assignment = UNASSIGNED;
					vars[varposx].assignment = UNASSIGNED;
					// each pair is unique, no need to look further
					std::cout << "REMOVED ASSIGMENT\n";
					break;
				}
			}
		}
		
		


		constraints.push_back(tuple);
		std::cout << "INJECTING!!!\n";
	}


	*/
}


bool Solver::backtrack(std::vector<_variable> vars, std::vector<_constraint_touple>& touples, int type)
{
	// base case
	// if all are assigned store assignments and return true
	if (unassigned(vars, touples) == -1)
	{
		copy_to_assign(vars);
		solved = true;
		return true;
	}


	// INSERT NEW CONSTRAINT 
	
	
	
	
	
	
	//add_constraints(touples, vars);

	remove_constraints(touples);
	
	restore_domain(vars);




	/// END INSERT NEW 





	// recursive case

	int next_unassigned_var_position = unassigned(vars, touples);
	int value_of_var;
	bool assigned = false;

	for (unsigned int i = 0; i < vars[next_unassigned_var_position].domain.size(); i++)
	{
		value_of_var = vars[next_unassigned_var_position].domain[i];
		if (is_consistant(value_of_var, next_unassigned_var_position, vars, touples))
		{
			vars[next_unassigned_var_position].assignment = value_of_var;

			// AC and MAC
			if (type == FC)
			{
				if (!pla(next_unassigned_var_position, vars, touples))
				{
					return false;
				}
			}
			else if (type == MAC)
			{
				if (!fla(next_unassigned_var_position, vars, touples))
				{
					return false;
				}
			}


			assigned = backtrack(vars, touples, type);
			if (assigned)
			{
				return true;
			}
		}

		// deleate from domain
		vars[next_unassigned_var_position].domain.erase(vars[next_unassigned_var_position].domain.begin() + i);
		i--;
	}

	return false;

}


// additional constraints to be added during runtime
void Solver::push_constraints(std::vector<_constraint_touple> constraints)
{
	for (unsigned int i = 0; i < constraints.size(); i++)
	{
		new_constraints.push(constraints[i]);
	}
}

// probability of adding/removing constraint when 
// add_constraint && remove constraint are ran
void Solver::set_probability(int probavility)
{
	_probability = probavility;
}




void Solver::remove_constraints(std::vector<_constraint_touple> & constraints)
{
	
	int rnumber = rand() % 100;

	if (rnumber < _probability)
	{
		
		// Select radnom position
		int remove_position = rand() % constraints.size();

		printf("REMOVED: %d %d\n", constraints[remove_position].x.var, constraints[remove_position].y.var);

		// Copy removed domains structure
		for (unsigned int i = 0; i < constraints[remove_position].deleted_domain.size(); i++)
		{
			reactivated_domains.push_back(constraints[remove_position].deleted_domain[i]);
		}

		// Remove constraint
		constraints.erase(constraints.begin() + remove_position);
	}
}


void Solver::restore_domain(std::vector<_variable>& vars)
{
	
	printf("IM HERE %d\n", reactivated_domains.size());
	int restore_var_position;
	std::vector<int>::iterator exists;


	for (unsigned int i = 0; i < reactivated_domains.size(); i++)
	{
		restore_var_position = variable_position(vars, reactivated_domains[i].var);
		printf("Checking variable: %d\n", restore_var_position);

		for (unsigned int j = 0; j < reactivated_domains[i].domain.size(); j++)
		{
			
			exists = std::find(vars[restore_var_position].domain.begin(), vars[restore_var_position].domain.begin(), reactivated_domains[i].domain[j]);
			
			if (exists != vars[restore_var_position].domain.end())
			{
				printf("RESTORING VARIABLE: %d, DOMAIN %d", vars[restore_var_position].var, reactivated_domains[i].domain[j]);
				vars[restore_var_position].domain.push_back(reactivated_domains[i].domain[j]);
			}
			
			
		}
	}

}


void Solver::set_deleating_constraint(std::vector<_constraint_touple>& constraints, _constraint_touple this_constraint, _variable var, int domain_member)
{
	
	std::vector<int>::iterator exists;
	bool found = false;
	// find matching constraint
	for (unsigned int i = 0; i < constraints.size(); i++)
	{
		// check if current constraint is this_constraint
		if ((constraints[i].x.var == this_constraint.x.var) && (constraints[i].y.var == this_constraint.y.var))
		{
			// check if this constraint already removed a value from this variables domain
			for (unsigned int j = 0; j < constraints[i].deleted_domain.size(); i++)
			{
				if (constraints[i].deleted_domain[j].var == var.var)
				{
					found = true;
					printf("ADDED TO RESTORED (existing): constraint: %d %d , variable: %d, domain: %d \n", constraints[i].x.var, constraints[i].y.var, var.var, domain_member);
					constraints[i].deleted_domain[j].domain.push_back(domain_member);
					break;
				}
			}
			
			// constraint has not removed any values from this variable, create whole new
			// _deleated_domains strt and push
			if (!found)
			{
				printf("ADDED TO RESTORED (new): constraint: %d %d , variable: %d, domain: %d \n", constraints[i].x.var, constraints[i].y.var, var.var, domain_member);
				_deleted_domains new_deleated;
				new_deleated.var = var.var;
				new_deleated.domain.push_back(domain_member);
				reactivated_domains.push_back(new_deleated);
			}
			break;
		}
	}
}
