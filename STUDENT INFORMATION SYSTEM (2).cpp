/******************************************************************************
Program Name:  Student Information System

MEMBERS:
Baguiao, Alanis Marie
Baldovino, Patrick Nicolai 
Pari-an, Carl Joshua 
Yu, Ericson John 
*******************************************************************************/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

void CLS() {
#ifdef _WIN64
    system("cls");
#else
    system("clear");
#endif
}

// Constants
const string adminUsername = "admin";
const string adminPassword = "admin123";
const string studentUsername = "student";
const string studentPassword = "student123";
const string dataFile = "student_records.txt";
const string recycleBinFile = "recycle_bin.txt";

// Structure definition
struct Student {
    int Id, yearLevel;
    string name, address, birthDate, gender;
    float score; // GPA
    Student() : Id(0), yearLevel(0), name(""), address(""), birthDate(""), gender(""), score(0.0f) {}
};

// Function prototypes
void addStudent(Student* records, int& count, bool isAdmin);
void updateStudent(Student* records, int count);
void displayStudent(Student* records, int count);
void deleteStudent(Student* records, int& count, bool isAdmin);
void save(Student* records, int count);
void load(Student* records, int& count, stringstream& errorMessage);
void adminControl(Student* records, int& count, stringstream& errorMessage);
void studentControl(Student* records, int& count, stringstream& errorMessage);
bool authenticate(const string& username, const string& password);
void registration(string& username, string& password);
bool getIntInput(int& value);
bool getFloatInput(float& value);

#include <iostream>
#include <string>

using namespace std;

// Assuming necessary function and struct definitions are included here

int main() {
    // Array to hold student records
    const int maxSize = 1000;
    Student records[maxSize];
    int count = 0;

    // Load existing records
    stringstream loadErrorMessage;


    // Registration
    string newUsername, newPassword;
    cout << "Do you want to register as a new user? (yes/no): ";
    string choice;
    cin >> choice;
    if (choice == "yes") {
        registration(newUsername, newPassword);
        CLS();
        cout << "Registration successful! Please login with your new credentials." << endl;;
        // You can continue with the rest of your program logic here
    } else {
        CLS();
        // Handle the case where the user chooses not to register
        cout << "Okay, let's continue with the existing users." << endl;
        // No need to return 0 here, just continue with the rest of the program
    }

    // Authentication
        string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    // Check if the user is admin or student
    bool isAdmin = false;
        if (authenticate(username, password)) {
            if (username == adminUsername)
                isAdmin = true;
                else
                CLS();
            cout << "Student account successfully logged in!" << endl;
        } else {
            cout << "Invalid username or password. Exiting..." << endl;
                return 1;
}

    // Determine control panel based on user type
    stringstream controlErrorMessage;
    if (isAdmin) {
        CLS();
            load(records, count, loadErrorMessage);
    if (!loadErrorMessage.str().empty()) {
        cout << "Error occurred while loading records:\n" << loadErrorMessage.str() << endl;
        return 1;
    }
        adminControl(records, count, controlErrorMessage);
    } else {
        CLS();
            load(records, count, loadErrorMessage);
    if (!loadErrorMessage.str().empty()) {
        cout << "Error occurred while loading records:\n" << loadErrorMessage.str() << endl;
        return 1;
    }
        studentControl(records, count, controlErrorMessage);
    }

    if (!controlErrorMessage.str().empty()) {
        cout << "Error occurred while operating:\n" << controlErrorMessage.str() << endl;
        return 1;
    }

    return 0;
}

// Add a student record
void addStudent(Student* records, int& count, bool isAdmin) {
    const int maxSize = 1000;
    if (!isAdmin && count >= 1) {
        CLS();
        cout << "You can only create one-time record." << endl;
        return;
    }
    if (count >= maxSize) {
        CLS();
        cout << "Maximum limit reached. Cannot add more students." << endl;
        return;
    }
    CLS();
    cout << "Enter Student Details:" << endl;
    cout << "ID: ";
    cin >> records[count].Id;
    cin.ignore(); // Clear the input buffer
    cout << "Name: ";
    getline(cin, records[count].name);
    cout << "Year Level: ";
    cin >> records[count].yearLevel;
    cin.ignore(); // Clear the input buffer
    cout << "Address: ";
    getline(cin, records[count].address);
    cout << "Birth Date: ";
    getline(cin, records[count].birthDate);
    cout << "Gender: ";
    getline(cin, records[count].gender);
    cout << "GPA Score (float, 2 decimal places precision): ";
    cin >> fixed >> setprecision(2) >> records[count].score;
    CLS();
    cout << "Student added successfully." << endl;
    count++;
}

