#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <ctype.h>
#include <math.h>

//the provided API file
#include "invertedIndex.h"
#include "TfIdfList.h"

TfIdfRep newTfIdfList(void) {
	TfIdfRep L = malloc(sizeof(*L));
	if (L == NULL) err(EX_OSERR, "couldn't allocate newFileList");
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}
/*
        int cmp = key.compareTo(x.key);
        if      (cmp < 0) return get(x.left, key);
        else if (cmp > 0) return get(x.right, key);
        else              return x.val;
*/
InvertedIndexBST searchInvertedIndexBST(InvertedIndexBST tree, char *word) {
    if (tree == NULL) {
        return NULL;
    } else if (strcmp(word, tree->word) < 0) {
        return searchInvertedIndexBST(tree->left, word);
    } else if (strcmp(word, tree->word) > 0) {
        return searchInvertedIndexBST(tree->right, word);
    } else {
        return tree;
    }
}

int fileListSize(FileList fileListNode) {
    int fileCount = 0;
    while (fileListNode != NULL) {
        fileCount++;
        fileListNode = fileListNode->next;
    }
    return fileCount;
}

void insertTfIdfInOrder(TfIdfRep L, double Idf, FileList fileNode) {
    
    assert(L != NULL);
    TfIdfList n = newTfIdfNode(fileNode, Idf);
   
    if (L->first == NULL) {
        L->first = L->last = n;

    //if inserting new head
    } else if (n->tfIdfSum > L->first->tfIdfSum) {
        n->next = L->first;
        L->first= n;


    } else if (n->tfIdfSum == L->first->tfIdfSum && strcmp(n->filename, L->first->filename) < 0) {
        n->next = L->first;
        L->first= n;

    //if inserting new tail
    } else if (n->tfIdfSum < L->last->tfIdfSum) {
        L->last->next = n;
        L->last = n;


    } else if (n->tfIdfSum == L->last->tfIdfSum && strcmp(n->filename, L->first->filename) > 0) {
        L->last->next = n;
        L->last = n;

    //if inserting new node in the middle
    } else {

        TfIdfList curr = L->first;
        while (curr->next != NULL) {
            TfIdfList temp = curr;
            curr = curr->next;
                
            if (n->tfIdfSum > curr->tfIdfSum) {
                n->next = curr;
                temp->next = n;
                break;
            } else if (n->tfIdfSum == curr->tfIdfSum && strcmp(n->filename, curr->filename) < 0) {
                n->next = curr;
                temp->next = n;
                break;
            }
        
        }
        
    }
    L->size++;
}

TfIdfList newTfIdfNode(FileList fileNode, double Idf) {
    TfIdfList n = malloc(sizeof(*n));
    if (n == NULL) {
        err(EX_OSERR, "couldn't allocate TfIdf node");
    }
    n->filename = malloc(sizeof(char) * (strlen(fileNode->filename) + 1));
    strcpy(n->filename, fileNode->filename);
    n->tfIdfSum = (double)fileNode->tf * Idf;
    n->next = NULL;
    return n;
}