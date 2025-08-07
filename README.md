Here I am building a Library Management system using C++.# Library Management System

## Overview
A C++ application with MySQL integration for managing library operations, featuring both administrative and user-level functionalities through a console interface.

## Technical Specifications

### Core Components
```c++
- Database: MySQL (via mysql.h API)
- Classes: Book, Student, Transaction
- Libraries: 
  - mysqlclient (C API)
  - mysqlcppconn (C++ connector)
  - Standard Library: <iomanip>, <vector>, <ctime>

  lib (
    Name VARCHAR(100) PRIMARY KEY,
    Author VARCHAR(100),
    Category VARCHAR(50),
    Quantity INT
)

student (
    Id VARCHAR(20) PRIMARY KEY,
    Name VARCHAR(100),
    Contact VARCHAR(50)
)

transactions (
    StudentId VARCHAR(20) FOREIGN KEY,
    BookName VARCHAR(100) FOREIGN KEY,
    IssueDate DATE,
    ReturnDate DATE,
    Returned BOOLEAN
)

Features
Admin Functions
✅ Book management (CRUD operations)
✅ Student registration
✅ Transaction monitoring
✅ Data visualization (formatted tables)

User Functions
🔹 Book borrowing/returning
🔹 Availability checks
🔹 Due date tracking

Prerequisites:

sudo apt install libmysqlclient-dev libmysqlcppconn-dev

Compilation:

g++ main.cpp -o library_manager \
  -I/usr/include/mysql \
  -lmysqlclient -lmysqlcppconn -lstdc++

  Execution:

  ./library_manager


Here is the Descriptive Flow of Code->
  https://drive.google.com/file/d/1zYht9IHsNxZj28b8aiojYBP9UY0_vxa1/view?usp=sharing
