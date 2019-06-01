#include <iostream>
#include <fstream>
#include "regular_customer.hpp"

using namespace orlov;
using namespace std;

int main() {

    regular_customer vlad{2978519690, 0, 0.1};
    //regular_customer julia{3207557189, 0, 0.15};

    fstream file;
    
    // saving to th file
    file.open("sample.dat", ios::out | ios::binary);
    if (!file) {
        cout << "Error in creating file...\n";
        return -1;
    }
    file.write((char *) &vlad, sizeof(vlad));
    //file.write((char *) &julia, sizeof(julia));
    file.close();
    cout << "Date saved into file the file.\n";

    // reading from the file
    file.open("sample.dat", ios::in | ios::binary);
    if (!file) {
        cout << "Error in opening file...\n";
        return -1;
    }

    regular_customer vlad_from_file{"0", 0, 0.0};
    //regular_customer julia_from_file;

    file.read((char *) &vlad_from_file, sizeof(vlad_from_file));

    while (!file.eof()) {
        cout<<"Reading the file..."<<endl;
        cout<<"VLAD DATA: "<<endl;
        vlad_from_file.display_regular_customer();
//
//        file.read((char *) &julia_from_file, sizeof(julia_from_file));
//        cout<<"JULIA DATA: "<<endl;
//        julia.display_regular_customer();
    }
    cout << "Date read from the file.\n";
    return 0;
}