// Update a student record
void updateStudent(Student* records, int count) {
    int idToUpdate;
    CLS();
    cout << "Enter the ID of the student to update: ";
    cin >> idToUpdate;

    bool found = false;
    for (int i = 0; i < count; ++i) {
        if (records[i].Id == idToUpdate) {
            found = true;
            cout << "Record found. Update the following fields:" << endl;
            cout << "Name (" << records[i].name << "): ";
            getline(cin, records[i].name); // Update name
            cout << "Year Level (" << records[i].yearLevel << "): ";
            cin >> records[i].yearLevel; // Update year level
            cin.ignore(); // Clear input buffer
            cout << "Address (" << records[i].address << "): ";
            getline(cin, records[i].address); // Update address
            cout << "Birth Date (" << records[i].birthDate << "): ";
            getline(cin, records[i].birthDate); // Update birth date
            cout << "Gender (" << records[i].gender << "): ";
            getline(cin, records[i].gender); // Update gender
            cout << "GPA Score (" << fixed << setprecision(2) << records[i].score << "): ";
            cin >> records[i].score; // Update GPA score
            CLS();
            cout << "Record updated successfully." << endl;
            break; // No need to continue searching
        }
    }

    if (!found) {
        CLS();
        cout << "Student with ID " << idToUpdate << " not found." << endl;
    }
}

// Display all student records in table format
void displayStudent(Student* records, int count) {
    if (count == 0) {
        CLS();
        cout << "No records to display." << endl;
        return;
    }

    // Limit for the address column width
    const int maxAddressWidth = 60;
    CLS();
    // Print table header
    cout << setw(10) << "ID" << setw(30) << "Name" << setw(12) << "Year Level" << setw(maxAddressWidth + 2) << "Address"
         << setw(20) << "Birth Date" << setw(10) << "Gender" << setw(8) << "GPA" << endl;

    // Print table separator
    cout << setfill('-') << setw(10 + 30 + 12 + maxAddressWidth + 2 + 20 + 10 + 8) << "-" << setfill(' ') << endl;

    // Print student records
    for (int i = 0; i < count; ++i) {
        // Truncate address if it exceeds the maximum width
        string truncatedAddress = records[i].address.substr(0, maxAddressWidth);
        if (records[i].address.length() > maxAddressWidth) {
            // Add ellipsis (...) to indicate truncation
            truncatedAddress += "...";
        }

        cout << setw(10) << records[i].Id
             << setw(30) << records[i].name
             << setw(12) << records[i].yearLevel
             << setw(maxAddressWidth + 2) << truncatedAddress
             << setw(20) << records[i].birthDate
             << setw(10) << records[i].gender
             << setw(8) << fixed << setprecision(2) << records[i].score << endl;
    }

    // Ask if the user wants to go back to the menu
    cout << "\nDo you want to go back to the menu? (yes/no): ";
    string choice;
    cin >> choice;
    if (choice == "yes") {
        // Clear input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        // Wait for user to press Enter before clearing screen
        cout << "Press Enter to go back to the menu...";
        cin.get();
        // Clear screen
        CLS();
    }
}



