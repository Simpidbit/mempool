#include <stdlib.h>
#include <strings.h>
#include "simpool.h"

simpool_t * create_simpool()
{
    simpool_t * pool = (simpool_t *) malloc(sizeof(simpool_t));
    pool->root = (simpool_node_t *) malloc(sizeof(simpool_node_t));
    bzero(pool->root, sizeof(simpool_node_t));
    pool->back = pool->root;
    pool->maxsiz = 0;
    return pool;
}

// called by malloc_simpool
static uint8_t
malloc_check_node(simpool_t * pool, uint64_t size,
                  simpool_node_t ** succeed, simmem_t ** nmem)
{
    uint8_t is_inherited = 0x00;

    // Search for succeed.
    simpool_node_t * cur = pool->root;
    for (;;) {
        if (!cur) break;
        if (cur->mem.memsiz >= size) {
            *succeed = cur;
            break;
        }
        cur = cur->next;
    }

    // Check whether there is a proper memory block or not.
    for (;;) {
        if (!cur) break;
        if (cur->mem.memsiz >= size && cur->mem.status == 0) {
            cur->mem.status = 1;
            bzero(cur->mem.ptr, cur->mem.memsiz);

            *nmem = (simmem_t *) malloc(sizeof(simmem_t));
            (*nmem)->ptr = cur->mem.ptr;
            (*nmem)->node = cur;

            is_inherited = 0x01;
            break;
        }
        cur = cur->next;
    }

    return is_inherited;
}

// called by malloc_simpool
static void
malloc_insert_node(simpool_t * pool, uint64_t size,
                   simpool_node_t * succeed, simmem_t ** nmem)
{
    simpool_node_t * nnod = (simpool_node_t *) malloc(sizeof(simpool_node_t));
    nnod->mem.memsiz = size;
    nnod->mem.ptr = malloc(size);
    nnod->mem.status = 1;
    *nmem = (simmem_t *) malloc(sizeof(simmem_t));
    (*nmem)->ptr = nnod->mem.ptr;
    (*nmem)->node = nnod;
    if (succeed) {
        nnod->last = succeed->last;
        nnod->next = succeed;
        succeed->last = nnod;
    } else {
        // succeed == NULL -> no one is more than this size.
        nnod->last = pool->back;
        nnod->next = NULL;
        pool->back->next = nnod;
        pool->back = nnod;
    }
}

simmem_t * malloc_simpool(simpool_t * pool, uint64_t size)
{
    simpool_node_t * succeed = NULL;
    simmem_t * nmem = NULL;
    uint8_t is_inherited = malloc_check_node(pool, size, &succeed, &nmem);
    if (is_inherited) {
        return nmem;
    } else {
        malloc_insert_node(pool, size, succeed, &nmem);
        return nmem;
    }
}

void free_simpool(simmem_t * mem)
{
    mem->node->mem.status = 0;
    free(mem);
}

void terminate_simpool(simpool_t * pool)
{
    simpool_node_t * cur = pool->root;

    // free the node, then free the pool.
    while (cur != 0) {
        if (cur->mem.status)
            free(cur->mem.ptr);
        if (cur->next) {
            cur = cur->next;
            free(cur->last);
        } else {
            free(cur);
            break;
        }
    }
    free(pool);
}
