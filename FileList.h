#include "invertedIndex.h"

//struct for maniuplating and storing nodes in File List
struct FileListRep {
	int size;                  // number of elements in the list
	FileList first; // pointer to node containing first value
	FileList last;  // pointer to node containing last value
};
typedef struct FileListRep *ListRep;

//generates a filelist, given a word and collection file
FileList generateFileList(char *word, char *collectionName);

//creates a new, empty file list
ListRep newFileList(void);

//inserts a key/value pair node into a Filelist
void insertFileListNodeInOrder(ListRep L,char *listFileName, double tf);

//creates and allocates memory for a new File List node
FileList newFileListNode(char *listFileName, double tf);
