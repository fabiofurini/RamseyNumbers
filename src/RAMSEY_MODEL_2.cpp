
#include "RAMSEY_MODEL_2.h"

//#define  PRINT_SOLUTION_MODEL_2
//#define PRINT_SOLUTION_CALLBACK
//#define PRINT_MODEL_2_LP

/***********************************************************************************/
void RAMSEY_MODEL_2_allocation(data *RAMSEY_instance)
/***********************************************************************************/
{
	RAMSEY_instance->n_variable_MODEL_2=2*RAMSEY_instance->PARAM_SIZE_GRAPH*RAMSEY_instance->PARAM_SIZE_GRAPH+1;

	RAMSEY_instance->time_clique_BB=0;
	RAMSEY_instance->time_clique_cplex=0;
	RAMSEY_instance->n_clique_calls_red=0;
	RAMSEY_instance->n_clique_calls_blue=0;
	RAMSEY_instance->n_cuts_red=0;
	RAMSEY_instance->n_cuts_blue=0;
	RAMSEY_instance->n_cuts_red_plus=0;
	RAMSEY_instance->n_cuts_blue_plus=0;

	RAMSEY_instance->X_CALLBACK=new double[RAMSEY_instance->n_variable_MODEL_2];
	RAMSEY_instance->cut_rmatind=new int[RAMSEY_instance->n_variable_MODEL_2];
	RAMSEY_instance->cut_rmatval=new double[RAMSEY_instance->n_variable_MODEL_2];

	RAMSEY_instance->X_MODEL_2=new double[RAMSEY_instance->n_variable_MODEL_2];

}

/***********************************************************************************/
void RAMSEY_MODEL_2_deallocation(data *RAMSEY_instance)
/***********************************************************************************/
{

	delete []RAMSEY_instance->cut_rmatind;
	delete []RAMSEY_instance->cut_rmatval;
	delete []RAMSEY_instance->X_CALLBACK;

	delete []RAMSEY_instance->X_MODEL_2;


}


