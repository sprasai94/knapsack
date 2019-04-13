#include "Ks.cuh"
#include <iostream>
#include<stdio.h>
#include<algorithm>
using namespace std;
unsigned int iDivUp(unsigned int a, unsigned int b)
{
	return (a % b != 0) ? (a / b + 1) : (a / b);	
}
class CThreadScaler
{
private:
	int Dg;
	int Db;
public:
	CThreadScaler(int NumThreads)
	{
		Db = min( 512, NumThreads);
		if(Db > 0)
		{
			Dg = iDivUp(NumThreads, Db);
		}else
		{
			Dg = 0;
		}
	}
	int Grids()
	{
		return Dg;
	}
	int Blocks()
	{
		return Db;
	}
};
static __inline__ __device__ int CudaGetTargetID()
{
	return blockDim.x * blockIdx.x + threadIdx.x;
}
__global__ void knapsackKernel(int *V,int *W,int *M,int Capacity,int i)
{
	int w = CudaGetTargetID();
	if (w > Capacity)
		return;
	if (i%2!=0)
	{
		if (W[i] <= w)
			M[Capacity+w] = max(V[i] + M[w-W[i]],M[w] );
        else
             M[Capacity+w] = M[w];
    }
	else
	{
		if (W[i] <= w)
			M[w] = max(V[i] + M[Capacity + (w-W[i])],M[Capacity + w]);
        else
            M[w] = M[Capacity+w];
	}
	
	__syncthreads();
}

void CudaFunctionCall(int *Value,int *Weight,int *Matrix,int NumofItems,int Capacity)
{
	CudaSafeCall(cudaSetDevice(0));
	CThreadScaler TS(Capacity+1);
	int i=0;
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start);
	while (i <NumofItems) 
	{
		knapsackKernel<<<TS.Grids(), TS.Blocks()>>>(Value, Weight, Matrix,Capacity,i);
		CudaSafeCall(cudaDeviceSynchronize());
		i++;
	}
	cudaEventRecord(stop);
	CudaSafeCall(cudaGetLastError());
	cudaEventSynchronize(stop);
	float milliseconds = 0;
	cudaEventElapsedTime(&milliseconds, start, stop);
	printf("Parallel Time in ms:%f\n",milliseconds);
	
	
}
