#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <iomanip>
#include <sstream>

using namespace std; 

const int MAX_RECORDS = 100;
const double MIN_PRICE = 12999.00;
const int ID_LENGTH = 8;

enum MenuOption { PRINT_Valid = 1, PRINT_Invalid, QUIT };

struct CarRecord {
    string carID, model, manufacturer;
    int quantity;
    double price;
};

void displayMenu();
void readRecords(CarRecord records[], int &count);
bool isValidCarID(const string &id);
bool isValidModel(const string &model);
void printValidRecords(const CarRecord records[], int count);
void writeInvalidRecord(const string &line, const string &errors);

int main() {
    CarRecord records[MAX_RECORDS];
    int recordCount = 0;

    readRecords(records, recordCount);

    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case PRINT_Valid: printValidRecords(records, recordCount); break;
            case PRINT_Invalid: cout << "Check 'error_log.txt' for invalid records.\n"; break;
            case QUIT: cout << "Exiting program...\n"; break;
            default: cout << "Invalid option! Try again.\n";
        }
    } while (choice != QUIT);

    return 0;
}

void displayMenu() {
    cout << "\n1. Print all valid car records\n";
    cout << "2. Print invalid records (Check error_log.txt)\n";
    cout << "3. Quit\n";
    cout << "Enter your choice: ";
}

void readRecords(CarRecord records[], int &count) {
    ifstream inputFile("car_inventory.txt");
    ofstream errorFile("error_log.txt");

    if (!inputFile) { cerr << "Error opening file!\n"; return; }

    string line;
    while (getline(inputFile, line)) {
        string carID, model, manufacturer;
        int quantity;
        double price;

        istringstream iss(line);
        iss >> carID >> model >> manufacturer >> quantity >> price;

        string errorMsg;
        if (!isValidCarID(carID)) errorMsg += " Invalid ID;";
        if (!isValidModel(model)) errorMsg += " Invalid Model;";
        if (price <= MIN_PRICE) errorMsg += " Price below limit;";

        if (!errorMsg.empty()) writeInvalidRecord(line, errorMsg);
        else if (count < MAX_RECORDS) records[count++] = {carID, model, manufacturer, quantity, price};
        else { cerr << "Array full. Cannot store more records.\n"; break; }
    }

    inputFile.close();
    errorFile.close();
}

bool isValidCarID(const string &id) {
    if (id.length() != ID_LENGTH) return false;
    for (int i = 0; i < 4; ++i) if (!isalpha(id[i]) || id[i] == 'O') return false;
    for (int i = 4; i < 8; ++i) if (id[i] < '1' || id[i] > '9') return false;
    return true;
}

bool isValidModel(const string &model) {
    if (model.length() < 3 || !isalpha(model[0])) return false;
    for (char ch : model) if (!isalnum(ch)) return false;
    return true;
}

void printValidRecords(const CarRecord records[], int count) {
    if (count == 0) { cout << "No valid records found.\n"; return; }

    cout << "\nValid Car Inventory:\n";
    cout << setw(10) << "Car ID" << setw(15) << "Model" << setw(20) << "Manufacturer"
         << setw(10) << "Qty" << setw(10) << "Price\n";
    cout << "-------------------------------------------------------------\n";

    for (int i = 0; i < count; ++i)
        cout << setw(10) << records[i].carID
             << setw(15) << records[i].model
             << setw(20) << records[i].manufacturer
             << setw(10) << records[i].quantity
             << setw(10) << fixed << setprecision(2) << records[i].price
             << "\n";
}

void writeInvalidRecord(const string &line, const string &errors) {
    ofstream errorFile("error_log.txt", ios::app);
    if (!errorFile) { cerr << "Error opening error file!\n"; return; }
    errorFile << line << " - Errors:" << errors << "\n";
    errorFile.close();
}


