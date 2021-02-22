#include "invertedIndex.h"

struct TfIdfListRep {
	int size;                  // number of elements in the list
	TfIdfList first; // pointer to node containing first value
	TfIdfList last;  // pointer to node containing last value
};
typedef struct TfIdfListRep *TfIdfRep;

//searches and returns a node in an inverted Index BST which matches a given key
InvertedIndexBST searchInvertedIndexBST(InvertedIndexBST tree, char *word);

//determines the size of the files, FileList
int fileListSize(FileList files);

//creates a new, empty TfIdf list
TfIdfRep newTfIdfList(void);

//inserts a node into the TfIdf list without duplicates
void insertTfIdfInOrder(TfIdfRep L, double Idf, FileList files);

//creates and allocates memory for a new TfIdf List node
TfIdfList newTfIdfNode(FileList fileNode, double Idf);
