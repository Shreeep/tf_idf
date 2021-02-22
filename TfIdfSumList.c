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
#include "TfIdfSumList.h"

bool searchTfIdf(TfIdfList TfIdfFiles, char *fileName) {
    if (TfIdfFiles == NULL) {
        return false;
    } else if (strcmp(TfIdfFiles->filename, fileName) == 0) {
        return true;
    } else {
        return searchTfIdf(TfIdfFiles->next, fileName);
    }
}

//deletes and returns the a TfIdf value from sumTfIdf list
double popTfIdf(TfIdfRep sumList, char *fileName) {
    //find node in sumList which matches
    double oldSum;
    //pop first node if it matches
    if (strcmp(sumList->first->filename, fileName) == 0) {
        oldSum = sumList->first->tfIdfSum;
        sumList->first = sumList->first->next;
    //pop any other node if it matches
    } else {
        TfIdfList curr = sumList->first;
        while (curr != NULL) {
            TfIdfList temp = curr;
            curr = curr->next;
            if (strcmp(curr->filename, sumList->last->filename) == 0) {
                oldSum = curr->tfIdfSum;                
                temp->next = curr->next;
                sumList->last = temp;
                break;
            } else if (strcmp(curr->filename, fileName) == 0) {
                oldSum = curr->tfIdfSum;                
                temp->next = curr->next;
                break;
            }
        }
    }
    sumList->size--;
    return oldSum;
}

void updateTfIdfSum(TfIdfRep sumList, TfIdfList List) {
    //find old tfIdf in sumList
    double oldSum = popTfIdf(sumList, List->filename);
    //delete old node
    insertUpdatedSum(sumList, oldSum, List);
}

void insertUpdatedSum(TfIdfRep L, double oldSum, TfIdfList List) {
    
    assert(L != NULL);
    TfIdfList n = updatedTfIdfNode(List, oldSum);
   
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


    } else if (n->tfIdfSum == L->last->tfIdfSum && strcmp(n->filename, L->last->filename) > 0) {
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

TfIdfList updatedTfIdfNode(TfIdfList sumFile, double oldTfIdf) {
    TfIdfList n = malloc(sizeof(*n));
    if (n == NULL) {
        err(EX_OSERR, "couldn't allocate TfIdf node");
    }
    n->filename = malloc(sizeof(char) * (strlen(sumFile->filename) + 1));
    strcpy(n->filename, sumFile->filename);
    n->tfIdfSum = sumFile->tfIdfSum + oldTfIdf;
    n->next = NULL;
    return n;
}




void insertSumTfIdfInOrder(TfIdfRep L, TfIdfList List) {

    assert(L != NULL);
    TfIdfList n = newSumTfIdfNode(List);
    
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


    } else if (n->tfIdfSum == L->last->tfIdfSum && strcmp(n->filename, L->last->filename) > 0) {
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

TfIdfList newSumTfIdfNode(TfIdfList List) {
    TfIdfList n = malloc(sizeof(*n));
    if (n == NULL) {
        err(EX_OSERR, "couldn't allocate TfIdf node");
    }
    n->filename = malloc(sizeof(char) * (strlen(List->filename) + 1));
    strcpy(n->filename, List->filename);
    n->tfIdfSum = List->tfIdfSum;
    n->next = NULL;
    return n;
}
