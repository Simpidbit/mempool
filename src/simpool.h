#ifndef MEMPOOL_SIMPOOL_H
#define MEMPOOL_SIMPOOL_H

#include <stdint.h>

// private
// look at simpool_node_t
// for node
typedef struct {
    uint64_t memsiz;
    void * ptr;
    uint8_t status;         // 0 - free;  1 - be using.
} simpool_memblk_t;

// private
// heap - free by terminate_simpool()
typedef struct simpool_node_t {
    struct simpool_node_t * last;
    struct simpool_node_t * next;
    simpool_memblk_t mem;
} simpool_node_t;

// public
// heap - free by terminate_simpool()
typedef struct {
    simpool_node_t * root;
    simpool_node_t * back;
    uint64_t maxsiz;
} simpool_t;

// public
// heap - free by free_simpool()
// for user
typedef struct {
    void * ptr;
    simpool_node_t * node;
} simmem_t;

simpool_t * create_simpool();
simmem_t * malloc_simpool(simpool_t * pool, uint64_t size);
void free_simpool(simmem_t * mem);
void terminate_simpool(simpool_t * pool);

#endif
