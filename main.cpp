#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <ctime>
using namespace std;

const char* HOST = "mysql-db";
const char* USER = "root";
const char* PW = "root";
const char* DB = "library-db";

// Utility functions
void clearScreen() {
    system("clear");
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

// Classes
class Student {
private:
    string Id;
    string Name;
    string Contact;
public:
    Student() : Id(""), Name(""), Contact("") {}

    void setId(string id) { Id = id; }
    void setName(string name) { Name = name; }
    void setContact(string contact) { Contact = contact; }

    string getId() const { return Id; }
    string getName() const { return Name; }
    string getContact() const { return Contact; }
};

class Book {
private:
    string Name;
    string Author;
    int Quantity;
    string Category;
public:
    Book() : Name(""), Author(""), Quantity(0), Category("") {}

    void setName(string name) { Name = name; }
    void setAuthor(string author) { Author = author; }
    void setQuantity(int quantity) { Quantity = quantity; }
    void setCategory(string category) { Category = category; }

    string getName() const { return Name; }
    string getAuthor() const { return Author; }
    int getQuantity() const { return Quantity; }
    string getCategory() const { return Category; }
};

class Transaction {
private:
    string studentId;
    string bookName;
    string issueDate;
    string returnDate;
    bool returned;
public:
    Transaction(string sid, string bname, string idate, string rdate, bool ret = false)
        : studentId(sid), bookName(bname), issueDate(idate), returnDate(rdate), returned(ret) {}

    string getStudentId() const { return studentId; }
    string getBookName() const { return bookName; }
    string getIssueDate() const { return issueDate; }
    string getReturnDate() const { return returnDate; }
    bool isReturned() const { return returned; }

    void markReturned() { returned = true; }
};

// Database functions
bool executeQuery(MYSQL* conn, const string& query) {
    if (mysql_query(conn, query.c_str())) {
        cout << "Error: " << mysql_error(conn) << endl;
        return false;
    }
    return true;
}

// Admin functions
void addBook(MYSQL* conn) {
    Book b;
    string name, author, category;
    int quantity;

    clearScreen();
    cout << "=== ADD NEW BOOK ===" << endl;
    cout << "Book Name: ";
    cin.ignore();
    getline(cin, name);
    b.setName(name);

    cout << "Author: ";
    getline(cin, author);
    b.setAuthor(author);

    cout << "Category: ";
    getline(cin, category);
    b.setCategory(category);

    cout << "Quantity: ";
    cin >> quantity;
    b.setQuantity(quantity);

    stringstream ss;
    ss << quantity;
    string Sq = ss.str();

    string query = "INSERT INTO lib (Name, Author, Category, Quantity) VALUES('" + 
                   b.getName() + "', '" + b.getAuthor() + "', '" + b.getCategory() + "', '" + Sq + "')";

    if (executeQuery(conn, query)) {
        cout << "\nBook Added Successfully!" << endl;
    }
    pauseScreen();
}

void addStudent(MYSQL* conn) {
    Student s;
    string id, name, contact;

    clearScreen();
    cout << "=== ADD NEW STUDENT ===" << endl;
    cout << "Student ID: ";
    cin >> id;
    s.setId(id);

    cout << "Student Name: ";
    cin.ignore();
    getline(cin, name);
    s.setName(name);

    cout << "Contact Info: ";
    getline(cin, contact);
    s.setContact(contact);

    string query = "INSERT INTO student (Id, Name, Contact) VALUES('" + 
                   s.getId() + "', '" + s.getName() + "', '" + s.getContact() + "')";

    if (executeQuery(conn, query)) {
        cout << "\nStudent Registered Successfully!" << endl;
    }
    pauseScreen();
}

void viewAllBooks(MYSQL* conn) {
    clearScreen();
    cout << "=== AVAILABLE BOOKS ===" << endl;
    cout << setw(20) << left << "Book Name" 
         << setw(20) << "Author" 
         << setw(15) << "Category" 
         << setw(10) << "Quantity" << endl;
    cout << string(65, '-') << endl;

    string query = "SELECT * FROM lib";
    if (executeQuery(conn, query)) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res))) {
                cout << setw(20) << left << row[0]  // Name
                     << setw(20) << row[1]          // Author
                     << setw(15) << row[2]          // Category
                     << setw(10) << row[3] << endl; // Quantity
            }
            mysql_free_result(res);
        }
    }
    pauseScreen();
}

