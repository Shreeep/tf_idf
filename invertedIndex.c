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
#include "InvertedIndexBST.h"
#include "FileList.h"
#include "TfIdfSumList.h"


#define MAX_WORD_LENGTH 100

//////////////////////////////////////////////////////////////////

char *normaliseWord(char *str);
InvertedIndexBST generateInvertedIndex(char *collectionFilename);
void printInvertedIndex(InvertedIndexBST tree);
TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D);
TfIdfList retrieve(InvertedIndexBST tree, char *searchWords[], int D);

//////////////////////////////////////////////////////////////////

char *normaliseWord(char *str) {

    //move pointer to first non-whitespaace
    int leadSpace = 0;
    while (isspace(str[leadSpace]) != 0) {
        leadSpace++;
        str++;
    }

    //terminate the string on the first trailing space
    for (int j = 0; j < (strlen(str) - leadSpace); j++) {
        if (isspace(str[j]) == 0 && isspace(str[j + 1]) != 0) {
            //str[j+1] is the first trail space
           str[j + 1] = '\0';
        }
    }

    //convert all characters to lowercase
    for (int k = 0; k < strlen(str); k++) {
        str[k] = tolower(str[k]);
    }

    //remove  . , ; ?  at the end of a word (only last instance)
    //e.g. dumb??? -> dumb?? when normalised

    if (str[strlen(str) - 1] == '.' || str[strlen(str) - 1] == ','
        || str[strlen(str) - 1] == ';' || str[strlen(str) - 1] == '?') {
            str[strlen(str)-1] = '\0';
        }

    return str;

}

InvertedIndexBST generateInvertedIndex(char *collectionFilename) {
    //create a tree
    InvertedIndexBST T = newInvertedIndexTree();
    FILE *collection = fopen(collectionFilename, "r");
	if (collection == NULL) {
		err(EX_OSERR, "collection file failed to open");
	}
    char fileName[MAX_WORD_LENGTH + 1];
	if (fileName == NULL) {
		err(EX_OSERR, "file name could not be allocated");
	}

    //store the file name in fileName
    while (fscanf(collection, "%s", fileName) != EOF) {
        //check if i need another variable for normaliseWord
        FILE *textFile = fopen(fileName, "r");
		if (textFile == NULL) {
			err(EX_OSERR, "text file failed to open");
		}
        char scannedWord[MAX_WORD_LENGTH + 1];
        //scannedWord = (char*) malloc((MAX_WORD_LENGTH + 1) * sizeof(char));


        while (fscanf(textFile, "%s", scannedWord) != EOF) {
            normaliseWord(scannedWord);
            T = insertInvertedIndexNode(T, scannedWord, collectionFilename);
        }
        //free(scannedWord);
        fclose(textFile);
    }
    //free(fileName);
    fclose(collection);
	return T;
}

void printInvertedIndex(InvertedIndexBST tree) {

	FILE *fileOut = fopen("invertedIndex.txt", "w");
	if (fileOut == NULL) {
		err(EX_OSERR, "couldn't allocate file output pointer");
	}
    printInvertedIndexInOrder(tree, fileOut);
	fclose(fileOut);
}

TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord, int D) {
    //create list
    TfIdfRep TfIdfListHead = newTfIdfList();
    //search for word in tree
    InvertedIndexBST InvertedIndexNode = searchInvertedIndexBST(tree, searchWord);
    FileList file = InvertedIndexNode->fileList;
    int numFilesContaingWord = fileListSize(file);
    double Idf = log10((double)D / (double)numFilesContaingWord);

    while (file != NULL) {
        insertTfIdfInOrder(TfIdfListHead, Idf, file);
        file = file->next;
    }
    

    return TfIdfListHead->first;
}


TfIdfList retrieve(InvertedIndexBST tree, char *searchWords[], int D) {
    TfIdfRep TfIdfSumList = newTfIdfList();
    int i = 0;
    bool isWordInFile;
    while (searchWords[i] != NULL) {
        TfIdfList wordTfIdfList = calculateTfIdf(tree, searchWords[i], D);
        while (wordTfIdfList != NULL) {            
            isWordInFile = searchTfIdf(TfIdfSumList->first, wordTfIdfList->filename);
            if (!isWordInFile) {
                insertSumTfIdfInOrder(TfIdfSumList, wordTfIdfList);
            } else {
                updateTfIdfSum(TfIdfSumList, wordTfIdfList);
            }
            wordTfIdfList = wordTfIdfList->next;
        } 
        i++;
    }
    return TfIdfSumList->first;
}


