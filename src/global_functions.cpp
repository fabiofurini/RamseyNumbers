

#include "global_functions.h"

/***********************************************************************************/
void FILL_RAMSEY_LOOK_UP(data *RAMSEY_instance)
/***********************************************************************************/
{

	RAMSEY_instance->LOOK_UP_RAMSEY=new int*[50];
	for(int i=0; i<50; i++)
	{
		RAMSEY_instance->LOOK_UP_RAMSEY[i]=new int[50];
	}

	for(int i=0; i<50; i++)
	{
		for(int j=0; j<50; j++)
		{
			RAMSEY_instance->LOOK_UP_RAMSEY[i][j]=-1;
		}
	}


	for(int i=1; i<50; i++)
	{
		RAMSEY_instance->LOOK_UP_RAMSEY[1][i]=1;
		RAMSEY_instance->LOOK_UP_RAMSEY[2][i]=i;
	}


	RAMSEY_instance->LOOK_UP_RAMSEY[3][3]=6;//opt
	RAMSEY_instance->LOOK_UP_RAMSEY[3][4]=9;//opt
	RAMSEY_instance->LOOK_UP_RAMSEY[3][5]=14;//opt
	RAMSEY_instance->LOOK_UP_RAMSEY[3][6]=18;//opt
	RAMSEY_instance->LOOK_UP_RAMSEY[3][7]=23;//opt
	RAMSEY_instance->LOOK_UP_RAMSEY[3][8]=28;//opt
	RAMSEY_instance->LOOK_UP_RAMSEY[3][9]=36;//opt

	RAMSEY_instance->LOOK_UP_RAMSEY[3][10]=42;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[3][11]=50;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[3][12]=59;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[3][13]=68;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[3][14]=77;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[3][15]=87;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[3][16]=97;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[3][17]=109;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[3][18]=120;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[3][19]=132;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[3][20]=145;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[3][21]=157;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[3][22]=171;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[3][23]=185;//UB

	RAMSEY_instance->LOOK_UP_RAMSEY[4][4]=18;//opt
	RAMSEY_instance->LOOK_UP_RAMSEY[4][5]=25;//opt
	RAMSEY_instance->LOOK_UP_RAMSEY[4][6]=41;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[4][7]=61;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[4][8]=84;//UB

	RAMSEY_instance->LOOK_UP_RAMSEY[4][9]=150;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[4][10]=149;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[4][11]=191;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[4][12]=238;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[4][13]=291;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[4][14]=349;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[4][15]=417;//UB


	RAMSEY_instance->LOOK_UP_RAMSEY[5][5]=48;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[5][6]=87;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[5][7]=143;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[5][8]=216;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[5][9]=316;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[5][10]=442;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[5][11]=633;//UB


	RAMSEY_instance->LOOK_UP_RAMSEY[6][6]=165;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[6][7]=298;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[6][8]=495;//UB
	RAMSEY_instance->LOOK_UP_RAMSEY[6][9]=780;//UB


	RAMSEY_instance->LOOK_UP_RAMSEY[7][7]=540;//UB


	for(int i=0; i<50; i++)
	{
		for(int j=i+1; j<50; j++)
		{
			RAMSEY_instance->LOOK_UP_RAMSEY[j][i]=RAMSEY_instance->LOOK_UP_RAMSEY[i][j];
		}
	}


	//		for(int i=1; i<11; i++)
	//		{
	//			for(int j=1; j<11; j++)
	//			{
	//				cout << RAMSEY_instance->LOOK_UP_RAMSEY[i][j] << "\t";
	//			}
	//			cout << endl;
	//		}
	//		cout << endl;


}

/***********************************************************************************/
void memory_allocation(data *RAMSEY_instance)
/***********************************************************************************/
{

	RAMSEY_instance->CLIQUE_SOL=new double[RAMSEY_instance->PARAM_SIZE_GRAPH];

	RAMSEY_instance->edge_fixing=new int*[RAMSEY_instance->PARAM_SIZE_GRAPH];

	for (int i = 0; i < RAMSEY_instance->PARAM_SIZE_GRAPH; i++)
	{
		RAMSEY_instance->edge_fixing[i]=new int[RAMSEY_instance->PARAM_SIZE_GRAPH];
	}


	RAMSEY_instance->CLIQUE_BLUE=new double[RAMSEY_instance->PARAM_SIZE_GRAPH];
	RAMSEY_instance->CLIQUE_RED=new double[RAMSEY_instance->PARAM_SIZE_GRAPH];

}

/***********************************************************************************/
void memory_deallocation(data *RAMSEY_instance)
/***********************************************************************************/
{

	for (int i = 0; i < RAMSEY_instance->PARAM_SIZE_GRAPH; i++)
	{
		delete [] RAMSEY_instance->edge_fixing[i];
	}
	delete [] 	RAMSEY_instance->edge_fixing;

	delete []RAMSEY_instance->CLIQUE_SOL;

	delete []RAMSEY_instance->CLIQUE_BLUE;
	delete []RAMSEY_instance->CLIQUE_RED;

}


/***********************************************************************************/
int position_r(data *RAMSEY_instance, int i,int j)
/***********************************************************************************/
{
	return  RAMSEY_instance->PARAM_SIZE_GRAPH*i+j;
}

/***********************************************************************************/
int position_b(data *RAMSEY_instance, int i, int j)
/***********************************************************************************/
{
	return RAMSEY_instance->PARAM_SIZE_GRAPH*RAMSEY_instance->PARAM_SIZE_GRAPH +RAMSEY_instance->PARAM_SIZE_GRAPH*i+j;
}

/***********************************************************************************/
int position_theta(data *RAMSEY_instance)
/***********************************************************************************/
{
	return 2*RAMSEY_instance->PARAM_SIZE_GRAPH*RAMSEY_instance->PARAM_SIZE_GRAPH;
}

/***********************************************************************************/
int magic_formula(int K,int l)
/***********************************************************************************/
{

	//K is the size of clique size, l-1 is the target clique size
	int m=l-1;
	int alpha=(K/m)+1;
	int nalpha_1=m*alpha-K;
	int nalpha=(K-((nalpha_1-1)*(alpha-1)))/alpha;

	//	cout << "alpha\t"<< alpha << endl;
	//	cout << "n(alpha-1)\t"<<  nalpha_1  << endl;
	//	cout << "n(alpha)\t"<<  nalpha  << endl;
	//	cout << "magic function value\t" <<  nalpha_1*(alpha-1)*(alpha-2)/2+nalpha*(alpha-1)*(alpha)/2 << endl;

	return nalpha_1*(alpha-1)*(alpha-2)/2+nalpha*(alpha-1)*(alpha)/2;

}
