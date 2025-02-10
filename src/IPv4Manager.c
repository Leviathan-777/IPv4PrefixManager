#include <stdio.h>
#include <stdlib.h>

#define MIN_MASK 0
#define MAX_MASK 32

typedef struct TrieNode 
{
    struct TrieNode *children[2];
    char mask;  /* Przechowuje maskę prefiksu, jeśli węzeł jest końcem prefiksu */
} TrieNode;

/*
 * Funkcja: create_node
 * --------------------
 * Opis: 
 *      Tworzy pusty węzeł struktury TrieNode.
 * 
 * Parametry:
 *      Brak
 *
 * Zwraca: 
 *      Pusty węzeł
*/
TrieNode *create_node() 
{
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));

    if (node)
    {
        node->children[0] = NULL; 
        node->children[1] = NULL;
        node->mask = -1;
    }

    return node;
}

/*
 * Funkcja: free_tree
 * --------------------
 * Opis: 
 *      Zwalnia pamięć usuwając dany węzeł i jego synów.
 * 
 * Parametry:
 *      *node - wskaźnik do danego węzła
 *
 * Zwraca: 
 *      Brak zwracanej wartości
*/
void free_tree(TrieNode *node) 
{
    if (!node)
    {
        return;
    }

    free_tree(node->children[0]);
    free_tree(node->children[1]);
    free(node);
}

/*
 * Funkcja: add
 * --------------------
 * Opis: 
 *      Dodaje nowy prefiks w formacie IPv4
 * 
 * Parametry:
 *      *root - wskaźnik do korzenia struktury danych ze zbiorem prefiksów
 *      base  - baza IP
 *      mask  - maska (0 - 32)
 *
 * Zwraca: 
 *      -1 - podane paramerty były błędne i operacja się nie powiodła,
 *      0  - prefiks już istnieje w zbiorze,
 *      1  - operacja dodawania nowego prefiksu się powiodła,
*/
int add(TrieNode *root, unsigned int base, char mask) 
{
    /* Niepoprawna wartość maski */
    if ( (mask < MIN_MASK) || (mask > MAX_MASK) )
    {
        return -1;
    }

    TrieNode *node = root;

    for (int i = MAX_MASK; i > (MAX_MASK - mask); i--) 
    {
        int bit = (base >> (i-1)) & 1;

        if (!node->children[bit]) 
        {
            node->children[bit] = create_node();
        }

        node = node->children[bit];
    }

    if (node->mask == -1) 
    {
        node->mask = mask; // Ustawienie maski tylko jeśli prefiks nie istniał
    }
    else
    {
        return 0;
    }

    return 1;
}

/*
 * Funkcja: del
 * --------------------
 * Opis: 
 *      Usuwa prefiks ze zbioru.
 * 
 * Parametry:
 *      *root - wskaźnik do korzenia struktury danych ze zbiorem prefiksów
 *      base  - baza IP
 *      mask  - maska (0 - 32)
 *
 * Zwraca: 
 *      -1 - niepoprawna wartość maski lub pusty zbiór
 *      1  - operacja usuwania prefiksu zakończona powodzeniem
*/
int del(TrieNode *root, unsigned int base, char mask) 
{
    /* Niepoprawna wartość maski lub pusty zbiór */
    if ( (!root) || (mask < MIN_MASK) || (mask > MAX_MASK) )
    {
        return -1;
    }

    TrieNode *node = root;
    
    for (int i = MAX_MASK; i > (MAX_MASK - mask); i--) 
    {
        int bit = (base >> (i-1)) & 1;
        node = node->children[bit];
    }

    node->mask = -1;

    return 1;
}

 /*
 * Funkcja: check
 * --------------------
 * Opis: 
 *      Sprawdza czy adres IP zawiera się w zbiorze prefiksów. 
 * 
 * Parametry:
 *      *root - wskaźnik do korzenia struktury danych ze zbiorem prefiksów
 *      ip    - adres IP
 *
 * Zwraca: 
 *      Maskę najmniejszego prefiksu (o największej wartości maski) w zbiorze, który zawiera wskazany adres. 
 *      Jeżeli IP nie zawiera się w zbiorze prefiksów zwraca -1.
 */
char check(TrieNode *root, unsigned int ip) 
{
    if (!root)
    {
        return -1;
    }

    TrieNode *node = root;
    char best_match = -1;

    for (int i = MAX_MASK; i > MIN_MASK; i--) 
    {
        int bit = (ip >> (i-1)) & 1;
        if (!node->children[bit])
        { 
            break;
        }

        node = node->children[bit];
        if (node->mask != -1)
        {
            best_match = node->mask;
        }
    }
    return best_match;
}

 /*
 * Funkcja: test
 * --------------------
 * Opis: 
 *      Funkcja zawierająca testy dla funkcji 'add', 'del' i 'check'.
 * 
 * Parametry:
 *      Brak
 *
 * Zwraca: 
 *      0
 */
void test() 
{
    TrieNode *root = create_node();

    /* Test funkcji add */
    printf("Dodaj 10.20.0.0/16: %d\n",                                      add(root, 0x0A140000, 16)); /* Powinno zwrócić 1 */
    printf("Dodaj 10.20.0.0/16 (Dodanie juz istniejacego prefiksu): %d\n",  add(root, 0x0A140000, 16)); /* Powinno zwrócić 0, dodanie istniejącego prefiksu */
    printf("Dodaj 80.64.128.0/20: %d\n",                                    add(root, 0x50408000, 20)); /* Powinno zwrócić 1 */
    printf("Dodaj 32.40.2.3/24: %d\n",                                      add(root, 0x1E280203, 24)); /* Powinno zwrócić 1 */
    printf("Dodaj 192.168.0.1/33 (niepoprawna maska): %d\n\n",              add(root, 0xC0A80001, 33)); /* Powinno zwrócić -1, niepoprawna maska */

    /* Test funkcji check */
    printf("Sprawdz 10.20.128.16: %d\n",  check(root, 0x0A148010)); /* Powinno zwrócić 16 */
    printf("Sprawdz 80.64.140.1: %d\n",   check(root, 0x50408C01)); /* Powinno zwrócić 20 */
    printf("Sprawdz 192.168.1.1: %d\n",   check(root, 0xC0A80101)); /* Powinno zwrócić -1 */
    printf("Sprawdz 32.40.10.14: %d\n\n", check(root, 0x1E28020E)); /* Powinno zwrócić 24 */

    /* Test funkcji del */
    printf("Usun 10.20.0.0/16: %d\n",  del(root, 0x0A140000, 16)); /* Powinno zwrócić 1 */
    printf("Sprawdz 10.20.128.16 po operacji usuniecia: %d\n\n", check(root, 0x0A148010)); /* Powinno zwrócić -1 */

    
    free_tree(root);
}

 /*
 * Funkcja: main
 * --------------------
 * Opis: 
 *      Główna funckja programu. Wywołuje testy.
 * 
 * Parametry:
 *      Brak
 *
 * Zwraca: 
 *      0
 */
int main() 
{
    test();
    return 0;
}