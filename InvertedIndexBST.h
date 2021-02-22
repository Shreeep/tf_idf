#include "invertedIndex.h"

//creates a new, empty inverted index BST
InvertedIndexBST newInvertedIndexTree(void);

//inserts a node into the inverted index BST
InvertedIndexBST insertInvertedIndexNode(InvertedIndexBST tree, char *word, char *collectionName);

//creates and allocates memory for a new node in the inverted index BST
InvertedIndexBST newInvertedIndexNode(char *word, char *collectionName);

//frees memory used by the BST
void freeBST(InvertedIndexBST tree);

//prints an individual node of the inverted index BST
void printInvertedIndexTreeNode(InvertedIndexBST tree, FILE *outputFile);

//prints the InvertedIndexBST using an InOrder search
void printInvertedIndexInOrder(InvertedIndexBST tree, FILE *outputFile);