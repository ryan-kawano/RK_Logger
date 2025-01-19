# RK Logger

Implementation of a logger in C++. It uses a ```std::queue<std::string>``` to hold log messages. It will continuously check the queue in a loop for new messages and print them when found. Utilizes extern variables to allow logging from multiple files. It will print the thread id and the function from where the logs were created. See the "demonstration" directory for example usage.

## Usage:
1. Determine where the logger will be instantiated. Most likely, this would be main or wherever the central point of the project is. Do the following steps in that file.
2. Import the header file ```log.h```.
3. Call the macro ```LOG_SETUP``` in order to setup some global variables.
4. Call the function ```rk::log::startLogThread()``` in order to start the log thread. This function will return the ```std::thread``` that it created. Save it in a variable in order to end the log thread later.
5. Call the macro ```LOG_VERIFY``` in order to verify that the log file was created and opened.
6. Log messages using the ```RK_LOG``` macro. Example usage:
```
int number = 5;
RK_LOG("The number is: ", number, "\n");
```
If this is run from main(), it will print:
```
[1][main]The number is: 5
```
7. At the end of the program, end the log thread via ```rk::log::endLogThread()```. Pass in the thread that was saved from ```rk::log::startLogThread()```. It will end the log thread and also close the output file.

## Notes:
1. It will not add new lines at the end of messages, so they must be added explicitly.
