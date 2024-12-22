/**
 * @file file1.cpp
 * @brief This is meant for demonstrating the use of the logger from a file
 * different than where the logger is instantiated.
 */
#include "logger/log.h"

namespace file1 {
    
/**
 * @brief Logs a message.
 * 
 * Simply logs a message. This is meant to demonstrate logging from a file different 
 * than where the logger was instantiated.
 */
void myFunc() {
    LOG("Inside other_file.cpp\n");
}

} // namespace file1
