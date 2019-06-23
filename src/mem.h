#pragma once

#include <gc.h>

// typed allocation, clear allocated storage automatically
#define talloc(T)        (T*)GC_MALLOC(sizeof(T))

// sized & typed allocation
#define stalloc(sz,T)    (T*)GC_MALLOC(sz * sizeof(T))

// sized & typed re-allocation
#define stralloc(p,sz,T) (T*)GC_REALLOC(p, sz * sizeof(T))

// typed allocation without further allcation on the new pointer
#define talloc_a(T)      (T*)GC_MALLOC_ATOMIC(sizeof(T))

// sized & typed allocation without further allcation on the new pointer
#define stalloc_a(sz,T)  (T*)GC_MALLOC_ATOMIC(sz * sizeof(T))
