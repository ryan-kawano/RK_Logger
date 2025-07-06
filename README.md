<a id="readme-top"></a>

[![Contributors][contributors-shield]][contributors-url]
[![Issues][issues-shield]][issues-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/ryan-kawano/RK_Logger">
    <img src="images/rk_logger_logo.png" alt="Logo" width="300" height="300">
  </a>

<h3 align="center">RK Logger</h3>

  <p align="center">
    A logging library written in C++.
    <br />
    <a href="#sample-output">View Sample Output</a>
    &middot;
    <a href="https://github.com/ryan-kawano/RK_Logger/issues/new?labels=bug&template=bug-report---.md">Report Bug</a>
    &middot;
    <a href="https://github.com/ryan-kawano/RK_Logger/issues/new?labels=enhancement&template=feature-request---.md">Request Feature</a>
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#features">Features</a>
    </li>
    <li>
        <a href="#sample-output">Sample Output</a>
    </li>
    <li>
        <a href="#built-with">Built With</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->

## About The Project

<strong>RK Logger</strong> is a logging library written in C++. It provides a standardized and convenient way of logging messages in applications.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Features

- <strong>Helpful Debugging Info</strong> - Prints helpful debugging info such as:
  - Timestamps.
  - Thread IDs.
  - Function names.
- <strong>Runtime Configuration File</strong> - Settings can be changed at runtime via a config file. Configurable settings include:
  - Month Format, i.e., `Jan` vs `01`.
  - Date Format, i.e., `MMDDYYYY` vs `YYYYMMDD`
  - Hour Format, i.e., `12 hour format` vs `24 hour format`.
  - Write to Log File, i.e., enable or disable log file output.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Sample output

<details>
<summary><strong>Click to view</strong></summary>

```text
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

</details>

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Built With

- [![C++][C++]][C++-url]
- [![CMake][CMake]][CMake-url]
- [![Google-Test][Google-Test]][Google-Test-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->

## Getting Started

### Prerequisites

- [CMake](https://cmake.org/download/) - Recommended. Technically, the project is buildable without CMake, but it can be tedious.
- A build system such as Make, Ninja, etc.
- A C++ compiler.

### Installation

1. Clone this repo as a submodule of your project.
2. Include the submodule as a project in your project's CMakeLists.txt file. See [src/demonstration/CMakeLists.txt](/src/demonstration/CMakeLists.txt) for an example.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->

## Usage

See the [demonstration](/src/demonstration) directory for an example project that uses the logger or see the example below.

<strong>Example usage:</strong>

Start the logger:

`std::thread logThread = rk::log::startLogger();`

Log a message using the `RK_LOG` macro:

`RK_LOG("Inside main\n");`

The macro can take multiple arguments separated by commas:

```
int number = 10;
std::string name = "Ryan";
RK_LOG("The number is ", number, ". The name is ", name, "\n");
```

The logger will not add newlines automatically at the end of messages, so they must be added explicitly.

Stop the logger at the end of the program using the thread that was returned from `rk::log::startLogger()`:

`rk::log::stopLogger(std::move(logThread));`

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->

## Acknowledgments

- [Best-README-Template](https://github.com/othneildrew/Best-README-Template) for this README template.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[contributors-shield]: https://img.shields.io/github/contributors/ryan-kawano/RK_Logger.svg?style=for-the-badge
[contributors-url]: https://github.com/ryan-kawano/RK_Logger/graphs/contributors
[issues-shield]: https://img.shields.io/github/issues/ryan-kawano/RK_Logger.svg?style=for-the-badge
[issues-url]: https://github.com/ryan-kawano/RK_Logger/issues
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: http://www.linkedin.com/in/ryan-kawano-74bab922b
[product-screenshot]: images/screenshot.png
[C++]: https://img.shields.io/badge/-C++-blue?logo=cplusplus
[C++-url]: https://en.wikipedia.org/wiki/C%2B%2B
[CMake]: https://img.shields.io/badge/CMake-064F8C?logo=cmake&logoColor=fff
[CMake-url]: https://cmake.org/
[Google-Test]: https://img.shields.io/badge/GoogleTest-blue.svg?style=flat-square
[Google-Test-url]: https://google.github.io/googletest/
