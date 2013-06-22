#include "hashmap.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <alloca.h>

/* *************************************************************** Strukturen */

/**@brief Struktur eines Hashmap-Eintrages.
 */
typedef struct
{
	char* key; /**< Schlüssel */
    int data; /**< Zeiger auf die Daten */
} hashmapEntry;

/**@brief Struktur der Hashmap.
 */
struct sHashmap
{
	hashmapEntry* array; /**< Array mit Daten-/Schlüsseltupeln */
	size_t size, /**< Gesamtgröße des Arrays */
	count; /**< Anzahl der gespeicherten Elemente */
};

/* ******************************************************* private Funktionen */

/**@brief Verdoppelt die Größe der Hashmap und fügt alle alten Elemente neu ein.
 */
static void rehash(hashmap* map);

/**@brief Fügt ein neues Element in die Hashmap ein bzw. aktualisiert ein
 * vorhandenes, ohne dass der Schlüssel kopiert wird.
 * 
 * @return 1, wenn neu hinzugefügt \n
 *         2, wenn aktualisiert
 */
static int insert(hashmap* map, int data, char* key);

/**@brief Sucht einen Hashmap-Eintrag anhand des Schlüssels oder gibt den
 * ersten freien Index zurück, falls dieser nicht gefunden werden kann.
 * 
 * @return Zeiger auf den entsprechenden Eintrag, falls existent \n
 *         0 sonst
 */
static hashmapEntry* find(const hashmap* map, const char* key);

/**@brief Vergleicht zwei Hashmap-Einträge und definiert dadurch eine totale
 * Ordnung.
 * @note Der Vergleich erfolgt anhand der Schlüssel lexikografisch, falls beide
 * existieren. Andernfalls dominiert der Eintrag mit dem existierenden
 * Schlüssel. Sonst sind die Einträge gleich.
 */
static int compare(const hashmapEntry* lhs, const hashmapEntry* rhs);

/**@brief Berechnet den SDBM 32-Bit Hashcode. (Sleepycats Berkeley DataBase)
 */
static unsigned long hash1(const char* rawKey)
{
	const unsigned char* s = (const unsigned char*) rawKey;
	unsigned long hash = 0;
	
	while (*s)
	{
		hash = *s + (hash << 6) + (hash << 16) - hash;
		++s;
	}
	
	return hash;
}

/**@brief Berechnet den FNV 32-Bit Hashcode. (Fowler/Noll/Vo)
 */
static unsigned long hash2(const char* rawKey)
{
	const unsigned char* s = (const unsigned char*) rawKey;
	unsigned long hash = 0;
	
	while (*s)
	{
		hash ^= (unsigned long) *s;
		hash += (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 24);
		++s;
	}
	
	return hash;
}

static void rehash(hashmap* map)
{
	size_t size = map->size;
	hashmapEntry* array = map->array;
	
	/* verdopple die Größe des Arrays */
	map->size <<= 1;
	map->array = (hashmapEntry*) calloc(sizeof(hashmapEntry), map->size);
	map->count = 0;

	/* füge die alten Elemente neu ein */
	while (size--)
	{
		if (array[size].key)
			insert(map, array[size].data, array[size].key);
	}
	
	/* gib den alten Speicher frei */
	free(array);
}

static hashmapEntry* find(const hashmap* map, const char* key)
{
	unsigned long index, step, initialIndex;
	hashmapEntry* freeEntry = 0;
	
	initialIndex = index = hash1(key) & (map->size - 1);
	
	/* erster Versuch */
	if (map->array[index].key)
	{
		if (!strcmp(map->array[index].key, key))
			return &map->array[index];
	}
	else if (!map->array[index].data)
	{
		return &map->array[index];
	}
	else
	{
		freeEntry = &map->array[index];
	}
	
	/* Kollision */
	step = (hash2(key) % (map->size - 1)) + 1;
	
	do
	{
		index = (index + step) & (map->size - 1);
		
		if (map->array[index].key)
		{
			if (!strcmp(map->array[index].key, key))
				return &map->array[index];
		}
		else if (!map->array[index].data)
		{
			return (freeEntry) ? freeEntry : &map->array[index];
		}
		else if (!freeEntry)
		{
			freeEntry = &map->array[index];
		}
	}
	while (index != initialIndex);
	
	return freeEntry;
}

