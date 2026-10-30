///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT OrderedMap μέσω ADT List (ταξινομημένη)
// + ADT Map (hash table) για γρήγορη αναζήτηση.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include "ADTOrderedMap.h"
#include "ADTList.h"
#include "ADTMap.h"
#include<assert.h>

//Σε αυτή την υλοποιήση χρησιμοποιώ την ταξινομημένη λίστα για τη σειρά και το Map για γρήγορη αναζήτηση
struct omap_node {
	Pointer key;
	Pointer value;
	OMap owner;
};

struct omap {
	List list;//ταξινομημένη λίστα που αποθηκεύει τα OMapNodes
	Map map; //map που αντιστοιχεί κάθε key σε ένα ListNode
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
	//όπως υλοποιήσα κάθε create όμως εδώ δημιουργώ και λίστα και map
	OMap omap = malloc(sizeof(*omap));
	omap->list = list_create( (DestroyFunc)destroy_map_node);
	omap->map=map_create(compare,NULL,NULL);
	omap->compare = compare;
	omap->destroy_key = destroy_key;
	omap->destroy_value = destroy_value;
	return omap;
	
}

int omap_size(OMap omap) {
	return map_size(omap->map);//επιστροφή του πλήθους των στοιχείων (αντίστοιχα θα μπορούσα να χρησιμοποιήσω την αντιστοιχη συνάρτηση του list)
}

Pointer omap_find(OMap omap, Pointer key) {
	ListNode  list_node=map_find(omap->map,key);//βρίσκω το node του συγκεκριμένου key απο το map
	if(list_node==NULL){//αν δεν υπάρχει επιστρέφω null
		return NULL;
	}
	OMapNode node =(OMapNode)list_node_value(omap->list,list_node);//αλλιώς παίρνω το OmapNode απο την λίστα και επιστρέφω το value του 
	return node->value;
}

void omap_insert(OMap omap, Pointer key, Pointer value) {
	ListNode list_node= map_find(omap->map,key);//βρίσκω το node του συγκεκριμένου key απο το map

	if (list_node != NULL) {//αν υπάρχει ήδη το συγκεκριμένο key
		OMapNode node = (OMapNode)list_node_value(omap->list, list_node);//παίρνω το OMapNode 
		//αν είναι διαφορετικός δείκτης καταστρέφω το key και το ενημερώνω με το νέο
		if (key != node->key ){
			map_remove(omap->map,node->key);

		if (omap->destroy_key != NULL)
			omap->destroy_key(node->key);

		node->key = key;
		map_insert(omap->map,node->key,list_node);
		}
		//αν είναι διαφορετικό το value καταστρέφω το value και το ενημερώνω με το νέο
		if (value != node->value && omap->destroy_value != NULL)
			omap->destroy_value(node->value);
		
	node->value=value;
	return;//τερματίζεται γιατί δεν χρειάζεται νεα εισαγωγή
	}

	//βρίσκω την σωστή θέση στη λίστα που θα μπει ο νέος κόμβος
	ListNode previous=LIST_BOF;
	for(ListNode i=list_first(omap->list);i!=LIST_EOF;i=list_next(omap->list,i)){
		OMapNode current=(OMapNode)list_node_value(omap->list,i);
		if(omap->compare(key,current->key)<0){
	
			break;
		}
		previous=i;
	}

	OMapNode new_node=malloc(sizeof(*new_node));
	new_node->key=key;
	new_node->value=value;
	new_node->owner=omap;

	list_insert_next(omap->list,previous,new_node);
	ListNode new_list_node;//κρατα το ListNode που μπήκε το νεο OMapNode
	if(previous==LIST_BOF)//αν έγινε η εισαγωγή στην αρχή της λίστας
		new_list_node=list_first(omap->list);//τότε το ListNode γίνεται πρώτο
	else 
		new_list_node=list_next(omap->list,previous);//αλλιώς μπαίνει αμέσως μετά το previous
	map_insert(omap->map,key,new_list_node);//ενημερώνουμε το map ώστε το key να δείχνει στο νέο ListNode

}

bool omap_remove(OMap omap, Pointer key) {
	ListNode list_node= map_find(omap->map,key);
	if(list_node==NULL)//αν το στοιχείο δεν βρεθεί
		return false;
	//Αλλιώς βρίσκω το αμέσως προηγούμενο ListNode ώστε να γίνει η αφαίρεση
	ListNode previous=list_previous(omap->list,list_node);

	map_remove(omap->map, key);//αφαιρώ από το map το ζεύγος key-ListNode
	list_remove_next(omap->list,previous);//και τον κόμβο απο την λίστα
	
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
	list_destroy(omap->list);//κατατστρογή της λίστας και το OMapNodes
	map_destroy(omap->map);//καταστροφή του map
	free(omap);//αποδέσμευση της δομής
}

OMapNode omap_first(OMap omap) {
	ListNode node=list_first(omap->list);//παίρνω το πρώτο κόμβο απο τη ταξινομημένη λίστα
	return node==LIST_EOF?OMAP_EOF:list_node_value(omap->list,node);
}

OMapNode omap_last(OMap omap) {
	ListNode node=list_last(omap->list);//παίρνω το τελευταίο κόμβο απο τη ταξινομημένη λίστα
	return node==LIST_BOF?OMAP_BOF:list_node_value(omap->list,node);
}

OMapNode omap_next(OMap omap, OMapNode node) {
	ListNode list_node=list_find_node(omap->list,node,(CompareFunc)compare_map_nodes);//βρίσκω στη λίστα το κόμβο που δόθηκε 
	ListNode next_node=list_next(omap->list,list_node);//παίρνω και τον επόμενο κόμβο
	if(next_node==LIST_EOF)//αν δεν υπάρχει άλλος κόμβος 
		return OMAP_EOF;
	return (OMapNode)list_node_value(omap->list,next_node);//αλλιώς επιστρέφουμε το OMapNode του επόμενου κόμβου
}

OMapNode omap_previous(OMap omap, OMapNode node) {
	//ίδια λογική με το next
	ListNode list_node=list_find_node(omap->list,node,(CompareFunc)compare_map_nodes);
	ListNode previous_node=list_previous(omap->list,list_node);
	
	return previous_node==LIST_BOF?OMAP_BOF:list_node_value(omap->list,previous_node);
}

OMapNode omap_find_node(OMap omap, Pointer key) {
	ListNode list_node= map_find(omap->map,key);
	if(list_node==NULL)//αν δεν υπάρχει τέτοιο key 
		return OMAP_EOF;
	return (OMapNode)list_node_value(omap->list,list_node);//αλλιώς επιστρέφω το OMapNode του αντίστοιχου ListNode
}

Pointer omap_node_key(OMap omap, OMapNode node) {
	return node->key;
}

Pointer omap_node_value(OMap omap, OMapNode node) {
	return node->value;
}

void omap_set_hash_function(OMap omap, HashFunc hash_func) {
	map_set_hash_function(omap->map,hash_func);//αφού υπάρχει το map μπορούμε να χρησιμοποιήσουμε και να ορίσουμε τη hash function
}
