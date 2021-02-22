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
#include "FileList.h"

#define MAX_WORD_LENGTH 100

//generates a File List
FileList generateFileList(char *word, char *collectionName) {
    //given a word and a collection of files, generate a linked list which contains the tf
    ListRep L = newFileList();
    //1. check if word is in a file in colletionName
    FILE *listCollection = fopen(collectionName, "r");
	if (listCollection == NULL) {
		err(EX_OSERR, "list collection file failed to open");
	}
    char listFileName[MAX_WORD_LENGTH + 1];
    //listFileName = malloc((MAX_WORD_LENGTH + 1) * sizeof(char));


    while (fscanf(listCollection, "%s", listFileName) != EOF) {

        int wordFreq = 0;
        int wordTotal = 0;

        FILE *listTextFile = fopen(listFileName, "r");
		if (listTextFile == NULL) {
			err(EX_OSERR, "list text file failed to open");
		}
        char listScannedWord[MAX_WORD_LENGTH + 1];
        //listScannedWord = malloc((MAX_WORD_LENGTH + 1) * sizeof(char));

        while (fscanf(listTextFile, "%s", listScannedWord) != EOF) {
            normaliseWord(listScannedWord);
            if (strcmp(listScannedWord, word) == 0) {
                wordFreq++;
            }
            wordTotal++;
        }
        if (wordFreq > 0) {
            double tf = (double)wordFreq / (double)wordTotal;
            insertFileListNodeInOrder(L, listFileName, tf);
        }


        //free(listScannedWord);
        fclose(listTextFile);

    }
    //free(listFileName);
    fclose(listCollection);

    return L->first;
}

//creates a new, empty FileList list
ListRep newFileList(void) {
	ListRep L = malloc(sizeof(*L));
	if (L == NULL) err(EX_OSERR, "couldn't allocate newFileList");
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

//insert a node in FileList
void insertFileListNodeInOrder(ListRep L, char *listFileName, double tf) {
    assert(L != NULL);

    FileList n = newFileListNode(listFileName, tf);
    if (L->first == NULL) {
        L->first = L->last = n;
        

    //if new head
    } else if (strcmp(listFileName, L->first->filename) < 0) {
        n->next = L->first;
        L->first = n;
        
    //if new tail
    } else if (strcmp(listFileName, L->last->filename) > 0) {
        L->last->next = n;
        L->last = n;
        

    } else {
        FileList curr = L->first; 
        while (curr != NULL) {
            FileList temp = curr;
            curr = curr->next;
            if (strcmp(listFileName, temp->filename) >= 0) {
                temp->next = n;
                n->next = curr;
                break;
            }
        }
        
    }
    L->size++;
}

//creates a new FileList node
FileList newFileListNode(char *listFileName, double tf) {
    FileList n = malloc(sizeof(*n));
    if (n == NULL) {
        err(EX_OSERR, "couldn't allocate FileList node");
    }
    n->filename = malloc(sizeof(char) * (strlen(listFileName) + 1));
    strcpy(n->filename, listFileName);
    n->tf = tf;
    n->next = NULL;
    return n;
}
