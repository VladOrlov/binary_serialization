#include "purchase.hpp"
#include "regular_customer.hpp"

#include <iostream>
#include <fstream>

purchase getPurchaseToRead();

using namespace orlov;
using namespace std;

int main() {

    regular_customer vlad{2978519690, 0, 0.1};
    regular_customer julia{3207557189, 0, 0.15};

    purchase novus{vlad, {
            {"potatoes", 6, true},
            {"bananas", 32, false}
    }};
    purchase furshet{julia, {
            {"vodka", 80, true},
            {"gin", 200, false},
            {"cognac", 150, false},
            {"martini", 195, false},
            {"whisky", 315, false},
    }};
    purchase eko_lavka{vlad, {
            {"dumplings", 75, true},
            {"olives", 50, false},
            {"cheesecake", 150, false},
            {"salami", 100, false}
    }};
    purchase pharmacy{julia, {
            {"aspirin", 20, false}
    }};



    fstream file;

    // saving to th file
    file.open("purchase.dat", ios::out | ios::binary);
    if (!file) {
        cout << "Error in creating file...\n";
        return -1;
    }

    std::vector all_purchases = {novus, furshet, eko_lavka, pharmacy};
    for (purchase p : all_purchases) {
        file.write((char *) &novus, sizeof(novus));
    }

    file.close();
    cout << "Date saved into file the file.\n";

    // reading from the file
    file.open("purchase.dat", ios::in | ios::binary);
    if (!file) {
        cout << "Error in opening file...\n";
        return -1;
    }

    do {
        purchase purchase_restored_1 = getPurchaseToRead();
        cout<<"Reading the file..."<<endl;

        file.read((char *) &purchase_restored_1, sizeof(purchase_restored_1));
        purchase_restored_1.print();
    }while (!file.eof());

    cout << "Deserialization complete.\n";
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