static int insert(hashmap* map, int data, char* key)
{
	hashmapEntry* entry;
	
	if (map->size == map->count)
		rehash(map);
	
	do
	{
		entry = find(map, key);
		
		if (entry)
		{
			entry->data = data;
			
			if (entry->key)
			{
				/* der Eintrag wird aktualisiert */
				free(key);
				return HASHMAP_UPDATE;
			}
			else
			{
				/* der Eintrag wird neu eingefügt */
				++map->count;
				entry->key = key;
				return HASHMAP_INSERT;
			}
		}
		
		/* da die Hashtabellen nicht die Primzahlbedingung erfüllen, ist es möglich,
		 * dass nicht alle Elemente besucht werden; wir müssen also neu hashen um
		 * einer Endlosschleife vorzubeugen */
		rehash(map);
	}
	while (1);
}

static int compare(const hashmapEntry* lhs, const hashmapEntry* rhs)
{
	return (lhs->key)	 ? (rhs->key) ? strcmp(lhs->key, rhs->key) : -1
	                   : (rhs->key) ? 1 : 0;
}

/* *************************************************** exportierte Funktionen */

hashmap* newHashmap(unsigned int hint)
{
	hashmap* map = (hashmap*) malloc(sizeof(hashmap));
	
	/* die Hashtabelle funktioniert erst ab 4 Einträgen (siehe insert) */
	if (hint < 4)
	{
		hint = 4;
	}
	else if (hint & (hint - 1))
	{
		unsigned int i = 1;
		
		/* finde die nächst größere Potenz von 2 */
		do
		{
			hint |= (hint >> i);
			i <<= 1;
		}
		while (i <= (sizeof(hint) << 2));
		++hint;
	}

	map->array = (hashmapEntry*) calloc(sizeof(hashmapEntry), hint);
	map->size = hint;
	map->count = 0;
	
	return map;
}

void deleteHashmap(hashmap* map)
{
	if (map)
	{
		unsigned long index = 0;
		
		do
		{
			if (map->array[index].key)
				free(map->array[index].key);
		}
		while (++index < map->size);

		free(map->array);
		free(map);
	}
}

int hashmapSet(hashmap* map, int data, const char* key)
{
	return (map && key && *key)  ? insert(map, data, strdup(key))
	                             : HASHMAP_ILLEGAL;
}

int hashmapGet(const hashmap* map, const char* key)
{
	if (map)
	{
		hashmapEntry* entry = find(map, key);
		
		if (entry && entry->key)
			return entry->data;
	}
	
    return -1;
}

int hashmapRemove(hashmap* map, const char* key)
{
    int res = -1;
	
	if (map)
	{
		hashmapEntry* entry = find(map, key);
		
		if (entry && entry->key)
		{
			--map->count;
			
			free(entry->key);
			entry->key = 0;
			res = entry->data;
			
            /* exist auf -2 zu setzen zeigt an, dass hier schon mal ein Element
			 * gespeichert wurde */
            entry->data = -2;
		}
	}
	return res;
}

void hashmapPrint(const hashmap* map, fHashmapPrinter printer)
{
	if (map)
	{
		hashmapEntry* array = (hashmapEntry*) alloca(sizeof(hashmapEntry)*map->size);
		int i;
		
		memcpy(array, map->array, sizeof(hashmapEntry) * map->size);
		qsort(array, map->size, sizeof(hashmapEntry),
		      (int(*)(const void*, const void*)) compare);
		
		for (i = 0; i < map->count; ++i)
		{
			if (printer)
				printer(array[i].key, array[i].data);
			else
                printf("%3i\tKey %-10s\tValue %d\n", i, array[i].key, array[i].data);
		}
	}
	else
		printf("Hashmap doesn't exist.\n");
}

