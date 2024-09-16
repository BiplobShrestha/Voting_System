#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class Customer {
public:
    string name;
    string password;
    double walletUSD = 100000;
    double walletNPR = 0;
    double expensesUSD = 0;
    double expensesNPR = 0;
    vector<string> bookedRestaurants;
    vector<string> bookedHotels;

    void registerCustomer() {
        cout << "\n==== Customer Registration ====\n";
        cout << "Enter your name: ";
        cin >> name;
        cout << "Enter your password: ";
        cin >> password;
        saveCustomerData();
        cout << "Registration complete!\n";
    }

    bool login() {
        cout << "\n==== Customer Login ====\n";
        ifstream inFile(name + ".txt");
        if (!inFile) {
            cout << "No such customer found! Please register first.\n";
            return false;
        }

        string inputName, inputPassword;
        while (true) {
            cout << "Enter your name: ";
            cin >> inputName;
            cout << "Enter your password: ";
            cin >> inputPassword;

            if (inputName == name && inputPassword == password) {
                cout << "Welcome, " << name << "!\n";
                return true;
            } else {
                cout << "Incorrect name or password! Try again.\n";
            }
        }
    }

    void loadCustomerData() {
        ifstream inFile(name + ".txt");
        if (!inFile) return;

        getline(inFile, name);
        getline(inFile, password);
        inFile >> walletUSD >> walletNPR >> expensesUSD >> expensesNPR;
        inFile.ignore();

        string booking;
        while (getline(inFile, booking)) {
            if (booking.find("Restaurant:") != string::npos) {
                bookedRestaurants.push_back(booking);
            } else if (booking.find("Hotel:") != string::npos) {
                bookedHotels.push_back(booking);
            }
        }
        inFile.close();
    }

    void saveCustomerData() {
        ofstream outFile(name + ".txt");
        outFile << name << "\n" << password << "\n" << walletUSD << "\n" << walletNPR << "\n" << expensesUSD << "\n" << expensesNPR << "\n";
        for (const auto& booking : bookedRestaurants) {
            outFile << booking << endl;
        }
        for (const auto& booking : bookedHotels) {
            outFile << booking << endl;
        }
        outFile.close();
    }

    void walletSection() {
        cout << "\n==== Wallet Details ====\n";
        cout << "USD: $" << walletUSD << endl;
        cout << "NPR: Rs " << walletNPR << endl;
        cout << "Expenses USD: $" << expensesUSD << endl;
        cout << "Expenses NPR: Rs " << expensesNPR << endl;
    }

    void currencyExchange() {
        cout << "\n==== Currency Exchange ====\n";
        char option;
        double amount;
        cout << "A. Hello Exchange Center (1 USD = 130 NPR)\n";
        cout << "B. Bye Exchange Center (1 USD = 132 NPR)\n";
        cout << "Select option: ";
        cin >> option;
        cout << "Enter amount to exchange in USD: ";
        cin >> amount;

        if (option == 'A' || option == 'a') {
            walletUSD -= amount;
            walletNPR += amount * 130;
        } else if (option == 'B' || option == 'b') {
            walletUSD -= amount;
            walletNPR += amount * 132;
        } else {
            cout << "Invalid option!\n";
            return;
        }

        expensesUSD += amount;
        saveCustomerData();
        cout << "Exchange complete!\n";
    }

    void viewRestaurants() {
        cout << "\n==== Available Restaurants ====\n";
        ifstream restaurantFile("restaurants.txt");
        string name, about;
        double price;

        while (getline(restaurantFile, name)) {
            getline(restaurantFile, about);
            restaurantFile >> price;
            restaurantFile.ignore();
            cout << name << endl;
            cout << "   About: " << about << endl;
            cout << "   Price: NPR " << price << " per day\n";
        }
        restaurantFile.close();
    }

    void viewHotels() {
        cout << "\n==== Available Hotels ====\n";
        ifstream hotelFile("hotels.txt");
        string name, about;
        double price;

        while (getline(hotelFile, name)) {
            getline(hotelFile, about);
            hotelFile >> price;
            hotelFile.ignore();
            cout << name << endl;
            cout << "   About: " << about << endl;
            cout << "   Price: NPR " << price << " per day\n";
        }
        hotelFile.close();
    }

    void book() {
        cout << "\n==== Booking Section ====\n";
        char choice;
        cout << "A. Restaurant\nB. Hotel\n0. Exit\n";
        cout << "Choose an option to book: ";
        cin >> choice;

        if (choice == 'A' || choice == 'a') {
            viewRestaurants();
            cout << "Enter restaurant name to book: ";
            string restaurant;
            cin.ignore();
            getline(cin, restaurant);

            double price = 0;
            ifstream restaurantFile("restaurants.txt");
            string name, about;
            while (getline(restaurantFile, name)) {
                getline(restaurantFile, about);
                restaurantFile >> price;
                restaurantFile.ignore();
                if (name == restaurant) {
                    break;
                }
            }
            restaurantFile.close();

            if (find(bookedRestaurants.begin(), bookedRestaurants.end(), "Restaurant:" + restaurant) != bookedRestaurants.end()) {
                cout << "Already booked!\n";
                return;
            }
            if (walletNPR >= price) {
                walletNPR -= price;
                expensesNPR += price;
                bookedRestaurants.push_back("Restaurant:" + restaurant);
                saveCustomerData();
                cout << "Booking successful!\n";
            } else {
                cout << "Insufficient funds!\n";
            }
        } else if (choice == 'B' || choice == 'b') {
            viewHotels();
            cout << "Enter hotel name to book: ";
            string hotel;
            cin.ignore();
            getline(cin, hotel);

            double priceNPR = 0;
            ifstream hotelFile("hotels.txt");
            string name, about;
            while (getline(hotelFile, name)) {
                getline(hotelFile, about);
                hotelFile >> priceNPR;
                hotelFile.ignore();
                if (name == hotel) {
                    break;
                }
            }
            hotelFile.close();

            if (find(bookedHotels.begin(), bookedHotels.end(), "Hotel:" + hotel) != bookedHotels.end()) {
                cout << "Already booked!\n";
                return;
            }
            if (walletNPR >= priceNPR) {
                walletNPR -= priceNPR;
                expensesNPR += priceNPR;
                bookedHotels.push_back("Hotel:" + hotel);
                saveCustomerData();
                cout << "Booking successful!\n";
            } else {
                cout << "Insufficient funds!\n";
            }
        } else if (choice == '0') {
            cout << "Exiting booking menu.\n";
        } else {
            cout << "Invalid choice!\n";
        }
    }
};

