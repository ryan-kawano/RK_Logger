/**
 * @file file1.h
 * @brief This is meant for demonstrating the use of the logger from a file
 * different than where the logger is instantiated.
 */
#ifndef FILE1_H
#define FILE1_H

#include "rk_logger/logger.h"

namespace file1 {
    
/**
 * @brief Logs a message.
 * 
 * Simply logs a message. This is meant to demonstrate logging from a file different 
 * than where the logger was instantiated.
 */
void myFunc() {
    RK_LOG("Inside other_file.cpp\n");
}

} // namespace file1

#endif
