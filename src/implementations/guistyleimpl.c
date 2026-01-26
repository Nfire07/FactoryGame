#include "../headers/core/guistyle.h"
#include "../headers/guistyleimpl.h"
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>


void* LoadStyleThreadFunction(void* arg) {
    StyleLoadStatus* status = (StyleLoadStatus*)arg;
    
    printf("INFO: Starting GUI style loading...\n");
    
    GuiLoadStyleGuistyle();
    
    printf("INFO: GUI style loaded successfully!\n");
    
    status->completed = true;
    status->success = true;
    
    return NULL;
}

int LoadCustomGuiStyle(){
    StyleLoadStatus styleStatus = {false, false};

    pthread_t styleThread;
    printf("INFO: Creating thread for style loading...\n");

    if (pthread_create(&styleThread, NULL, LoadStyleThreadFunction, &styleStatus) != 0) {
        fprintf(stderr, "ERROR: Failed to create style loading thread!\n");
        return 1;
    }

    printf("INFO: Waiting for style loading to complete...\n");
    pthread_join(styleThread, NULL);

    if (!styleStatus.completed || !styleStatus.success) {
        fprintf(stderr, "ERROR: Style loading failed!\n");
        return 1;
    }

    printf("INFO: Style loaded successfully, continuing initialization...\n");
    return 0;
}

