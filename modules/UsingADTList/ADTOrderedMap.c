///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT OrderedMap μέσω ADT List (ταξινομημένη).
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include "ADTOrderedMap.h"
#include "ADTList.h"
#include <assert.h>

struct omap_node {//κάθε κόμβος του map αποθηκεύει ενα ζεύγος key-value 
	Pointer key;
	Pointer value;
	OMap owner;//δείχνει που ανήκει ο κόμβος
};

struct omap {// το ordered map αποτελείται απο μια sorted list
	List list;
	CompareFunc compare;//συνάρτηση για σύγκριση των keys
	DestroyFunc destroy_key, destroy_value;//destroy funcs για keys και values
};

static int compare_map_nodes(OMapNode a, OMapNode b) {//συγκρίνει δύο map nodes με βάση τα keys τους
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
	
	//δέσμευση μνήμης για το OrderedMap, δημιουργεία της λίστας και αρχικοποιήση των συναρτήσεων της δομής
	OMap omap = malloc(sizeof(*omap));
	omap->list = list_create( (DestroyFunc)destroy_map_node);
	omap->compare = compare;
	omap->destroy_key = destroy_key;
	omap->destroy_value = destroy_value;
	return omap;
}

int omap_size(OMap omap) {
	return list_size(omap->list);
}

Pointer omap_find(OMap omap, Pointer key) {
	struct omap_node search_node = { .key = key, .owner = omap };//προσωρινό node για αναζήτηση με βάση το key

	OMapNode node = (OMapNode)list_find(omap->list, &search_node,(CompareFunc)compare_map_nodes);
	return node == NULL ? NULL : node->value;// αν δεν υπάρχει επιστρέφει NULL αλλιώς το value του node
}

void omap_insert(OMap omap, Pointer key, Pointer value) {
	struct omap_node search_node = { .key = key, .owner = omap };

	OMapNode node = (OMapNode)list_find(omap->list, &search_node,(CompareFunc)compare_map_nodes);//αναζήτηση για το στοιχείο που είναι ίσο με το search_node(συγκρίνει τα keys των 2 κόμβων)

	// αν υπάρχει ήδη το key ,ενημερώνο το συγκεκριμένο node και ταστρέφω αν χρειάζεται τα key,value ώστε να ενημερωθούν και αυτα
	if (node != NULL) {
		if (key != node->key && omap->destroy_key != NULL)
			omap->destroy_key(node->key);//καταστρέφουμε το παλιό key

		if (value != node->value && omap->destroy_value != NULL)
			omap->destroy_value(node->value);

		node->key = key;
		node->value = value;
		return;//αφου ενημερώθηκαν δεν χρειάζεται εισαγωγή
	}
	ListNode previous=LIST_BOF;//ξεκινώ απο την αρχή 
	for(ListNode i=list_first(omap->list);i!=LIST_EOF;i=list_next(omap->list,i)){//και διασχίζω τους κόμβους ένα-ένα
		OMapNode current=(OMapNode)list_node_value(omap->list,i);//κάθε φορα παίρνω το map node 
		if(omap->compare(key,current->key)<0){//βρίσκω και σταματώ στο 1ο μεγαλύτερο key ώστε το νεο node να μπει στη σωστή θέση
			break;//αν βρεθεί σταματά η διαδικασία
		}
		previous=i;
	}

	OMapNode new_node=malloc(sizeof(*new_node));//δεσμεύω μνήμη για τον νεό κόμβο του OrderedMap
	new_node->key=key;
	new_node->value=value;
	new_node->owner=omap;

	list_insert_next(omap->list,previous,new_node);
}

bool omap_remove(OMap omap, Pointer key) {
	//αρχικα η ίδια διαδικασία με το insert
	struct omap_node search_node = { .key = key, .owner = omap };
	ListNode node =list_find_node(omap->list, &search_node,(CompareFunc)compare_map_nodes);
	if(node==LIST_EOF)//αν δεν υπάρχει ο κόμβος επιστρέφεται false αφού δεν έγινε αφαίρεση
		return false;
	ListNode previous=list_previous(omap->list,node);//βρίσκουμε το προηγούμενο κόμβο αφού στη συνέχεια γίνεται remove next
	list_remove_next(omap->list,previous);

	return true;//return true γιατί γινεται η αφαίρεση
}

DestroyFunc omap_set_destroy_key(OMap omap, DestroyFunc destroy_key) {
	//κρατώ την παλιά destroy function των keys ώστε να οριστεί η νέα και επιστρέφω την παλιά
	DestroyFunc old = omap->destroy_key;
	omap->destroy_key = destroy_key;
	return old;
}

DestroyFunc omap_set_destroy_value(OMap omap, DestroyFunc destroy_value) {
	//κρατώ την παλιά destroy function των values ώστε να οριστεί η νέα και επιστρέφω την παλιά
	DestroyFunc old = omap->destroy_value;
	omap->destroy_value = destroy_value;
	return old;
}

void omap_destroy(OMap omap) {
	//καταστρέφω την λίστα και κατα συνέπεια όλα τα map nodes
	list_destroy(omap->list);
	free(omap);//και αποδεσμεύω τη μνήμη όλης της δομής
}

OMapNode omap_first(OMap omap) {
	//βρίσκω το πρώτο κόμβο της λίστας, αν δεν είναι αδεια και το επιστρέφω 
	ListNode node=list_first(omap->list);
	return node==LIST_EOF?OMAP_EOF:list_node_value(omap->list,node);
}

OMapNode omap_last(OMap omap) {
	//βρίσκω τον τελευταίο κόμβο της λίστας, αν δεν είναι αδεια το επιστρέφω
	ListNode node=list_last(omap->list);
	return node==LIST_BOF?OMAP_BOF:list_node_value(omap->list,node);
}

OMapNode omap_next(OMap omap, OMapNode node) {
	ListNode list_node=list_find_node(omap->list,node,(CompareFunc)compare_map_nodes);//συγκρίνοντας τα keys βρίσκω τον αντίστοιχο κόμβο της λίστας
	//βρίσκω τον επόμενο κόμβο και αν υπάρχει το επιστρέφω  
	ListNode next_node=list_next(omap->list,list_node);
	if(next_node==LIST_EOF)
		return OMAP_EOF;
	return list_node_value(omap->list,next_node);
}

OMapNode omap_previous(OMap omap, OMapNode node) {
	//χρησιμοποιώ την ίδια λογική με την next
	ListNode list_node=list_find_node(omap->list,node,(CompareFunc)compare_map_nodes);
	ListNode previous_node=list_previous(omap->list,list_node);
	return previous_node==LIST_BOF?OMAP_BOF:list_node_value(omap->list,previous_node);
}

OMapNode omap_find_node(OMap omap, Pointer key) {
	struct omap_node search_node = { .key = key, .owner = omap };
	//βρίσκω τον κόμβο της λίστας και αν υπάρχει το επιστρέφω
	ListNode node =list_find_node(omap->list, &search_node,(CompareFunc)compare_map_nodes);
	return node==LIST_EOF?OMAP_EOF:list_node_value(omap->list,node);
}

Pointer omap_node_key(OMap omap, OMapNode node) {
	return node->key;
}

Pointer omap_node_value(OMap omap, OMapNode node) {
	return node->value;
}

void omap_set_hash_function(OMap omap, HashFunc hash_func) {
}//η υλοποιήση γίνεται με list άρα δεν χρειάζεται hash function

