#include <cstdint>
#include <limits>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

#define FILE_NAME "emp.dat"

#include "ranged_value.hpp"
#include "util.hpp"

#include <cstdint>
#include <limits>
#include <string>
#include <utility>
#include <variant>

using namespace std;
using namespace orlov;

namespace orlov {
    struct taxpayer_id_int_ranges {
        static constexpr std::uint64_t min = 1;
        static constexpr std::uint64_t max = 10e10;
    };
    using taxpayer_id_int_t = ranged_value<std::uint64_t, taxpayer_id_int_ranges>;
    using taxpayer_id_t = std::variant<std::monostate, taxpayer_id_int_t, std::string>;

    struct amount_ranges {
        static constexpr double min = 0;
        static constexpr double max = std::numeric_limits<double>::max();
    };
    using amount_t = ranged_value<double, amount_ranges>;

    struct discount_ranges {
        static constexpr double min = 0;
        static constexpr double max = 1 - std::numeric_limits<double>::epsilon();
    };
    using discount_t = ranged_value<double, discount_ranges>;

    class regular_customer final {
    private:
        taxpayer_id_t taxpayer_id_;
        amount_t amount_spent_;
        discount_t discount_rate_;

    public:
        explicit
        regular_customer(taxpayer_id_t taxpayer_id, amount_t amount_spent = 0, discount_t discount_rate = 0) :
                taxpayer_id_{taxpayer_id},
                amount_spent_{amount_spent},
                discount_rate_{discount_rate} {

        }

        auto get_amount_spent() const {
            return amount_spent_.get();
        }

        void set_amount_spent(amount_t::underlying_t value) {
            amount_spent_.set(value);
        }

        auto get_discount_rate() const {
            return discount_rate_.get();
        }

        auto get_id() const {
            std::string str;
            return std::visit(visitor_options{
                    [&str](taxpayer_id_int_t arg) {
                        str = std::to_string(arg.get());
                    }
            });
        }

        void set_discount_rate(discount_t::underlying_t value) {
            discount_rate_.set(value);
        }

        void display_regular_customer() {
            cout << "CUSTOMER ID: " << get_id() << endl
                 << "CUSTOMER AMOUNT: " << get_amount_spent() << endl
                 << "CUSTOMER DISCOUNT_RATE: " << get_discount_rate() << endl;
        }
    };
}

//    class Employee {
//    private :
//        int 	empID;
//        char 	empName[100] ;
//        char 	designation[100];
//        int 	ddj,mmj,yyj;
//        int 	ddb,mmb,yyb;
//    public  :
//        //function to read employee details
//        void readEmployee(){
//            cout<<"EMPLOYEE DETAILS"<<endl;
//            cout<<"ENTER EMPLOYEE ID : " ;
//            cin>>empID;
//            cin.ignore(1);
//            cout<<"ENTER  NAME OF THE EMPLOYEE : ";
//            cin.getline(empName,100);
//
//            cout<<"ENTER DESIGNATION : ";
//            cin.getline(designation,100);
//
//            cout<<"ENTER DATE OF JOIN:"<<endl;
//            cout<<"DATE : "; cin>>ddj;
//            cout<<"MONTH: "; cin>>mmj;
//            cout<<"YEAR : "; cin>>yyj;
//
//            cout<<"ENTER DATE OF BIRTH:"<<endl;
//            cout<<"DATE : "; cin>>ddb;
//            cout<<"MONTH: "; cin>>mmb;
//            cout<<"YEAR : "; cin>>yyb;
//        }
//        //function to write employee details
//        void displayEmployee(){
//            cout<<"EMPLOYEE ID: "<<empID<<endl
//                <<"EMPLOYEE NAME: "<<empName<<endl
//                <<"DESIGNATION: "<<designation<<endl
//                <<"DATE OF JOIN: "<<ddj<<"/"<<mmj<<"/"<<yyj<<endl
//                <<"DATE OF BIRTH: "<<ddb<<"/"<<mmb<<"/"<<yyb<<endl;
//        }
//    };
//
//};