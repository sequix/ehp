#pragma once

#include <semaphore.h>

// lock_t is a binary semaphore within a signle process.
typedef sem_t* lock_t;

lock_t lock_new(void);

#define lock_close(lk) sem_destroy((sem_t*)lk)

#define lock_lock(lk) sem_wait((sem_t*)lk)

#define lock_unlock(lk) sem_post((sem_t*)lk)
