#include "../headers/core/guistyle.h"
#include "../headers/guistyleimpl.h"

#include <iostream>
#include <ostream>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

StyleLoad::StyleLoad() {

}

StyleLoad::StyleLoad(const bool completed, const bool success) {
    this->completed = completed;
    this->success = success;
}

void* StyleLoad::LoadStyleThreadFunction(void* arg) {
    auto* status = static_cast<StyleLoad *>(arg);

    std::cout << "INFO: Starting GUI style loading..." << std::endl;
    
    GuiLoadStyleGuistyle();

    std::cout << "INFO: GUI style loaded successfully!" << std::endl;
    
    status->completed = true;
    status->success = true;
    
    return nullptr;
}

int StyleLoad::LoadCustomGuiStyle() {
    StyleLoad styleStatus = {false, false};

    pthread_t styleThread;
    std::cout << "INFO: Creating thread for style loading..." << std::endl;

    if (pthread_create(&styleThread, nullptr, LoadStyleThreadFunction, &styleStatus) != 0) {
        std::cerr << "ERROR: Failed to create style loading thread!" << std::endl;
        return 1;
    }

    std::cout << "INFO: Waiting for style loading to complete..." << std::endl;
    pthread_join(styleThread, nullptr);

    if (!styleStatus.completed || !styleStatus.success) {
        std::cerr << "ERROR: Style loading failed!" << std::endl;
        return 1;
    }

    std::cout << "INFO: Style loaded successfully, continuing initialization..." << std::endl;
    return 0;
}