class Admin {
public:
    string username = "Biplob";
    string password = "333";
    map<string, pair<string, double>> restaurants;
    map<string, pair<string, double>> hotels;

    Admin() {
        loadAdminData();
    }

    bool login() {
        cout << "\n==== Admin Login ====\n";
        string inputUsername, inputPassword;
        while (true) {
            cout << "Enter admin username: ";
            cin >> inputUsername;
            cout << "Enter admin password: ";
            cin >> inputPassword;

            if (inputUsername == username && inputPassword == password) {
                cout << "Admin login successful!\n";
                return true;
            } else {
                cout << "Incorrect username or password! Try again.\n";
            }
        }
    }

    void addHotel() {
        cout << "\n==== Add Hotel ====\n";
        string hotelName, about;
        double pricePerDay;
        cout << "Enter hotel name: ";
        cin.ignore();
        getline(cin, hotelName);
        cout << "Enter about the hotel: ";
        getline(cin, about);
        cout << "Enter price per day (in NPR): ";
        cin >> pricePerDay;
        hotels[hotelName] = {about, pricePerDay};
        saveAdminData();
        cout << "Hotel added successfully!\n";
    }

    void addRestaurant() {
        cout << "\n==== Add Restaurant ====\n";
        string restaurantName, about;
        double pricePerDay;
        cout << "Enter restaurant name: ";
        cin.ignore();
        getline(cin, restaurantName);
        cout << "Enter about the restaurant: ";
        getline(cin, about);
        cout << "Enter price per day (in NPR): ";
        cin >> pricePerDay;
        restaurants[restaurantName] = {about, pricePerDay};
        saveAdminData();
        cout << "Restaurant added successfully!\n";
    }
    void viewAllHotels() {
        cout << "\n==== List of All Hotels ====\n";
        for (const auto& hotel : hotels) {
            cout << "Hotel Name: " << hotel.first << endl;
            cout << "   About: " << hotel.second.first << endl;
            cout << "   Price per day: NPR " << hotel.second.second << endl;
        }
    }

