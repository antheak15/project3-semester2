//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT OrderedMap.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTOrderedMap.h"


static uint hash_int(Pointer value) {
	return *(int*)value;
}


// Συνάρτηση σύγκρισης ακεραίων (ως Pointer)
int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}

// Επιστρέφει έναν ακέραιο σε νέα μνήμη με τιμή value
int* create_int(int value) {
	int* p = malloc(sizeof(int));
	*p = value;
	return p;
}

// Ανακατεύει έναν πίνακα δεικτών
void shuffle(int* array[], int n) {
	for (int i = 0; i < n; i++) {
		int j = i + rand() / (RAND_MAX / (n - i) + 1);
		int* t = array[j];
		array[j] = array[i];
		array[i] = t;
	}
}

// Μετρητής destroy κλήσεων
int destroy_count = 0;

void destroy_int(Pointer value) {
	destroy_count++;
	free(value);
}


void test_create(void) {
	OMap omap = omap_create(compare_ints, NULL, NULL);
	omap_set_hash_function(omap, hash_int);		// no-op για μη-hash υλοποιήσεις
	omap_set_destroy_key(omap, NULL);
	omap_set_destroy_value(omap, NULL);

	TEST_ASSERT(omap != NULL);
	TEST_ASSERT(omap_size(omap) == 0);
	TEST_ASSERT(omap_first(omap) == OMAP_EOF);
	TEST_ASSERT(omap_last(omap) == OMAP_BOF);

	omap_destroy(omap);
}


void test_insert(void) {
	OMap omap = omap_create(compare_ints, free, free);
	omap_set_hash_function(omap, hash_int);

	int N = 500;
	int** keys   = malloc(N * sizeof(*keys));
	int** values = malloc(N * sizeof(*values));

	for (int i = 0; i < N; i++) {
		keys[i]   = create_int(i);
		values[i] = create_int(i * 2);
	}

	// Εισαγωγή σε τυχαία σειρά
	int* order = malloc(N * sizeof(int));
	for (int i = 0; i < N; i++) order[i] = i;
	for (int i = 0; i < N; i++) {
		int j = i + rand() / (RAND_MAX / (N - i) + 1);
		int tmp = order[j]; order[j] = order[i]; order[i] = tmp;
	}

	for (int i = 0; i < N; i++) {
		int k = order[i];
		omap_insert(omap, keys[k], values[k]);
		TEST_ASSERT(omap_size(omap) == i + 1);
		TEST_ASSERT(omap_find(omap, keys[k]) == values[k]);
	}

	// Εξακρίβωση ότι βρίσκουμε όλα τα στοιχεία
	for (int i = 0; i < N; i++)
		TEST_ASSERT(omap_find(omap, keys[i]) == values[i]);

	// Αντικατάσταση: ίδιο key pointer, νέο value
	int* new_val = create_int(9999);
	omap_insert(omap, keys[0], new_val);	// values[0] θα γίνει free
	TEST_ASSERT(omap_size(omap) == N);		// το size δεν αλλάζει
	TEST_ASSERT(omap_find(omap, keys[0]) == new_val);

	// Αντικατάσταση: νέο (ισοδύναμο) key pointer, νέο value
	int* new_key = create_int(*keys[1]);
	int* new_val2 = create_int(1111);
	omap_insert(omap, new_key, new_val2);	// keys[1] και values[1] θα γίνουν free
	TEST_ASSERT(omap_size(omap) == N);
	TEST_ASSERT(omap_find(omap, new_key) == new_val2);

	omap_destroy(omap);	// κάνει free όλα τα κλειδιά και τιμές
	free(keys);
	free(values);
	free(order);
}


