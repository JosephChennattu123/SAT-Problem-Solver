#include "list.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "err.h"
#include "util.h"

/**
 * Struct for encapsulating a single list element.
 */
typedef struct ListItem {
    struct ListItem* next;  // pointer to the next element (NULL if last)
    void* data;             // pointer to the data
} ListItem;

List mkList(void) {
    List res;
    res.head = NULL;
    return res;
}

void clearList(List* s) {
    ListItem* p = s->head;
    while (p != NULL) {
        ListItem* n = p->next;
        free(p);

        p = n;
    }
    s->head = NULL;
}

void push(List* s, void* data) {
    ListItem* p = (ListItem*)malloc(sizeof(ListItem));
    if (p == NULL) {
        return;
    }

    p->data = data;
    p->next = s->head;
    s->head = p;
}

void* peek(List* s) {
    if (s->head == NULL) {
        fprintf(stderr, "Stack is empty. Cannot peek.\n");
        return NULL;
    }

    return s->head->data;
}

void pop(List* s) {
    /*ListItem* p = (ListItem*)malloc(sizeof(ListItem));
    if (p == NULL) {
        return;
    }
    if (s->head->next == NULL) {
        s->head = NULL;
    }
    p->next = s->head->next;
    p->data = s->head->next->data;
    free(s->head);
    s->head = p;*/
    char p = isEmpty(s);
    if (p == '1') {
        return;
    }
    ListItem* q = s->head;
    s->head = s->head->next;
    free(q);
}

char isEmpty(List* s) { return s->head == NULL; }

ListIterator mkIterator(List* list) {
    ListIterator res;
    res.list = list;
    res.prev = NULL;
    res.current = list->head;

    return res;
}

void* getCurr(ListIterator* it) {
    assert(it->current != NULL);
    return it->current->data;
}

void next(ListIterator* it) {
    assert(isValid(it));
    it->prev = it->current;
    it->current = it->current->next;
}

char isValid(ListIterator* it) { return it->current != NULL; }
