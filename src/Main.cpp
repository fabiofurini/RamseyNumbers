#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <float.h>

using namespace std;

#include "global_variables.h"
#include "global_functions.h"

#include "RAMSEY_MODEL_1.h"
#include "RAMSEY_MODEL_2.h"



/***********************************************************************************/
int main(int argc, char** argv)
/***********************************************************************************/
{

	data RAMSEY_instance;

	//////////////////////////////////////////////////////////////////////////

	if (argc == 13)
	{
		RAMSEY_instance.PARAM_SIZE_GRAPH=atoi(argv[1]);
		RAMSEY_instance.PARAM_M=atoi(argv[2]);
		RAMSEY_instance.PARAM_N=atoi(argv[3]);
		RAMSEY_instance.PARAM_ALGO=atoi(argv[4]);
		RAMSEY_instance.PARAM_OPTIONS=atoi(argv[5]);
		RAMSEY_instance.PARAM_CIRCULANT=atoi(argv[6]);
		RAMSEY_instance.PARAM_TIME_LIMIT=atof(argv[7]);
		RAMSEY_instance.PARAM_STRONGER_CUTS=atoi(argv[8]);
		RAMSEY_instance.PARAM_CPLEX=atoi(argv[9]);
		RAMSEY_instance.PARAM_AMTS=atoi(argv[10]);
		RAMSEY_instance.PARAM_TOUT_AMTS=atof(argv[11]);
		RAMSEY_instance.PARAM_K_CUTS=atoi(argv[12]);
	}
	else
	{
		cout << "**WRONG INPUT**\n\n";
		exit(-1);
	}

	cout << "\n**************************************\n\n";
	cout << "PARAM_SIZE_GRAPH->\t" <<RAMSEY_instance.PARAM_SIZE_GRAPH << endl;
	cout << "PARAM_M->\t" <<RAMSEY_instance.PARAM_M << endl;
	cout << "PARAM_N->\t" <<RAMSEY_instance.PARAM_N << endl;
	cout << "PARAM_ALGO->\t" <<RAMSEY_instance.PARAM_ALGO << endl;
	cout << "PARAM_OPTIONS->\t" <<RAMSEY_instance.PARAM_OPTIONS << endl;
	cout << "PARAM_CIRCULANT->\t" <<RAMSEY_instance.PARAM_CIRCULANT << endl;
	cout << "PARAM_TIME_LIMIT->\t" <<RAMSEY_instance.PARAM_TIME_LIMIT << endl;
	cout << "PARAM_STRONGER_CUTS->\t" <<RAMSEY_instance.PARAM_STRONGER_CUTS << endl;
	cout << "PARAM_CPLEX->\t" <<RAMSEY_instance.PARAM_CPLEX << endl;
	cout << "PARAM_AMTS->\t" <<RAMSEY_instance.PARAM_AMTS << endl;
	cout << "PARAM_TOUT_AMTS->\t" <<RAMSEY_instance.PARAM_TOUT_AMTS << endl;
	cout << "PARAM_K_CUTS->\t" <<RAMSEY_instance.PARAM_K_CUTS << endl;
	cout << "\n**************************************\n";
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////
	memory_allocation(&RAMSEY_instance);
	/////////////////////////////////////

	/////////////////////////////////////
	FILL_RAMSEY_LOOK_UP(&RAMSEY_instance);
	/////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//Logger::SetInformationLevel(LOGGER_ERROR);		/* highest level of logs, minimizes output-PSS */
	Logger::SetInformationLevel(LOGGER_INFO);

	RAMSEY_instance.initialize_ug(RAMSEY_instance.PARAM_SIZE_GRAPH);

	for (int i = 0; i < RAMSEY_instance.PARAM_SIZE_GRAPH; i++)
	{
		for (int j = 0; j < RAMSEY_instance.PARAM_SIZE_GRAPH; j++)
		{
			RAMSEY_instance.edge_fixing[i][j]=1;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////



	if(RAMSEY_instance.PARAM_ALGO==1)
	{

		cout << "\n\n****RAMSEY BRANCH-AND-CUT ALGORITHM 1****\n";

		RAMSEY_MODEL_1_load(&RAMSEY_instance);

		RAMSEY_MODEL_1_solve(&RAMSEY_instance);

		RAMSEY_MODEL_1_free(&RAMSEY_instance);

	}

	if(RAMSEY_instance.PARAM_ALGO==2)
	{

		cout << "\n\n****RAMSEY BRANCH-AND-CUT ALGORITHM 2****\n";

		RAMSEY_MODEL_2_load(&RAMSEY_instance);

		RAMSEY_MODEL_2_solve(&RAMSEY_instance);

		RAMSEY_MODEL_2_free(&RAMSEY_instance);

	}


	/////////////////////////////////////
	memory_deallocation(&RAMSEY_instance);
	/////////////////////////////////////


	printf("\nDONE!\n\n");

	return 1;
}



