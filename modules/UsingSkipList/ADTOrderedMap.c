///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT OrderedMap μέσω Skip List.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include "ADTOrderedMap.h"

struct omap_node {
};

struct omap {
};

OMap omap_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value) {
	return NULL;
}

int omap_size(OMap omap) {
	return 0;
}

Pointer omap_find(OMap omap, Pointer key) {
	return NULL;
}

void omap_insert(OMap omap, Pointer key, Pointer value) {
}

bool omap_remove(OMap omap, Pointer key) {
	return false;
}

DestroyFunc omap_set_destroy_key(OMap omap, DestroyFunc destroy_key) {
	return NULL;
}

DestroyFunc omap_set_destroy_value(OMap omap, DestroyFunc destroy_value) {
	return NULL;
}

void omap_destroy(OMap omap) {
}

OMapNode omap_first(OMap omap) {
	return OMAP_EOF;
}

OMapNode omap_last(OMap omap) {
	return OMAP_BOF;
}

OMapNode omap_next(OMap omap, OMapNode node) {
	return OMAP_EOF;
}

OMapNode omap_previous(OMap omap, OMapNode node) {
	return OMAP_BOF;
}

OMapNode omap_find_node(OMap omap, Pointer key) {
	return OMAP_EOF;
}

Pointer omap_node_key(OMap omap, OMapNode node) {
	return NULL;
}

Pointer omap_node_value(OMap omap, OMapNode node) {
	return NULL;
}

void omap_set_hash_function(OMap omap, HashFunc hash_func) {
}
