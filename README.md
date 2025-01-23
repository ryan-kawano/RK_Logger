# RK Logger

Implementation of a logger in C++. It uses a ```std::queue<std::string>``` to hold log messages. It will continuously check the queue in a loop for new messages and print them when found. If there are no messages, it'll wait until messages are added before proceeding again. Allows logging from multiple files to one central log queue. It will print the time, the thread id, and the function from where the logs were created. The timestamp can be modified to different formats. It will also write the logs to a log file. See the `demonstration` directory for example usage and the section below for example output.

## Example output
```
Getting RK Logger config file
Trying to open RK Logger config file at path "C:\\Users\\user1\\My_Projects\\rk_logger\\src\\demonstration\\rk_config.txt"
Successfully opened the RK Logger config file
Updating config with MM_DD_YYYY (numerical value: 0)
Updating config with MONTH_NUM (numerical value: 0)
[01-19-2025|20:10:02.640][1][main]Inside main
[01-19-2025|20:10:02.640][1][main]The number is 10. The name is Ryan
[01-19-2025|20:10:02.640][1][myFunc]Inside other_file.cpp
[01-19-2025|20:10:02.647][3][operator()]Inside other thread
[01-19-2025|20:10:02.648][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 15
[01-19-2025|20:10:02.648][5][operator()]Random event happened. Printing a log for it. Random events remaining: 15
[01-19-2025|20:10:04.645][5][operator()]Random event happened. Printing a log for it. Random events remaining: 14
[01-19-2025|20:10:05.646][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 14
[01-19-2025|20:10:05.646][5][operator()]Random event happened. Printing a log for it. Random events remaining: 13
[01-19-2025|20:10:08.653][5][operator()]Random event happened. Printing a log for it. Random events remaining: 12
[01-19-2025|20:10:08.653][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 13
[01-19-2025|20:10:11.660][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 12
[01-19-2025|20:10:11.660][5][operator()]Random event happened. Printing a log for it. Random events remaining: 11
[01-19-2025|20:10:12.661][5][operator()]Random event happened. Printing a log for it. Random events remaining: 10
[01-19-2025|20:10:14.662][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 11
[01-19-2025|20:10:14.662][5][operator()]Random event happened. Printing a log for it. Random events remaining: 9
[01-19-2025|20:10:17.667][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 10
[01-19-2025|20:10:17.667][5][operator()]Random event happened. Printing a log for it. Random events remaining: 8
[01-19-2025|20:10:17.668][5][operator()]Random event happened. Printing a log for it. Random events remaining: 7
[01-19-2025|20:10:20.679][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 9
[01-19-2025|20:10:22.682][5][operator()]Random event happened. Printing a log for it. Random events remaining: 6
[01-19-2025|20:10:23.680][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 8
[01-19-2025|20:10:24.686][5][operator()]Random event happened. Printing a log for it. Random events remaining: 5
[01-19-2025|20:10:26.696][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 7
[01-19-2025|20:10:29.701][5][operator()]Random event happened. Printing a log for it. Random events remaining: 4
[01-19-2025|20:10:29.701][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 6
[01-19-2025|20:10:29.702][5][operator()]Random event happened. Printing a log for it. Random events remaining: 3
[01-19-2025|20:10:30.715][5][operator()]Random event happened. Printing a log for it. Random events remaining: 2
[01-19-2025|20:10:30.715][5][operator()]Random event happened. Printing a log for it. Random events remaining: 1
[01-19-2025|20:10:32.712][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 5
[01-19-2025|20:10:35.718][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 4
[01-19-2025|20:10:38.733][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 3
[01-19-2025|20:10:41.745][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 2
[01-19-2025|20:10:44.749][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 1
```
## Usage:
The project can be compiled as a static library with the provided CMake file or compiled manually. The source code can also be used directly in a project and compiled with that project. See the `demonstration` directory for an example of how to use the logger.

## Notes:
1. It will not add new lines at the end of messages, so they must be added explicitly.
