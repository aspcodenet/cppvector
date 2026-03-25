#include <iostream>


// 1. By default COPY (copy by value) is SHALLOW
// which is just copy the things  on the stack

//2. Move semantics - when we move an object, we transfer ownership of the resources from one object to another, which can be more efficient than copying, especially for objects that manage resources like dynamic memory, file handles, etc. The move constructor and move assignment operator are used to implement move semantics in C++. When an object is moved, the resources are transferred to the new object, and the old object is left in a valid but unspecified state. This can help avoid unnecessary copying and improve performance in certain situations.

class ShallowBuffer {
public:
    int* data;

    ShallowBuffer() {
        data = new int[10]; // Allocate memory on the heap
        std::cout << "Memory allocated at: " << data << "\n";
    }

    ~ShallowBuffer() {
        std::cout << "Attempting to free memory at: " << data << "\n";
        delete[] data; 
        std::cout << "Memory successfully freed!\n";
    }
};

void triggerBug() {
    ShallowBuffer obj1; // Constructor runs
    
    // The "Bug" happens here:
    // Default copy constructor copies the ADDRESS of obj1.data into obj2.data.
    ShallowBuffer obj2 = obj1; 
    
    std::cout << "obj1.data: " << obj1.data << "\n";
    std::cout << "obj2.data: " << obj2.data << "\n";
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
//Instead of making a costly copy of the data, the new object simply plucks the pointer out of the temporary object
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
