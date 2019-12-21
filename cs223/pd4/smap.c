#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// #define __STRICT_ANSI__


#include "smap.h"


#define MULTIPLIER (37)
#define SMAP_INITIAL_CAPACITY 100
#define TABLESIZE_MULTIPLIER (2)
#define GROWTH_FACTOR (2)
#define MAX_LOAD_FACTOR (1)
// #define TABLE_GROW_DENSITY (2)

// typedef struct entry entry;

struct entry{
	unsigned long hash;
	char *key;
	void *value;
	struct entry *next;
};

struct smap{
	int size;
	int numElements;
	struct entry **table; // INVARIANT: always at least one free slot in table
	int (*hash)(const char *s);
};





// static void *MemCopy(const void *x, void *arg);
int smap_table_find_key(const struct entry *table, const char *key, int (*hash)(const char *), int size, int capacity);
void smap_embiggen(smap *m, double n);
static struct entry *dictFetch(const smap *d, const char *key);

/**
 * Returns a hash value for the given string.
 *
 * @param s a string, non-NULL
 * @return an int
 */

int smap_default_hash(const char *s){
	// if (s == NULL)
	//    {
	//        return 0;
	//    }
	size_t h;
	unsigned const char *us;
	us = (unsigned const char *) s;
	h = 0;
	while(*us != '\0') {
		h = h * MULTIPLIER + *us;
		us++;
	}
	return h;

}


/**
 * Creates an empty map that uses the given hash function.
 *
 * @param h a pointer to a function that takes a string and returns
 * its hash code, non-NULL
 * @return a pointer to the new map or NULL if it could not be created;
 * it is the caller's responsibility to destroy the map
 */

smap *smap_create(int (*h)(const char *s)){

	smap *result = malloc(sizeof(smap));
	if (result == NULL){
		return NULL;
	}
	result->size = SMAP_INITIAL_CAPACITY;
	result->numElements = 0;
	result->hash = h;
	result->table = malloc(sizeof(*(result->table)) * SMAP_INITIAL_CAPACITY);
	if(result->table == NULL) {
		free(result);
		return NULL; 
	}
	// result->size = (result->table != NULL ? SMAP_INITIAL_CAPACITY : 0);
	for (int i = 0; i < result->size; i++)
	{
		result->table[i] = 0;
	}

	return result;
}



/**
 * Returns the number of (key, value) pairs in the given map.
 *
 * @param m a pointer to a map, non-NULL
 * @return the size of m
 */

int smap_size(const smap *m){
	return m->numElements;
}


/**
 * Adds a copy of the given key with value to this map.
 * If the key is already present then the old value is replaced.
 * The caller retains ownership of the value.  If key is new
 * and space could not be allocated then there is no effect on the map
 * and the return value is false.
 *
 * @param m a pointer to a map, non-NULL
 * @param key a string, non-NULL
 * @param value a pointer
 * @return true if the put was successful, false otherwise
 */
bool smap_put(smap *m, const char *key, void *value){
	// printf("size %d", m->size);
	int tablePosition; 
	unsigned long h; int i;
	struct entry *e;
	h = m->hash(key);
	i = h % m->size;
	for(e = m->table[i]; e != 0; e = e->next) {
		if(strcmp(key, e->key) == 0) { /* found it */
			e->value = value;
			return true;
		}
	}

	e = malloc(sizeof(*e)); 
	if(e == NULL) {
		return false;
	}

	e->hash = h;
	char *copy = malloc(strlen(key) + 1);
	strcpy(copy, key);
	if (copy != NULL)
	{
		e->key = copy;
		e->value = value;

		tablePosition = e->hash % m->size;
		e->next = m->table[tablePosition];
		m->table[tablePosition] = e;
		m->numElements++;
		if(m->numElements > m->size * 0.5) { /* grow and rehash */
			smap_embiggen(m, 2);
		} 
		return true;
	}

	return false;


}

/**
 * Determines if the given key is present in this map.
 *
 * @param m a pointer to a map, non-NULL
 * @param key a string, non-NULL
 * @return true if key is present in this map, false otherwise
 */
bool smap_contains_key(const smap *m, const char *key){
	unsigned long h; int i;
	struct entry *e;
	h = m->hash(key);
	i = h % m->size;   
	// printf("i %d", i);
	// printf("hash = %ld", h);
	for(e = m->table[i]; e != 0; e = e->next) {
		// printf("e->key %s\n", e->key);
		if(strcmp(key, e->key) == 0) { /* found it */
			// printf("hit");
			return true;
			//e->hash == h && 


		} 
	}
	/* didn't find it */
	// printf("didn't find\n");
	return false; 

}

/**
 * Returns the value associated with the given key in this map.
 * If the key is not present in this map then the returned value is
 * NULL.  The value returned is the original value passed to smap_put,
 * and it remains the responsibility of whatever called smap_put to
 * release the value (no ownership transfer results from smap_get).
 *
 * @param m a pointer to a map, non-NULL
 * @param key a string, non-NULL
 * @return the assocated value, or NULL if they key is not present
 */
void *smap_get(smap *m, const char *key){

	struct entry *e;
	e = dictFetch(m, key); 
	if(e != 0) {

		return e->value; 

	} 
	else {

		return 0; 
	}

}

