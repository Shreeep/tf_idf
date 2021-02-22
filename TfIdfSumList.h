#include "invertedIndex.h"
#include "TfIdfList.h"

//returns true if a file exists within a fileList, false otherwise
bool searchTfIdf(TfIdfList fileList, char *fileName);

//inserts a SumTfIdf node in sorted order
// follows descending TfIdfSum and then ascening fileName
void insertSumTfIdfInOrder(TfIdfRep L, TfIdfList List);

//after a TfIdf sum has been updated, reinsert this node into the TfIdfList
void insertUpdatedSum(TfIdfRep sumList, double oldSum, TfIdfList List);

//creates and allocates memory for a new SumTfIdf node
TfIdfList newSumTfIdfNode(TfIdfList List);

//updates the value of an existing node in the TfIdfList
void updateTfIdfSum(TfIdfRep L, TfIdfList List);

//node which has been updated, to be reinserted into the TfIdfList
TfIdfList updatedTfIdfNode(TfIdfList sumFile, double oldTfIdf);

//removes and returns a TfIdf node from a TfIdfList
double popTfIdf(TfIdfRep sumList, char *fileName);