void viewAllStudents(MYSQL* conn) {
    clearScreen();
    cout << "=== REGISTERED STUDENTS ===" << endl;
    cout << setw(15) << left << "Student ID" 
         << setw(25) << "Name" 
         << setw(20) << "Contact" << endl;
    cout << string(60, '-') << endl;

    string query = "SELECT * FROM student";
    if (executeQuery(conn, query)) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res))) {
                cout << setw(15) << left << row[0]  // ID
                     << setw(25) << row[1]          // Name
                     << setw(20) << row[2] << endl; // Contact
            }
            mysql_free_result(res);
        }
    }
    pauseScreen();
}

void viewBorrowedBooks(MYSQL* conn) {
    clearScreen();
    cout << "=== BORROWED BOOKS ===" << endl;
    cout << setw(15) << left << "Student ID" 
         << setw(20) << "Book Name" 
         << setw(15) << "Issue Date" 
         << setw(15) << "Return Date" 
         << setw(10) << "Status" << endl;
    cout << string(75, '-') << endl;

    string query = "SELECT * FROM transactions";
    if (executeQuery(conn, query)) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res))) {
                cout << setw(15) << left << row[0]  // Student ID
                     << setw(20) << row[1]          // Book Name
                     << setw(15) << row[2]          // Issue Date
                     << setw(15) << row[3]          // Return Date
                     << setw(10) << (atoi(row[4]) ? "Returned" : "Borrowed") << endl;
            }
            mysql_free_result(res);
        }
    }
    pauseScreen();
}

void adminMenu(MYSQL* conn) {
    bool exit = false;
    while (!exit) {
        clearScreen();
        cout << "=== ADMINISTRATION MENU ===" << endl;
        cout << "1. Add New Book" << endl;
        cout << "2. View All Books" << endl;
        cout << "3. Register New Student" << endl;
        cout << "4. View All Students" << endl;
        cout << "5. View Borrowed Books" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Enter Choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: addBook(conn); break;
            case 2: viewAllBooks(conn); break;
            case 3: addStudent(conn); break;
            case 4: viewAllStudents(conn); break;
            case 5: viewBorrowedBooks(conn); break;
            case 0: exit = true; break;
            default: cout << "Invalid choice!" << endl; pauseScreen();
        }
    }
}

// User functions
bool studentExists(MYSQL* conn, const string& studentId) {
    string query = "SELECT * FROM student WHERE Id = '" + studentId + "'";
    if (executeQuery(conn, query)) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res) {
            bool exists = mysql_num_rows(res) > 0;
            mysql_free_result(res);
            return exists;
        }
    }
    return false;
}

bool bookExists(MYSQL* conn, const string& bookName) {
    string query = "SELECT * FROM lib WHERE Name = '" + bookName + "'";
    if (executeQuery(conn, query)) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res) {
            bool exists = mysql_num_rows(res) > 0;
            mysql_free_result(res);
            return exists;
        }
    }
    return false;
}

int getBookQuantity(MYSQL* conn, const string& bookName) {
    string query = "SELECT Quantity FROM lib WHERE Name = '" + bookName + "'";
    if (executeQuery(conn, query)) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res) {
            MYSQL_ROW row = mysql_fetch_row(res);
            int quantity = row ? atoi(row[0]) : 0;
            mysql_free_result(res);
            return quantity;
        }
    }
    return 0;
}

void borrowBook(MYSQL* conn, const string& studentId) {
    clearScreen();
    viewAllBooks(conn);
    
    string bookName;
    cout << "\nEnter Book Name to Borrow: ";
    cin.ignore();
    getline(cin, bookName);

    if (!bookExists(conn, bookName)) {
        cout << "Book not found in the library!" << endl;
        pauseScreen();
        return;
    }

    int quantity = getBookQuantity(conn, bookName);
    if (quantity <= 0) {
        cout << "This book is currently out of stock!" << endl;
        pauseScreen();
        return;
    }

    // Get current date (Linux version)
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    char issueDate[20], returnDate[20];
    strftime(issueDate, sizeof(issueDate), "%Y-%m-%d", ltm);
    
    // Calculate return date (1 month later)
    ltm->tm_mon += 1;
    mktime(ltm);  // Normalize the time structure
    strftime(returnDate, sizeof(returnDate), "%Y-%m-%d", ltm);

    // Record transaction
    string transaction = "INSERT INTO transactions (StudentId, BookName, IssueDate, ReturnDate, Returned) VALUES('" + 
                         studentId + "', '" + bookName + "', '" + issueDate + "', '" + returnDate + "', '0')";

    // Update book quantity
    string updateBook = "UPDATE lib SET Quantity = Quantity - 1 WHERE Name = '" + bookName + "'";

    if (executeQuery(conn, transaction) && executeQuery(conn, updateBook)) {
        cout << "\nBook borrowed successfully! Please return by " << returnDate << endl;
    }
    pauseScreen();
}

