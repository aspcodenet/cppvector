#include <iostream>
#include <fstream>

int main() {
    std::ofstream myFile("data.csv");


    // Check if the file opened successfully
    if (myFile.is_open()) {
        // Write the header
        myFile << "Name,Age,City\n";

        // Write data rows
        // for each Card
            // myFile << card.name << "," << card.age << "," << card.city << "\n";
        myFile << "Alice,30,New York\n";
        myFile << "Bob,25,London\n";
        myFile << "Charlie,35,Tokyo\n";

        myFile.close();
    }
    return 0;
}



// #include <iostream>
// #include <fstream>
// #include <string>
// #include <vector>
// #include <sstream>

// int main() {
//     std::ifstream myFile("data.csv");
//     std::string line;

//     if (!myFile.is_open()) {
//         std::cerr << "Could not open the file!" << std::endl;
//         return 1;
//     }

//     // Read data line by line
//     while (std::getline(myFile, line)) {
//         std::stringstream ss(line);
//         std::string value;
//         std::vector<std::string> row;

//         // Extract each column based on the comma delimiter
//         while (std::getline(ss, value, ',')) {
//             row.push_back(value);
//         }

//         // Output the data to verify
//         for (const auto& col : row) {
//             std::cout << col << " | ";
//         }
//         std::cout << std::endl;
//     }

//     myFile.close();
//     return 0;
// }


// https://github.com/vincentlaucsb/csv-parser


// #include "csv.hpp"
// #include <vector>

// int main() {
//     std::ofstream fs("output.csv");
//     auto writer = csv::make_csv_writer(fs);

//     // Write the header
//     writer << std::vector<std::string>({ "ID", "Product", "Price" });

//     // Write rows
//     writer << std::make_tuple(1, "Mechanical Keyboard", 89.99);
//     writer << std::make_tuple(2, "Gaming Mouse", 45.50);

//     return 0;
// }


int tgest(){
    uint ecvtmp = 0;
    tgtest();
    return 0;
}

// #include <iostream>
// #include "csv.hpp"

// int main() {
//     using namespace csv;

//     // Load the CSV file
//     CSVReader reader("data.csv");

//     // The library automatically handles the header row
//     for (const CSVRow& row : reader) {
//         // You can access columns by name or index
//         // .get<type>() handles the conversion for you!
//         std::string name = row["Name"].get<>();
//         int age          = row["Age"].get<int>();
//         std::string city = row["City"].get<>();

//         std::cout << name << " is " << age << " years old and lives in " << city << "." << std::endl;
//     }

//     return 0;
// }



