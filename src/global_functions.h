#ifndef GLOBAL_FUNCTIONS_HEADER
#define GLOBAL_FUNCTIONS_HEADER


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <errno.h>
#include <cstring>
#include <cstdlib>

using namespace std;

#include "global_variables.h"

#include "CLIQUE_CPLEX.h"
#include "check_solution.h"

/***********************************************************************************/
void FILL_RAMSEY_LOOK_UP(data *RAMSEY_instance);
/***********************************************************************************/

/***********************************************************************************/
void memory_allocation(data *RAMSEY_instance);
/***********************************************************************************/

/***********************************************************************************/
void memory_deallocation(data *RAMSEY_instance);
/***********************************************************************************/

/***********************************************************************************/
int position_r(data *RAMSEY_instance, int i,int j);
/***********************************************************************************/

/***********************************************************************************/
int position_b(data *RAMSEY_instance, int i, int j);
/***********************************************************************************/

/***********************************************************************************/
int position_theta(data *RAMSEY_instance);
/***********************************************************************************/

/***********************************************************************************/
int magic_formula(int K,int l);
/***********************************************************************************/




#endif
