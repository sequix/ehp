#include "lock.h"
#include "mem.h"
#include <semaphore.h>

lock_t lock_new(void)
{
    sem_t *sem = talloc(sem_t);
    sem_init(sem, 0, 1);
    return sem;
}
