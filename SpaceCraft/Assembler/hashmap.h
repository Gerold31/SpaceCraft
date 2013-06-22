
#ifndef HASHMAP_H_INCLUDED
#define HASHMAP_H_INCLUDED

/* anonyme Strukturen verhindern den Zugriff auf interne Daten von außerhalb */
typedef struct sHashmap hashmap;

#define HASHMAP_ILLEGAL 0   /**< illegaler Zugriff */
#define HASHMAP_INSERT 1    /**< neues Element angelegt */
#define HASHMAP_UPDATE 2    /**< bestehendes Element aktualisiert */

/**@brief Typedef für eine Funktion, die ein Datenelement ausgibt.
 */
typedef void(*fHashmapPrinter)(const char* key, int datum);

/**@brief Erstellt eine neue Hashmap in der angegebenen Größe.
 * @note Die Hashmap vergrößert sich selbstständig bei Bedarf.
 * 
 * @param hint  Hinweis auf die nötige Kapazität
 * 
 * @return Zeiger auf die Hashmap
 */
extern hashmap* newHashmap(unsigned int hint);

/**@brief Löscht die Hashmap.
 * @note Die gespeicherten Daten bleiben davon unberührt.
 * 
 * @param map   Zielhashmap
 */
extern void deleteHashmap(hashmap* map);

/**@brief Fügt ein neues Element in die Hashmap ein oder aktualisiert ein
 * vorhandenes.
 * 
 * @param map   Zielhashmap
 * @param data  Zeiger auf die zu speichernden Daten
 * @param key   Datenschlüssel
 * @return HASHMAP_ILLEGAL, falls der Schlüssel illegal ist (NULL oder leer) \n
 *         HASHMAP_INSERT, falls neu eingefügt \n
 *         HASHMAP_UPDATE, falls aktualisiert
 */
extern int hashmapSet(hashmap* map, int data, const char* key);

/**@brief Gibt das Element mit dem gegebenen Schlüssel zurück.
 * 
 * @param map   Zielhashmap
 * @param key   Datenschlüssel
 * @return Datum zum Schlüssel, falls vorhanden \n
 *         1 sonst
 */
extern int hashmapGet(const hashmap* map, const char* key);

/**@brief Entfernt das Element mit dem gegebenen Schlüssel aus der Hashmap und
 * gibt es zurück.
 * 
 * @param map   Zielhashmap
 * @param key   Datenschlüssel
 * @return Datum zum gelöschten Schlüssel, falls existent \n
 *         -1 sonst
 */
extern int hashmapRemove(hashmap* map, const char* key);

/**@brief Gibt diverse Kenngrößen zur Hashmap aus.
 * 
 * @param map      Zielhashmap
 * @param printer  Funktion, die ein Element der Hashmap ausgibt
 * @note Kann 0 sein. In diesem Fall wird der Zeiger auf das Element ausgegeben.
 */
extern void hashmapPrint(const hashmap* map, fHashmapPrinter printer);

#endif
