# Library Management System

A robust C++ console application for managing library operations including book inventory, user management, and book lending functionality.

## Features

- Book Management
  - Add new books with details (title, author, ISBN, category, quantity)
  - Display all books in the library
  - Filter books by category
  - Search books by title, author, or ISBN
  - Track available copies of each book

- User Management
  - Automatic user creation when issuing books
  - Track borrowed books for each user
  - Display user's currently borrowed books

- Book Lending
  - Issue books to users
  - Return books from users
  - Prevent duplicate book issuance
  - Track book availability

- Data Persistence
  - Automatic saving of book data to `books.txt`
  - Automatic saving of user data to `users.txt`
  - Data loading on program startup

## Book Categories

- Mathematics
- Computer Science
- Physics
- Literature
- Others

## Technical Details

### Dependencies
- C++ Standard Library
- File I/O streams
- STL Containers (map, vector)
- String manipulation

### Data Structures
- `Book`: Stores book information including:
  - Name
  - Author
  - ISBN
  - Category
  - Quantity
  - List of users who have borrowed copies

- `User`: Stores user information including:
  - Name
  - List of borrowed book ISBNs

### File Format

#### books.txt
```
ISBN|Title|Author|CategoryID|Quantity|IssuedTo1,IssuedTo2,...
```

#### users.txt
```
Username|BorrowedISBN1,BorrowedISBN2,...
```

## Usage

1. Compile the program:
```bash
g++ library_management.cpp -o library
```

2. Run the executable:
```bash
./library
```

3. Use the menu options:
   - 1: Add a new book
   - 2: Display all books
   - 3: Display books by category
   - 4: Search for books
   - 5: Issue a book
   - 6: Return a book
   - 7: Display user's borrowed books
   - 8: Exit

## Error Handling

The system includes error checking for:
- Invalid book/user lookups
- Duplicate book issuance
- Book availability
- Invalid menu choices

## Future Improvements

Potential enhancements could include:
- User authentication
- Fine calculation for overdue books
- Book reservation system
- Enhanced search capabilities
- GUI interface
- Database integration instead of file storage
- Multi-user concurrent access support

## Contributing

Feel free to fork this project and submit pull requests for any improvements.

## License

This project is open source and available under the MIT License.