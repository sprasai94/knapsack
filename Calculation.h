#ifndef Calculation_h
#define Calculation_h
#include<vector>
using namespace std;
void HostMemoryAllocation(int* &value,int* &weight,int* &matrix,int numofitem,int capacity);
void DeviceMemoryAllocation(int* &value,int* &weight,int* &matrix,int numofitem,int capacity);
void InitializeInput(int* &V,int* &W,int NUM_ITEMS,int &capacity,int VRange,int WRange);
void HostToDeviceCopy(int *h_Value,int *h_Weight,int *h_Matrix,int *d_Value,int *d_Weight,int *d_Matrix,int NumofItems,int Capacity);
void DeviceToHostCopy(int *h_Matrix,int *d_Matrix,int NumofItems,int Capacity);
int SerialCalculation(int *h_Value,int *h_Weight,int* h_Matrix,vector<int>&Selectionindex,int NumofItems,int Capacity);
void DisplayPair(int *h_Value,int *h_Weight,int *h_Matrix,vector<int>SelectionIndex);

#endif