    void viewAllRestaurants() {
        cout << "\n==== List of All Restaurants ====\n";
        for (const auto& restaurant : restaurants) {
            cout << "Restaurant Name: " << restaurant.first << endl;
            cout << "   About: " << restaurant.second.first << endl;
            cout << "   Price per day: NPR " << restaurant.second.second << endl;
        }
    }

    void saveAdminData() {
        ofstream outFile("hotels.txt");
        for (const auto& hotel : hotels) {
            outFile << hotel.first << "\n" << hotel.second.first << "\n" << hotel.second.second << "\n";
        }
        outFile.close();

        outFile.open("restaurants.txt");
        for (const auto& restaurant : restaurants) {
            outFile << restaurant.first << "\n" << restaurant.second.first << "\n" << restaurant.second.second << "\n";
        }
        outFile.close();
    }

    void loadAdminData() {
        ifstream inFile("hotels.txt");
        string name, about;
        double price;
        while (getline(inFile, name)) {
            getline(inFile, about);
            inFile >> price;
            inFile.ignore();
            hotels[name] = {about, price};
        }
        inFile.close();

        inFile.open("restaurants.txt");
        while (getline(inFile, name)) {
            getline(inFile, about);
            inFile >> price;
            inFile.ignore();
            restaurants[name] = {about, price};
        }
        inFile.close();
    }

    void adminMenu() {
        while (true) {
            cout << "\n==== Admin Menu ====\n";
            cout << "1. Add Hotel\n2. Add Restaurant\n3. View All Hotels\n4. View All Restaurants\n5. Logout\n";
            cout << "Select an option: ";
            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    addHotel();
                    break;
                case 2:
                    addRestaurant();
                    break;
                case 3:
                    viewAllHotels();
                    break;
                case 4:
                    viewAllRestaurants();
                    break;
                case 5:
                    cout << "Logging out...\n";
                    return;
                default:
                    cout << "Invalid choice! Please try again.\n";
            }
        }
    }
};

int main() {
    Customer customer;
    Admin admin;
    int mainChoice;

    while (true) {
        cout << "\n==== Smart Visit System ====\n";
        cout << "1. Customer\n2. Admin\n3. Exit\n";
        cout << "Select an option: ";
        cin >> mainChoice;

        switch (mainChoice) {
            case 1:
                while (true) {
                    cout << "\n==== Customer Menu ====\n";
                    cout << "1. Register\n2. Login\n3. Wallet\n4. Currency Exchange\n5. Book\n6. Logout\n";
                    cout << "Select an option: ";
                    int customerChoice;
                    cin >> customerChoice;

                    switch (customerChoice) {
                        case 1:
                            customer.registerCustomer();
                            break;
                        case 2:
                            if (customer.login()) {
                                customer.loadCustomerData();
                            }
                            break;
                        case 3:
                            customer.walletSection();
                            break;
                        case 4:
                            customer.currencyExchange();
                            break;
                        case 5:
                            customer.book();
                            break;
                        case 6:
                            cout << "Logging out...\n";
                            goto MainMenu;
                        default:
                            cout << "Invalid choice! Please try again.\n";
                    }
                }
                break;
            case 2:
                if (admin.login()) {
                    admin.adminMenu();
                }
                break;
            case 3:
                cout << "Exiting system...\n";
                return 0;
            default:
                cout << "Invalid option! Please try again.\n";
        }
    }
    
    MainMenu:
    return 0;
}
