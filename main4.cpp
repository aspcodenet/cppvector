#include <iostream>


// WHEN IS AN OBJECT COPIED?
// 1. When an object is passed by value to a function.
// 2. When an object is returned by value from a function.
// 3. When an object is initialized from another object of the same type (copy initialization).
// 4. When an object is assigned to another object of the same type (copy assignment).

// COPY IS MEMBERWISE COPY 
// so when you have heap allocated kemory in your class THEN it could be a problem

// IF THERE IS A NEED FOR DESTRUCTOR THEN THERE IS A NEED FOR COPY CONSTRUCTOR AND COPY ASSIGNMENT OPERATOR
// RULES OF FIVE







struct Player {
    int jerseyNumber;
    std::string name;

    Player(int number, const std::string& playerName) : jerseyNumber(number), name(playerName) {
        std::cout << "Constructor called for: " << name << "\n";
    }

    Player(const Player& other) : jerseyNumber(other.jerseyNumber), name(other.name) {
        std::cout << "Copy constructor called for: " << name << "\n";
    }

    ~Player() {
        std::cout << "Destructor called for: " << name << "\n";
    }
};

// Player GetSmartest(...){

// }


void print(Player p) {
    std::cout << "Player: " << p.name << ", Jersey Number: " << p.jerseyNumber << std::endl;
}

void test(){
    Player p1(10, "Alice");
    print(p1); // This will cause a copy of p1 to be made when passed to the print function, which will call the copy constructor

}


// 1. By default COPY (copy by value) is SHALLOW
// which is just copy the things  on the stack

//2. Move semantics - when we move an object, we transfer ownership of the resources from one object to another, which can be more efficient than copying, especially for objects that manage resources like dynamic memory, file handles, etc. The move constructor and move assignment operator are used to implement move semantics in C++. When an object is moved, the resources are transferred to the new object, and the old object is left in a valid but unspecified state. This can help avoid unnecessary copying and improve performance in certain situations.

class ShallowBuffer {
    int* data;
    int test;
public:

    // I will create a copy contructor NOW!!
    ShallowBuffer(const ShallowBuffer& other) {
        data = new int[10]; // Request NEW memory
        for(int i=0; i<10; ++i) {
            data[i] = other.data[i]; // Copy the actual values
        }
        std::cout << "Deep copy created at: " << data << "\n";
    }
    ShallowBuffer() {
        data = new int[10]; // Allocate memory on the heap
        std::cout << "Memory allocated at: " << data << "\n";
    }
    // move constructor
    ShallowBuffer(ShallowBuffer&& other) noexcept
        : data(other.data) { // 1. Take the pointer
        
        other.data = nullptr; // 2. "Null out" the old pointer (CRITICAL)
        std::cout << "Moved memory from old object to new one.\n";
    }

    ~ShallowBuffer() {
        std::cout << "Attempting to free memory at: " << data << "\n";
        delete[] data; // if data is nullptr, delete[] will safely do nothing, so we don't need to check for nullptr before deleting
                        // 
        std::cout << "Memory successfully freed!\n";
    }
    // copy assignment operator - TO AN EXISTING OBJECT
    ShallowBuffer& operator=(const ShallowBuffer& other) {
        if (this == &other) {
            return *this; // Handle self-assignment
        }

        // THESE ARE YOUR CUSTOM
        delete[] data; // Free existing resource

        data = new int[10]; // Allocate new memory
        for(int i=0; i<10; ++i) {
            data[i] = other.data[i]; // Copy the actual values
        }
        std::cout << "Copy assignment operator called, new memory allocated at: " << data << "\n";
        // END THESE ARE YOUR CUSTOM

        return *this;
    }

    // move assignment operator
    ShallowBuffer& operator=(ShallowBuffer&& other) noexcept {
        if (this == &other) {
            return *this; // Handle self-assignment
        }

        // THESE ARE YOUR CUSTOM
        delete[] data; // Free existing resource

        data = other.data; // Take ownership of the resource
        other.data = nullptr; // Null out the old pointer
        std::cout << "Move assignment operator called, memory moved to: " << data << "\n";
        // END THESE ARE YOUR CUSTOM

        return *this;
    }
};

