#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

// Book categories
enum class Category {
    MATHEMATICS,
    COMPUTER_SCIENCE,
    PHYSICS,
    LITERATURE,
    OTHERS
};

// Convert Category to string for display and storage
string categoryToString(Category cat) {
    switch(cat) {
        case Category::MATHEMATICS: return "Mathematics";
        case Category::COMPUTER_SCIENCE: return "Computer Science";
        case Category::PHYSICS: return "Physics";
        case Category::LITERATURE: return "Literature";
        case Category::OTHERS: return "Others";
        default: return "Unknown";
    }
}

// Book structure with more details
struct Book {
    string name;
    string author;
    string isbn;
    Category category;
    int quantity;
    vector<string> issuedTo;  // Can be issued to multiple users
    
    Book() : name(""), author(""), isbn(""), category(Category::OTHERS), quantity(0) {} // Default constructor
    Book(string n, string a, string i, Category c, int q = 1) 
        : name(n), author(a), isbn(i), category(c), quantity(q) {}
};

// User structure to track borrowed books
struct User {
    string name;
    vector<string> borrowedBooks;  // Store ISBNs of borrowed books
    
    User() : name("") {} // Default constructor
    User(string n) : name(n) {}
};

// Library class with enhanced features
class Library {
private:
    map<string, Book> books;  // ISBN -> Book
    map<string, User> users;  // username -> User
    string bookFile = "books.txt";
    string userFile = "users.txt";

    void saveBooks() {
        ofstream file(bookFile);
        if (file.is_open()) {
            for (const auto& pair : books) {
                const Book& book = pair.second;
                file << book.isbn << "|" 
                     << book.name << "|" 
                     << book.author << "|"
                     << static_cast<int>(book.category) << "|"
                     << book.quantity << "|";
                
                // Save issued books
                for (const string& user : book.issuedTo) {
                    file << user << ",";
                }
                file << "\n";
            }
            file.close();
        }
    }

    void saveUsers() {
        ofstream file(userFile);
        if (file.is_open()) {
            for (const auto& pair : users) {
                const User& user = pair.second;
                file << user.name << "|";
                for (const string& book : user.borrowedBooks) {
                    file << book << ",";
                }
                file << "\n";
            }
            file.close();
        }
    }

    void loadBooks() {
        ifstream file(bookFile);
        string line;
        
        if (file.is_open()) {
            while (getline(file, line)) {
                stringstream ss(line);
                string isbn, name, author, categoryStr, quantityStr, issuedToStr;
                
                getline(ss, isbn, '|');
                getline(ss, name, '|');
                getline(ss, author, '|');
                getline(ss, categoryStr, '|');
                getline(ss, quantityStr, '|');
                getline(ss, issuedToStr);

                Category category = static_cast<Category>(stoi(categoryStr));
                int quantity = stoi(quantityStr);

                Book book(name, author, isbn, category, quantity);

                // Parse issued books
                stringstream issuesSS(issuedToStr);
                string user;
                while (getline(issuesSS, user, ',')) {
                    if (!user.empty()) {
                        book.issuedTo.push_back(user);
                    }
                }

                books[isbn] = book;
            }
            file.close();
        }
    }

    void loadUsers() {
        ifstream file(userFile);
        string line;
        
        if (file.is_open()) {
            while (getline(file, line)) {
                stringstream ss(line);
                string name, booksStr;
                
                getline(ss, name, '|');
                getline(ss, booksStr);

                User user(name);

                // Parse borrowed books
                stringstream booksSS(booksStr);
                string book;
                while (getline(booksSS, book, ',')) {
                    if (!book.empty()) {
                        user.borrowedBooks.push_back(book);
                    }
                }

                users[name] = user;
            }
            file.close();
        }
    }

public:
    Library() {
        loadBooks();
        loadUsers();
    }

    void addBook(string name, string author, string isbn, Category category, int quantity = 1) {
        if (books.find(isbn) != books.end()) {
            books[isbn].quantity += quantity;
        } else {
            books[isbn] = Book(name, author, isbn, category, quantity);
        }
        saveBooks();
        cout << "Book added successfully!\n";
    }

    void displayBooks(Category category = Category::OTHERS, bool showAll = true) {
        if (books.empty()) {
            cout << "No books in library.\n";
            return;
        }

        cout << "\nLibrary Books:\n";
        cout << string(50, '-') << "\n";
        
        for (const auto& pair : books) {
            const Book& book = pair.second;
            if (showAll || book.category == category) {
                cout << "ISBN: " << book.isbn << "\n"
                     << "Title: " << book.name << "\n"
                     << "Author: " << book.author << "\n"
                     << "Category: " << categoryToString(book.category) << "\n"
                     << "Available: " << (book.quantity - book.issuedTo.size()) << "/"
                     << book.quantity << "\n";
                if (!book.issuedTo.empty()) {
                    cout << "Issued to: ";
                    for (const string& user : book.issuedTo) {
                        cout << user << ", ";
                    }
                    cout << "\n";
                }
                cout << string(50, '-') << "\n";
            }
        }
    }

