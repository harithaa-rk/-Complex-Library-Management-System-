 #include <iostream>
 #include <fstream>
 #include <sstream>
 #include <vector>
 #include <ctime>
 #include <string>
 #include <algorithm>
 using namespace std;
 class LibraryItem {
 protected:
 string title;
 string author;
 int itemID;
 bool availability;
 public:
 // Base class: LibraryItem
 LibraryItem(const string& _title, const string& _author, int _itemID, bool _availability = true)
 : title(_title), author(_author), itemID(_itemID), availability(_availability) {}
 virtual ~LibraryItem() {}
 string getTitle() const { return title; }
 string getAuthor() const { return author; }
 int getItemID() const { return itemID; }
 bool isAvailable() const { return availability; }
 void setTitle(const string& _title) { title = _title; }
 void setAuthor(const string& _author) { author = _author; }
 void setItemID(int _itemID) { itemID = _itemID; }
 void setAvailability(bool _availability) { availability = _availability; }
 virtual void displayDetails() const {
 cout << "Title: " << title << endl;
 cout << "Author: " << author << endl;
 cout << "Item ID: " << itemID << endl;
 cout << "Availability: " << (availability ? "Available" : "Not Available") << endl;
 }
 virtual void saveToFile(ofstream& file) const = 0;       
// Pure virtual function(polymorphism) to save to file
class Book : public LibraryItem {                                  
// Derived class: Book
 private:
 string genre;                                                               
int pages;
 public:
 //genre , pages are private members
 Book(const string& _title, const string& _author, int _itemID, const string& _genre, int _pages, 
bool _availability = true)
 : LibraryItem(_title, _author, _itemID, _availability), genre(_genre), pages(_pages) {}
 // constructor to initialise libraryItem’s genre and pages
 void displayDetails() const override                         
{
 LibraryItem::displayDetails();
 cout << "Genre: " << genre << endl;
 cout << "Pages: " << pages << endl;
 }
 void saveToFile(ofstream& file) const override {
 //override - calls base class 
file << title << ";" << author << ";" << itemID << ";" << genre << ";" << pages << ";" << 
availability << endl;
 }
 };
 class AudioCD : public LibraryItem {
 private:
 string duration;                                                        
string artist;
 public:
 // Derived class: AudioCD
 //duration and artist are private members
 AudioCD(const string& _title, const string& _author, int _itemID, const string& _duration, 
const string& _artist, bool
 _availability = true)
 : LibraryItem(_title, _author, _itemID, _availability), duration(_duration), artist(_artist) {}
 void displayDetails() const override {
 LibraryItem::displayDetails();
 cout << "Duration: " << duration << endl;
 cout << "Artist: " << artist << endl;
 }
 void saveToFile(ofstream& file) const override                               
//overriding to savetofile
{
 file << title << ";" << author << ";" << itemID << ";" << duration << ";" << artist << ";" << 
availability << endl;
 }
 };
 // Base class: User
 class User {
 protected:
 string id;
 string password;
 string role;
 vector<pair<int, time_t>> borrowedItems; // Pair of itemID and expiry date
 double fine;
 public:
 User(const string& _id, const string& _password, const string& _role)
 : id(_id), password(_password), role(_role), fine(0.0) {}  
virtual ~User() {}
 string getId() const { return id; }
 // constructor to initialise role, and password
 string getPassword() const { return password; }
 string getRole() const { return role; }
 void addBorrowedItem(int itemID, time_t expiryDate) {
 borrowedItems.push_back({itemID, expiryDate});
 }
 bool isItemBorrowed(int itemID) const {
 return find_if(borrowedItems.begin(), borrowedItems.end(), [itemID](const pair<int, time_t>& 
p) { return p.first ==
 itemID; }) != borrowedItems.end();
 }
 time_t getExpiryDate(int itemID) const {
 for (const auto& item : borrowedItems) {
 if (item.first == itemID) {
 return item.second;
 }
 }
 return 0;
 }
 void returnItem(int itemID) {
 borrowedItems.erase(remove_if(borrowedItems.begin(), borrowedItems.end(),
 [itemID](const pair<int, time_t>& p) { return p.first == itemID; }), borrowedItems.end());
 }
 void addFine(double amount) {
 fine += amount;
 }
 void displayDetails() const {
 cout << "ID: " << id << endl;
 cout << "Role: " << role << endl;
 cout << "Fine: $" << fine << endl;
 cout << "Borrowed Items:" << endl;
 for (const auto& item : borrowedItems) {
 cout << "Item ID: " << item.first << ", Expiry Date: " << ctime(&item.second);
 }
 }
 };
 // Class: Library
 class Library {
 private:
 vector<LibraryItem*> items;                                               
public:
 Library() {
 loadItems();
 }
 ~Library() {
 saveItems();
 for (auto& item : items) {
 delete item;
 }
 }
 void loadItems() {
 ifstream bookFile("books.txt");
 ifstream audioCDFile("audioCDs.txt");
 string line;
 while (getline(bookFile, line)) {
 istringstream iss(line);
 string title, author, genre, availabilityStr;
 int itemID, pages;
 bool availability;
 char delim;
 if (getline(iss, title, ';') && getline(iss, author, ';') && (iss >> itemID >> delim) && getline(iss, 
genre, ';') && (iss >>
    pages >> delim) && (iss >> availability)) {
 items.push_back(new Book(title, author, itemID, genre, pages, availability));
 }
 }
 while (getline(audioCDFile, line)) {
 istringstream iss(line);
 string title, author, duration, artist, availabilityStr;
 int itemID;
 bool availability;
 char delim;
 if (getline(iss, title, ';') && getline(iss, author, ';') && (iss >> itemID >> delim) && getline(iss, 
duration, ';') &&
 getline(iss, artist, ';') && (iss >> availability)) {
 items.push_back(new AudioCD(title, author, itemID, duration, artist, availability));
 }
 }
 bookFile.close();
 audioCDFile.close();
 }
 void saveItems() {
 ofstream bookFile("books.txt");
 ofstream audioCDFile("audioCDs.txt");
 for (const auto& item : items) {
 if (dynamic_cast<Book*>(item)) {
 item->saveToFile(bookFile);
 } else if (dynamic_cast<AudioCD*>(item)) {
 item->saveToFile(audioCDFile);
 }
 }
 bookFile.close();
 audioCDFile.close();
 }
 void addItem(LibraryItem* item) {
 items.push_back(item);
 saveItems(); // Save to file immediately after adding
 }
 void removeItem(int itemID) {
 for (auto it = items.begin(); it != items.end(); ++it) {
 if ((*it)->getItemID() == itemID) {
 delete *it;
 items.erase(it);
 cout << "Item with ID " << itemID << " removed from the library." << endl;
 saveItems(); // Save to file immediately after removing
 return;
 }
 }
 cout << "Item with ID " << itemID << " not found in the library." << endl;
 }
 LibraryItem* getItemById(int itemID) {
 for (auto& item : items) {
 if (item->getItemID() == itemID) {
 return item;
 }
 }
 return nullptr;
 }
 void searchItemByTitle(const string& title) const {
 bool found = false;
 for (const auto& item : items) {
 if (item->getTitle() == title) {
 item->displayDetails();
 found = true;
 cout << "-------------------" << endl;
 }
 }
 if (!found) {
 cout << "No items found with the title \"" << title << "\"" << endl;
 }
 }
 void displayAllItems() const {
 if (items.empty()) {
 cout << "No items in the library." << endl;
 } else {
 for (const auto& item : items) {
 item->displayDetails();
 cout << "-------------------" << endl;
 }
 }
 }
 };
 // Function to get current date plus given days
 time_t getExpiryDate(int days) {
 time_t now = time(nullptr);
 return now + days * 24 * 60 * 60;
 }
 // Function to calculate fine if any
 double calculateFine(time_t expiryDate) {
 time_t now = time(nullptr);
 double finePerDay = 1.0; // Example fine per day
 if (now > expiryDate) {
 double daysLate = difftime(now, expiryDate) / (24 * 60 * 60);
 return daysLate * finePerDay;
 }
 return 0.0;
 }
 void loadUsers(vector<User*>& users) {
 ifstream userFile("users.txt");
 string line;
 while (getline(userFile, line)) {
 istringstream iss(line);
 string id, password, role;
 // Function to load users from file
 if (getline(iss, id, ';') && getline(iss, password, ';') && getline(iss, role)) {
 users.push_back(new User(id, password, role));
 }
 }
 userFile.close();
 }
 // Function to save users to file
 void saveUsers(const vector<User*>& users) {
 ofstream userFile("users.txt");
 for (const auto& user : users) {
 userFile << user->getId() << ";" << user->getPassword() << ";" << user->getRole() << endl;
 }
 userFile.close();
 }
 // Function to register a new user
 void registerUser(vector<User*>& users) {
 string id, password, role;
 cout << "Enter new user ID: ";
 cin >> id;
 cout << "Enter new user password: ";
 cin >> password;
 cout << "Enter role (student/librarian): ";
 cin >> role;
 for (const auto& user : users) {
 if (user->getId() == id) {
 // Check if user already exists
 cout << "User ID already exists. Registration failed." << endl;
 return;
 }
 }
 users.push_back(new User(id, password, role));
 saveUsers(users);
 cout << "User registered successfully!" << endl;
 }
 User* login(const vector<User*>& users) {
 string id, password;
 cout << "Enter user ID: ";
 cin >> id;
 cout << "Enter password: ";
 cin >> password;
 for (const auto& user : users) {
 // Function to login
 if (user->getId() == id && user->getPassword() == password) {
 cout << "Login successful!" << endl;
 return user;
 }
 }
 cout << "Login failed. Incorrect ID or password." << endl;
 return nullptr;
 }
 // Main menu function
 void mainMenu(User* loggedInUser, Library& library) {
 int choice;
 do {
 cout << "\n--- Main Menu ---\n";                                         
//To be displayed
 cout << "1. View all items\n";
 cout << "2. Search item by title\n";
 cout << "3. Borrow item\n";
 cout << "4. Return item\n";
 cout << "5. Add new item (Librarians only)\n";
 cout << "6. Remove item (Librarians only)\n";
 cout << "7. View account details\n";
 cout << "8. Logout\n";
 cout << "Enter your choice: ";
 cin >> choice;
 switch (choice) {
 case 1:
 library.displayAllItems();
 break;
 case 2: {
 string title;
 cout << "Enter item title: ";
 cin.ignore();
 getline(cin, title);
 library.searchItemByTitle(title);
 break;
 }
 case 3: {
 int itemID;
 cout << "Enter item ID to borrow: ";
 cin >> itemID;
 LibraryItem* item = library.getItemById(itemID);
 if (item && item->isAvailable()) {
 item->setAvailability(false);
 time_t expiryDate = getExpiryDate(14); // Borrow period of 14 days
 loggedInUser->addBorrowedItem(itemID, expiryDate);
 cout << "Item borrowed successfully! Return by: " << ctime(&expiryDate);
 } else {
 cout << "Item is not available or does not exist." << endl;
 }
 break;
 }
 case 4: {
 int itemID;
 cout << "Enter item ID to return: ";
 cin >> itemID;
 if (loggedInUser->isItemBorrowed(itemID)) {
 time_t expiryDate = loggedInUser->getExpiryDate(itemID);
 double fine = calculateFine(expiryDate);
 if (fine > 0) {
 cout << "Item returned late. Fine: $" << fine << endl;
 loggedInUser->addFine(fine);
 } else {
 cout << "Item returned on time. No fine." << endl;
 }
 library.getItemById(itemID)->setAvailability(true);
 loggedInUser->returnItem(itemID);
 } else {
 cout << "Item not borrowed by you or does not exist." << endl;
 }
 break;
 }
 case 5:
 if (loggedInUser->getRole() == "librarian") {
 int itemType;
 cout << "Add new item:\n1. Book\n2. Audio CD\nEnter your choice: ";
 cin >> itemType;
 if (itemType == 1) {
 string title, author, genre;
 int itemID, pages;
 cout << "Enter title: ";
 cin.ignore();
 getline(cin, title);
 cout << "Enter author: ";
 getline(cin, author);
 cout << "Enter item ID: ";
 cin >> itemID;
 cout << "Enter genre: ";
 cin.ignore();
 getline(cin, genre);
 cout << "Enter pages: ";
 cin >> pages;
 library.addItem(new Book(title, author, itemID, genre, pages));
 cout << "Book added successfully!" << endl;
 } else if (itemType == 2) {
 string title, author, duration, artist;
 int itemID;
 cout << "Enter title: ";
 cin.ignore();
 getline(cin, title);
 cout << "Enter author: ";
 getline(cin, author);
 cout << "Enter item ID: ";
 cin >> itemID;
 cout << "Enter duration: ";
 cin.ignore();
 getline(cin, duration);
 cout << "Enter artist: ";
 getline(cin, artist);
 library.addItem(new AudioCD(title, author, itemID, duration, artist));
 cout << "Audio CD added successfully!" << endl;
 } else {
 cout << "Invalid choice." << endl;
 }
 } else {
 cout << "Permission denied. Only librarians can add items." << endl;
 }
 break;
 case 6:
 if (loggedInUser->getRole() == "librarian") {
 int itemID;
 cout << "Enter item ID to remove: ";
 cin >> itemID;
 library.removeItem(itemID);
 } else {
 cout << "Permission denied. Only librarians can remove items." << endl;
 }
 break;
 case 7:
 loggedInUser->displayDetails();
 break;
 case 8:
 cout << "Logging out...\n";
 break;
 default:
 cout << "Invalid choice. Please try again.\n";
 }
 } while (choice != 8);
 }
 int main() {
 vector<User*> users;
 Library library;
 loadUsers(users);
 int choice;
 do {
 cout << "\n--- Welcome to the Library Management System ---\n";
 cout << "1. Login\n";
 cout << "2. Register\n";
 cout << "3. Exit\n";
 cout << "Enter your choice: ";
 cin >> choice;
 switch (choice) {
 case 1: {
 User* loggedInUser = login(users);
 if (loggedInUser) {
 mainMenu(loggedInUser, library);
 }
 break;
 }
 case 2:
 registerUser(users);
 break;
 case 3:
 cout << "Exiting the system. Goodbye!\n";
 break;
 default:
 cout << "Invalid choice. Please try again.\n";
 }
 } while (choice != 3);
 saveUsers(users);
 for (auto user : users) {
 delete user;
 }
 return 0;
 }