void triggerBug() {
    ShallowBuffer obj1; // Constructor runs
    ShallowBuffer obj3;
    // The "Bug" happens here:
    // Default copy constructor copies the ADDRESS of obj1.data into obj2.data.
    int i = 12; // create + assigm
    i = 12; // assign

    ShallowBuffer obj2 = obj1; // Copy constructor is called, but it does a shallow copy, so obj2.data points to the same memory as obj1.data 
                                // this is the default copy constructor that the compiler generates, which does a member-wise copy, so it copies the pointer value, not the actual data on the heap, which leads to both obj1 and obj2 pointing to the same memory location
    obj2 = obj3;     // Copy assignment operator is called, but it also does a shallow copy, so obj2.data now points to the same memory as obj3.data, and the memory that obj1.data was pointing to is leaked because we lost the reference to it without freeing it first
    obj2 = obj2;

    ShallowBuffer obj5 = ShallowBuffer(); // Move constructor is called, which creates a temporary object and then moves it into obj5, so obj5.data points to the memory allocated for the temporary object, and the temporary object's destructor will free that memory when it goes out of scope, leaving obj5 with a dangling pointer (obj5.data will point to freed memory)
    obj5 = ShallowBuffer(); // Move assignment operator is called, which creates a temporary object and then moves it into obj5, so obj5.data points to the memory allocated for the temporary object, and the temporary object's destructor will free that memory when it goes out of scope, leaving obj5 with a dangling pointer (obj5.data will point to freed memory)


    std::string str = std::string("Hello");

    //ShallowBuffer obj5 = std::move(obj1); // Move constructor is called, which transfers ownership of the memory from obj1 to obj5, and leaves obj1 in a valid but unspecified state (in this case, obj1.data is set to nullptr)
    // obj1 is now pointing to nulllptr 
    
} // End of scope: obj2 and obj1 are destroyed here

int main() {
    std::cout << "Starting main function.\n";
    triggerBug();
    std::cout << "Exiting main function.\n";
    return 0;
}


// Fixing med COPY CONSTRUCTIOR
// ShallowBuffer(const ShallowBuffer& other) {
//     data = new int[10]; // Request NEW memory
//     for(int i=0; i<10; ++i) {
//         data[i] = other.data[i]; // Copy the actual values
//     }
//     std::cout << "Deep copy created at: " << data << "\n";
// }



// 2 Move
//Instead of making a costly copy of the data, = shallow copy which we control - performance
// Deep Copy builds an exact replica of that house on a new plot of land. Move Semantics just hands them the keys to the existing house and changes the address on the mailbox.

// When we "move" an object, we use an rvalue reference (&&). This tells the compiler: "This source object is temporary; feel free to gut it for part
// #include <iostream>

// class MoveBuffer {
// public:
//     int* data;

//     MoveBuffer() : data(new int[10]) {
//         std::cout << "Allocated memory at: " << data << "\n";
//     }

//     // --- MOVE CONSTRUCTOR ---
//     MoveBuffer(MoveBuffer&& other) noexcept 
//         : data(other.data) { // 1. Take the pointer
        
//         other.data = nullptr; // 2. "Null out" the old pointer (CRITICAL)
//         std::cout << "Moved memory from old object to new one.\n";
//     }

//     ~MoveBuffer() {
//         if (data != nullptr) {
//             std::cout << "Freeing memory at: " << data << "\n";
//             delete[] data;
//         } else {
//             std::cout << "Nothing to free (pointer is null).\n";
//         }
//     }
// };

// WHEN IS MOVE CONSTRUCTOR CALLED?
// 1. When an object is initialized with a temporary (rvalue) object.
// 2. When std::move is used to explicitly indicate that an object can be "moved from".
// 3. When returning a local object from a function (NRVO - Named Return Value Optimization can sometimes elide this).
