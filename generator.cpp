#include "generator.h"


Generator::Generator()
{
	// user entered data
	n = -1;
	p = -1;
	alpha = -1;
	r = -1;
	// derived data
	number_variables = -1;
	number_constraints = -1;
	number_incompatables = -1;
	domain = -1;
	error = 0;
}



/*
	SETTER FUNCTIONS w/ error checking
	/////////////////////////////////////////////////////////////////////////////////////////////////
*/
bool Generator::setAlpha(double value)
{
	
	if ((value >= 0.00) && (value <= 1.00))
	{
		error = 0;
		alpha = value;
		return true;
	}
	else
	{
		error = 1;
		return false;
	}
}

bool Generator::setN(int value)
{
	if (value > 0)
	{
		error = 0;
		n = value;
		return  true;
	}
	else
	{
		error = 2;
		return false;
	}
}

bool Generator::setP(double value)
{
	if ((value >= 0.00) && (value <= 1.00))
	{
		error = 0;
		p = value;
		return true;
	}
	else
	{
		error = 3;
		return false;
	}
}


bool Generator::setR(double value)
{
	if ((value >= 0.00) && (value <= 1.00))
	{
		error = 0;
		r = value;
		return true;
	}
	else
	{
		error = 4;
		return false;
	}
}


/*
	GETTER FUNCTIONS
	/////////////////////////////////////////////////////////////////////////////////////////////////
*/
int Generator::getN()
{
	return n;
}

double Generator::getAlpha()
{
	return alpha;
}

double Generator::getP()
{
	return p;
}

double Generator::getR()
{
	return r;
}
/*
	END GETTER FUNCTIONS
*/

/*
	ERROR FUNCTION
	get_error default returns error number
	get_error(true) returns error number and prints error message
	/////////////////////////////////////////////////////////////////////////////////////////////////
*/
int Generator::get_error(bool val)
{
	if (val == true)
	{
		
		switch (error)
		{
		case 1: std::cout << "Parameter error: alpha\n"; break;
		case 2: std::cout << "Parameter error: n\n"; break;
		case 3: std::cout << "Parameter error: p\n"; break;
		case 4: std::cout << "Parameter error: r\n"; break;
		case 5: std::cout << "Attempting to run generator with missing or invalid data.\n"; break;
		case 6: std::cout << "Parameter error: domain\n"; break;

		default:
		std::cout << "Unknown error: n\n"; break;
		}

		return error;
	}
	else
	{
		return error;
	}
}




/*
	GENERATOR FUNCTIONS
	/////////////////////////////////////////////////////////////////////////////////////////////////
*/


/*
	Calculate domain
*/
int Generator::calc_domain()
{
	if (!error)
	{
		return (int)nearbyint(pow(n, alpha));
	}
	else
	{
		return -1;
	}
}
/*
	Calculate number of constraints :- n * r * log2(n)
*/
int Generator::calc_constraints()
{
	if (!error)
	{
		return (int)nearbyint(n * r * log(n));
	}
	else
	{
		return -1;
	}
}

int Generator::calc_incompatables()
{
	if (domain < 0)
	{
		error = 6;
		return -1;
	}
	else if (error)
	{
		return -1;
	}
	else
	{
		return (int)nearbyint(p * pow(domain, 2));
	}
}

bool Generator::unique_touple(int a, int b)
{
	if (a == b)
	{
		return false;
	}
	else
	{
		for (int i = 0; i < constraint_touples.size(); i++)
		{
			if ((constraint_touples[i].x.var == variables[a].var) && (constraint_touples[i].y.var == variables[b].var))
			{
				return false;
			}
		}
		return true;
	}
}
// This function makes sure constraints are unique.
// Meaning if a constraint is generated (1 , 0), same constraint is not generate again.
// COnstraint (0, 1) is however acceptable
bool Generator::unique_incompatable(int a, int b, int touple)
{
	
	for (int i = 0; i < constraint_touples[touple].constraints.size(); i++)
	{
		if ((constraint_touples[touple].constraints[i].x == a) && (constraint_touples[touple].constraints[i].y == b))
			return false;
	}
	
	return true;
}

bool Generator::create()
{
	if (error)
	{
		error = 5;
		return false;
	}

	// BEGIN GENERATOR
	
	

	// POPULATE VARIABLES AMD
	// GENERATE DOMAIN FOREACH
	domain = calc_domain();
	_variable curr_var;

	for (int i = 0; i < n; i++)
	{
		curr_var.var = i;
		curr_var.assignment = UNASSIGNED;
		for (int t = 0; t < domain; t++)
		{
			curr_var.domain.push_back(t);
		}
		variables.push_back(curr_var);
		curr_var.domain.clear();
	}
	// DOMAIN GENERATED
	// AND VARIABLES POPULATED

	// GENERATE CONSTRAINTS TOUPLE
	time_t t = 1;
	srand(time(NULL) + (++t));
	int a, b;
	number_constraints = calc_constraints();
	for (int i = 0; i < number_constraints; i++)
	{
		// make sure variables are distnct
		_constraint_touple touple;

		
		do{
			a = rand() % n;
			b = rand() % n;
		} while (!unique_touple(a, b));
		touple.x = variables[a];
		touple.y = variables[b];
		// push touple into array
		constraint_touples.push_back(touple);
	}
	// CONSTRAINTS TOUPLES GENERATED


	// GENERATE INCOMPATABLES FOR EACH TOUPLE
	number_incompatables = calc_incompatables();
	for (int i = 0; i < constraint_touples.size(); i++)
	{
		for (int t = 0; t < number_incompatables; t++)
		{
			_constraint_values values;
			do{
			a = rand() % domain;
			b = rand() % domain;
			} while (!unique_incompatable(a, b, i));
			values.x = a;
			values.y = b;
			constraint_touples[i].constraints.push_back(values);
		}


	}
	// GENERATED INCOMPATABLES FOR EACH TOUPLE

	// END GENERATOR 

	return true;
}

// Values enclosed by <1, 0> are variables
// Values enclosed by [(0, 0)] are constraints 
// Output format should look like <1, 0> [(0, 0), ...]
void Generator::print()
{
	for (int i = 0; i < constraint_touples.size(); i++)
	{
		std::cout << "<" << constraint_touples[i].x.var << "," << constraint_touples[i].y.var << "> : [";
		for (int t = 0; t < constraint_touples[i].constraints.size(); t++)
		{
			std::cout << "(" << constraint_touples[i].constraints[t].x << "," << constraint_touples[i].constraints[t].y << ") ";
		}
		std::cout << "]" << std::endl;
	}


}

std::vector<_constraint_touple> Generator::return_csp()
{
	return constraint_touples;
}

std::vector<_variable> Generator::return_var()
{
	return variables;
}
