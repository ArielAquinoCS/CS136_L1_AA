#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

const int MAX_RECORDS = 100;
const double MIN_PRICE = 12999.00;
const int ID_LENGTH = 8;

enum MenuOption { PRINT_VALID = 1, PRINT_INVALID, QUIT };

struct CarRecord {
    string carId, model, manufacturer;
    int quantity;
    double price;
};

void displayMenu();
void readRecords(CarRecord records[], int &count);
bool isValidCarId(const string &carId);
bool isValidModel(const string &model);
void printValidRecords(const CarRecord records[], int count);
void writeInvalidRecord(const string &line, const string &errors);

int main() {
    CarRecord records[MAX_RECORDS];
    int recordCount = 0;
    readRecords(records, recordCount);

    int choice = 0;
    while (choice != QUIT) {  // Loop naturally until the user chooses to quit
        displayMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number between 1 and 3.\n";
            choice = 0;  // Ensures the loop continues
        } else if (choice == PRINT_VALID) {
            printValidRecords(records, recordCount);
        } else if (choice == PRINT_INVALID) {
            cout << "Check 'error_log.txt' for invalid records.\n";
        } else if (choice != QUIT) {
            cout << "Invalid option! Try again.\n";
        }
    }

    cout << "Exiting program...\n";
    return 0;
}

void displayMenu() {
    cout << "\n\n"
         << "1. Print all valid car records\n"
         << "2. Print invalid records (Check error_log.txt)\n"
         << "3. Quit\n"
         << "Enter your choice: ";
}

void readRecords(CarRecord records[], int &count) {
    ifstream inputFile("car_inventory.txt");
    ofstream errorFile("error_log.txt", ios::trunc);
    bool hasErrors = false;  // Track if errors were logged

    if (!inputFile) {
        cerr << "Error: Input file 'car_inventory.txt' not found!\n";
        cerr << "Please create the file and restart the program.\n";
    } else {
        string line;
        while (getline(inputFile, line) && count < MAX_RECORDS) {
            string carId, model, manufacturer;
            int quantity;
            double price;

            istringstream iss(line);
            iss >> carId >> model >> manufacturer >> quantity >> price;

            string errorMsg;
            if (!isValidCarId(carId)) errorMsg += " Invalid ID;";
            if (!isValidModel(model)) errorMsg += " Invalid Model;";
            if (price <= MIN_PRICE) errorMsg += " Price below limit;";

            if (!errorMsg.empty()) {
                writeInvalidRecord(line, errorMsg);
                hasErrors = true;
            } else {
                records[count++] = {carId, model, manufacturer, quantity, price};
            }
        }
    }

    if (count == 0) {
        cerr << "Warning: The input file exists but is empty or contains only invalid records.\n";
    }

    inputFile.close();
    errorFile.close();

    if (!hasErrors) {
        remove("error_log.txt");
    }
}

bool isValidCarId(const string &carId) {
    if (carId.length() != ID_LENGTH) return false;
    for (int i = 0; i < 4; ++i) if (!isalpha(carId[i]) || carId[i] == 'O') return false;
    for (int i = 4; i < 8; ++i) if (carId[i] < '1' || carId[i] > '9') return false;
    return true;
}

bool isValidModel(const string &model) {
    if (model.length() < 3 || !isalpha(model[0])) return false;
    for (char ch : model) if (!isalnum(ch)) return false;
    return true;
}

void printValidRecords(const CarRecord records[], int count) {
    if (count == 0) {
        cout << "No valid records found.\n";
    } else {
        cout << "\nValid Car Inventory:\n"
             << setw(10) << "Car ID" << setw(15) << "Model" << setw(20) << "Manufacturer"
             << setw(10) << "Qty" << setw(10) << "Price\n"
             << "-------------------------------------------------------------\n";

        for (int i = 0; i < count; ++i)
            cout << setw(10) << records[i].carId
                 << setw(15) << records[i].model
                 << setw(20) << records[i].manufacturer
                 << setw(10) << records[i].quantity
                 << setw(10) << fixed << setprecision(2) << records[i].price
                 << "\n";
    }
}

void writeInvalidRecord(const string &line, const string &errors) {
    ofstream errorFile("error_log.txt", ios::app);
    if (errorFile) {
        errorFile << line << " - Errors:" << errors << "\n";
    } else {
        cerr << "Error opening error file!\n";
    }
}
