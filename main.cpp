#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

bool exit_flag = false;

// Helper functions to get the user's answer to whether they want to see the check
std::string get_user_answer_string(std::string question) {
    std::string answer;
    std::cout << question;
    std::getline(std::cin >> std::ws, answer);
    return answer;
}
// Each function first calls get_user_answer_string to get input from user and then checks if the input is valid for the desired type. 
// If it's not valid, it prompts the user to enter the input again. 
// If the input is valid, it converts it to the desired type and returns it.
char get_user_answer_char(std::string question) {
    std::string user_input = get_user_answer_string(question);
    while (user_input.length() != 1) {
        std::cout << "Invalid input. Please enter only one character: ";
        user_input = get_user_answer_string("");
    }
    return user_input[0];
}
int get_user_answer_int(std::string question) {
    std::string user_input = get_user_answer_string(question);
    bool valid_input = true;
    for (char c : user_input) {
        if (!isdigit(c)) {
            valid_input = false;
            break;
        }
    }
    while (!valid_input) {
        std::cout << "Invalid input. Please enter an integer: ";
        user_input = get_user_answer_string("");
        valid_input = true;
        for (char c : user_input) {
            if (!isdigit(c)) {
                valid_input = false;
                break;
            }
        }
    }
    return std::stoi(user_input);
}
double get_user_answer_double(std::string question) {
    std::string user_input = get_user_answer_string(question);
    bool valid_input = true;
    for (char c : user_input) {
        if (!std::isdigit(c) && c != '.') {
            valid_input = false;
            break;
        }
    }
    while (!valid_input) {
        std::cout << "Invalid input. Please enter a decimal number: ";
        user_input = get_user_answer_string("");
        valid_input = true;
        for (char c : user_input) {
            if (!std::isdigit(c) && c != '.') {
                valid_input = false;
                break;
            }
        }
    }
    return std::stod(user_input);
}

class Item {
private:
    double price;       // stores the price of the item
    std::string name;   // stores the name of the item

public:
    // Default constructor that sets the name to an empty string and price to 0.0
    Item() : name(""), price(0.0) {}

    // Parameterized constructor that sets the name and price of the item
    Item(std::string name, double price) : name(name), price(price) {}

    // Virtual function that returns the name of the item as a string
    virtual std::string get_name() const { return name; }

    // Virtual function that returns the price of the item as a double
    virtual double get_price() const { return 0; }
};

class MenuItem : public Item {
private:
    double price;

public:
    // Constructor to set the name and price of the item
    MenuItem(std::string name, double price) : Item(name, price), price(price) {}

    // Getter function to retrieve the price of the item
    double get_price() const override { return price; }

    // Setter for price
    void set_price(double price) { this->price = price; }
};

class Menu {
private:
    std::vector<MenuItem*> items;

public:
    // Display the menu and its items
    void display_menu() const {
        std::cout << "Menu:\n";
        for (int i = 0; i < items.size(); i++) {
            std::cout << "\t" << i + 1 << ". " << items[i]->get_name() << " - $" << items[i]->get_price() << std::endl;
        }
    }

    // Add a new menu item to the menu
    void add_item(MenuItem* item) {
        items.push_back(item);
    }

    // Remove a menu item from the menu
    void remove_item(MenuItem* item) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (*it == item) {
                delete* it;
                items.erase(it);
                break;
            }
        }
    }

    // Update the price of a menu item in the menu
    void update_price(MenuItem* item, double new_price) {
        item->set_price(new_price);
    }

    // Check if a menu item exists in the menu
        MenuItem * find_item(const std::string & name) const {
        for (auto item : items) {
            if (item->get_name() == name) {
                return item;
            }
        }
        return nullptr;
    }
};

class Order {
protected:
    std::vector<std::pair<MenuItem*, int>> items;

public:
    // Add a new menu item and its quantity to the order
    void add_item_to_order(MenuItem* item, int quantity) {
        items.push_back(std::make_pair(item, quantity));
    }

    // Remove a menu item and its quantity from the order
    void remove_item_from_order(int index) {
        if (index >= 0 && index < items.size()) {
            delete items[index].first;
            items.erase(items.begin() + index);
        }
    }

    // Calculate the total cost of the order
    double calculate_total() const {
        double total = 0;
        for (auto item : items) {
            total += item.first->get_price() * item.second;
        }
        return total;
    }

    // Display the order receipt in a .txt file
    void display_order(bool append) const {
        std::ofstream file;
        if (append) file.open("order.txt", std::ios::app);
        else  file.open("order.txt");
        file << "Order Receipt:\n";
        for (int i = 0; i < items.size(); i++) {
            file << i + 1 << ". " << items[i].first->get_name() << " - $" << items[i].first->get_price() << " x " << items[i].second << std::endl;
        }
        file << "Total: $" << calculate_total() << std::endl << std::endl;
        file.close();
    }
};


