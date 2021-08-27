# Simpool

>  A simple memory pool by C.



It doesn't have any dependency, just depends on std C.

It's simple!



* **Simple example**:

  ```c
  #include <stdio.h>
  #include <stdlib.h>
  #include <simpool.h>
  
  int main()
  {
    const char * str = "Hello Simpool!";
    
    // Init simpool.
    simpool_t * pool = create_simpool();
    
    // Apply for memory from simpool.
    simmem_t * m = malloc_simpool(pool, strlen(str) + 1);
    
    // Just use the memory that is pointed by m->ptr;
    memcpy(m->ptr, str, strlen(str) + 1);
    
    printf("str is %s\n", (char *) m->ptr);
    
    // Free the memory from simpool.
    free_simpool(m);
    
    // Terminate simpool.
    terminate_simpool(pool);
    return 0;
  }
  ```

  When you compile, please link the shared library of simpool.