    void searchBooks(string query) {
        cout << "\nSearch Results:\n";
        cout << string(50, '-') << "\n";
        
        bool found = false;
        for (const auto& pair : books) {
            const Book& book = pair.second;
            // Convert both strings to lowercase for case-insensitive search
            string lowerQuery = query;
            string lowerName = book.name;
            string lowerAuthor = book.author;
            transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
            transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            transform(lowerAuthor.begin(), lowerAuthor.end(), lowerAuthor.begin(), ::tolower);
            
            if (lowerName.find(lowerQuery) != string::npos || 
                lowerAuthor.find(lowerQuery) != string::npos ||
                book.isbn.find(query) != string::npos) {
                cout << "ISBN: " << book.isbn << "\n"
                     << "Title: " << book.name << "\n"
                     << "Author: " << book.author << "\n"
                     << "Category: " << categoryToString(book.category) << "\n"
                     << "Available: " << (book.quantity - book.issuedTo.size()) 
                     << "/" << book.quantity << "\n"
                     << string(50, '-') << "\n";
                found = true;
            }
        }
        
        if (!found) {
            cout << "No books found matching '" << query << "'\n";
        }
    }

    void issueBook(string isbn, string userName) {
        if (books.find(isbn) == books.end()) {
            cout << "Book not found.\n";
            return;
        }

        Book& book = books[isbn];
        if (book.issuedTo.size() >= book.quantity) {
            cout << "No copies available for issue.\n";
            return;
        }

        // Create user if doesn't exist
        if (users.find(userName) == users.end()) {
            users[userName] = User(userName);
        }

        User& user = users[userName];
        
        // Check if user already has this book
        if (find(user.borrowedBooks.begin(), user.borrowedBooks.end(), isbn) 
            != user.borrowedBooks.end()) {
            cout << "User already has this book.\n";
            return;
        }

        book.issuedTo.push_back(userName);
        user.borrowedBooks.push_back(isbn);
        
        saveBooks();
        saveUsers();
        cout << "Book issued successfully!\n";
    }

    void returnBook(string isbn, string userName) {
        if (books.find(isbn) == books.end() || users.find(userName) == users.end()) {
            cout << "Book or user not found.\n";
            return;
        }

        Book& book = books[isbn];
        User& user = users[userName];

        // Remove book from user's borrowed books
        auto it = find(user.borrowedBooks.begin(), user.borrowedBooks.end(), isbn);
        if (it != user.borrowedBooks.end()) {
            user.borrowedBooks.erase(it);
            
            // Remove user from book's issued list
            auto userIt = find(book.issuedTo.begin(), book.issuedTo.end(), userName);
            if (userIt != book.issuedTo.end()) {
                book.issuedTo.erase(userIt);
            }
            
            saveBooks();
            saveUsers();
            cout << "Book returned successfully!\n";
        } else {
            cout << "User has not borrowed this book.\n";
        }
    }

    void displayUserBooks(string userName) {
        if (users.find(userName) == users.end()) {
            cout << "User not found.\n";
            return;
        }

        const User& user = users[userName];
        if (user.borrowedBooks.empty()) {
            cout << "User has no borrowed books.\n";
            return;
        }

        cout << "\nBooks borrowed by " << userName << ":\n";
        cout << string(50, '-') << "\n";
        
        for (const string& isbn : user.borrowedBooks) {
            if (books.find(isbn) != books.end()) {
                const Book& book = books[isbn];
                cout << "ISBN: " << book.isbn << "\n"
                     << "Title: " << book.name << "\n"
                     << "Author: " << book.author << "\n"
                     << "Category: " << categoryToString(book.category) << "\n"
                     << string(50, '-') << "\n";
            }
        }
    }
};

int main() {
    Library library;
    int choice;
    string bookName, authorName, isbn, userName;
    
    while (true) {
        cout << "\nLibrary Management System\n";
        cout << string(25, '=') << "\n";
        cout << "1. Add Book\n";
        cout << "2. Display All Books\n";
        cout << "3. Display Books by Category\n";
        cout << "4. Search Books\n";
        cout << "5. Issue Book\n";
        cout << "6. Return Book\n";
        cout << "7. Display User's Books\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                cout << "Enter book name: ";
                getline(cin, bookName);
                cout << "Enter author name: ";
                getline(cin, authorName);
                cout << "Enter ISBN: ";
                getline(cin, isbn);
                cout << "Enter category (0-Mathematics, 1-Computer Science, "
                     << "2-Physics, 3-Literature, 4-Others): ";
                int cat;
                cin >> cat;
                cout << "Enter quantity: ";
                int quantity;
                cin >> quantity;
                library.addBook(bookName, authorName, isbn, 
                              static_cast<Category>(cat), quantity);
                break;
            }
            case 2:
                library.displayBooks();
                break;
            case 3: {
                cout << "Enter category (0-Mathematics, 1-Computer Science, "
                     << "2-Physics, 3-Literature, 4-Others): ";
                int cat;
                cin >> cat;
                library.displayBooks(static_cast<Category>(cat), false);
                break;
            }
            case 4: {
                cout << "Enter search term: ";
                string searchTerm;
                getline(cin, searchTerm);
                library.searchBooks(searchTerm);
                break;
            }
            case 5: {
                cout << "Enter ISBN: ";
                getline(cin, isbn);
                cout << "Enter user name: ";
                getline(cin, userName);
                library.issueBook(isbn, userName);
                break;
            }
            case 6: {
                cout << "Enter ISBN: ";
                getline(cin, isbn);
                cout << "Enter user name: ";
                getline(cin, userName);
                library.returnBook(isbn, userName);
                break;
            }
            case 7: {
                cout << "Enter user name: ";
                getline(cin, userName);
                library.displayUserBooks(userName);
                break;
            }
            case 8:
                cout << "Thank you for using the Library Management System!\n";
                return 0;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
}