/***********************************************************************************/
int CPXPUBLIC mycutcallback_LAZY_MODEL_2(CPXCENVptr env,void *cbdata,int wherefrom,void *cbhandle,int *useraction_p)
/***********************************************************************************/
{

	(*useraction_p)=CPX_CALLBACK_DEFAULT;

	data *RAMSEY_instance=(data *) cbhandle;


	RAMSEY_instance->status=CPXgetcallbacknodex(env,cbdata,wherefrom,RAMSEY_instance->X_CALLBACK,0,RAMSEY_instance->n_variable_MODEL_2-1);
	if(RAMSEY_instance->status!=0)
	{
		printf("cannot get the x\n");
		exit(-1);
	}


#ifdef PRINT_SOLUTION_CALLBACK

	double _OBJ_VALUE;
	RAMSEY_instance->status=CPXgetcallbacknodeobjval(env,cbdata,wherefrom,&_OBJ_VALUE);
	if(RAMSEY_instance->status!=0){
		printf("cannot get the x\n");
		exit(-1);
	}

	double _BEST_INTEGER;
	RAMSEY_instance->status=CPXgetcallbackinfo(env,cbdata,wherefrom,CPX_CALLBACK_INFO_BEST_INTEGER,&_BEST_INTEGER);
	if(RAMSEY_instance->status!=0){
		printf("cannot get the x\n");
		exit(-1);
	}
	double _BEST_REMAINING;
	RAMSEY_instance->status=CPXgetcallbackinfo(env,cbdata,wherefrom,CPX_CALLBACK_INFO_BEST_REMAINING,&_BEST_REMAINING);
	if(RAMSEY_instance->status!=0){
		printf("cannot get the x\n");
		exit(-1);
	}
	int _NODE;
	RAMSEY_instance->status=CPXgetcallbackinfo(env,cbdata,wherefrom,CPX_CALLBACK_INFO_NODE_COUNT,&_NODE);
	if(RAMSEY_instance->status!=0){
		printf("cannot get the x\n");
		exit(-1);
	}

	cout << "\n\n******************************************************\n\n";

	cout << _BEST_INTEGER << " _BEST_INTEGER " << _BEST_REMAINING  << " _BEST_REMAINING " << " _NODE " << _NODE <<  endl;

	cout << "_OBJ_VALUE\t" << _OBJ_VALUE << endl;

	cout << "\n\nB:\n";
	for(int i=0; i<RAMSEY_instance->PARAM_SIZE_GRAPH; i++)
	{
		for(int j=0; j<RAMSEY_instance->PARAM_SIZE_GRAPH; j++)
		{
			cout << (int)(RAMSEY_instance->X_CALLBACK[position_b(RAMSEY_instance,i,j)]+0.5);
		}
		cout << endl;
	}
	cout << endl;

	cout << "\n\nR:\n";
	for(int i=0; i<RAMSEY_instance->PARAM_SIZE_GRAPH; i++)
	{
		for(int j=0; j<RAMSEY_instance->PARAM_SIZE_GRAPH; j++)
		{
			cout << (int)(RAMSEY_instance->X_CALLBACK[position_r(RAMSEY_instance,i,j)]+0.5);
		}
		cout << endl;
	}
	cout << endl;


	cout << "THETA\t" << RAMSEY_instance->X_CALLBACK[position_theta(RAMSEY_instance)] << endl;
	cin.get();

#endif

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	RAMSEY_instance->n_clique_calls_blue++;

	if(RAMSEY_instance->PARAM_CPLEX==1)
	{
		clock_t time_start_b=clock();
		clique_blue_solve_cplex(RAMSEY_instance);
		clock_t time_end_b=clock();
		RAMSEY_instance->time_clique_cplex+=(double)(time_end_b-time_start_b)/(double)CLOCKS_PER_SEC;
	}
	else
	{
		int edges=0;
		for(int i=0;i<RAMSEY_instance->PARAM_SIZE_GRAPH;i++)
		{
			for(int j=i+1;j<RAMSEY_instance->PARAM_SIZE_GRAPH;j++)
			{
				if(RAMSEY_instance->X_CALLBACK[position_b(RAMSEY_instance,i,j)]<0.5){RAMSEY_instance->edge_fixing[i][j]=0;}
				else{RAMSEY_instance->edge_fixing[i][j]=1;edges++;}
			}
		}

		for (int i = 0; i < RAMSEY_instance->PARAM_SIZE_GRAPH; i++){RAMSEY_instance->CLIQUE_SOL[i]=0;}
		clock_t time_start_b_BB=clock();


		RAMSEY_instance->CLIQUE_VAL=RAMSEY_instance->clique_solve_BB_edge_fixing
				(
						RAMSEY_instance->edge_fixing,
						RAMSEY_instance->CLIQUE_SOL,
						RAMSEY_instance->PARAM_CIRCULANT,
						RAMSEY_instance->PARAM_SIZE_GRAPH,
						RAMSEY_instance->PARAM_AMTS,
						RAMSEY_instance->PARAM_TOUT_AMTS,
						RAMSEY_instance->PARAM_CIRCULANT,
						0
				)	;

		clock_t time_end_b_BB=clock();
		RAMSEY_instance->time_clique_BB+=(double)(time_end_b_BB-time_start_b_BB)/(double)CLOCKS_PER_SEC;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef PRINT_SOLUTION_CALLBACK
	cout << "CLIQUE_VAL BLUE\t" << RAMSEY_instance->CLIQUE_VAL << endl;
	cout << "CLIQUE BLUE\n";
	for(int i=0; i<RAMSEY_instance->PARAM_SIZE_GRAPH; i++){cout << (int)(RAMSEY_instance->CLIQUE_SOL[i]+0.5);}cout << endl;
	cin.get();
#endif



	if( (int) (RAMSEY_instance->X_CALLBACK[position_theta(RAMSEY_instance)] +0.5) < (int) (RAMSEY_instance->CLIQUE_VAL+0.5) )
	{

		RAMSEY_instance->cut_RHS=RAMSEY_instance->CLIQUE_VAL;
		RAMSEY_instance->nzcnt=0;


		for(int i=0; i<RAMSEY_instance->PARAM_SIZE_GRAPH; i++)
		{
			for(int j=i+1; j<RAMSEY_instance->PARAM_SIZE_GRAPH; j++)
			{
				if(RAMSEY_instance->CLIQUE_SOL[i]>0.5 && RAMSEY_instance->CLIQUE_SOL[j]>0.5)
				{
					RAMSEY_instance->cut_rmatind[RAMSEY_instance->nzcnt]=position_r(RAMSEY_instance,i,j);
					RAMSEY_instance->cut_rmatval[RAMSEY_instance->nzcnt]=1.0;
					RAMSEY_instance->nzcnt++;
				}
			}
		}

		RAMSEY_instance->cut_rmatind[RAMSEY_instance->nzcnt]=position_theta(RAMSEY_instance);
		RAMSEY_instance->cut_rmatval[RAMSEY_instance->nzcnt]=1.0;
		RAMSEY_instance->nzcnt++;

		RAMSEY_instance->status=CPXcutcallbackadd (env,cbdata,wherefrom,RAMSEY_instance->nzcnt,RAMSEY_instance->cut_RHS,'G',RAMSEY_instance->cut_rmatind,RAMSEY_instance->cut_rmatval,CPX_USECUT_FORCE);
		if(RAMSEY_instance->status!=0){printf("CPXcutcallbackadd\n");exit(-1);}
		RAMSEY_instance->n_cuts_blue++;
		(*useraction_p)=CPX_CALLBACK_SET;


	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//part on the red
	RAMSEY_instance->n_clique_calls_red++;

	if(RAMSEY_instance->PARAM_CPLEX==1)
	{
		clock_t time_start_r=clock();
		clique_red_solve_cplex(RAMSEY_instance);
		clock_t time_end_r=clock();
		RAMSEY_instance->time_clique_cplex+=(double)(time_end_r-time_start_r)/(double)CLOCKS_PER_SEC;
	}
	else
	{
		int edges=0;
		for(int i=0;i<RAMSEY_instance->PARAM_SIZE_GRAPH;i++)
		{
			for(int j=i+1;j<RAMSEY_instance->PARAM_SIZE_GRAPH;j++)
			{
				if(RAMSEY_instance->X_CALLBACK[position_r(RAMSEY_instance,i,j)]<0.5){RAMSEY_instance->edge_fixing[i][j]=0;}
				else{RAMSEY_instance->edge_fixing[i][j]=1;edges++;}
			}
		}

		for (int i = 0; i < RAMSEY_instance->PARAM_SIZE_GRAPH; i++){RAMSEY_instance->CLIQUE_SOL[i]=0;}
		clock_t time_start_r_BB=clock();

		RAMSEY_instance->CLIQUE_VAL=RAMSEY_instance->clique_solve_BB_edge_fixing
				(
						RAMSEY_instance->edge_fixing,
						RAMSEY_instance->CLIQUE_SOL,
						RAMSEY_instance->PARAM_CIRCULANT,
						RAMSEY_instance->PARAM_N,
						RAMSEY_instance->PARAM_AMTS,
						RAMSEY_instance->PARAM_TOUT_AMTS,
						RAMSEY_instance->PARAM_CIRCULANT,
						0
				);


		clock_t time_end_r_BB=clock();
		RAMSEY_instance->time_clique_BB+=(double)(time_end_r_BB-time_start_r_BB)/(double)CLOCKS_PER_SEC;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef PRINT_SOLUTION_CALLBACK
	cout << "CLIQUE_VAL RED\t" << RAMSEY_instance->CLIQUE_VAL << endl;
	cout << "CLIQUE RED\n";
	for(int i=0; i<RAMSEY_instance->PARAM_SIZE_GRAPH; i++){cout << (int)(RAMSEY_instance->CLIQUE_SOL[i]+0.5);}cout << endl;
#endif


	if(RAMSEY_instance->CLIQUE_VAL >= RAMSEY_instance->PARAM_N )
	{

		if(RAMSEY_instance->PARAM_STRONGER_CUTS==0 || RAMSEY_instance->CLIQUE_VAL == RAMSEY_instance->PARAM_N)
		{

			RAMSEY_instance->cut_RHS=RAMSEY_instance->CLIQUE_VAL-RAMSEY_instance->PARAM_N+1;
			RAMSEY_instance->nzcnt=0;


			for(int i=0; i<RAMSEY_instance->PARAM_SIZE_GRAPH; i++)
			{
				for(int j=i+1; j<RAMSEY_instance->PARAM_SIZE_GRAPH; j++)
				{
					if(RAMSEY_instance->CLIQUE_SOL[i]>0.5 && RAMSEY_instance->CLIQUE_SOL[j]>0.5)
					{
						RAMSEY_instance->cut_rmatind[RAMSEY_instance->nzcnt]=position_b(RAMSEY_instance,i,j);
						RAMSEY_instance->cut_rmatval[RAMSEY_instance->nzcnt]=1.0;
						RAMSEY_instance->nzcnt++;
					}
				}
			}

			RAMSEY_instance->status=CPXcutcallbackadd (env,cbdata,wherefrom,RAMSEY_instance->nzcnt,RAMSEY_instance->cut_RHS,'G',RAMSEY_instance->cut_rmatind,RAMSEY_instance->cut_rmatval,CPX_USECUT_FORCE);
			if(RAMSEY_instance->status!=0){printf("CPXcutcallbackadd\n");exit(-1);}
			RAMSEY_instance->n_cuts_red++;
			(*useraction_p)=CPX_CALLBACK_SET;
		}
		else
		{

			RAMSEY_instance->cut_RHS=magic_formula(RAMSEY_instance->CLIQUE_VAL,RAMSEY_instance->PARAM_N);
			RAMSEY_instance->nzcnt=0;


			for(int i=0; i<RAMSEY_instance->PARAM_SIZE_GRAPH; i++)
			{
				for(int j=i+1; j<RAMSEY_instance->PARAM_SIZE_GRAPH; j++)
				{
					if(RAMSEY_instance->CLIQUE_SOL[i]>0.5 && RAMSEY_instance->CLIQUE_SOL[j]>0.5)
					{
						RAMSEY_instance->cut_rmatind[RAMSEY_instance->nzcnt]=position_b(RAMSEY_instance,i,j);
						RAMSEY_instance->cut_rmatval[RAMSEY_instance->nzcnt]=1.0;
						RAMSEY_instance->nzcnt++;
					}
				}
			}

			RAMSEY_instance->status=CPXcutcallbackadd (env,cbdata,wherefrom,RAMSEY_instance->nzcnt,RAMSEY_instance->cut_RHS,'G',RAMSEY_instance->cut_rmatind,RAMSEY_instance->cut_rmatval,CPX_USECUT_FORCE);
			if(RAMSEY_instance->status!=0){printf("CPXcutcallbackadd\n");exit(-1);}
			RAMSEY_instance->n_cuts_red_plus++;
			(*useraction_p)=CPX_CALLBACK_SET;

		}

	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	return 0;
}

/***********************************************************************************/
void RAMSEY_MODEL_2_parameter_setting(data *RAMSEY_instance)
/***********************************************************************************/
{

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// * Set printing *

	CPXsetintparam (RAMSEY_instance->env_MODEL_2, CPX_PARAM_SCRIND, CPX_ON);

	// * Set number of CPU*

	RAMSEY_instance->status = CPXsetintparam (RAMSEY_instance->env_MODEL_2, CPX_PARAM_THREADS, 1);
	if (RAMSEY_instance->status)
	{
		printf ("error for CPX_PARAM_EPRHS\n");
	}

	// * Set time limit *

	RAMSEY_instance->status = CPXsetdblparam (RAMSEY_instance->env_MODEL_2, CPX_PARAM_TILIM,RAMSEY_instance->PARAM_TIME_LIMIT);
	if (RAMSEY_instance->status)
	{
		printf ("error for CPX_PARAM_EPRHS\n");
	}


	// * Set MIP ENPHASIS *

	RAMSEY_instance->status = CPXsetintparam (RAMSEY_instance->env_MODEL_2, CPX_PARAM_MIPEMPHASIS,CPX_MIPEMPHASIS_OPTIMALITY);
	if (RAMSEY_instance->status)
	{
		printf ("error for CPX_MIPEMPHASIS_OPTIMALITY\n");
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// * MIP node selection strategy *

	//	0
	//CPX_NODESEL_DFS
	//Depth-first search
	//1
	//CPX_NODESEL_BESTBOUND
	//Best-bound search; default
	//2
	//CPX_NODESEL_BESTEST
	//Best-estimate search
	//3
	//CPX_NODESEL_BESTEST_ALT
	//Alternative best-estimate search

	RAMSEY_instance->status = CPXsetintparam (RAMSEY_instance->env_MODEL_2, CPX_PARAM_NODESEL,CPX_NODESEL_DFS);
	if (RAMSEY_instance->status)
	{
		printf ("error for CPX_PARAM_MIPEMPHASIS\n");
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//	//	-1	CPX_VARSEL_MININFEAS	Branch on variable with minimum infeasibility
	//	//0	CPX_VARSEL_DEFAULT	Automatic: let CPLEX choose variable to branch on; default
	//	//1	CPX_VARSEL_MAXINFEAS	Branch on variable with maximum infeasibility
	//	//2	CPX_VARSEL_PSEUDO	Branch based on pseudo costs
	//	//3	CPX_VARSEL_STRONG	Strong branching
	//	//4	CPX_VARSEL_PSEUDOREDUCED	Branch based on pseudo reduced costs
	//
	//	RAMSEY_instance->status = CPXsetintparam (RAMSEY_instance->env_MODEL_2, CPXPARAM_MIP_Strategy_VariableSelect,CPX_VARSEL_DEFAULT);
	//			if (RAMSEY_instance->status)
	//			{
	//				printf ("error for CPX_PARAM_MIPEMPHASIS\n");
	//			}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	CPXsetintparam(RAMSEY_instance->env_MODEL_2, CPX_PARAM_MIPCBREDLP, CPX_OFF);        // let MIP callbacks work on the original model
	//	CPXsetintparam(RAMSEY_instance->env_MODEL_2, CPX_PARAM_PRELINEAR, CPX_OFF);              // assure linear mappings between the presolved and original models
	CPXsetintparam(RAMSEY_instance->env_MODEL_2, CPX_PARAM_REDUCE, CPX_PREREDUCE_PRIMALONLY);
	//	CPXsetintparam(RAMSEY_instance->env_MODEL_2, CPXPARAM_Preprocessing_Reduce, CPX_PREREDUCE_PRIMALONLY);
	//	CPXsetintparam(RAMSEY_instance->env_MODEL_2, CPXPARAM_Preprocessing_Linear, 0);


	RAMSEY_instance->status = CPXsetlazyconstraintcallbackfunc(RAMSEY_instance->env_MODEL_2,mycutcallback_LAZY_MODEL_2,RAMSEY_instance);
	if (RAMSEY_instance->status)
	{
		printf ("error for CPXsetlazyconstraintcallbackfunc\n");
	}


}

/***********************************************************************************/
double RAMSEY_MODEL_2_solve(data *RAMSEY_instance)
/***********************************************************************************/
{


	//////////////////////////////////////////////////
	RAMSEY_MODEL_2_parameter_setting(RAMSEY_instance);
	//////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////

	clock_t time_start=clock();

	RAMSEY_instance->status=CPXmipopt(RAMSEY_instance->env_MODEL_2,RAMSEY_instance->lp_MODEL_2);
	if(RAMSEY_instance->status!=0)
	{
		printf("error in CPXmipopt\n");
		exit(-1);
	}

	cout << "DONE with MIPopt\n\n";

	clock_t time_end=clock();

	double RAMSEY_time=(double)(time_end-time_start)/(double)CLOCKS_PER_SEC;
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// * getting the solution

	bool SOL_FOUND=true;
	RAMSEY_instance->status=CPXgetmipx(RAMSEY_instance->env_MODEL_2,RAMSEY_instance->lp_MODEL_2,RAMSEY_instance->X_MODEL_2,0,RAMSEY_instance->n_variable_MODEL_2-1);
	if(RAMSEY_instance->status!=0)
	{
		SOL_FOUND=false;
		printf("error in CPXgetmipx\n");
	}

	RAMSEY_instance->objval=-1;
	RAMSEY_instance->status=CPXgetmipobjval(RAMSEY_instance->env_MODEL_2,RAMSEY_instance->lp_MODEL_2,&(RAMSEY_instance->objval));
	if(RAMSEY_instance->status!=0)
	{
		printf("error in CPXgetmipobjval\n");
	}


	RAMSEY_instance->bestobjval=-1;
	RAMSEY_instance->status=CPXgetbestobjval(RAMSEY_instance->env_MODEL_2,RAMSEY_instance->lp_MODEL_2,&(RAMSEY_instance->bestobjval));
	if(RAMSEY_instance->status!=0)
	{
		printf("error in CPXgetbestobjval\n");
	}

	RAMSEY_instance->lpstat=CPXgetstat(RAMSEY_instance->env_MODEL_2,RAMSEY_instance->lp_MODEL_2);
	RAMSEY_instance->nodecount = CPXgetnodecnt(RAMSEY_instance->env_MODEL_2, RAMSEY_instance->lp_MODEL_2);

	int cur_numcols=CPXgetnumcols(RAMSEY_instance->env_MODEL_2,RAMSEY_instance->lp_MODEL_2);
	int cur_numrows=CPXgetnumrows(RAMSEY_instance->env_MODEL_2,RAMSEY_instance->lp_MODEL_2);

	cout << "objval ->\t" << RAMSEY_instance->objval << endl;
	cout << "bestobjval ->\t" << RAMSEY_instance->bestobjval << endl;
	cout << "RAMSEY_time ->\t" << RAMSEY_time << endl;
	cout << "lpstat ->\t" << RAMSEY_instance->lpstat << endl;
	cout << "nodecount ->\t" << RAMSEY_instance->nodecount << endl;


	cout << "time_clique_BB ->\t" << RAMSEY_instance->time_clique_BB << endl;
	cout << "time_clique_cplex ->\t" << RAMSEY_instance->time_clique_cplex << endl;
	cout << "n_clique_calls_red ->\t" << RAMSEY_instance->n_clique_calls_red << endl;
	cout << "n_clique_calls_blue ->\t" << RAMSEY_instance->n_clique_calls_blue << endl;
	cout << "n_cuts_red\t" << RAMSEY_instance->n_cuts_red << endl;
	cout << "n_cuts_blue\t"<< RAMSEY_instance->n_cuts_blue<< endl;
	cout << "n_cuts_red_plus\t"<< RAMSEY_instance->n_cuts_red_plus<< endl;
	cout << "n_cuts_blue_plus\t"<< RAMSEY_instance->n_cuts_blue_plus<< endl;




	if(RAMSEY_instance->objval!=-1)
	{

#ifdef		PRINT_SOLUTION_MODEL_2
		cout << "\n\nR:\n";
		for(int i=0; i<RAMSEY_instance->PARAM_SIZE_GRAPH; i++)
		{
			for(int j=0; j<RAMSEY_instance->PARAM_SIZE_GRAPH; j++)
			{
				cout << (int)(RAMSEY_instance->X_MODEL_2[position_r(RAMSEY_instance,i,j)]+0.5);
			}
			cout << endl;
		}
		cout << endl;

		cout << "\n\nB:\n";
		for(int i=0; i<RAMSEY_instance->PARAM_SIZE_GRAPH; i++)
		{
			for(int j=0; j<RAMSEY_instance->PARAM_SIZE_GRAPH; j++)
			{
				cout << (int)(RAMSEY_instance->X_MODEL_2[position_b(RAMSEY_instance,i,j)]+0.5);
			}
			cout << endl;
		}
		cout << endl;
#endif

		////////////////////////////////////////////////////////////////////////
		bool OK_SOL=check_solution(RAMSEY_instance,RAMSEY_instance->X_MODEL_2);
		if(!OK_SOL)
		{
			cout << "NOT A FEASIBLE M and N coloring!!!!\n\n\n";
		}
		////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////
		char dummy_file[10000];
		sprintf(dummy_file,"colorings/col_m%d_n%d_SIZE%d_circulant%d.txt",RAMSEY_instance->PARAM_M,RAMSEY_instance->PARAM_N,RAMSEY_instance->PARAM_SIZE_GRAPH,RAMSEY_instance->PARAM_CIRCULANT);

		cout << dummy_file << endl;

		ofstream out(dummy_file);

		out << RAMSEY_instance->PARAM_SIZE_GRAPH << endl;

		for(int i=0; i<RAMSEY_instance->PARAM_SIZE_GRAPH; i++)
		{
			for(int j=0; j<RAMSEY_instance->PARAM_SIZE_GRAPH; j++)
			{
				out << (int)(RAMSEY_instance->X_MODEL_2[position_b(RAMSEY_instance,i,j)]+0.5) << " ";
			}
			out << endl;
		}
		out << endl;

		out.close();
		////////////////////////////////////////////////////////////////////////////////////////

	}






	//////////////////////////////////////////////////////////////
	ofstream info_SUMMARY("info_RAMSEY.txt", ios::app);
	info_SUMMARY << fixed

			<< RAMSEY_instance->objval  << "\t"
			<< RAMSEY_instance->bestobjval  << "\t"
			<< RAMSEY_instance->lpstat << "\t"
			<< RAMSEY_instance->nodecount  << "\t"
			<<  RAMSEY_time << "\t"
			<< cur_numcols  << "\t"
			<< cur_numrows  << "\t"
			<< RAMSEY_instance->time_clique_cplex << "\t"
			<< RAMSEY_instance->time_clique_BB << "\t"
			<< RAMSEY_instance->n_clique_calls_red << "\t"
			<< RAMSEY_instance->n_clique_calls_blue << "\t"
			<< RAMSEY_instance->n_cuts_red<< "\t"
			<< RAMSEY_instance->n_cuts_blue<< "\t"
			<< 		RAMSEY_instance->n_cuts_red_plus<< "\t"
			<< 		RAMSEY_instance->n_cuts_blue_plus<< "\t"

			<<RAMSEY_instance->PARAM_SIZE_GRAPH << "\t"
			<<RAMSEY_instance->PARAM_M << "\t"
			<<RAMSEY_instance->PARAM_N << "\t"
			<<RAMSEY_instance->PARAM_ALGO << "\t"
			<<RAMSEY_instance->PARAM_OPTIONS << "\t"
			<<RAMSEY_instance->PARAM_CIRCULANT << "\t"
			<<RAMSEY_instance->PARAM_TIME_LIMIT << "\t"
			<<RAMSEY_instance->PARAM_STRONGER_CUTS << "\t"
			<<RAMSEY_instance->PARAM_CPLEX << "\t"


			<<RAMSEY_instance->PARAM_AMTS << "\t"
			<<RAMSEY_instance->PARAM_TOUT_AMTS << "\t"
			<<RAMSEY_instance->PARAM_K_CUTS << "\t"

			<< endl;
	info_SUMMARY.close();
	//////////////////////////////////////////////////////////////

	/////////////////////////////////////////////
	clique_free_cplex(RAMSEY_instance);
	/////////////////////////////////////////////


	/////////////////////////////////////////////
	RAMSEY_MODEL_2_deallocation(RAMSEY_instance);
	/////////////////////////////////////////////


	return RAMSEY_instance->objval;

}

/***********************************************************************************/
void RAMSEY_MODEL_2_free(data *RAMSEY_instance)
/***********************************************************************************/
{

	RAMSEY_instance->status=CPXfreeprob(RAMSEY_instance->env_MODEL_2,&(RAMSEY_instance->lp_MODEL_2));
	if(RAMSEY_instance->status!=0) {printf("error in CPXfreeprob\n");exit(-1);}

	RAMSEY_instance->status=CPXcloseCPLEX(&(RAMSEY_instance->env_MODEL_2));
	if(RAMSEY_instance->status!=0) {printf("error in CPXcloseCPLEX\n");exit(-1);}

}


/***********************************************************************************/
void RAMSEY_MODEL_2_load(data *RAMSEY_instance)
/***********************************************************************************/
{


	/////////////////////////////////////////////
	RAMSEY_MODEL_2_allocation(RAMSEY_instance);
	/////////////////////////////////////////////

	/////////////////////////////////////////////
	clique_load_cplex(RAMSEY_instance);
	/////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	RAMSEY_instance->env_MODEL_2=CPXopenCPLEX(&(RAMSEY_instance->status));
	if(RAMSEY_instance->status!=0)
	{
		printf("cannot open CPLEX environment\n");
		exit(-1);
	}

	RAMSEY_instance->lp_MODEL_2=CPXcreateprob(RAMSEY_instance->env_MODEL_2,&(RAMSEY_instance->status),"RAMSEY");
	if(RAMSEY_instance->status!=0)
	{
		printf("cannot create problem\n");
		exit(-1);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	RAMSEY_instance->ccnt=RAMSEY_instance->n_variable_MODEL_2;

	cout << endl;
	cout << "number of variables\t" << RAMSEY_instance->ccnt << endl;
	cout << "number of edges\t" << (RAMSEY_instance->PARAM_SIZE_GRAPH*(RAMSEY_instance->PARAM_SIZE_GRAPH-1))/2 << endl;
	cout << endl;


	RAMSEY_instance->obj=(double*) calloc(RAMSEY_instance->ccnt,sizeof(double));
	RAMSEY_instance->lb=(double*) calloc(RAMSEY_instance->ccnt,sizeof(double));
	RAMSEY_instance->ub=(double*) calloc(RAMSEY_instance->ccnt,sizeof(double));
	RAMSEY_instance->xctype=(char*) calloc(RAMSEY_instance->ccnt,sizeof(char));

	RAMSEY_instance->colname=(char**) calloc(RAMSEY_instance->ccnt,sizeof(char*));
	for(int i=0;i<RAMSEY_instance->ccnt;i++){RAMSEY_instance->colname[i]=(char*) calloc(1000,sizeof(char));}

	int counter=0;

	for(int i=0; i<RAMSEY_instance->PARAM_SIZE_GRAPH; i++)
	{
		for(int j=0; j<RAMSEY_instance->PARAM_SIZE_GRAPH; j++)
		{

			RAMSEY_instance->obj[counter]=0.0;

			RAMSEY_instance->lb[counter]=0.0;
			RAMSEY_instance->ub[counter]=1.0;
			RAMSEY_instance->xctype[counter]='B';

			if(i>=j){RAMSEY_instance->ub[counter]=0.0;}

			//cout << position_r(RAMSEY_instance,i,j) << "\t" << counter<< endl;

			sprintf(RAMSEY_instance->colname[counter], "r%d_%d",i,j);
			counter++;
		}
	}

	for(int i=0; i<RAMSEY_instance->PARAM_SIZE_GRAPH; i++)
	{
		for(int j=0; j<RAMSEY_instance->PARAM_SIZE_GRAPH; j++)
		{

			RAMSEY_instance->obj[counter]=0.0;
			RAMSEY_instance->lb[counter]=0.0;
			RAMSEY_instance->ub[counter]=1.0;
			RAMSEY_instance->xctype[counter]='B';

			if(i>=j){RAMSEY_instance->ub[counter]=0.0;}

			//cout << position_b(RAMSEY_instance,i,j) << "\t" << counter<< endl;

			sprintf(RAMSEY_instance->colname[counter], "b%d_%d",i,j);
			counter++;
		}
	}


	RAMSEY_instance->obj[counter]=1.0;
	RAMSEY_instance->lb[counter]=0.0;
	RAMSEY_instance->ub[counter]=RAMSEY_instance->PARAM_SIZE_GRAPH;

	if(RAMSEY_instance->PARAM_OPTIONS==2)
	{
		cout << "****THETA LOWER BOUND\t" << RAMSEY_instance->PARAM_M-1 << "\t****\n"<<endl;
		RAMSEY_instance->lb[counter]=RAMSEY_instance->PARAM_M-1;
	}

	RAMSEY_instance->xctype[counter]='I';

	cout << position_theta(RAMSEY_instance) << "\t" << counter<< endl;

	sprintf(RAMSEY_instance->colname[counter], "theta");
	counter++;


	RAMSEY_instance->status=CPXnewcols(RAMSEY_instance->env_MODEL_2,RAMSEY_instance->lp_MODEL_2,RAMSEY_instance->ccnt,RAMSEY_instance->obj,RAMSEY_instance->lb,RAMSEY_instance->ub,RAMSEY_instance->xctype,RAMSEY_instance->colname);
	if(RAMSEY_instance->status!=0)
	{
		printf("error in CPXnewcols\n");
		exit(-1);
	}

	free(RAMSEY_instance->obj);
	free(RAMSEY_instance->lb);
	free(RAMSEY_instance->ub);
	free(RAMSEY_instance->xctype);

	for(int i=0;i<RAMSEY_instance->ccnt;i++){free(RAMSEY_instance->colname[i]);}
	free(RAMSEY_instance->colname);


	cout << "\n\n****MINIMIZATION\n\n";
	CPXchgobjsen(RAMSEY_instance->env_MODEL_2,RAMSEY_instance->lp_MODEL_2,CPX_MIN);




	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// * each edge is or red or blue r_e + b_e =1*

	for(int i=0; i<RAMSEY_instance->PARAM_SIZE_GRAPH; i++)
	{
		for(int j=i+1; j<RAMSEY_instance->PARAM_SIZE_GRAPH; j++)
		{

			RAMSEY_instance->rcnt=1;
			RAMSEY_instance->nzcnt=2;
			RAMSEY_instance->rhs=(double*) calloc(RAMSEY_instance->rcnt,sizeof(double));
			RAMSEY_instance->sense=(char*) calloc(RAMSEY_instance->rcnt,sizeof(double));

			RAMSEY_instance->rhs[0]=1.0;

			RAMSEY_instance->sense[0]='E';

			RAMSEY_instance->rmatbeg=(int*) calloc(RAMSEY_instance->rcnt,sizeof(int));
			RAMSEY_instance->rmatind=(int*) calloc(RAMSEY_instance->nzcnt,sizeof(int));
			RAMSEY_instance->rmatval=(double*) calloc(RAMSEY_instance->nzcnt,sizeof(double));


			RAMSEY_instance->rmatval[0]=1.0;
			RAMSEY_instance->rmatind[0]=position_r(RAMSEY_instance,i,j);

			RAMSEY_instance->rmatval[1]=1.0;
			RAMSEY_instance->rmatind[1]=position_b(RAMSEY_instance,i,j);


			RAMSEY_instance->status=CPXaddrows(RAMSEY_instance->env_MODEL_2,RAMSEY_instance->lp_MODEL_2,0,RAMSEY_instance->rcnt,RAMSEY_instance->nzcnt,RAMSEY_instance->rhs,RAMSEY_instance->sense,RAMSEY_instance->rmatbeg,RAMSEY_instance->rmatind,RAMSEY_instance->rmatval,NULL,NULL);
			if(RAMSEY_instance->status!=0)
			{
				printf("error in CPXaddrows\n");
				exit(-1);
			}

			free(RAMSEY_instance->rmatbeg);
			free(RAMSEY_instance->rmatval);
			free(RAMSEY_instance->rmatind);
			free(RAMSEY_instance->rhs);
			free(RAMSEY_instance->sense);

		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////



	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(RAMSEY_instance->PARAM_CIRCULANT==1)
	{
		//forcing circulant solutions
		for(int i=1; i<RAMSEY_instance->PARAM_SIZE_GRAPH; i++)
		{
			for(int j=i+1; j<RAMSEY_instance->PARAM_SIZE_GRAPH; j++)
			{

				///////////////////////////////////////////////////////////////////////////////////////////////////////
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				RAMSEY_instance->rcnt=1;
				RAMSEY_instance->nzcnt=2;
				RAMSEY_instance->rhs=(double*) calloc(RAMSEY_instance->rcnt,sizeof(double));
				RAMSEY_instance->sense=(char*) calloc(RAMSEY_instance->rcnt,sizeof(double));

				RAMSEY_instance->rhs[0]=0.0;
				RAMSEY_instance->sense[0]='E';


				RAMSEY_instance->rmatbeg=(int*) calloc(RAMSEY_instance->rcnt,sizeof(int));
				RAMSEY_instance->rmatind=(int*) calloc(RAMSEY_instance->nzcnt,sizeof(int));
				RAMSEY_instance->rmatval=(double*) calloc(RAMSEY_instance->nzcnt,sizeof(double));


				RAMSEY_instance->rmatval[0]=1.0;
				RAMSEY_instance->rmatind[0]=position_b(RAMSEY_instance,i,j);

				RAMSEY_instance->rmatval[1]=-1.0;
				RAMSEY_instance->rmatind[1]=position_b(RAMSEY_instance,i-1,j-1);


				RAMSEY_instance->status=CPXaddrows(RAMSEY_instance->env_MODEL_2,RAMSEY_instance->lp_MODEL_2,0,RAMSEY_instance->rcnt,RAMSEY_instance->nzcnt,RAMSEY_instance->rhs,RAMSEY_instance->sense,RAMSEY_instance->rmatbeg,RAMSEY_instance->rmatind,RAMSEY_instance->rmatval,NULL,NULL);
				if(RAMSEY_instance->status!=0)
				{
					printf("error in CPXaddrows\n");
					exit(-1);
				}

				free(RAMSEY_instance->rmatbeg);
				free(RAMSEY_instance->rmatval);
				free(RAMSEY_instance->rmatind);
				free(RAMSEY_instance->rhs);
				free(RAMSEY_instance->sense);
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				///////////////////////////////////////////////////////////////////////////////////////////////////////

			}
		}

		//forcing palindrome solutions
		for(int i=1; i< (int) (RAMSEY_instance->PARAM_SIZE_GRAPH - 1 ) / 2+1; i++)
		{

			///////////////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			RAMSEY_instance->rcnt=1;
			RAMSEY_instance->nzcnt=2;
			RAMSEY_instance->rhs=(double*) calloc(RAMSEY_instance->rcnt,sizeof(double));
			RAMSEY_instance->sense=(char*) calloc(RAMSEY_instance->rcnt,sizeof(double));

			RAMSEY_instance->rhs[0]=0.0;
			RAMSEY_instance->sense[0]='E';


			RAMSEY_instance->rmatbeg=(int*) calloc(RAMSEY_instance->rcnt,sizeof(int));
			RAMSEY_instance->rmatind=(int*) calloc(RAMSEY_instance->nzcnt,sizeof(int));
			RAMSEY_instance->rmatval=(double*) calloc(RAMSEY_instance->nzcnt,sizeof(double));


			RAMSEY_instance->rmatval[0]=1.0;
			RAMSEY_instance->rmatind[0]=position_b(RAMSEY_instance,0,i);

			RAMSEY_instance->rmatval[1]=-1.0;
			RAMSEY_instance->rmatind[1]=position_b(RAMSEY_instance,0,RAMSEY_instance->PARAM_SIZE_GRAPH-i);


			RAMSEY_instance->status=CPXaddrows(RAMSEY_instance->env_MODEL_2,RAMSEY_instance->lp_MODEL_2,0,RAMSEY_instance->rcnt,RAMSEY_instance->nzcnt,RAMSEY_instance->rhs,RAMSEY_instance->sense,RAMSEY_instance->rmatbeg,RAMSEY_instance->rmatind,RAMSEY_instance->rmatval,NULL,NULL);
			if(RAMSEY_instance->status!=0)
			{
				printf("error in CPXaddrows\n");
				exit(-1);
			}

			free(RAMSEY_instance->rmatbeg);
			free(RAMSEY_instance->rmatval);
			free(RAMSEY_instance->rmatind);
			free(RAMSEY_instance->rhs);
			free(RAMSEY_instance->sense);
			///////////////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////////////////////

		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	if(RAMSEY_instance->PARAM_K_CUTS==1)
	{

		if(RAMSEY_instance->LOOK_UP_RAMSEY[RAMSEY_instance->PARAM_M-1][RAMSEY_instance->PARAM_N]!=-1)
		{

			cout << "\nLOOK_UP_RAMSEY BLUE\t" << RAMSEY_instance->LOOK_UP_RAMSEY[RAMSEY_instance->PARAM_M-1][RAMSEY_instance->PARAM_N] << endl <<endl;

			for(int i=0; i<RAMSEY_instance->PARAM_SIZE_GRAPH; i++)
			{

				///////////////////////////////////////////////////////////////////////////////////////////////////////
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				RAMSEY_instance->rcnt=1;
				RAMSEY_instance->nzcnt=RAMSEY_instance->PARAM_SIZE_GRAPH;
				RAMSEY_instance->rhs=(double*) calloc(RAMSEY_instance->rcnt,sizeof(double));
				RAMSEY_instance->sense=(char*) calloc(RAMSEY_instance->rcnt,sizeof(double));

				RAMSEY_instance->rhs[0]=0.0;
				RAMSEY_instance->sense[0]='L';


				RAMSEY_instance->rmatbeg=(int*) calloc(RAMSEY_instance->rcnt,sizeof(int));
				RAMSEY_instance->rmatind=(int*) calloc(RAMSEY_instance->nzcnt,sizeof(int));
				RAMSEY_instance->rmatval=(double*) calloc(RAMSEY_instance->nzcnt,sizeof(double));

				int counter_local=0;
				for(int j=0; j<RAMSEY_instance->PARAM_SIZE_GRAPH; j++)
				{
					if(i==j){continue;}
					RAMSEY_instance->rmatval[counter_local]=1.0;
					RAMSEY_instance->rmatind[counter_local++]=position_b(RAMSEY_instance,min(i,j),max(i,j));
				}


				int coeff=min(RAMSEY_instance->LOOK_UP_RAMSEY[RAMSEY_instance->PARAM_M-1][RAMSEY_instance->PARAM_N],RAMSEY_instance->PARAM_SIZE_GRAPH);

				RAMSEY_instance->rhs[0]=(coeff-1);

				RAMSEY_instance->status=CPXaddrows(RAMSEY_instance->env_MODEL_2,RAMSEY_instance->lp_MODEL_2,0,RAMSEY_instance->rcnt,RAMSEY_instance->nzcnt,RAMSEY_instance->rhs,RAMSEY_instance->sense,RAMSEY_instance->rmatbeg,RAMSEY_instance->rmatind,RAMSEY_instance->rmatval,NULL,NULL);
				if(RAMSEY_instance->status!=0)
				{
					printf("error in CPXaddrows\n");
					exit(-1);
				}

				free(RAMSEY_instance->rmatbeg);
				free(RAMSEY_instance->rmatval);
				free(RAMSEY_instance->rmatind);
				free(RAMSEY_instance->rhs);
				free(RAMSEY_instance->sense);
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				///////////////////////////////////////////////////////////////////////////////////////////////////////




			}
		}

		if(RAMSEY_instance->LOOK_UP_RAMSEY[RAMSEY_instance->PARAM_M][RAMSEY_instance->PARAM_N-1]!=-1)
		{

			cout << "\nLOOK_UP_RAMSEY RED\t" << RAMSEY_instance->LOOK_UP_RAMSEY[RAMSEY_instance->PARAM_M][RAMSEY_instance->PARAM_N-1] << endl <<endl;

			for(int i=0; i<RAMSEY_instance->PARAM_SIZE_GRAPH; i++)
			{

				///////////////////////////////////////////////////////////////////////////////////////////////////////
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				RAMSEY_instance->rcnt=1;
				RAMSEY_instance->nzcnt=RAMSEY_instance->PARAM_SIZE_GRAPH;
				RAMSEY_instance->rhs=(double*) calloc(RAMSEY_instance->rcnt,sizeof(double));
				RAMSEY_instance->sense=(char*) calloc(RAMSEY_instance->rcnt,sizeof(double));

				RAMSEY_instance->rhs[0]=0.0;
				RAMSEY_instance->sense[0]='L';


				RAMSEY_instance->rmatbeg=(int*) calloc(RAMSEY_instance->rcnt,sizeof(int));
				RAMSEY_instance->rmatind=(int*) calloc(RAMSEY_instance->nzcnt,sizeof(int));
				RAMSEY_instance->rmatval=(double*) calloc(RAMSEY_instance->nzcnt,sizeof(double));

				int counter_local=0;
				for(int j=0; j<RAMSEY_instance->PARAM_SIZE_GRAPH; j++)
				{
					if(i==j){continue;}
					RAMSEY_instance->rmatval[counter_local]=1.0;
					RAMSEY_instance->rmatind[counter_local++]=position_r(RAMSEY_instance,min(i,j),max(i,j));
				}

				int coeff=min(RAMSEY_instance->LOOK_UP_RAMSEY[RAMSEY_instance->PARAM_M][RAMSEY_instance->PARAM_N-1],RAMSEY_instance->PARAM_SIZE_GRAPH);

				RAMSEY_instance->rhs[0]=(coeff-1);


				RAMSEY_instance->status=CPXaddrows(RAMSEY_instance->env_MODEL_2,RAMSEY_instance->lp_MODEL_2,0,RAMSEY_instance->rcnt,RAMSEY_instance->nzcnt,RAMSEY_instance->rhs,RAMSEY_instance->sense,RAMSEY_instance->rmatbeg,RAMSEY_instance->rmatind,RAMSEY_instance->rmatval,NULL,NULL);
				if(RAMSEY_instance->status!=0)
				{
					printf("error in CPXaddrows\n");
					exit(-1);
				}

				free(RAMSEY_instance->rmatbeg);
				free(RAMSEY_instance->rmatval);
				free(RAMSEY_instance->rmatind);
				free(RAMSEY_instance->rhs);
				free(RAMSEY_instance->sense);
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				///////////////////////////////////////////////////////////////////////////////////////////////////////




			}
		}
	}



#ifdef	PRINT_MODEL_2_LP
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// * writing the created ILP model on a file *
	RAMSEY_instance->status=CPXwriteprob(RAMSEY_instance->env_MODEL_2,RAMSEY_instance->lp_MODEL_2,"RAMSEY_MODEL_2.lp",NULL);
	if(RAMSEY_instance->status!=0)
	{
		printf("error in CPXwriteprob\n");
		exit(-1);
	}
	cout << "INITIAL MASTER WRITTEN\n";
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif


}
