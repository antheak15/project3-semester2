///////////////////////////////////////////////////////////
//
// ADT OrderedMap
//
// Abstract ordered map. Παρέχει γρήγορη αντιστοίχιση
// key => value, και ταυτόχρονα διατεταγμένη διάσχιση
// με βάση τα κλειδιά.
//
///////////////////////////////////////////////////////////

#pragma once // #include το πολύ μία φορά

#include "common_types.h"


// Ένας ordered map αναπαριστάται από τον τύπο OMap

typedef struct omap* OMap;


// Δημιουργεί και επιστρέφει έναν ordered map, στον οποίο τα κλειδιά συγκρίνονται με βάση
// τη συνάρτηση compare.
// Αν destroy_key ή/και destroy_value != NULL, τότε καλείται destroy_key(key)
// ή/και destroy_value(value) κάθε φορά που αφαιρείται ένα στοιχείο.

OMap omap_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value);

// Επιστρέφει τον αριθμό στοιχείων που περιέχει το map.

int omap_size(OMap omap);

// Προσθέτει το κλειδί key με τιμή value. Αν υπάρχει κλειδί ισοδύναμο με key,
// τα παλιά key & value αντικαθίστανται από τα νέα.

void omap_insert(OMap omap, Pointer key, Pointer value);

// Αφαιρεί το κλειδί που είναι ισοδύναμο με key από το map, αν υπάρχει.
// Επιστρέφει true αν βρέθηκε τέτοιο κλειδί, διαφορετικά false.

bool omap_remove(OMap omap, Pointer key);

// Επιστρέφει την τιμή που έχει αντιστοιχιστεί στο συγκεκριμένο key,
// ή NULL αν το key δεν υπάρχει στο map.

Pointer omap_find(OMap omap, Pointer key);

// Αλλάζει τη συνάρτηση που καλείται σε κάθε αφαίρεση/αντικατάσταση key/value.
// Επιστρέφει την προηγούμενη τιμή της συνάρτησης.

DestroyFunc omap_set_destroy_key  (OMap omap, DestroyFunc destroy_key  );
DestroyFunc omap_set_destroy_value(OMap omap, DestroyFunc destroy_value);

// Ελευθερώνει όλη τη μνήμη που δεσμεύει το map.
// Οποιαδήποτε λειτουργία πάνω στο map μετά το destroy είναι μη ορισμένη.

void omap_destroy(OMap omap);


// Διάσχιση του map μέσω κόμβων ////////////////////////////////////////////////////////
//
// Η σειρά διάσχισης σέβεται τη διάταξη των κλειδιών (αύξουσα).

// Οι σταθερές αυτές συμβολίζουν εικονικούς κόμβους _πριν_ τον πρώτο
// και _μετά_ τον τελευταίο κόμβο του map.
#define OMAP_BOF (OMapNode)0
#define OMAP_EOF (OMapNode)0

typedef struct omap_node* OMapNode;

// Επιστρέφει τον πρώτο (ελάχιστο) κόμβο του map, ή OMAP_EOF αν δεν υπάρχει.

OMapNode omap_first(OMap omap);

// Επιστρέφει τον τελευταίο (μέγιστο) κόμβο του map, ή OMAP_BOF αν δεν υπάρχει.

OMapNode omap_last(OMap omap);

// Επιστρέφει τον επόμενο κόμβο (με μεγαλύτερο κλειδί), ή OMAP_EOF αν δεν υπάρχει.

OMapNode omap_next(OMap omap, OMapNode node);

// Επιστρέφει τον προηγούμενο κόμβο (με μικρότερο κλειδί), ή OMAP_BOF αν δεν υπάρχει.

OMapNode omap_previous(OMap omap, OMapNode node);

// Επιστρέφει το κλειδί του κόμβου node.

Pointer omap_node_key(OMap omap, OMapNode node);

// Επιστρέφει την τιμή του κόμβου node.

Pointer omap_node_value(OMap omap, OMapNode node);

// Βρίσκει και επιστρέφει τον κόμβο που αντιστοιχεί στο κλειδί key,
// ή OMAP_EOF αν το κλειδί δεν υπάρχει στο map.

OMapNode omap_find_node(OMap omap, Pointer key);


//// Επιπλέον για υλοποιήσεις βασισμένες σε hashing //////////////////

// Τύπος συνάρτησης κατακερματισμού

typedef uint (*HashFunc)(Pointer);

// Ορίζει τη συνάρτηση κατακερματισμού για το map.
// Χρειάζεται μόνο για υλοποιήσεις που χρησιμοποιούν hash table εσωτερικά.
// Για τις υπόλοιπες υλοποιήσεις είναι no-op.
// Πρέπει να κληθεί μετά την omap_create και πριν από οποιαδήποτε άλλη συνάρτηση.

void omap_set_hash_function(OMap omap, HashFunc hash_func);
