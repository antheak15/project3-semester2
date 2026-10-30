///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT OrderedMap μέσω ADT Map.
//
///////////////////////////////////////////////////////////

#include "ADTMap.h"
#include <stdlib.h>
#include "ADTOrderedMap.h"
#include <assert.h>


// ΣΗΜΕΙΩΣΗ
// Το struct omap_node δεν χρειάζεται σε αυτή την υλοποίηση, θα κάνουμε απλά cast ένα MapNode σε OMapNode

struct omap {
	Map map; //το Map αποθηκεύει ζεύγη key-value και διαχειρίζεται τις destroy funcs
	CompareFunc compare;// χρειάζεται ώστε τα στοιχεία να παραμείνουν ταξινομημένα
};

OMap omap_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value) {
	assert(compare != NULL);	// LCOV_EXCL_LINE

	OMap omap = malloc(sizeof(*omap));
	omap->map=map_create(compare,destroy_key,destroy_value);//δημιουργεία map
	omap->compare = compare;
	return omap;
}

// για τις παρακάτω υλοποιήσεις μπορούμε να χρησιμοποιήσουμε απευθείας τις συναρτήσεις του module ADTMap 
int omap_size(OMap omap) {
	return map_size(omap->map);
}

Pointer omap_find(OMap omap, Pointer key) {
	return map_find(omap->map,key);
}

void omap_insert(OMap omap, Pointer key, Pointer value) {
	 map_insert(omap->map,key, value);
}

bool omap_remove(OMap omap, Pointer key) {
	return map_remove(omap->map, key);
}

DestroyFunc omap_set_destroy_key(OMap omap, DestroyFunc destroy_key) {
	return map_set_destroy_key (omap->map,destroy_key);
}

DestroyFunc omap_set_destroy_value(OMap omap, DestroyFunc destroy_value) {
	return map_set_destroy_value(omap->map,destroy_value);
}

void omap_destroy(OMap omap) {
	map_destroy(omap->map);
	free(omap);
}

OMapNode omap_first(OMap omap) {
	MapNode first=map_first(omap->map);
	if(first==MAP_EOF)// αν το map είναι άδειο δεν υπάρχει 1ο στοιχείο
		return OMAP_EOF;

	for(MapNode i=map_next(omap->map,first);i!=MAP_EOF;i=map_next(omap->map,i)){// το πρώτο key στο map δεν είναι απαραίτητα το ελάχιστο άρα διασχίζω το map μέχρι να βρω το ελάχιστο 
		if(omap->compare(map_node_key(omap->map,i),map_node_key(omap->map,first))<0) // χρησιμοποιώ την συνάρτηση map_node_key() ώστε να παίρνω το key του node i
			first=i;
	}

	return (OMapNode)first;// κάνω cast το node ως OMapNode όπως προορίζει η συνάρτηση 
}

OMapNode omap_last(OMap omap) {
	MapNode last=map_first(omap->map);// παίρνω τον αρχικό κόμβο επειδή πάλι πρέπει να γίνει διάσχιση στοιχείων
	if(last==MAP_EOF)
		return OMAP_BOF;

	for(MapNode i=map_next(omap->map,last);i!=MAP_EOF;i=map_next(omap->map,i)){
		if(omap->compare(map_node_key(omap->map,i),map_node_key(omap->map,last))>0)// σε αυτή την περίπτωση θέλουμε το μεγαλύτερο key
			last=i;
	}

	return (OMapNode)last;
}

OMapNode omap_next(OMap omap, OMapNode node) {
	MapNode next=MAP_EOF;//θεωρώ ότι δεν υπάρχει επόμενο στοιχείο 

	for(MapNode i=map_first(omap->map);i!=MAP_EOF;i=map_next(omap->map,i)){//διασχίζω τους κόμβους του map
		if(omap->compare(map_node_key(omap->map,i),map_node_key(omap->map,(MapNode)node))>0){//αναζητώ key μεγαλύτερο απο αυτό του node
			if(next==MAP_EOF || omap->compare(map_node_key(omap->map,i),map_node_key(omap->map,next))<0)// και το μικρότερο απο αυτά ώστε να βρεθεί το ακριβώς επόμενο

				next=i;
		}
			
	}

	return (OMapNode)next;
}

OMapNode omap_previous(OMap omap, OMapNode node) {
	// κάνω την ίδια διαδικασία με το next 
	//με τη διαφορά πως αναζητώ το μεγαλύτερο απο τα μικρότερα keys του κόμβου που δόθηκε
	MapNode previous=MAP_EOF;
	for(MapNode i=map_first(omap->map);i!=MAP_EOF;i=map_next(omap->map,i)){
		if(omap->compare(map_node_key(omap->map,i),map_node_key(omap->map,(MapNode)node))<0){
			if(previous==MAP_EOF || omap->compare(map_node_key(omap->map,i),map_node_key(omap->map,previous))>0)

				previous=i;
		}
			
	}

	return (OMapNode)previous;
}

OMapNode omap_find_node(OMap omap, Pointer key) {
	return  (OMapNode)map_find_node(omap->map,key);//βρίσκω το MapNode του key που δόθηκε
}

Pointer omap_node_key(OMap omap, OMapNode node) {
	return map_node_key(omap->map,(MapNode)node);//επιστροφή του key του node που δόθηκε
}

Pointer omap_node_value(OMap omap, OMapNode node) {
	return map_node_value(omap->map,(MapNode)node);//επιστροφή του value του node που δόθηκε
}

void omap_set_hash_function(OMap omap, HashFunc hash_func) {
	map_set_hash_function(omap->map,hash_func);//χρησιμοποιώ την συνάρτηση του module για να ορίσω την hash function 
}
