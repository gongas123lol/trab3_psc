#include <stdio.h>
#include <stdlib.h>

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