// Delete a student record
void deleteStudent(Student* records, int& count, bool isAdmin) {
        if (count == 0) {
        CLS();
        cout << "No records to delete." << endl;
        return;
    }

    int idToDelete;
        CLS();
    cout << "Enter the ID of the student to delete: ";
    cin >> idToDelete;

    bool found = false;
    for (int i = 0; i < count; ++i) {
        if (records[i].Id == idToDelete) {
            found = true;
            if (isAdmin) {
                // Hard delete
                // Move the last record to the position of the record to be deleted
                records[i] = records[count - 1];
                cout << "Record deleted successfully." << endl;
            } else {
                // Soft delete
                // Append the soft deleted record to the recycle bin file
                ofstream recycleBin(recycleBinFile, ios::app);
                if (recycleBin.is_open()) {
                    recycleBin << records[i].Id << ","
                               << records[i].name << ","
                               << records[i].yearLevel << ","
                               << records[i].address << ","
                               << records[i].birthDate << ","
                               << records[i].gender << ","
                               << fixed << setprecision(2) << records[i].score << endl;
                    recycleBin.close();
                    cout << "Record moved to recycle bin." << endl;
                } else {
                    cout << "Error: Unable to open recycle bin file." << endl;
                }
            }
            // Remove the soft deleted record from the active records
            count--;
            break;
        }
    }

    if (!found) {
        CLS();
        cout << "Student with ID " << idToDelete << " not found." << endl;
    }
}

// Permanently delete records from the recycle bin
void deleteFromRecycleBin() {
    CLS();
    ofstream recycleBin(recycleBinFile);
    if (recycleBin.is_open()) {
        recycleBin << ""; // Clear the recycle bin file
        recycleBin.close();
        cout << "Recycle bin cleared successfully." << endl;
    } else {
        cout << "Error: Unable to open recycle bin file." << endl;
    }
}

// Retrieve soft deleted records from the recycle bin
void retrieveFromRecycleBin(Student* records, int& count) {
    CLS();
    ifstream recycleBin(recycleBinFile); // Open the recycle bin file
    if (recycleBin.is_open()) {
        Student student;
        while (recycleBin >> student.Id) {
            // Read the remaining fields directly from the file
            recycleBin.ignore(); // Ignore the comma
            getline(recycleBin, student.name, ',');
            recycleBin >> student.yearLevel;
            recycleBin.ignore(); // Ignore the comma
            getline(recycleBin, student.address, ',');
            getline(recycleBin, student.birthDate, ',');
            getline(recycleBin, student.gender, ',');
            recycleBin >> student.score;

            // Add the retrieved record to the active records
            records[count] = student;
            count++;
        }
        recycleBin.close(); // Close the recycle bin file after retrieval
        deleteFromRecycleBin(); // Clear the recycle bin file after retrieval
        cout << "Records retrieved from recycle bin successfully." << endl;
    } else {
        cout << "Error: Unable to open recycle bin file." << endl;
    }
}

// Save student records to a file
void save(Student* records, int count) {
        ofstream outFile(dataFile);  // Fixed
    if (outFile.is_open()) {
        for (int i = 0; i < count; ++i) {
            outFile << records[i].Id << ","
                    << records[i].name << ","
                    << records[i].yearLevel << ","
                    << records[i].address << ","
                    << records[i].birthDate << ","
                    << records[i].gender << ","
                    << fixed << setprecision(2) << records[i].score << endl;
        }
        outFile.close();
        cout << "Records saved successfully." << endl;
    } else {
        cout << "Error: Unable to open file for writing." << endl;
    }
}

// Load student records from a file
void load(Student* records, int& count, stringstream& errorMessage) {
    ifstream inFile(dataFile);
    if (inFile.is_open()) {
        Student student;
        while (inFile >> student.Id) {
            inFile.ignore(); // Ignore the comma
            getline(inFile, student.name, ',');
            inFile >> student.yearLevel;
            inFile.ignore(); // Ignore the comma
            getline(inFile, student.address, ',');
            getline(inFile, student.birthDate, ',');
            getline(inFile, student.gender, ',');
            inFile >> student.score;
            inFile.ignore(); // Ignore the newline character

            // Add the loaded record to the array
            records[count] = student;
            count++;
        }
        inFile.close();
        cout << "Records loaded successfully." << endl;
    } else {
        cout << "\tNo existing records found." << endl;
    }
}

