#include "purchase.hpp"
#include "regular_customer.hpp"

#include <iostream>
#include <fstream>

purchase getPurchaseToRead();

using namespace orlov;
using namespace std;

int main() {

    regular_customer customer_v{2978519690, 0, 0.1};
    regular_customer julia{3207557189, 0, 0.15};

    purchase novus{customer_v, {
            {"potatoes", 6, true},
            {"bananas", 32, false}
    }};

    cout<< novus[0] << endl;

    purchase furshet{julia, {
            {"vodka", 80, true},
            {"gin", 200, false},
            {"cognac", 150, false},
            {"martini", 195, false},
            {"whisky", 315, false},
    }};
    purchase eko_lavka{customer_v, {
            {"dumplings", 75, true},
            {"olives", 50, false},
            {"cheesecake", 150, false},
            {"salami", 100, false}
    }};
    purchase pharmacy{julia, {
            {"aspirin", 20, false}
    }};

    std::vector all_customers = {customer_v, julia};
    for (regular_customer c : all_customers) {


    }

    std::ofstream ofile("customer.bin", std::ios::binary);
    customer_v.serialize_binary(ofile);
    ofile.close();

//    std::ifstream infile("customer.bin", std::ios_base::binary);
//    const regular_customer &customer = regular_customer::deserialize_binary(infile);
//    cout << customer.serialize();
//
//    ofile.write((char *) &julia.serialize_binary().get(), sizeof(vlad.serialize_binary().get()));
//    data_description binary_data = customer_v.serialize_binary();
//    ofile.write((char*) customer_v.serialize_binary().get, sizeof(customer_v));

//    std::vector all_purchases = {novus, furshet, eko_lavka, pharmacy};
//    std::vector all_customers = {customer_v, julia};

//    for (regular_customer customer : all_customers) {
//        data_description binary_data = customer.serialize_binary();
//        ofile.write((char*) binary_data.getData(), sizeof(binary_data.size));
//    }
//    ofile.close();

//
//    std::ifstream infile("customer.bin", std::ios_base::binary);
//    std::vector<char> buffer((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
//    char* arr[buffer.size()];
//    std::copy(buffer.begin(), buffer.end(), arr);
//
//    regular_customer::deserialize_binary(arr);

//    cout << customer.serialize();

//    ifstream file;
//
//    // saving to th file
//    file.open("purchase.dat", ios::out | ios::binary);
//    if (!file) {
//        cout << "Error in creating file...\n";
//        return -1;
//    }
//
//    file.write((char *) &vlad.serialize_binary().get(), sizeof(vlad.serialize_binary().get()));
//
//    std::ifstream inFile("customer.bin", std::ios_base::binary);
//
//    inFile.seekg(0, std::ios_base::end);
//    size_t length = inFile.tellg();
//    inFile.seekg(0, std::ios_base::beg);
//
//    std::vector<char> buffer;
//    buffer.reserve(length);
//    std::copy( std::istreambuf_iterator<char>(inFile),
//               std::istreambuf_iterator<char>(),
//               std::back_inserter(buffer) );
//
//    char arr[buffer.size()];
//    std::copy(buffer.begin(), buffer.end(), arr);
//
//    std::vector<regular_customer> customers;
//    customers.push_back(regular_customer::deserialize_binary(arr));
//
//    cout << customer_v.serialize();

//    ifstream file;
//
//    // saving to th file
//    file.open("purchase.dat", ios::out | ios::binary);
//    if (!file) {
//        cout << "Error in creating file...\n";
//        return -1;
//    }
//
//    file.write((char *) &vlad, sizeof(vlad));
//    file.write((char *) &julia, sizeof(julia));

//    file.write((char *) &novus, sizeof(novus));
//    file.write((char *) &furshet, sizeof(furshet));
//    file.write((char *) &eko_lavka, sizeof(eko_lavka));
//    file.write((char *) &pharmacy, sizeof(pharmacy));
//    std::vector all_purchases = {novus, furshet, eko_lavka, pharmacy};
//    for (purchase p : all_purchases) {
//        file.write((char *) &p, sizeof(p));
//    }

//    file.close();
//    cout << "Serialization complete.\n";
//
////    for(int i = 0; i < 10000; ++i) {
//        // reading from the file
//        file.open("purchase.dat", ios::in | ios::binary);
//        if (!file) {
//            cout << "Error in opening file...\n";
//            return -1;
//        }
//
//        do {
//            cout << "Reading the file..." << endl;
//
//            purchase purchase_restored = getPurchaseToRead();
//            file.read((char *) &purchase_restored, sizeof(purchase_restored));
////            if (purchase_restored.get_customer().get_id() != "0") {
//                purchase_restored.print();
////            }
//        } while (!file.eof());
//        file.close();
////    }
//    cout << "Deserialization complete.\n";
    return 0;
}

purchase getPurchaseToRead() {
    regular_customer customer_restored{"0", 0, 0.0};
    purchase purchase_restored{customer_restored, {
            {"", 0, true},
            {"", 0, false}
    }};
    return purchase_restored;
}