class Manager {
private:
    bool is_manager = false;
    std::string password;
    std::vector<Order*> orders;
    Menu* menu;

public:
    Manager(std::string password) : password(password) {
        menu = new Menu();
    }

    ~Manager() {
        for (Order* order : orders) {
            delete order;
        }
        delete menu;
    }

    // - - - FOR MANAGER
    // Add a new dish to the menu
    void add_dish(MenuItem* item) {
        if (is_manager) menu->add_item(item);
    }

    // Remove a dish from the menu
    void remove_dish(MenuItem* item) {
        if (is_manager) menu->remove_item(item);
    }

    // Update the price of an existing dish in the menu
    void update_dish_price(MenuItem* item, double new_price) {
        if (is_manager) menu->update_price(item, new_price);
    }

    // Display ALL orders made so far
    void display_orders() {
        if (is_manager) {
            std::ofstream file;
            file.open("order.txt");
            std::cout << "You can check all orders in 'order.txt'!\n" << std::endl;
            for (Order* order : orders) {
                order->display_order(true);
            }
        }
    }

    // Login to access Manager functions
    bool login(std::string user_password) {
        if (user_password == password) {
            std::cout << "Login successful!" << std::endl;
            is_manager = true;
            return true;
        }
        else {
            std::cout << "Incorrect password!" << std::endl;
            is_manager = false;
            return false;
        }
    }

    // Logout to prevent others from using Manager functions
    void logout() { is_manager = false; }

    // - - - FOR CLIENT
    // Display the menu
    void display_menu() {
        menu->display_menu();
    }

    // Generate a new order and add it to the list of orders
    void generate_order() {
        // create a new Order object to hold the user's order and display menu
        Order* order = new Order();
        menu->display_menu();

        int num_items = get_user_answer_int("\nNow, please, make your order!\nEnter the number of items you want to order: ");

        // prompt the user for each item they want to order and add it to the order object
        for (int i = 0; i < num_items; i++) {
            MenuItem* item = get_menu_item();

            // if the user's selection is invalid, decrement the counter and continue to the next iteration of the loop
            if (item == nullptr) {
                i--;
                continue;
            }
            // prompt the user for the quantity of the item they want to order and add it to the order object
            int item_quantity = get_user_answer_int("Enter the quantity of the item: ");

            order->add_item_to_order(new MenuItem(item->get_name(), item->get_price()), item_quantity);
        }

        // add the completed order to the list of orders
        orders.push_back(order);
        std::cout << "Order added successfully!" << std::endl;

        // prompt the user to see the check 
        char answer = get_user_answer_char("Do you want to see the check? (y/N)\n");

        // process the user's answer
        switch (answer) {
        case 'y':
            order->display_order(false);
            std::cout << "You can check your check in file named 'order.txt'!\nThank you for your order!\n\n\n";
            break;
        case 'N':
            std::cout << "Okay-Okay, thank you!\n\n";
            break;
        default:
            // if the user's answer is invalid, display an error message
            std::cout << "I have no idea what does it mean, but I suppose you dont want to!\n\n";
            break;
        }
    }

    // - - - HELP FUNCTIONS
    // Helper function to get a menu item from the user
    MenuItem* get_menu_item() {
        double item_price;

        std::string item_name = get_user_answer_string("Enter the name of the item: ");
        MenuItem* item = menu->find_item(item_name);

        if (item == nullptr) {
            std::cout << "You entered '" << item_name << "'. Unfortunately we don't have such item in the menu, try one more time!\n";
            return nullptr;
        }
        return item;
    }

    // Getter
    Menu* get_menu() {
        return menu;
    }
};

// function to handle interactions with a user who is a client 
void work_with_client(Manager* client) {
    std::cout << "\nHI-HI-HI My dear client! Here is our menu:\n";
    client->generate_order();
}

// Add a new dish to the menu
void add_dish(Manager* user) {
    std::string new_item_name = get_user_answer_string("Enter name of new item: ");
    double new_item_price = get_user_answer_double("Enter price of new item: ");

    user->add_dish(new MenuItem(new_item_name, new_item_price));
    std::cout << "Dish added to menu!" << std::endl;
}

