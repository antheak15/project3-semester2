///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT OrderedMap μέσω ADT Set.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>
#include "ADTOrderedMap.h"
#include "ADTSet.h"

struct omap_node {
	Pointer key;
	Pointer value;
	OMap owner;
};

struct omap {
	Set set;
	CompareFunc compare;
	DestroyFunc destroy_key, destroy_value;
};

static int compare_map_nodes(OMapNode a, OMapNode b) {
	return a->owner->compare(a->key, b->key);
}

// Συνάρτηση που καταστρέφει ένα map node
static void destroy_map_node(OMapNode node) {
	if (node->owner->destroy_key != NULL)
		node->owner->destroy_key(node->key);

	if (node->owner->destroy_value != NULL)
		node->owner->destroy_value(node->value);

	free(node);
}

OMap omap_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value) {
	assert(compare != NULL);	// LCOV_EXCL_LINE

	OMap omap = malloc(sizeof(*omap));
	omap->set = set_create((CompareFunc)compare_map_nodes, (DestroyFunc)destroy_map_node);
	omap->compare = compare;
	omap->destroy_key = destroy_key;
	omap->destroy_value = destroy_value;
	return omap;
}

int omap_size(OMap omap) {
	return set_size(omap->set);
}

Pointer omap_find(OMap omap, Pointer key) {
	struct omap_node search_node = { .key = key, .owner = omap };

	OMapNode node = set_find(omap->set, &search_node);
	return node == NULL ? NULL : node->value;
}

void omap_insert(OMap omap, Pointer key, Pointer value) {
	struct omap_node search_node = { .key = key, .owner = omap };

	OMapNode node = set_find(omap->set, &search_node);
	if (node != NULL) {
		if (key != node->key && omap->destroy_key != NULL)
			omap->destroy_key(node->key);

		if (value != node->value && omap->destroy_value != NULL)
			omap->destroy_value(node->value);

		node->key = key;
		node->value = value;
		return;
	}

	node = malloc(sizeof(*node));
	node->key = key;
	node->value = value;
	node->owner = omap;

	set_insert(omap->set, node);
}

bool omap_remove(OMap omap, Pointer key) {
	struct omap_node search_node = { .key = key, .owner = omap };

	OMapNode node = set_find(omap->set, &search_node);
	if (node == NULL)
		return false;

	set_remove(omap->set, node);

	return true;
}

DestroyFunc omap_set_destroy_key(OMap omap, DestroyFunc destroy_key) {
	DestroyFunc old = omap->destroy_key;
	omap->destroy_key = destroy_key;
	return old;
}

DestroyFunc omap_set_destroy_value(OMap omap, DestroyFunc destroy_value) {
	DestroyFunc old = omap->destroy_value;
	omap->destroy_value = destroy_value;
	return old;
}

void omap_destroy(OMap omap) {
	// destroy το set, τα περιεχόμενα θα τα κάνει free η destroy_map_node
	set_destroy(omap->set);

	// free το ίδιο το map
	free(omap);
}

OMapNode omap_first(OMap omap) {
	SetNode node = set_first(omap->set);
	return node == SET_EOF ? OMAP_EOF : set_node_value(omap->set, node);
}

OMapNode omap_last(OMap omap) {
	SetNode node = set_last(omap->set);
	return node == SET_BOF ? OMAP_BOF : set_node_value(omap->set, node);
}

OMapNode omap_next(OMap omap, OMapNode node) {
	SetNode set_node = set_find_node(omap->set, node);
	SetNode next_node = set_next(omap->set, set_node);
	return next_node == SET_EOF ? OMAP_EOF : set_node_value(omap->set, next_node);
}

OMapNode omap_previous(OMap omap, OMapNode node) {
	SetNode set_node = set_find_node(omap->set, node);
	SetNode previous_node = set_previous(omap->set, set_node);
	return previous_node == SET_BOF ? OMAP_BOF : set_node_value(omap->set, previous_node);
}

OMapNode omap_find_node(OMap omap, Pointer key) {
	struct omap_node search_node = { .key = key, .owner = omap };

	return set_find(omap->set, &search_node);
}

Pointer omap_node_key(OMap omap, OMapNode node) {
	return node->key;
}

Pointer omap_node_value(OMap omap, OMapNode node) {
	return node->value;
}

void omap_set_hash_function(OMap omap, HashFunc hash_func) {
}