void test_remove(void) {
	OMap omap = omap_create(compare_ints, free, free);
	omap_set_hash_function(omap, hash_int);

	int N = 200;
	int** keys   = malloc(N * sizeof(*keys));
	int** values = malloc(N * sizeof(*values));

	for (int i = 0; i < N; i++) {
		keys[i]   = create_int(i);
		values[i] = create_int(i * 3);
		omap_insert(omap, keys[i], values[i]);
	}

	// Αφαίρεση μισών στοιχείων (ζυγών)
	for (int i = 0; i < N; i += 2) {
		int* search = create_int(i);
		TEST_ASSERT(omap_remove(omap, search));
		TEST_ASSERT(omap_find(omap, search) == NULL);
		free(search);
	}
	TEST_ASSERT(omap_size(omap) == N / 2);

	// Αφαίρεση που δεν υπάρχει
	int* not_found = create_int(N + 100);
	TEST_ASSERT(!omap_remove(omap, not_found));
	free(not_found);

	// Τα μονά εξακολουθούν να υπάρχουν
	for (int i = 1; i < N; i += 2)
		TEST_ASSERT(omap_find(omap, keys[i]) == values[i]);

	omap_destroy(omap);
	free(keys);
	free(values);
}


void test_iterate(void) {
	OMap omap = omap_create(compare_ints, free, free);
	omap_set_hash_function(omap, hash_int);

	int N = 300;
	int** keys = malloc(N * sizeof(*keys));

	// Εισαγωγή σε τυχαία σειρά
	int* order = malloc(N * sizeof(int));
	for (int i = 0; i < N; i++) order[i] = i;
	for (int i = 0; i < N; i++) {
		int j = i + rand() / (RAND_MAX / (N - i) + 1);
		int tmp = order[j]; order[j] = order[i]; order[i] = tmp;
	}

	for (int i = 0; i < N; i++) {
		keys[i] = create_int(order[i]);
		omap_insert(omap, keys[i], create_int(order[i] * 2));
	}

	// Διάσχιση προς τα εμπρός: κλειδιά σε αύξουσα σειρά
	int count = 0;
	int prev_key = -1;
	for (OMapNode node = omap_first(omap); node != OMAP_EOF; node = omap_next(omap, node)) {
		int* key   = omap_node_key(omap, node);
		int* value = omap_node_value(omap, node);
		TEST_ASSERT(*key > prev_key);			// αύξουσα σειρά
		TEST_ASSERT(*value == *key * 2);		// σωστή τιμή
		prev_key = *key;
		count++;
	}
	TEST_ASSERT(count == N);

	// Διάσχιση προς τα πίσω: κλειδιά σε φθίνουσα σειρά
	count = 0;
	prev_key = N;
	for (OMapNode node = omap_last(omap); node != OMAP_BOF; node = omap_previous(omap, node)) {
		int* key = omap_node_key(omap, node);
		TEST_ASSERT(*key < prev_key);			// φθίνουσα σειρά
		prev_key = *key;
		count++;
	}
	TEST_ASSERT(count == N);

	// Έλεγχος find_node
	int search_val = N / 2;
	int* search = create_int(search_val);
	OMapNode found = omap_find_node(omap, search);
	TEST_ASSERT(found != OMAP_EOF);
	TEST_ASSERT(*(int*)omap_node_key(omap, found) == search_val);
	free(search);

	// Αναζήτηση ανύπαρκτου
	int* missing = create_int(N + 999);
	TEST_ASSERT(omap_find_node(omap, missing) == OMAP_EOF);
	free(missing);

	omap_destroy(omap);
	free(keys);
	free(order);
}


void test_destroy(void) {
	destroy_count = 0;
	OMap omap = omap_create(compare_ints, destroy_int, destroy_int);
	omap_set_hash_function(omap, hash_int);

	int N = 100;
	for (int i = 0; i < N; i++)
		omap_insert(omap, create_int(i), create_int(i));

	// set_destroy_key / set_destroy_value επιστρέφουν σωστά
	TEST_ASSERT(omap_set_destroy_key(omap, destroy_int)   == destroy_int);
	TEST_ASSERT(omap_set_destroy_value(omap, destroy_int) == destroy_int);

	omap_destroy(omap);
	TEST_ASSERT(destroy_count == 2 * N);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "omap_create",  test_create  },
	{ "omap_insert",  test_insert  },
	{ "omap_remove",  test_remove  },
	{ "omap_iterate", test_iterate },
	{ "omap_destroy", test_destroy },

	{ NULL, NULL } // τερματισμός
};
