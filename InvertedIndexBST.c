#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <ctype.h>
#include <math.h>

#include "invertedIndex.h"
#include "InvertedIndexBST.h"
#include "FileList.h"

//creates a new inverted index bst
InvertedIndexBST newInvertedIndexTree(void) {
    return NULL;
}

//inserts nodes in the inverted index bst
InvertedIndexBST insertInvertedIndexNode(InvertedIndexBST tree, char *word, char *collectionName) {
    if (tree == NULL) {
        return newInvertedIndexNode(word, collectionName);

    } else if (strcmp(word, tree->word) < 0) {
        tree->left = insertInvertedIndexNode(tree->left, word, collectionName);

    } else if (strcmp(word, tree->word) > 0) {
        tree->right = insertInvertedIndexNode(tree->right, word, collectionName);

    } else if (strcmp(word, tree->word) == 0) {
        //no duplicates so do nothing
    }
    return tree;
}

//creates new nodes in the inverted index bst
InvertedIndexBST newInvertedIndexNode(char *word, char *collectionName) {
    InvertedIndexBST new = malloc(sizeof(*new));
    if (new == NULL) {
        err(EX_OSERR, "couldn't allocate BST node");
    }

    new->word = malloc(sizeof(char) * (strlen(word) + 1));
    strcpy(new->word, word);
    new->fileList = generateFileList(word, collectionName);
    new->left = NULL;
    new->right = NULL;
    return new;
}

void freeBST(InvertedIndexBST tree) {
    if (tree == NULL) {
        return;
    }

    while (tree->fileList != NULL) {
        FileList temp = tree->fileList;
        tree->fileList = tree->fileList->next;
        free(temp);
    }

    freeBST(tree->left);
    freeBST(tree->right);
    free(tree);
}

void printInvertedIndexInOrder(InvertedIndexBST tree, FILE *outputFile) {
    if (tree == NULL) {
		return;
	}
    printInvertedIndexInOrder(tree->left, outputFile);
    printInvertedIndexTreeNode(tree, outputFile);
    printInvertedIndexInOrder(tree->right, outputFile);
}

void printInvertedIndexTreeNode(InvertedIndexBST tree, FILE *outputFile) {
	fprintf(outputFile, "%s ", tree->word);
	FileList listNode = tree->fileList;
	while (listNode != NULL) {
		fprintf(outputFile, "%s ", listNode->filename);
		fprintf(outputFile, "(%f) ", listNode->tf);
		listNode = listNode->next;
	}
	fprintf(outputFile, "\n");
}
