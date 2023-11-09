#define ENGINE_EXPORTS
#include "EngineCore.h" 
#include <stdio.h>

ENGINE_API void initializeEngine() {
    printf("Engine initialized.\n");
}

ENGINE_API void updateEngine() {
    printf("Engine updated.\n");
}