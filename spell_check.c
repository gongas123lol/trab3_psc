#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

// Google said:
//Qual é a palavra mais longa da língua portuguesa? A palavra mais extensa da língua portuguesa é "pneumoultramicroscopicossilicovulcanoconiótico" com 46 letras.
// sendo 46, toleramos comprimento máximo de 55 carateres.
#define MAX_WORD_LENGTH 55
const char punctuation[] = ",.;-:— «?»";


// Define the hashtable structure
typedef struct {
    GHashTable *hashTable;
} Dictionary;

// Function to create and initialize a dictionary
Dictionary *dictionary_create() {
    Dictionary *dictionary = (Dictionary *)malloc(sizeof(Dictionary));
    if (dictionary != NULL) {
        dictionary->hashTable = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
    }
    return dictionary;
}

// Function to add words from a file to the dictionary
void dictionary_add(Dictionary *dictionary, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char word[MAX_WORD_LENGTH];

    while (fscanf(file, "%s", word) == 1) {
        // Convert the word to lowercase
        for (size_t i = 0; i < strlen(word); i++) {
            word[i] = tolower(word[i]);
        }
        g_hash_table_insert(dictionary->hashTable, strdup(word), GINT_TO_POINTER(1));
       // printf("added: %s\n", word);
    }

    fclose(file);
}

// Function to check the spelling of a text
void spell_check(Dictionary *dictionary, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char word[MAX_WORD_LENGTH];
    int linha = 1, coluna = 1;

    while (fscanf(file, "%s", word) == 1) {
				
		//iterate through the word, from the beggining, to see if there is any punctuation.
		
		char *start = word;
		while (*start != '\0' && strchr(",.;-:— «?»", *start) != NULL) {
			start++;
		}

		// lets move the "start" pointer AKA  the REAL beginning of the word, to the word memory position.
		if (start > word) {
			memmove(word, start, strlen(start) + 1);
		}

		//iterate through the end of the word to check if there is more punctuation. ex: "assério!!!"
		size_t len = strlen(word);
		while (len > 0 && strchr(",.;-:— «?»", word[len - 1]) != NULL) {
			word[--len] = '\0';
		}

        // the word must be in lowercase in order to sucessfully check.
        for (size_t i = 0; i < len; i++) {
            word[i] = tolower(word[i]);
        }
		
		//sometimes punctuation can be missinterpreted as word, and when its corrected by the code above, it becomes "".
		if(strlen(word) < 1){
			continue;
		}
		
        //if not found, it will print the coordinates of that word.
        if (!g_hash_table_contains(dictionary->hashTable, word)) {
            printf("word not found: |%s| in Line: %d, Column: %d\n", word, linha, coluna);
        }

        coluna += len + 1;

        if (strchr(word, '\n') != NULL) {
            linha++;
            coluna = 1;
        }
    }

    fclose(file);
}

// Function to free the memory used by the dictionary
void dictionary_destroy(Dictionary *dictionary) {
    g_hash_table_destroy(dictionary->hashTable);
    free(dictionary);
}

int main(int argc, char *argv[]) {
    const char *texto = NULL;
    Dictionary *dicionario = dictionary_create();

    //check for the arguments given by the user, when the function is called.
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            texto = argv[++i];
        } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
    // add the words to the empty dictionary.
            dictionary_add(dicionario, argv[++i]);
        } else {
            printf("Invalid option: %s\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    //Check if both -t and -d options are provided.
    if (texto != NULL && dicionario->hashTable == NULL){
		printf("You must provide the -t option (file to check). /n");
		
	} 
	if(texto == NULL && dicionario->hashTable != NULL) {
        printf("You must provide the -d option (dictionary file).\n");
        return EXIT_FAILURE;
    }

    if (texto != NULL) {
        spell_check(dicionario, texto);
    }

    //free out the memory used by the dictionary.
    dictionary_destroy(dicionario);

    return EXIT_SUCCESS;
}
