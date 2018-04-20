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
#include <stdint.h>

typedef struct HashNode HashNode;
typedef struct HashTable HashTable;
typedef int bool;

/*	Structura reprezentand un nod din hash table
 */
struct HashNode {
	char *word;
	HashNode *next;
};

/*	Structura reprezentand hash table-ul
 */
struct HashTable {
	unsigned int size;
	HashNode **buckets;
};

HashTable *createHashTable(int size);

int insert(char *word, HashTable *hashTable);

int remove_word(char *target, HashTable *hashTable);

bool find(char *target, HashTable *hashTable);

int print_bucket(HashTable *hashTable, int index, char *file);

int print_hashTable(HashTable *hashTable, char *file);

HashTable *resize(HashTable *hashTable, char *size_modification);

int clear(HashTable *hashTable);