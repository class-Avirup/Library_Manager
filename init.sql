-- Create the database schema if it doesn't exist
CREATE DATABASE IF NOT EXISTS library_db;

USE library_db;

CREATE TABLE IF NOT EXISTS lib (
    Name VARCHAR(100) PRIMARY KEY,
    Author VARCHAR(100),
    Category VARCHAR(50),
    Quantity INT
);

CREATE TABLE IF NOT EXISTS student (
    Id VARCHAR(20) PRIMARY KEY,
    Name VARCHAR(100),
    Contact VARCHAR(50)
);

CREATE TABLE IF NOT EXISTS transactions (
    StudentId VARCHAR(20),
    BookName VARCHAR(100),
    IssueDate DATE,
    ReturnDate DATE,
    Returned BOOLEAN,
    FOREIGN KEY (StudentId) REFERENCES student(Id),
    FOREIGN KEY (BookName) REFERENCES lib(Name)
);

-- Optional: Add some initial data
INSERT IGNORE INTO lib VALUES
('The Great Gatsby', 'F. Scott Fitzgerald', 'Classic', 5),
('To Kill a Mockingbird', 'Harper Lee', 'Fiction', 3),
('1984', 'George Orwell', 'Dystopian', 4);