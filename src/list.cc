#include "list.h"
#include "mem.h"
#include "log.h"
#include <string.h>

static list_node_t list_new_node(node_t value,
        list_node_t prev=NULL, list_node_t next=NULL)
{
    list_node_t node = talloc(list_node_st);
    if (!node) {
        log_error("list_new_node: talloc failed");
        return NULL;
    }
    node->value = value;
    node->prev = prev;
    node->next = next;
    return node;
}

list_t list_new(void)
{
    list_t lst = talloc(list_st);
    if (!lst) {
        log_error("list_new: talloc failed");
        return NULL;
    }
    memset(lst, 0, sizeof(list_st));
    return lst;
}

int list_push_front(list_t lst, node_t value)
{
    list_node_t new_head = list_new_node(value, lst->tail, lst->head);
    if (!new_head) {
        return -1;
    }
    if (++lst->len == 1) {
        lst->head = lst->tail = new_head;
    } else {
        lst->head->prev = new_head;
        lst->tail->next = new_head;
        lst->head = new_head;
    }
    return 0;
}

node_t list_pop_front(list_t lst)
{
    if (lst->len == 0) {
        return NULL;
    }
    list_node_t node = lst->head;

    if (--lst->len == 0) {
        lst->head = lst->tail = NULL;
    } else {
        lst->head = node->next;
        lst->head->prev = lst->tail;
        lst->tail->next = lst->head;
    }
    return node->value;
}

int list_push_back(list_t lst, node_t value)
{
    list_node_t new_tail = list_new_node(value, lst->tail, lst->head);
    if (!new_tail) {
        return -1;
    }
    if (++lst->len == 1) {
        lst->head = lst->tail = new_tail;
    } else {
        lst->head->prev = new_tail;
        lst->tail->next = new_tail;
        lst->tail = new_tail;
    }
    return 0;
}

node_t list_pop_back(list_t lst)
{
    if (lst->len == 0) {
        return NULL;
    }
    list_node_t node = lst->tail;

    if (--lst->len == 0) {
        lst->head = lst->tail = NULL;
    } else {
        lst->tail = node->prev;
        lst->head->prev = lst->tail;
        lst->tail->next = lst->head;
    }
    return node->value;
}
