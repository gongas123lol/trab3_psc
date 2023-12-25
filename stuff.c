#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>


void dictionary_add(Dictionary *dictionary, const char *filename) {
	
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Read the file line by line
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
		
		const key = sumofASCII(line);
		// talvez implementar uma hashtable
		
    }
    fclose(file);
}

int sumofASCII(const char *line) {
    int sum = 0;
    const char *curr = line;

    while (*curr != '\0') {
        sum += *curr;
        curr++;
    }

    return sum;
}
//----------------------------------------------------------------
typedef struct Dictionary {
    GHashTable *hashTable;
} Dictionary;

Dictionary *dictionary_create() {
    Dictionary *dictionary = (Dictionary *)malloc(sizeof(Dictionary));
    if (dictionary != NULL) {
        dictionary->hashTable = g_hash_table_new(g_str_hash, g_str_equal);
    }
    return dictionary;
}

// Add words from a file to the dictionary
void dictionary_add(Dictionary *dictionary, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char word[100]; //100 caracteres deve ser suficiente
	

    while (fscanf(file, "%s", word) == 1) {
        // Use the word as both the key and the value in the hashtable
        g_hash_table_insert(dictionary->hashTable, strdup(word), strdup(word));
    }

    fclose(file);
}

int dictionary_lookup(Dictionary *dictionary, const char *word) {
    return g_hash_table_contains(dictionary->hashTable, word);
}

// Destroy the dictionary and free associated resources
void dictionary_destroy(Dictionary *dictionary) {
    g_hash_table_destroy(dictionary->hashTable);
    free(dictionary);
}