// Remove a dish from the menu
void remove_dish(Manager* user) {
    std::string item_name_to_remove = get_user_answer_string("Enter the name of the dish you want to remove: ");
    MenuItem* item_to_remove = user->get_menu()->find_item(item_name_to_remove);

    if (item_to_remove != nullptr) {
        user->remove_dish(item_to_remove);
        std::cout << "Dish removed from menu!" << std::endl;
    }
    else {
        std::cout << "Dish not found on menu!" << std::endl;
    }
}

// Update the price of an existing dish on the menu
void update_dish(Manager* user) {
    std::string item_name_to_update = get_user_answer_string("Enter the name of the dish you want to update: ");
    MenuItem* item_to_update = user->get_menu()->find_item(item_name_to_update);

    if (item_to_update != nullptr) {
        double new_price = get_user_answer_double("Enter the new price of the dish: ");
        user->update_dish_price(item_to_update, new_price);
        std::cout << "Dish price updated!" << std::endl;
    }
    else {
        std::cout << "Dish not found on menu!" << std::endl;
    }
}

// function to handle interactions with a user who is a manager
void work_with_manager(Manager* user) {
    if (user->login(get_user_answer_string("Enter manager password: "))) {
        int choice;
        do {
            std::cout << "What do you want to do?\n"
                "1. Add a new dish to the menu\n"
                "2. Remove a dish from the menu\n"
                "3. Update the price of an existing dish on the menu\n"
                "4. Display all orders made so far\n"
                "5. Logout\n";
            choice = get_user_answer_int("Enter your choice: ");

            switch (choice) {
            case 1:
                // Add a new dish to the menu
                add_dish(user);
                break;

            case 2:
                // Remove a dish from the menu
                remove_dish(user);
                break;

            case 3:
                // Update the price of an existing dish on the menu
                update_dish(user);
                break;

            case 4:
                // Display all orders made so far
                user->display_orders();
                break;

            case 5:
                // Logout
                user->logout();
                std::cout << "Logged out!" << std::endl;
                break;

            default:
                std::cout << "Invalid choice!" << std::endl;
                break;
            }
        } while (choice != 5);
    }
    else {
        std::cout << "Unfortunetly we cannot trust you!\n\n";
    }
}

// function to handle interactions with a user who is not a client or a manager
void work_with_nobody(Manager* user) {
    std::cout << "\nEven if you are nobody, you can check our menu!\n\n";
    user->display_menu();

    // ask the user if they want to order something
    char answer = get_user_answer_char("\nMaybe you want to order something? (y/N)\n"); 

    // process the user's answer
    switch (answer) {
    case 'y':
        // if the user wants to order something, call the work_with_client function
        // and pass the menu object to it
        std::cout << "Very good, very good! Now lets start our conversation again, but now I will talk with my client!\n\n";
        work_with_client(user);
        break;
    case 'N':
        // if the user does not want to order anything, say goodbye
        std::cout << "Well, it is okay, hope to see you next time! :)\n\n";
        exit_flag = true;
        break;
    default:
        // if the user's answer is invalid, display an error message
        std::cout << "I have no idea what does it mean, but I suppose you dont want anything!\n\n";
        exit_flag = true;
        break;
    }
}

int main() {
    // create a menu object
    Manager user("12345abcde");

    // add items to the menu using the MenuItem constructor using managers rights
    user.login("12345abcde");
    user.add_dish(new MenuItem("Lemon Juice", 7.99));
    user.add_dish(new MenuItem("Apple", 9.99));
    user.add_dish(new MenuItem("Bannana", 9.99));
    user.add_dish(new MenuItem("Pancakes", 11.99));
    user.add_dish(new MenuItem("Spinach & Feta (Hand Tossed, Thin Crust or Brooklyn Style) Pizza", 15.99));
    user.logout();

    while (!exit_flag) {
        // ask the user who they are
        std::cout << "Hello, Welcome to my Restaurant! Who are you?\n";
        std::cout << "\tA. I am a client!\n";
        std::cout << "\tB. I am a manager!\n";
        std::cout << "\tC. I am a just a stranger passing by!\n";
        char answer = get_user_answer_char("Enter who you are (A, B or C) : I am ");

        // process the user's answer
        switch (answer) {
        case 'A':
            // call the work_with_client function and pass the menu object to it
            work_with_client(&user);
            break;
        case 'B':
            // call the work_with_manager function and pass the menu object to it
            work_with_manager(&user);
            break;
        case 'C':
            // call the work_with_nobody function and pass the menu object to it
            work_with_nobody(&user);
            break;
        default:
            // print an error message if the user's answer is invalid
            std::cout << "\nExcuse me, I an unable to continue the converstion with you, bcs I dont know who you are!";
            exit_flag = true;
            break;
        }
    }

    // end of the program
    std::cout << "GOOD-GOOD-GOODBYE";
    return 0;
}