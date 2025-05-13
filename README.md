# RK Logger

A logger library in C++. Provides a standardized way of logging messages in applications. It prints helpful debugging info such as:
* Timestamp.
* Thread ID.
* Function name from which the message was logged.

Settings can be changed at runtime via a config file. Some settings are:
* Month Format, i.e., `Jan` vs `01`.
* Date Format, i.e., `MMDDYYYY` vs `YYYYMMDD`
* Hour Format, i.e., `12 hour format` vs `24 hour format`.

 It uses a ```std::queue<std::string>``` to hold log messages. It will continuously check the queue in a loop for new messages and print them when found. If there are no messages, it'll wait until messages are added before proceeding again. Allows logging from multiple files to one central log queue. It will print the time, the thread id, and the function from where the logs were created. The timestamp can be modified to different formats. It will also write the logs to a log file. See the `demonstration` directory for example usage and the section below for example output.

## Example output
```
[RKLogger Config] Getting RK Logger config file
[RKLogger Config] Trying to open RK Logger config file at path "C:\\Users\\user1\\rk_logger\\src\\demonstration\\rk_config.txt"
[RKLogger Config] Successfully opened the RK Logger config file
[RKLogger Config] Updating config key "date_format" with value "MM_DD_YYYY"
[RKLogger Config] Updating config key "month_format" with value "MONTH_NUM"
[RKLogger Config] Updating config key "hour_format" with value "12"
[RKLogger Config] Updating config key "write_to_log_file" with value "ENABLE"
[RKLogger Time] Updating timestamp functions
[RKLogger Time] Updating month function
[RKLogger Time] Updating date function
[RKLogger Time] Updating time function
Writing to log file: logs_05-12-2025_07-41-27.004 PM.txt
[05-12-2025|07:41:27.005 PM][1][main]Inside main
[05-12-2025|07:41:27.005 PM][1][main]The number is 10. The name is Ryan
[05-12-2025|07:41:27.005 PM][1][myFunc]Inside other_file.cpp
[05-12-2025|07:41:27.012 PM][3][operator()]Inside other thread
[05-12-2025|07:41:27.013 PM][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 15
[05-12-2025|07:41:27.013 PM][5][operator()]Random event happened. Printing a log for it. Random events remaining: 15
[05-12-2025|07:41:30.018 PM][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 14
[05-12-2025|07:41:31.732 PM][5][operator()]Random event happened. Printing a log for it. Random events remaining: 14
[05-12-2025|07:41:33.025 PM][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 13
[05-12-2025|07:41:33.730 PM][5][operator()]Random event happened. Printing a log for it. Random events remaining: 13
[05-12-2025|07:41:34.937 PM][5][operator()]Random event happened. Printing a log for it. Random events remaining: 12
[05-12-2025|07:41:36.028 PM][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 12
[05-12-2025|07:41:36.644 PM][5][operator()]Random event happened. Printing a log for it. Random events remaining: 11
[05-12-2025|07:41:39.036 PM][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 11
[05-12-2025|07:41:39.305 PM][5][operator()]Random event happened. Printing a log for it. Random events remaining: 10
[05-12-2025|07:41:41.567 PM][5][operator()]Random event happened. Printing a log for it. Random events remaining: 9
[05-12-2025|07:41:42.040 PM][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 10
[05-12-2025|07:41:42.879 PM][5][operator()]Random event happened. Printing a log for it. Random events remaining: 8
[05-12-2025|07:41:45.040 PM][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 9
[05-12-2025|07:41:45.195 PM][5][operator()]Random event happened. Printing a log for it. Random events remaining: 7
[05-12-2025|07:41:46.906 PM][5][operator()]Random event happened. Printing a log for it. Random events remaining: 6
[05-12-2025|07:41:47.183 PM][5][operator()]Random event happened. Printing a log for it. Random events remaining: 5
[05-12-2025|07:41:48.047 PM][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 8
[05-12-2025|07:41:50.518 PM][5][operator()]Random event happened. Printing a log for it. Random events remaining: 4
[05-12-2025|07:41:50.958 PM][5][operator()]Random event happened. Printing a log for it. Random events remaining: 3
[05-12-2025|07:41:51.057 PM][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 7
[05-12-2025|07:41:54.064 PM][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 6
[05-12-2025|07:41:55.726 PM][5][operator()]Random event happened. Printing a log for it. Random events remaining: 2
[05-12-2025|07:41:55.960 PM][5][operator()]Random event happened. Printing a log for it. Random events remaining: 1
[05-12-2025|07:41:57.073 PM][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 5
[05-12-2025|07:42:00.075 PM][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 4
[05-12-2025|07:42:03.076 PM][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 3
[05-12-2025|07:42:06.076 PM][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 2
[05-12-2025|07:42:09.084 PM][4][operator()]Printing a message every 3 seconds. Timer messages remaining: 1
```
## Usage:
The project can be compiled as a static library with the provided CMake file or compiled manually. The source code can also be used directly in a project and compiled with that project. See the `demonstration` directory for an example of how to use the logger.

## Notes:
1. It will not add new lines at the end of messages, so they must be added explicitly.
