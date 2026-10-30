# Κάνοντας compile το <foo>_test.c με μια υλοποίηση <foo>.c του
# συγκεκριμένου τύπου, παράγουμε ένα test για την υλοποίηση αυτή.

COMMON = ADTOrderedMap_test.o

# Υλοποίηση μέσω ADT Set (παρέχεται ήδη στο modules/UsingAVL/)
UsingADTSet_ADTOrderedMap_test_OBJS = $(COMMON) \
    $(MODULES)/UsingADTSet/ADTOrderedMap.o \
    $(MODULES)/UsingAVL/ADTSet.o

# Υλοποίηση μέσω ADT Map (παρέχεται ήδη στο modules/UsingHashTable/)
UsingADTMap_ADTOrderedMap_test_OBJS = $(COMMON) \
    $(MODULES)/UsingADTMap/ADTOrderedMap.o \
    $(MODULES)/UsingHashTable/ADTMap.o

# Υλοποίηση μέσω ADT List (ταξινομημένη) (παρέχεται ήδη στο modules/UsingLinkedList/)
UsingADTList_ADTOrderedMap_test_OBJS = $(COMMON) \
    $(MODULES)/UsingADTList/ADTOrderedMap.o \
    $(MODULES)/UsingLinkedList/ADTList.o

# Υλοποίηση μέσω ADT List (ταξινομημένη) + ADT Map (παρέχεται ήδη στα modules/)
UsingADTListAndMap_ADTOrderedMap_test_OBJS = $(COMMON) \
    $(MODULES)/UsingADTListAndMap/ADTOrderedMap.o \
    $(MODULES)/UsingHashTable/ADTMap.o \
    $(MODULES)/UsingLinkedList/ADTList.o

# Υλοποίηση μέσω Skip List
UsingSkipList_ADTOrderedMap_test_OBJS = $(COMMON) \
    $(MODULES)/UsingSkipList/ADTOrderedMap.o


# Ο βασικός κορμός του Makefile
include ../common.mk
