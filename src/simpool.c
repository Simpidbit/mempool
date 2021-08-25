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

simmem_t * malloc_simpool(simpool_t * pool, uint64_t size)
{
    simpool_node_t * succeed = 0;
    uint8_t succeed_flag = 0;
    // Check whether there is a proper memory block or not.
    if (size <= pool->maxsiz) {
        simpool_node_t * cur = pool->root;
        while (cur != 0) {
            if (cur->mem.memsiz >= size) {
                if (!succeed_flag) {
                    succeed = cur;
                    succeed_flag = 1;
                }
                if (cur->mem.status == 0) {
                    simmem_t * m = (simmem_t *) malloc(sizeof(simmem_t));
                    cur->mem.status = 1;
                    m->ptr = cur->mem.ptr;
                    m->node = cur;
                    return m;
                } else {
                    cur = cur->next;
                    continue;
                }
            } else {
                cur = cur->next;
                continue;
            }
        }
    }

    // If no proper memory block, then create a new memory block.
    simpool_node_t * newnod = (simpool_node_t *) malloc(sizeof(simpool_node_t));
    if (!succeed_flag) {
        simpool_node_t * cur = pool->root;
        while (cur != 0) {
            if (cur->mem.memsiz >= size) {
                succeed = cur;
                succeed_flag = 1;
                break;
            } else {
                cur = cur->next;
                continue;
            }
        }
        if (!succeed_flag) {
            succeed = 0;
            succeed_flag = 1;
        }
    }

    newnod->mem.memsiz = size;
    newnod->mem.ptr = malloc(size);
    newnod->mem.status = 1;
    if (succeed) {
        newnod->last = succeed->last;
        newnod->next = succeed;
        succeed->last = newnod;
    } else {
        newnod->last = pool->back;
        newnod->next = 0;
        pool->back->next = newnod;
        pool->back = pool->back->next;
    }

    simmem_t * m = (simmem_t *) malloc(sizeof(simmem_t));
    m->ptr = newnod->mem.ptr;
    m->node = newnod;

    if (size > pool->maxsiz)
        pool->maxsiz = size;
    return m;
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
