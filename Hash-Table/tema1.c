/**
 * Tema 1 SO - Hash table
 *
 * Copyright (C) 2018, Oana-Bianca Bouleanu 333CA
 *	<oana.bouleanu@stud.acs.upb.ro>
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "utils.h"
#include "functions.h"

#define BUFFSIZE 20000

/*	Functia identifica(,) comanda primita si apeleaza functia
 *	corespunzatoare. Verifica, in prealabil, parametrii
 *	comenzilor si, ulterior, rezultatul returnat
 *	de functii si returneaza erori, in caz ca au survenit
 *	probleme in executia functiilor.
 */
void interpret_command(char *token, HashTable **hashTable)
{
	FILE *fp;
	unsigned int size;
	int result;
	size_t index;
	bool found;
	char *output_file;

	if (strcmp(token, "add") == 0) {
		token = strtok(NULL, " ");
		DIE(token == NULL, "Usage: add word");

		result = insert(token, *hashTable);
		DIE(result < 0, "Error in insert!");
		return;
	} else if (strcmp(token, "remove") == 0) {
		token = strtok(NULL, " ");
		DIE(token == NULL, "Usage: remove word");

		result = remove_word(token, *hashTable);
		DIE(result < 0, "Error in remove!");
		return;
	} else if (strcmp(token, "find") == 0) {
		token = strtok(NULL, " ");
		DIE(token == NULL, "Usage: find search_word");

		output_file = strtok(NULL, " ");
		found = find(token, *hashTable);

		if (output_file != NULL)
			fp = fopen(output_file, "a");

		if (found) {
			if (output_file != NULL)
				fprintf(fp, "True\n");
			else
				printf("True\n");
		} else {
			if (output_file != NULL)
				fprintf(fp, "False\n");
			else
				printf("False\n");
		}
		if (output_file != NULL) {
			fprintf(fp, "\n");
			fclose(fp);
		} else
			printf("\n");

		return;
	} else if (strcmp(token, "clear") == 0) {
		size = (*hashTable)->size;

		result = clear(*hashTable);
		DIE(result < 0, "Error in clear!");
		*hashTable = createHashTable(size);
		return;
	} else if (strcmp(token, "print_bucket") == 0) {
		token = strtok(NULL, " ");
		DIE(token == NULL, "Usage: print bucket_index");

		index = atoi(token);
		DIE((index <= 0 && strcmp(token, "0") != 0) ||
			index > (*hashTable)->size - 1, "Invalid index!");

		output_file = strtok(NULL, " ");
		result = print_bucket(*hashTable, index, output_file);
		DIE(result < 0, "Error in print_bucket!");

		return;
	} else if (strcmp(token, "print") == 0) {
		token = strtok(NULL, " ");
		result = print_hashTable(*hashTable, token);
		DIE(result < 0, "Error in print!");
		return;
	} else if (strcmp(token, "resize") == 0) {
		token += strlen("resize") + 1;
		DIE(token == NULL, "Usage: resize double/halve");

		*hashTable = resize(*hashTable, token);
		DIE(hashTable == NULL, "Error in resize!");

		return;
	}
}

/*	Verifica validitatea comenzii command, mai exact,
 *	daca aceasta exista printre comenzile aplicabile
 *	hash table-ului.
 *	Returneaza 1 in caz de comanda valida, -1, altfel.
 */
int command_is_valid(char *command)
{
	char *token;

	token = strtok(command, " ");

	if
		(
		token == NULL ||
		strcmp(token, "add") == 0 ||
		strcmp(token, "remove") == 0 ||
		strcmp(token, "clear") == 0 ||
		strcmp(token, "print_bucket") == 0 ||
		strcmp(token, "print") == 0 ||
		strcmp(token, "find") == 0) {
			return 1;
	} else if (strcmp(token, "resize") == 0) {
		token = strtok(NULL, " ");

		if (strcmp(token, "double") == 0 ||
			strcmp(token, "halve") == 0) {
			return 1;
		}
	}

	return -1;
}

int main(int argc, char *argv[])
{
	char buffer[BUFFSIZE];
	FILE *fp;
	int f;
	char *token;
	HashTable *hashTable;
	int valid_command;

	/*	Verificare argumente(numarul acestora, validitatea
	 *  size-ului pentru hash)
	 */
	DIE(argc < 2, "Usage: ./tema1 hash_size input_files(optional)");
	DIE(atoi(argv[1]) <= 0, "Invalid hash size");
	hashTable = createHashTable(atoi(argv[1]));
	DIE(hashTable == NULL, "Couldn't allocate hash");

	/*	Citire din fisierele de intrare primite ca argumente
	 */
	if (argc > 2) {
		for (f = 2; f < argc; f++) {
			fp = fopen(argv[f], "r");

			while (fgets(buffer, BUFFSIZE, fp) != NULL) {
				token = strtok(buffer, "\n");

				valid_command = command_is_valid(token);
				DIE(valid_command < 0, "Invalid command");

				if (token != NULL)
					interpret_command(token, &hashTable);

			}

			fclose(fp);
		}
	} else {
		/*	Preluare comenzi de la consola
		 */
		while (fgets(buffer, BUFFSIZE, stdin) != NULL) {
			token = strtok(buffer, "\n");

			valid_command = command_is_valid(token);
			DIE(valid_command < 0, "Invalid command");

			if (token != NULL)
				interpret_command(token, &hashTable);
		}
	}

	/*	Eliberare memorie
	 */
	clear(hashTable);

	return 0;
}
