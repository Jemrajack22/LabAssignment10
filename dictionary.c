#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{	
    struct Trie *children[26];
    int isEndOfWord;
};

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct Trie *node = pTrie;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!node->children[index]) {
            node->children[index] = (struct Trie *)malloc(sizeof(struct Trie));
            for (int j = 0; j < 26; j++) {
                node->children[index]->children[j] = NULL;
            }
            node->children[index]->isEndOfWord = 0;
        }
        node = node->children[index];
    }
    node->isEndOfWord = 1;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct Trie *node = pTrie;
    int count = 0;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!node->children[index]) {
            return 0; // Word not found in the trie
        }
        node = node->children[index];
    }
    if (node->isEndOfWord) {
        count++; // Increment count if the node represents the end of a word
    }
    return count;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
     if (pTrie) {
        for (int i = 0; i < 26; i++) {
            deallocateTrie(pTrie->children[i]);
        }
        free(pTrie);
    }
    return NULL;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    if (pTrie) {
        for (int i = 0; i < 26; i++) {
            pTrie->children[i] = NULL;
        }
        pTrie->isEndOfWord = 0;
    }
    return pTrie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        return 0;
    }
    
    int numWords = 0;
    char word[100];
    while (fscanf(file, "%s", word) != EOF) {
        // Allocate memory for the word and copy it into the array
        pInWords[numWords] = (char *)malloc(strlen(word) + 1); // +1 for null terminator
        strcpy(pInWords[numWords], word);
        numWords++;
    }
    
    fclose(file);
    return numWords;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}