#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// Each property is a node in the linked list.
struct Property {
    string name;
    int cost;
    string owner;
    Property* next;

    Property(const string& propertyName, int propertyCost)
        : name(propertyName), cost(propertyCost),
          owner("Unowned"), next(nullptr) {}
};

class PropertyLinkedList {
private:
    Property* head;
    Property* tail;
    int size;

public:
    PropertyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~PropertyLinkedList() {
        Property* current = head;

        while (current != nullptr) {
            Property* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    // Adds a property to the linked list.
    void add(const string& name, int cost) {
        Property* newProperty = new Property(name, cost);

        if (head == nullptr) {
            head = tail = newProperty;
        } else {
            tail->next = newProperty;
            tail = newProperty;
        }

        size++;
    }

    // Searches for a property by name.
    Property* search(const string& name) const {
        Property* current = head;

        for (int i = 0; i < size && current != nullptr; i++) {
            if (current->name == name) {
                return current;
            }

            current = current->next;
        }

        return nullptr;
    }

    // Removes a property from the linked list.
    bool remove(const string& name) {
        Property* current = head;
        Property* previous = nullptr;

        for (int i = 0; i < size && current != nullptr; i++) {
            if (current->name == name) {
                if (previous == nullptr) {
                    head = current->next;
                } else {
                    previous->next = current->next;
                }

                if (current == tail) {
                    tail = previous;
                }

                delete current;
                size--;

                return true;
            }

            previous = current;
            current = current->next;
        }

        return false;
    }

    // Prints every item in the linked list.
    void print() const {
        Property* current = head;

        for (int i = 0; i < size && current != nullptr; i++) {
            cout << current->name << " ($" << current->cost << ")";

            if (current->next != nullptr && i < size - 1) {
                cout << " -> ";
            }

            current = current->next;
        }

        cout << endl;
    }

    Property* getHead() const {
        return head;
    }

    int getSize() const {
        return size;
    }

    // Connects the last property back to the first property.
    void makeCircular() {
        if (tail != nullptr) {
            tail->next = head;
        }
    }

    // Breaks the circular connection before deleting the list.
    void breakCircularLink() {
        if (tail != nullptr) {
            tail->next = nullptr;
        }
    }
};

// Stores player info
struct Player {
    string name;
    int money;
    Property* position;

    Player(const string& playerName, Property* startingPosition)
        : name(playerName), money(1500),
          position(startingPosition) {}
};

// Creates the circular Monopoly board.
class CircularBoard {
private:
    PropertyLinkedList properties;

public:
    CircularBoard() {
        properties.add("Luke's Diner", 120);
        properties.add("The Dragonfly Inn", 220);
        properties.add("Stars Hollow Town Square", 180);
        properties.add("Gilmore Mansion", 260);
        properties.add("Hickory Stick Bookshop", 140);
        properties.add("Miss Patty's Dance Studio", 200);
        properties.add("Yale University", 350);
        properties.add("Kim's Antiques", 240);
        properties.add("Doose's Market", 160);
        properties.add("Chilton Academy", 320);

        properties.makeCircular();
    }

    ~CircularBoard() {
        properties.breakCircularLink();
    }

    Property* start() const {
        return properties.getHead();
    }

    int size() const {
        return properties.getSize();
    }

    // Prints all properties on the board.
    void printBoard() const {
        cout << "\nGilmore Girls Monopoly Board:\n";

        Property* current = start();

        for (int i = 0; i < size(); i++) {
            cout << setw(2) << i + 1 << ". "
                 << current->name << " ($"
                 << current->cost << ")\n";

            current = current->next;
        }
    }
};

// Tests adding, searching, removing, and printing.
void testLinkedList() {
    cout << "Testing the linked list:\n";

    PropertyLinkedList list;

    list.add("Stars Hollow", 100);
    list.add("Luke's Diner", 150);
    list.add("Yale University", 300);

    cout << "List after adding items: ";
    list.print();

    cout << "Searching for Luke's Diner: ";

    if (list.search("Luke's Diner") != nullptr) {
        cout << "found\n";
    } else {
        cout << "not found\n";
    }

    cout << "Removing Stars Hollow: ";

    if (list.remove("Stars Hollow")) {
        cout << "successful\n";
    } else {
        cout << "failed\n";
    }

    cout << "List after removing an item: ";
    list.print();

    cout << "Searching for Stars Hollow: ";

    if (list.search("Stars Hollow") == nullptr) {
        cout << "not found\n\n";
    } else {
        cout << "found\n\n";
    }
};

// Simulates the Monopoly game.
void playGame(const CircularBoard& board) {
    vector<Player> players = {
        Player("Rory", board.start()),
        Player("Lorelai", board.start()),
        Player("Jess", board.start())
    };

    // Ten predetermined dice rolls.
    int diceRolls[10] = {4, 7, 3, 6, 8, 5, 2, 9, 4, 6};

    cout << "PART 2: Gilmore Girl's Monopoly!!!\n";
    cout << "Starting money: $1500 per player\n";

    // Simulates 10 turns.
    for (int turn = 0; turn < 10; turn++) {
        Player& player = players[turn % players.size()];
        int roll = diceRolls[turn];

        // Move around the circular board.
        for (int spaces = 0; spaces < roll; spaces++) {
            player.position = player.position->next;
        }

        Property* landed = player.position;

        cout << "\nTurn " << turn + 1 << ": "
             << player.name << " rolled " << roll
             << " and landed on "
             << landed->name << ".\n";

        // Buy the property if it is unowned.
        if (landed->owner == "Unowned") {
            if (player.money >= landed->cost) {
                player.money -= landed->cost;
                landed->owner = player.name;

                cout << player.name << " bought "
                     << landed->name << " for $"
                     << landed->cost << ".\n";
            } else {
                cout << player.name
                     << " cannot afford this property.\n";
            }
        } else {
            // Players cannot buy an owned property.
            cout << landed->name
                 << " is already owned by "
                 << landed->owner << ".\n";
        }

        cout << player.name << " has $"
             << player.money << " remaining.\n";
    }

    // Prints the final results.
    cout << "\nFINAL RESULTS:\n";

    for (const Player& player : players) {
        cout << player.name << " has $"
             << player.money << " and owns: ";

        Property* current = board.start();
        bool ownsProperty = false;

        for (int i = 0; i < board.size(); i++) {
            if (current->owner == player.name) {
                if (ownsProperty) {
                    cout << ", ";
                }

                cout << current->name;
                ownsProperty = true;
            }

            current = current->next;
        }

        if (!ownsProperty) {
            cout << "none";
        }

        cout << endl;
    }
}

int main() {
    // testing linked list
    testLinkedList();

    // creates players and prints the board.
    CircularBoard board;
    board.printBoard();

    // begin's game
    playGame(board);

    return 0;
}