/**
 * Removes the given key and its associated value from the given map if
 * the key is present.  The return value is NULL and there is no effect
 * on the map if the key is not present.
 *
 * @param m a pointer to a map, non-NULL
 * @param key a key, non-NULL
 * @return the value associated with the key
 */
void *smap_remove(smap *m, const char *key){
	unsigned long h; 
	int i;
	// struct entry *e;
	void *value = smap_get(m, key);
	struct entry *temp, *prev;
	// void *temp;
	h = m->hash(key);
	i = h % m->size;   
	// printf("i %d", i);
	// printf("hash = %ld", h);
	temp = m->table[i];

	if(temp != 0 && (strcmp(key, temp->key) == 0)) {
		m->table[i] = temp->next;
		free(temp->key);
		free(temp);
		m->numElements--;
		return value;
	}


	while (temp != 0 && (strcmp(key, temp->key) != 0)) {
		// printf("e->key %s\n", e->key);
		prev = temp;
		temp = temp->next;
	}

	if (temp == 0){
		return value;
	}

	prev->next = temp->next;
	free(temp->key);
	free(temp);
	m->numElements--;


	if(m->numElements < m->size * 0.125) { /* grow and rehash */
			smap_embiggen(m, 0.5);
	} 
	return value;

}



/**
 * Calls the given function for each (key, value) pair in this map, passing
 * the extra argument as well.  This function does not add or remove from
 * the map.
 *
 * @param m a pointer to a map, non-NULL
 * @param f a pointer to a function that takes a key, a value, and an
 * extra argument, and does not add to or remove from the map, no matter
 * what the extra argument is{} non-NULL
 * @param arg a pointer
 */

void smap_for_each(smap *m, void (*f)(const char *, void *, void *), void *arg){
	struct entry *e;
	for (int i = 0; i<m->size; i++){
		if (m->table[i] != 0){
			for(e = m->table[i]; e != 0; e = e->next) {
				f(e->key, e->value, arg);
			}
		}
	}
}

/**
 * Returns a dynamically allocated array containing pointers to the keys in the
 * given map.  It is the caller's responsibility to free the array,
 * but the map retains ownership of the keys.  If there is a memory
 * allocation error then the returned value is NULL.  If the map is empty
 * then the returned value is NULL.
 *
 * @param m a pointer to a map, non NULL
 * @return a pointer to a dynamically allocated array of pointer to the keys
 */
const char **smap_keys(smap *m){
	struct entry *e;
	int count = 0;
	const char **keyArray = malloc(m->numElements*sizeof(char *));
	for (int i = 0; i<m->size; i++){
		if (m->table[i] != 0){
			for(e = m->table[i]; e != 0; e = e->next) {
				keyArray[count] = e->key;
				count++;
			}
		}
	}
	return keyArray;
}

/**
 * Destroys the given map.
 *
 * @param m a pointer to a map, non-NULL
 */
void smap_destroy(smap *m){
	int i;
	struct entry *e; 
	struct entry *next;
	for(i = 0; i < m->size; i++) {
		for(e = m->table[i]; e != 0; e = next) {
			next = e->next;
			free(e->key);
			// free(e->value);
			// d->valueOps.delete(e->value, d->valueOps.arg);
			free(e);
		} }
	free(m->table);
	free(m);
}

void smap_embiggen(smap *m, double n)
{
	// char *word = "word0";
	// // printf("after numElements %d", m->numElements);
	// if (smap_contains_key(m, word)){
	// 	printf("yes");
	// }
	// printf("no");
	struct entry **old_table; 
	int old_size;
	int i;
	struct entry *e;
	struct entry *next; 
	int new_pos;
	/* save old table */
	old_table = m->table;
	old_size = m->size;
	/* make new table */
	m->size = (int)ceil(m->size * n);
	m->table = malloc(sizeof(*(m->table)) * m->size); 
	if(m->table == NULL) {
		/* put the old one back */
		m->table = old_table;
		m->size = old_size;
		return;
	}
	/* else */
	/* clear new table */
	for(i = 0; i < m->size; i++) 
		m->table[i] = 0;
	/* move all elements of old table to new table */
	for(i = 0; i < old_size; i++) {
		for(e = old_table[i]; e != 0; e = next) {

			next = e->next;
			/* find the position in the new table */
			new_pos = e->hash % m->size;
			e->next = m->table[new_pos];
			m->table[new_pos] = e;
		} 

	}
	/* don't need this any more */
	free(old_table);

	// *word = "word0";
	// printf("after numElements %d \n", m->numElements);
	// if (smap_contains_key(m, word)){
	// 	printf("yes");
	// }
	// printf("no");

}



static struct entry *dictFetch(const smap *d, const char *key) {

	unsigned long h; int i;
	struct entry *e;
	h = d->hash(key);
	i = h % d->size;   
	// printf("i %d", i);
	// printf("hash = %ld", h);
	for(e = d->table[i]; e != 0; e = e->next) {
		// printf("e->key %s\n", e->key);
		if(strcmp(key, e->key) == 0) { /* found it */
			// printf("hit");
			return e;
			//e->hash == h && 


		} 
	}
	/* didn't find it */
	// printf("didn't find\n");
	return 0; 
}

// static void *MemCopy(const void *x, void *arg)
// {
//   void *x2;
//   x2 = malloc((size_t) arg); memcpy(x2, x, (size_t) arg); 
//   return x2;
// }


