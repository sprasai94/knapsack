#include<iostream>
#include <ctime>
#include<cstdlib>
#include<vector>
#include "Calculation.h"
#include "Ks.cuh"
#include<time.h>
using namespace std;

int main(int argc, char **argv)
{
    const int NumofItems = 100000;
    int Capacity=10000000;
    const int V_Range = 100;
    const int W_Range = 100;
    int *h_Value,*d_Value ;
    int *h_Weight,*d_Weight;
    int *h_Matrix,*d_Matrix;
    int maxsum =0;
    vector<int>SelectionIndex;
    srand(time(NULL));
	cout<<"Start of Program:"<<endl;
	HostMemoryAllocation(h_Value,h_Weight,h_Matrix,NumofItems,Capacity);
	InitializeInput(h_Value,h_Weight,NumofItems,Capacity,V_Range,W_Range);
	cout<<"Capacity:"<<Capacity<<endl;
	DeviceMemoryAllocation(d_Value,d_Weight,d_Matrix,NumofItems,Capacity);
	HostToDeviceCopy(h_Value,h_Weight,h_Matrix,d_Value,d_Weight,d_Matrix,NumofItems,Capacity);
	//h_Matrix = new int[(NumofItems+1)*(Capacity+1)];
	clock_t start = clock(); 
    //maxsum = SerialCalculation(h_Value,h_Weight,h_Matrix,SelectionIndex,NumofItems,Capacity);
	clock_t stop = clock(); 
	double elapsed = (double)(stop - start) * 1000.0 / CLOCKS_PER_SEC;
    cout<<"Sum:"<<maxsum<<endl;
	cout<<"Execution time:"<<elapsed<<endl;
    //DisplayPair(h_Value,h_Weight,h_Matrix,SelectionIndex);

	CudaFunctionCall(d_Value,d_Weight,d_Matrix,NumofItems,Capacity);
	DeviceToHostCopy(h_Matrix,d_Matrix,NumofItems,Capacity);
	maxsum = (NumofItems % 2 != 0) ? h_Matrix[Capacity] : h_Matrix[Capacity +Capacity];
	cout<<"Parallel Result:"<<maxsum<<endl;;
	//cout<<"end of Program:"<<endl;
	return 0;
}
