#include <time.h>
#include<cstdlib>
#include<algorithm>
#include<iostream>
#include"Calculation.h"
#include"Ks.cuh"
using namespace std;
void HostMemoryAllocation(int* &Value,int* &Weight,int* &Matrix,int NumofItems,int Capacity)
{
	Value = new int[NumofItems];
	Weight = new int[NumofItems];
	Matrix = new int[ 2 *(Capacity+1)];
}
void DeviceMemoryAllocation(int* &Value,int* &Weight,int* &Matrix,int NumofItems,int Capacity)
{
	CudaSafeCall(cudaMalloc((void**)&Value,NumofItems * sizeof(int)));
	CudaSafeCall(cudaMalloc((void**)&Weight,NumofItems * sizeof(int)));
	CudaSafeCall(cudaMalloc((void**)&Matrix,2 *(Capacity+1) * sizeof(int)));
}
void HostToDeviceCopy(int *h_Value,int *h_Weight,int *h_Matrix,int *d_Value,int *d_Weight,int*d_Matrix,int NumofItems,int Capacity)
{
	CudaSafeCall(cudaMemcpy(d_Value, h_Value, NumofItems * sizeof(int), cudaMemcpyHostToDevice));
	CudaSafeCall(cudaMemcpy(d_Weight, h_Weight, NumofItems * sizeof(int), cudaMemcpyHostToDevice));
	//CudaSafeCall(cudaMemcpy(d_Matrix, h_Matrix, (NumofItems+1)*(Capacity+1) * sizeof(int), cudaMemcpyHostToDevice));
}
void DeviceToHostCopy(int *h_Matrix,int *d_Matrix,int NumofItems,int Capacity)
{
	memset(h_Matrix, 0, 2 *(Capacity+1) * sizeof(int));
	CudaSafeCall(cudaMemcpy(h_Matrix, d_Matrix, 2 *(Capacity+1) * sizeof(int), cudaMemcpyDeviceToHost));
}
void InitializeInput(int* &V,int* &W,int NUM_ITEMS,int &Capacity,int V_Range,int W_Range)
{
	//Capacity = 0;
    for(int i=0;i<NUM_ITEMS;i++)
        {
            V[i]=(rand()%V_Range) + 1;
            W[i]=(rand()%W_Range) + 1;
			//Capacity+= W[i];
        }
	//Capacity= int(Capacity/3);
}
int SerialCalculation(int *V,int *W,int* M,vector<int>&Index,int NumofItems,int Capacity)
{
    memset(M, 0, sizeof(M));
	for(int i =0;i<NumofItems;i++)
    {
		if (i%2!=0)
        {
            for (int w=0;w <= Capacity;w++)
            {
                if (W[i] <= w)
                    M[Capacity+w] = max(V[i] + M[w-W[i]],M[w] );
                else
                    M[Capacity+w] = M[w];
            }
		}
        else
        {
            for (int w=1;w <= Capacity;w++)
            {
                if (W[i] <= w)
                    M[w] = max(V[i] + M[Capacity + (w-W[i])],M[Capacity + w]);
                else
                    M[w] = M[Capacity+w];
            }
        }
 }
    return (NumofItems % 2 != 0)? M[Capacity] : M[Capacity +Capacity];
}

void DisplayPair(int *Value,int *Weight,int *Matrix,vector<int>Index)
{
    cout<<"Serial Calculation Result:"<<endl;
    for(int i=0;i<Index.size();i++)
        cout<<"("<<Value[Index[i]]<<","<<Weight[Index[i]]<<"),";
    cout<<endl;

}