// Admin control panel
void adminControl(Student* records, int& count, stringstream& errorMessage) {
    int choice;
    do {
        cout << "\n\tAdmin Control Panel" << endl;
        cout << "\t--------------------------------------------------" << endl;
        cout << "\t[1] Add Student" << endl;
        cout << "\t[2] Update Student" << endl;
        cout << "\t[3] Display Student" << endl;
        cout << "\t[4] Delete Student" << endl;
        cout << "\t[5] Save Records" << endl;
        cout << "\t[6] Permanently Delete Records from Recycle Bin" << endl;
        cout << "\t[7] Retrieve Records from Recycle Bin" << endl;
        cout << "\t[8] Exit" << endl;
        cout << "\t-------------------------------------------------" << endl;
        cout << "\tEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addStudent(records, count, true);
                break;
            case 2:
                updateStudent(records, count);
                break;
            case 3:
                displayStudent(records, count);
                break;
            case 4:
                deleteStudent(records, count, true);
                break;
            case 5:
                save(records, count);
                break;
            case 6:
                deleteFromRecycleBin();
                break;
            case 7:
                retrieveFromRecycleBin(records, count);
                break;
            case 8:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    } while (choice != 8);
}

// Student control panel
void studentControl(Student* records, int& count, stringstream& errorMessage) {
    bool createdRecord = false; // Flag to track if the student has created a record
    int choice;

    // Check if the student has already created a record
    if (count == 0) {
        // If not, allow the student to create one record
        addStudent(records, count, false);
        createdRecord = true;
    }

    // Display options for updating the record or exiting
    do {
        cout << "\n\tStudent Control Panel" << endl;
        cout << "\t----------------------------------" << endl;
        cout << "\t[1] Update My Information" << endl;
        cout << "\t[2] Show My Record" << endl;
        cout << "\t[3] Exit" << endl;
        cout << "\t----------------------------------" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (createdRecord) {
                    // If the record has been created, allow the student to update it
                    updateStudent(records, count);
                    save(records, count);
                } else {
                    cout << "You have not created a record yet." << endl;
                }
                break;
            case 2:
                if (createdRecord) {
                    // If the record has been created, display it
                    cout << "\nYour Record:" << endl;
                    displayStudent(records, count);
                } else {
                    cout << "You have not created a record yet." << endl;
                }
                break;
            case 3:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    } while (choice != 3);
}

// Authenticate username and password

bool authenticate(const string& username, const string& password) {
    if (username == adminUsername && password == adminPassword) {
        return true; // Admin credentials match
    } else {
        string storedUsername, storedPassword;
        ifstream userFile("users.txt");
        if (userFile.is_open()) {
            while (userFile >> storedUsername >> storedPassword) {
                if (username == storedUsername && password == storedPassword) {
                    userFile.close();
                    return true; // Regular user credentials match
                }
            }
            userFile.close();
        } else {
            cout << "Error: Unable to open user file for reading." << endl;
        }
        return false; // No matching credentials found
    }
}

// Registration function
void registration(string& username, string& password) {
    string confirmPassword;
    cout << "Enter your desired username: ";
    cin >> username;
    cout << "Enter your desired password: ";
    cin >> password;
    cout << "Confirm your password: ";
    cin >> confirmPassword;
    if (password != confirmPassword) {
        cout << "Passwords do not match. Please try again." << endl;
        registration(username, password); // Re-prompt for registration
    } else {
        // Save the username and password to a file for future logins
        ofstream userFile("users.txt", ios::app);
        if (userFile.is_open()) {
            userFile << username << " " << password << endl;
            userFile.close();
            cout << "Registration successful! Username: " << username << ", Password: " << password << endl; // Debug output
        } else {
            cout << "Error: Unable to open file for writing." << endl;
        }
    }
}


// Function to get integer input with error handling
bool getIntInput(int& value) {
    while (!(cin >> value)) {
        cout << "Invalid input. Please enter an integer: ";
        cin.clear(); // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
    }
    return true;
}

// Function to get float input with error handling
bool getFloatInput(float& value) {
        while (!(cin >> value)) {
        cout << "Invalid input. Please enter a floating-point number: ";
        cin.clear(); // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
    }
    return true;
}
