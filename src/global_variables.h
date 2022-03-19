#ifndef VARIABLE_local_HEADER
#define VARIABLE_local_HEADER


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

//#include </home/venus/ILOG/CPLEX_Studio128/cplex/include/ilcplex/cplex.h>
#include </home/fabio/ILOG/CPLEX_Studio_AcademicResearch201/cplex/include/ilcplex/cplex.h>


#include <../copt/graph/graph_gen.h>
#include <../copt/graph/algorithms/decode.h>
#include <../copt/utils/common.h>
#include <../copt/utils/logger.h>
#include <../copt/copt/clique/clique.h>
#include "../copt/copt/clique/interdiction/interdiction_ramsey.h"


typedef class data
{

public:

	/////////////////////////////////////INPUT/////////////////////////////////////
	int   PARAM_SIZE_GRAPH;
	int   PARAM_M;
	int   PARAM_N;
	int   PARAM_ALGO;//1 for the feasibility model and 2 the generalized mode
	int   PARAM_OPTIONS;//with ALGO 2, set it to 1 to impose theta <= m -1
	int   PARAM_CIRCULANT;//1 to impose circulant constraints
	double  PARAM_TIME_LIMIT;
	int   PARAM_STRONGER_CUTS;//1 to use better RHS in the clique cuts
	int   PARAM_CPLEX;//1 to use CPLEX to separate clique cuts
	int   PARAM_AMTS;//set to 1 to use AMTS in the separation
	double	PARAM_TOUT_AMTS;//time limit for the execution of AMTS
	int   PARAM_K_CUTS;//1 to insert Kalfleish inequalities
	////////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////CPLEX/////////////////////////////////////
	CPXENVptr env_clique,env_MODEL_1,env_MODEL_2;
	CPXLPptr lp_clique,lp_MODEL_1,lp_MODEL_2;
	int status,ccnt,rcnt,nzcnt,lpstat,nodecount,numrows,numcols;
	int* rmatbeg,*rmatind,*cmatbeg, *cmatind;
	double *rmatval,*cmatval,*rngval,*obj, *lb, *ub,*rhs,coef,objval,bestobjval;
	char *xctype,*sense;
	char **rowname,**colname;
	////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////
	int **LOOK_UP_RAMSEY;
	////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////
	double *CLIQUE_SOL;
	double CLIQUE_VAL;
	double magic_value;
	////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////
	double *CLIQUE_BLUE;
	int size_CLIQUE_BLUE;
	double time_CLIQUE_BLUE;
	double *CLIQUE_RED;
	int size_CLIQUE_RED;
	double time_CLIQUE_RED;
	////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////
	double *X_CALLBACK;
	double   cut_RHS;
	int    * cut_rmatind;
	double * cut_rmatval;
	////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////
	int n_variable_MODEL_1;

	double *X_MODEL_1;
	////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////
	int n_variable_MODEL_2;

	double *X_MODEL_2;
	////////////////////////////////////////////////////////////////////////////////




	////////////////////////////////////////////////////////////////////////////////
	double time_clique_BB;
	double time_clique_cplex;
	int  n_clique_calls_red;
	int  n_clique_calls_blue;
	int  n_cuts_red;
	int  n_cuts_blue;
	int  n_cuts_red_plus;
	int  n_cuts_blue_plus;
	////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//BBS STUFF
	interRamsey < CliqueXRD<ugraph, 128 > > ci;//128 is the meaning of the universe

	int **edge_fixing;

	/***********************************************************************************/
	void initialize_ug(int number_of_vertices)
	/***********************************************************************************/
	{

		clock_t time_start_creating=clock();

		ci.init(number_of_vertices, PARAM_N, PARAM_M);

		clock_t time_end_creating=clock();

	}


	/***********************************************************************************/
	void finalize_ug()
	/***********************************************************************************/
	{

		//nothing to do

	}


	/***********************************************************************************/
	int clique_solve_BB_edge_fixing(int **fixing, double *solution, bool circulant, int TARGET /* ramsey n or ramsey m */,  bool flag_AMTS, double TOUT_AMTS, int BRUTE_FORCE, int RANDOM_VCIRC)
	/***********************************************************************************/
	{

		base_interRamsey::result_t r;

		ci.set_edges(fixing);

		r=ci.run_edge(circulant, flag_AMTS, TOUT_AMTS, TARGET, BRUTE_FORCE, RANDOM_VCIRC);

		if(r.w == -1){

			return 0;
		}

		//ASSERT!
		if(r.w!=r.sol.size())
		{
			cout<<"w:"<<r.w<<"\t PROBLEM->sol_size:"<<r.sol.size()<<endl;
			exit(-1);
		}

		for(int i=0;i<r.w;i++)
		{
			solution[r.sol[i]]=1;
		}

		return r.w;

	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



} data;



#endif
