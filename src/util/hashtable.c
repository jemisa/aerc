/*
 * util/hashtable.c - implements a generic hashtable
 */
#include <stdbool.h>
#include <stdlib.h>

#include "util/hashtable.h"

hashtable_t *create_hashtable(size_t buckets, unsigned int (*hash_function)(const void *)) {
	hashtable_t *table = malloc(sizeof(hashtable_t));
	table->hash = hash_function;
	table->bucket_count = buckets;
	table->buckets = calloc(buckets, sizeof(hashtable_entry_t));
	return table;
}

void free_bucket(hashtable_entry_t *bucket) {
	if (bucket) {
		free_bucket(bucket->next);
		free(bucket);
	}
}

void free_hashtable(hashtable_t *table) {
	for (size_t i = 0; i < table->bucket_count; ++i) {
		free_bucket(table->buckets[i]);
	}
	free(table);
}

bool hashtable_contains(hashtable_t *table, const void *key) {
	unsigned int hash = table->hash(key);
	unsigned int bucket = hash % table->bucket_count;
	hashtable_entry_t *entry = table->buckets[bucket];
	if (entry) {
		if (entry->key != hash) {
			while (entry->next) {
				entry = entry->next;
				if (!entry || entry->key == hash) {
					break;
				}
			}
		}
	} else {
		return false;
	}
	return true;
}

void *hashtable_get(hashtable_t *table, const void *key) {
	unsigned int hash = table->hash(key);
	unsigned int bucket = hash % table->bucket_count;
	hashtable_entry_t *entry = table->buckets[bucket];
	if (entry) {
		if (entry->key != hash) {
			while (entry->next) {
				entry = entry->next;
				if (!entry || entry->key == hash) {
					break;
				}
			}
		}
	} else {
		return NULL;
	}
	return entry->value;
}

void *hashtable_set(hashtable_t *table, const void *key, void *value) {
	unsigned int hash = table->hash(key);
	unsigned int bucket = hash % table->bucket_count;
	hashtable_entry_t *entry = table->buckets[bucket];
	hashtable_entry_t *previous = NULL;

	if (entry) {
		if (entry->key != hash) {
			while (entry->next) {
				previous = entry;
				entry = entry->next;
				if (!entry || entry->key == hash) {
					break;
				}
			}
		}
	}

	if (entry == NULL) {
		entry = calloc(1, sizeof(hashtable_entry_t));
		entry->key = hash;
		table->buckets[bucket] = entry;
		if (previous) {
			previous->next = entry;
		}
	}

	void *old = entry->value;
	entry->value = value;
	return old;
}

void *hashtable_del(hashtable_t *table, const void *key) {
	unsigned int hash = table->hash(key);
	unsigned int bucket = hash % table->bucket_count;
	hashtable_entry_t *entry = table->buckets[bucket];
	hashtable_entry_t *previous = NULL;

	if (entry) {
		if (entry->key != hash) {
			while (entry->next) {
				previous = entry;
				entry = entry->next;
				if (!entry || entry->key == hash) {
					break;
				}
			}
		}
	}

	if (entry == NULL) {
		return NULL;
	} else {
		void *old = entry->value;
		if (previous) {
			previous->next = entry->next;
		} else {
			table->buckets[bucket] = NULL;
		}
		free(entry);
		return old;
	}
}