void returnBook(MYSQL* conn, const string& studentId) {
    clearScreen();
    cout << "=== YOUR BORROWED BOOKS ===" << endl;
    
    string query = "SELECT BookName, IssueDate, ReturnDate FROM transactions WHERE StudentId = '" + 
                   studentId + "' AND Returned = '0'";
    
    vector<string> borrowedBooks;
    
    if (executeQuery(conn, query)) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res) {
            MYSQL_ROW row;
            int count = 0;
            while ((row = mysql_fetch_row(res))) {
                cout << ++count << ". " << row[0] << " (Issued: " << row[1] << ", Due: " << row[2] << ")" << endl;
                borrowedBooks.push_back(row[0]);
            }
            mysql_free_result(res);
        }
    }

    if (borrowedBooks.empty()) {
        cout << "You have no books to return." << endl;
        pauseScreen();
        return;
    }

    cout << "\nEnter the number of the book to return: ";
    int choice;
    cin >> choice;

    if (choice < 1 || choice > borrowedBooks.size()) {
        cout << "Invalid choice!" << endl;
        pauseScreen();
        return;
    }

    string bookName = borrowedBooks[choice - 1];

    // Update transaction
    string updateTrans = "UPDATE transactions SET Returned = '1' WHERE StudentId = '" + 
                         studentId + "' AND BookName = '" + bookName + "' AND Returned = '0'";
    
    // Update book quantity
    string updateBook = "UPDATE lib SET Quantity = Quantity + 1 WHERE Name = '" + bookName + "'";

    if (executeQuery(conn, updateTrans) && executeQuery(conn, updateBook)) {
        cout << "\nBook returned successfully!" << endl;
    }
    pauseScreen();
}

void userMenu(MYSQL* conn) {
    clearScreen();
    string studentId;
    cout << "Enter Your Student ID: ";
    cin >> studentId;

    if (!studentExists(conn, studentId)) {
        cout << "Student ID not found. Please register first." << endl;
        pauseScreen();
        return;
    }

    bool exit = false;
    while (!exit) {
        clearScreen();
        cout << "=== USER MENU ===" << endl;
        cout << "1. View Available Books" << endl;
        cout << "2. Borrow a Book" << endl;
        cout << "3. Return a Book" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "Enter Choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: viewAllBooks(conn); break;
            case 2: borrowBook(conn, studentId); break;
            case 3: returnBook(conn, studentId); break;
            case 0: exit = true; break;
            default: cout << "Invalid choice!" << endl; pauseScreen();
        }
    }
}

// Main function
int main() {
    MYSQL* conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, HOST, USER, PW, DB, 3306, NULL, 0)) {
        cout << "Error: " << mysql_error(conn) << endl;
        return 1;
    }

    // Create tables if they don't exist
    string createLibTable = "CREATE TABLE IF NOT EXISTS lib ("
                           "Name VARCHAR(100) PRIMARY KEY, "
                           "Author VARCHAR(100), "
                           "Category VARCHAR(50), "
                           "Quantity INT)";
    
    string createStudentTable = "CREATE TABLE IF NOT EXISTS student ("
                               "Id VARCHAR(20) PRIMARY KEY, "
                               "Name VARCHAR(100), "
                               "Contact VARCHAR(50))";
    
    string createTransTable = "CREATE TABLE IF NOT EXISTS transactions ("
                             "StudentId VARCHAR(20), "
                             "BookName VARCHAR(100), "
                             "IssueDate DATE, "
                             "ReturnDate DATE, "
                             "Returned BOOLEAN, "
                             "FOREIGN KEY (StudentId) REFERENCES student(Id), "
                             "FOREIGN KEY (BookName) REFERENCES lib(Name))";
    
    executeQuery(conn, createLibTable);
    executeQuery(conn, createStudentTable);
    executeQuery(conn, createTransTable);

    bool exit = false;
    while (!exit) {
        clearScreen();
        cout << "=== LIBRARY MANAGEMENT SYSTEM ===" << endl;
        cout << "1. Administration" << endl;
        cout << "2. User" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter Choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: adminMenu(conn); break;
            case 2: userMenu(conn); break;
            case 0: exit = true; break;
            default: cout << "Invalid choice!" << endl; pauseScreen();
        }
    }

    mysql_close(conn);
    cout << "Goodbye!" << endl;
    return 0;
}