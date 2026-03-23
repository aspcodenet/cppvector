#include <iostream>
#include <vector>
#include <string>
#include <cstring>

// 1. how does a vector grow
// 2. each allocation of vector is a malloc(heap) allocation, so we can count how many times the vector grows by counting the number of malloc calls
// 3. reserve
// 3.5. remove reserve
// 4. printVector - pass by value, so it will create a copy of the vector, which will call the copy constructor of Player for each element in the vector, and then call the destructor for each element in the vector when the function ends
// 5. add reserve 
// 6. still some allocations!
// 7. add const &
// 8. Emplace_back - construct the object in place, so it will call the constructor of Player directly, without creating a temporary object, which will avoid the copy constructor and move constructor calls, and also avoid the destructor calls for the temporary objects
// 9. Hurra bara en! Men det behövs inte heller - use std::array
// 10. Copy and move constructors - when we push back a player into the vector, it will call the copy constructor of Player to create a copy of the player object, and then call the destructor for the temporary player object that was created during the push back operation. If we use emplace_back instead of push_back, it will call the constructor of Player directly, without creating a temporary object, which will avoid the copy constructor and move constructor calls, and also avoid the destructor calls for the temporary objects.

int allocationCount = 0;
void *operator new(std::size_t size) {
    ++allocationCount;
    return malloc(size);
}

int descructorCount = 0;
int constructorCount = 0;
int copyConstructorCount = 0;
int moveConstructorCount = 0;

struct Player {
    int jerseyNumber;

    std::string name;

    Player(int number, const std::string& playerName) : jerseyNumber(number), name(playerName) {
        ++constructorCount;
    }
    // //copy constructor
    Player(const Player& other) : jerseyNumber(other.jerseyNumber), name(other.name) {
        ++copyConstructorCount;
    }
    // move constructor
    Player(Player&& other) noexcept : jerseyNumber(other.jerseyNumber), name(std::move(other.name)) {
        ++moveConstructorCount;
    }


    virtual ~Player() {
        ++descructorCount;
    }
};


void printVector(const std::vector<Player> &players) {
    for (const Player &player : players) {
        std::cout << "Player: " << player.name << ", Jersey Number: " << player.jerseyNumber << std::endl;
    }
}

// int test(int a){ // COPY BY VALUE
//     a = 12;
//     return 0;
// }

void runIt(){
    // int b = 13;
    // test(b);
    //std::cout << "Value of b after test: " << b << std::endl; // will print 13, because the value of b is not changed in the test function, since it is passed by value, which means that a copy of b is created in the test function, and the changes made to a do not affect b

    std::vector<Player> players; 
    //players.reserve(100); // reserve space for 100 players to avoid multiple allocations as we push back players

 // DURING EXECUTION 527 PLAYERS ARE BEING CREATED
    for(int i = 0; i < 100; ++i) {
        // test(i);
        // test(140+1);
        
        // iN THIS EXAMPLE we will be able to use a MOVE constructor
        //players.push_back(Player(i + 1, "Player" + std::to_string(i + 1))); // Copnstructor will be called
        players.emplace_back(i + 1, "Player" + std::to_string(i + 1)); // construct the player object in place, which will avoid the copy constructor and move constructor calls, and also avoid the destructor calls for the temporary objects


        // Player p = Player(i + 1, "Player" + std::to_string(i + 1)); // Contructior is called
        // players.push_back(p); // copy constructor is called to create a copy of p in the vector, and then the destructor is called for the temporary player object that was created during the push back operation
        // lvalue and rvalue references - if we use push_back with a temporary object, it will call the move constructor of Player to create a new player object in the vector, and then call the destructor for the temporary player object that was created during the push back operation. If we use emplace_back instead of push_back, it will call the constructor of Player directly, without creating a temporary object, which will avoid the copy constructor and move constructor calls, and also avoid the destructor calls for the temporary objects
        
        //players.emplace_back(i + 1, "Player" + std::to_string(i + 1)); // construct the player object in place, which will avoid the copy constructor and move constructor calls, and also avoid the destructor calls for the temporary objects
    }
    
    printVector(players);
}


class String{
public: 
    String(const char *s) {
        data = new char[std::strlen(s) + 1];
        std::strcpy(data, s);
        std::cout << "Constructor called for: " << s << std::endl;
    }

    ~String() {
        delete [] data;
        std::cout << "Destructor called" << std::endl;
    }
private:
    char* data;
    int size;    
};


int main() {

    String s1("Hello");

    String s3(s1); 
    String s4(String("Hello")); 
    String s2 = s1;

    
    runIt();

    std::cout << "Total malloc(heap) allocations - thats vector growing: " << allocationCount << std::endl;
    std::cout << "Total player destructors called: " << descructorCount << std::endl;
    std::cout << "Total player constructors called: " << constructorCount << std::endl;
    std::cout << "Total player copy constructors called: " << copyConstructorCount << std::endl;
    std::cout << "Total player move constructors called: " << moveConstructorCount << std::endl;

    return 0;
}