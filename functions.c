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

#define false 0
#define true 1

/*	Functia aloca memorie pentru hash table, mai exact, size
 *	bucket-uri.
 *	In caz de succes, returneaza hash-ul creat, altfel, o eroare.
 */
HashTable *createHashTable(int size)
{
	HashTable *hashTable = malloc(sizeof(HashTable));

	if (hashTable == NULL)
		return NULL;

	hashTable->size = size;
	hashTable->buckets = calloc(size, sizeof(HashNode *));

	if (hashTable->buckets == NULL)
		return NULL;

	return hashTable;
}

/*	Functia insereaza cuvantul word in hash table-ul hashTable.
 *	In caz de succes, returneaza 1, daca word exista deja in hash,
 *	returneaza 2, iar survine o eroare de alocare a memoriei, -1.
 */
int insert(char *word, HashTable *hashTable)
{
	HashNode *insertIndexNode;
	HashNode *newNode;
	unsigned int index;
	char *newWord;

	index = hash(word, hashTable->size);
	newNode = malloc(sizeof(HashNode));

	if (newNode == NULL || strlen(word) == 0)
		return -1;

	if (find(word, hashTable))
		return 2;

	newWord = malloc(strlen(word) + 1);

	if (newNode == NULL)
		return -1;

	strcpy(newWord, word);
	newNode->word = newWord;
	newNode->next = NULL;

	insertIndexNode = hashTable->buckets[index];

	if (insertIndexNode == NULL)
		hashTable->buckets[index] = newNode;
	else {
		for (    ; insertIndexNode->next != NULL;
				insertIndexNode = insertIndexNode->next)
			;
		insertIndexNode->next = newNode;
	}

	return 1;
}

/*	Functia sterge cuvantul target din hash table-ul
 *	hashTable, daca acesta exista.
 *	In caz de succes, se va returna valoarea 1,
 *  daca nu exista cuvantul, 2, altfel, -1.
 */
int remove_word(char *target, HashTable *hashTable)
{
	HashNode *next;
	HashNode *currentNode;
	HashNode *previousNode;
	unsigned int index;

	if (!find(target, hashTable))
		return 2;

	index = hash(target, hashTable->size);

	if (hashTable->buckets[index] != NULL) {
		if (strcmp(hashTable->buckets[index]->word, target) == 0) {
			next = hashTable->buckets[index]->next;

			free(hashTable->buckets[index]->word);
			free(hashTable->buckets[index]);
			hashTable->buckets[index] = NULL;

			hashTable->buckets[index] = next;

			return 1;

		} else {
			currentNode = hashTable->buckets[index];
			previousNode = hashTable->buckets[index];

			while (currentNode->next != NULL) {
				previousNode = currentNode;
				currentNode = currentNode->next;

				if (strcmp(currentNode->word, target) == 0) {
					previousNode->next = currentNode->next;
					free(currentNode->word);
					free(currentNode);
					currentNode = NULL;

					return 1;
				}
			}
		}
	}

	return -1;
}

/*	Functia cauta cuvantul target in hash table-ul hashTable.
 *	In caz acesta se gaseste in hash, se returneaza true, in caz
 *	contrar, false.
 */
bool find(char *target, HashTable *hashTable)
{
	HashNode *currentNode;
	unsigned int i = 0;

	for (i = 0; i < hashTable->size; i++) {
		if (hashTable->buckets[i] != NULL) {
			if (strcmp(hashTable->buckets[i]->word, target) == 0)
				return true;

			currentNode = hashTable->buckets[i];

			while (currentNode->next != NULL) {
				currentNode = currentNode->next;

				if (strcmp(currentNode->word, target) == 0)
					return true;
			}
		}
	}

	return false;

}

/*	Functia printeaza bucket-ul cu numarul index din hash table-ul
 *	hashTable, in fisierul file sau la consola, daca acesta este
 *	NULL.
 *	In caz de succes, functia intoarce 1, altfel, -1.
 */
int print_bucket(HashTable *hashTable, int index, char *file)
{
	HashNode *printNode;
	FILE *fp;

	printNode = hashTable->buckets[index];

	if (printNode == NULL)
		return 1;

	if (file != NULL) {
		fp = fopen(file, "a");

		if (fp == NULL)
			return -1;
	}

	while (printNode != NULL) {
		if (file != NULL)
			fprintf(fp, "%s ", printNode->word);
		else
			printf("%s ", printNode->word);

		printNode = printNode->next;
	}

	if (file != NULL) {
		fprintf(fp, "\n");
		fclose(fp);
	} else
		printf("\n");

	return 1;

}

/*	Functia printeaza intregul hash table hashTable in fisierul file
 *	sau la consola, daca acesta este NULL.
 *	In caz de succes, functia intoarce 1, altfel, -1.
 */
int print_hashTable(HashTable *hashTable, char *file)
{
	HashNode *printNode;
	FILE *fp;
	unsigned int i = 0;

	if (file != NULL) {
		fp = fopen(file, "a");

		if (fp == NULL)
			return -1;
	}

	for (i = 0; i < hashTable->size; i++) {
		printNode = hashTable->buckets[i];

		while (printNode != NULL) {
			if (file != NULL)
				fprintf(fp, "%s ", printNode->word);
			else
				printf("%s ", printNode->word);

			printNode = printNode->next;
		}

		if (hashTable->buckets[i] != NULL) {
			if (file != NULL)
				fprintf(fp, "\n");
			else
				printf("\n");
		}

	}

	if (file != NULL) {
		fprintf(fp, "\n");
		fclose(fp);
	} else
		printf("\n");

	return 1;
}

/*	Functia returneaza un nou hashTable cu numar bucket-uri
 *	dublat/injumatatit fata de hash-ul primit ca parametru.
 */
HashTable *resize(HashTable *hashTable, char *size_modification)
{
	HashTable *resizedHashTable;
	HashNode *t;
	HashNode *next;
	unsigned int i = 0;

	if (strcmp(size_modification, "double") == 0)
		resizedHashTable = createHashTable(hashTable->size * 2);
	else if (strcmp(size_modification, "halve") == 0)
		resizedHashTable = createHashTable(hashTable->size / 2);

	for (i = 0; i < hashTable->size; i++) {
		if (hashTable->buckets[i] != NULL) {

			t = hashTable->buckets[i];
			insert(t->word, resizedHashTable);
			t = t->next;

			while (t != NULL) {
				next = t->next;

				insert(t->word, resizedHashTable);

				free(t->word);
				free(t);

				t = next;
			}

			free(hashTable->buckets[i]->word);
			free(hashTable->buckets[i]);
		}
	}

	free(hashTable->buckets);
	free(hashTable);

	return resizedHashTable;
}

/*	Functia distruge hash-ul hashTable, eliberand memoria alocata
 *	pentru acesta.
 *	Va fi returnata valoarea 1.
 */
int clear(HashTable *hashTable)
{
	unsigned int i = 0;
	HashNode *t;
	HashNode *next;

	for (i = 0; i < hashTable->size; i++) {
		if (hashTable->buckets[i] != NULL) {
			t = hashTable->buckets[i];
			t = t->next;

			while (t != NULL) {
				next = t->next;

				free(t->word);
				free(t);
				t = next;
			}

			free(hashTable->buckets[i]->word);
			free(hashTable->buckets[i]);
		}

	}

	free(hashTable->buckets);
	free(hashTable);

	return 1;

}