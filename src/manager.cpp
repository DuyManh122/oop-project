#include <ctime>
#include "manager.h"
#include "person.h"
#include "apartment.h"



bool checkValidRoom(uint16_t idRoom);
bool checkValidDay(Date date);
int16_t Sub2Date(Date Date1, Date Date2);
bool Compare2Date(Date Date1, Date Date2);


bool Compare2Date(Date Date1, Date Date2) { //return true if Date2 is equal or larger than Date1
    if (Date2.year > Date1.year) return true;
    else if (Date2.year == Date1.year && Date2.month > Date1.month) return true;
    else if (Date2.year == Date1.year && Date2.month == Date1.month && Date2.day > Date1.day) return true;
    else return false;
}


int16_t Sub2Date(Date Date1, Date Date2) {
    Date SmallerDate, BiggerDate;
    int8_t symbolSign; 
    bool compareValue = Compare2Date(Date1,Date2);
    if (compareValue) {
        SmallerDate = Date1;
        BiggerDate = Date2;
        symbolSign = 1;
    } else {
        SmallerDate = Date2;
        BiggerDate = Date1;
        symbolSign = -1;
    }
    int day_count_smaller_year = 0;
    int day_count_bigger_year = 0; 
    int dayofmonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31}; 
    for (int i = 1; i <= SmallerDate.month; i++) {
        if (i == SmallerDate.month) day_count_smaller_year += SmallerDate.day;
        else if((i == 2) && ((SmallerDate.year % 400 == 0) || ((SmallerDate.year % 4 == 0)&&(SmallerDate.year%100!=0))))
            day_count_smaller_year += 29;
        else day_count_smaller_year += dayofmonth[i-1];
    }

    for (int i = SmallerDate.year; i <= BiggerDate.year; i ++) {
        if (i < BiggerDate.year) {
            if ((i % 400 == 0) || ((i % 4 == 0) && (i % 100 != 0))) day_count_bigger_year += 366;
            else day_count_bigger_year += 365;
        } else {
            for (int j = 1; j <= BiggerDate.month; j++) {
                if (j == BiggerDate.month) day_count_bigger_year += BiggerDate.day;
                else if((j == 2) && ((i % 400 == 0) || ((i % 4 == 0) && (i % 100 != 0))))
                    day_count_bigger_year += 29;
                else day_count_bigger_year += dayofmonth[j-1];
            }
        }
    }
    return (day_count_bigger_year - day_count_smaller_year) * symbolSign;
}

//function will check and return true if idRoom is valid
bool checkValidRoom(uint16_t idRoom) {
    if ( idRoom < 100 || idRoom > 999) return false;       //idRoom is a 3-digit number
    if ( idRoom % 100 > 10 || idRoom % 100 == 0) return false;                   //on each floor only have 10 rooms 
    return true;
}

//function to validate date and return true if date is valid
bool checkValidDay(Date date) {
    uint8_t  d = date.day;
    uint8_t  m = date.month;
    uint16_t y = date.year;

    //check year
    if (y < 1900 || y > 2100) return false;

    //check month 
    if (m < 1 || m > 12) return false;

    //check date
    //months have 31 days
    if ( (d > 31 || d < 1) && ( (m == 1) || (m == 3) || (m==5) || (m==7) || (m==8) || (m==10) || (m==12) ))
        return false;
    //months have 30 days
    if ( (d > 30 || d < 1) && ( (m == 4) || (m == 6) || (m==9) || (m==11)) )
        return false;
    //February && no leap year
    if ( (d > 28 || d < 1) && ((y%400!=0) && ((y%4 != 0)||(y%100==0))) && (m == 2)) return false;
    //February && leep year
    if ( (d > 29 || d < 1) && ((y%400==0) || ((y%4 == 0)&&(y%100!=0))) && (m == 2)) return false;
    return true;
}



Tenant Manager::writeTenantWhenApartmentIsEmpty(uint16_t &IdRoom) {
    Apartment temp;
    temp.id = IdRoom;
    std::string nameTemp, cccdTemp;
    std::cout << "Enter Name: ";
    std::getline(std::cin,nameTemp);
    Date tempbirthday;
    int day, month, year;
    do{ std::cout << "\nEnter Date Of Birth \n";
        std::cout << "Enter Day: ";     std::cin >> day   ; std::cin.ignore();
        std::cout << "Enter Month: ";   std::cin >> month ; std::cin.ignore();
        std::cout << "Enter Year: ";    std::cin >> year  ; std::cin.ignore();
        tempbirthday.day = day;
        tempbirthday.month = month;
        tempbirthday.year = year;
        if(!checkValidDay(tempbirthday)) std::cout << "\nDate is not valid. Try again!\n";
    } while(!checkValidDay(tempbirthday));
    std::cout << "Enter CCCD: ";
    std::getline(std::cin,cccdTemp);
    time_t now_time = time(0);
    struct tm *today = localtime(&now_time);
    uint8_t ageTemp = today->tm_year + 1900 - tempbirthday.year;
    int OwnerOption;
    do {std::cout << "Is tenant the owner of this apartment?\n 0.No\n 1.Yes\nEnter Answer (0 or 1): ";
        std::cin >> OwnerOption; std::cin.ignore();
    }while (OwnerOption != 1 && OwnerOption != 0);
    if (OwnerOption == 1) {
        temp.ownerName = nameTemp;
    }
    else temp.ownerName = "Not Owner";
    temp.isRented = true;
    temp.maxPerson = 4;
    int dayRent, monthRent, yearRent;
    do{ std::cout << "\nEnter Date To Rent \n";
        std::cout << "Enter Day: ";     std::cin >> dayRent   ; std::cin.ignore();
        std::cout << "Enter Month: ";   std::cin >> monthRent ; std::cin.ignore();
        std::cout << "Enter Year: ";    std::cin >> yearRent  ; std::cin.ignore();
        temp.startRent.day = dayRent;
        temp.startRent.month = monthRent;
        temp.startRent.year = yearRent; 
        if(!checkValidDay(temp.startRent)) std::cout << "\nDate is not valid. Try again!\n";
    } while(!checkValidDay(temp.startRent));
    int monthDeposit = 0;
    std::cout << "Enter the number of months deposited: ";
    std::cin >> monthDeposit; std::cin.ignore();

    temp.endRent.day   = temp.startRent.day ;
    temp.endRent.month = (temp.startRent.month + monthDeposit )  % 12;
    temp.endRent.year  = temp.startRent.year + (temp.startRent.month + monthDeposit )  / 12;  

    std::string payment_mt;
    do{ std::cout << "\nEnter Payment Method (CASH, DEBIT_CARD, CREDIT_CARD): ";
        std::cin >> payment_mt; std::cin.ignore();
        if  (payment_mt == "0" || payment_mt == "CASH" || payment_mt == "Cash") {
            temp.paymentMethod = CASH;
            break;
        } else if  (payment_mt == "1" || payment_mt == "DEBIT_CARD" || payment_mt == "Debit_Card") {
            temp.paymentMethod = DEBIT_CARD;
            break;
        } else if  (payment_mt == "2" || payment_mt == "CREDIT_CARD" || payment_mt == "Credit_Card") {
            temp.paymentMethod = CREDIT_CARD;
            break;
        }
        printf("\n Invalid Input\n");
    } while(1);

    Date CurrentDate;
    CurrentDate.day = today->tm_mday;
    CurrentDate.month = today->tm_mon + 1;
    CurrentDate.year = today->tm_year + 1900;
    temp.maxDayPastDue = Sub2Date(CurrentDate,temp.endRent);
    Tenant var(temp, nameTemp, cccdTemp, tempbirthday, ageTemp);
    std::cout << "Input Information\n";
    std::cout << "Name: " << nameTemp << std::endl ;
    std::cout << "Date Of Birth: " << (int)tempbirthday.day << " / " << (int)tempbirthday.month << " / " << (int)tempbirthday.year;
    std::cout << "\nCCCD: " << cccdTemp;
    std::cout << "\nAge: " << (int)ageTemp;
    std::cout << "\nID Room: " << temp.id;
    std::cout << "\nMax Person " << (int)temp.maxPerson;
    std::cout << "\nOwner Name " << temp.ownerName;
    std::cout << "\nStart Rent " << (int)temp.startRent.day << " / " << (int)temp.startRent.month << " / "  << (int)temp.startRent.year;
    std::cout << "\nEnd Rent " << (int)temp.endRent.day << " / " << (int)temp.endRent.month << " / "  << (int)temp.endRent.year;
    std::cout << "\nIs rented " << temp.isRented;
    std::cout << "\nMax Day Past Due " << temp.maxDayPastDue;
    std::cout << "\nPayment Method " << temp.paymentMethod << std::endl;
    return var;
}


void Manager::displayApartmentInfo(Apartment var) {
    std::cout << "\nApartment Information" ;
    std::cout << "\nApartment ID: " << var.id;
    std::cout << "\nMax Person " << (int)var.maxPerson;
    std::cout << "\nOwner Name " << var.ownerName;
    std::cout << "\nStart Rent " << (int)var.startRent.day << " / " << (int)var.startRent.month << " / "  << (int)var.startRent.year;
    std::cout << "\nEnd Rent " << (int)var.endRent.day << " / " << (int)var.endRent.month << " / "  << (int)var.endRent.year;
    std::cout << "\nIs rented " << var.isRented;
    std::cout << "\nMax Day Past Due " << var.maxDayPastDue;
    std::cout << "\nPayment Method " << var.paymentMethod << std::endl;
}

Tenant Manager::extendApartmentEndRent(Tenant& var, uint16_t extendedRentMonth) {
    int dayofmonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31}; 
    Apartment Apart = var.getApartmentInfo();
    Apart.endRent.year  =  Apart.endRent.year + ( ((Apart.endRent.month + extendedRentMonth) > 12) ? (Apart.endRent.month + extendedRentMonth) / 12 : 0);
    Apart.endRent.month = ((Apart.endRent.month + extendedRentMonth) > 12) ? ((Apart.endRent.month + extendedRentMonth) - 12) : (Apart.endRent.month + extendedRentMonth) ; 
    Apart.endRent.day =  Apart.endRent.day > dayofmonth[Apart.endRent.month-1] ? dayofmonth[Apart.endRent.month-1] : Apart.endRent.day;
    time_t now_time = time(0);
    struct tm *today = localtime(&now_time);
    Date CurrentDate;
    CurrentDate.day = today->tm_mday;
    CurrentDate.month = today->tm_mon + 1;
    CurrentDate.year = today->tm_year + 1900;
    Apart.maxDayPastDue = Sub2Date(CurrentDate,Apart.endRent);
    Tenant var_new(Apart,var.getTenantName(), var.getTenantCCCD(), var.getTenantDateOfBirth(), var.getTenantAge());
    return var_new;
}

void Manager::displayTenantInfo(Tenant var) {
    Date DoB = var.getTenantDateOfBirth();
    std::cout << "\nTenant Personal Information";
    std::cout << "\nName: " << var.getTenantName();
    std::cout << "\nAge: " << (int)var.getTenantAge();
    std::cout << "\nDate Of Birth: " << (int)DoB.day << " / " << (int)DoB.month << " / " << (int)DoB.year;
    std::cout << "\nCCCD: " << var.getTenantCCCD();
    
}
Tenant Manager::writeTenantWhenApartmentIsOccupied(Apartment& Apart) {
    std::string nameTemp, cccdTemp;
    std::cout << "Enter Name: ";
    std::getline(std::cin,nameTemp);
    Date tempbirthday;
    int day, month, year;
    do{ std::cout << "\nEnter Date Of Birth \n";
        std::cout << "Enter Day: ";     std::cin >> day   ; std::cin.ignore();
        std::cout << "Enter Month: ";   std::cin >> month ; std::cin.ignore();
        std::cout << "Enter Year: ";    std::cin >> year  ; std::cin.ignore();
        tempbirthday.day = day;
        tempbirthday.month = month;
        tempbirthday.year = year;
        if(!checkValidDay(tempbirthday)) std::cout << "\nDate is not valid. Try again!\n";
    } while(!checkValidDay(tempbirthday));
    std::cout << "Enter CCCD: ";
    std::getline(std::cin,cccdTemp);
    time_t now_time = time(0);
    struct tm *today = localtime(&now_time);
    uint8_t ageTemp = today->tm_year + 1900 - tempbirthday.year;
    Tenant Tent(Apart,nameTemp, cccdTemp, tempbirthday, ageTemp);
    std::cout << "Input Information\n";
    std::cout << "Name: " << nameTemp << std::endl ;
    std::cout << "Date Of Birth: " << (int)tempbirthday.day << " / " << (int)tempbirthday.month << " / " << (int)tempbirthday.year;
    std::cout << "\nCCCD: " << cccdTemp;
    std::cout << "\nAge: " << (int)ageTemp;    
    return Tent;
}
uint16_t Manager::getApartmentID(Apartment& var) {
    return var.id;
}

int16_t Manager::getPastDayAsDue(Apartment& var)  {
    return var.maxDayPastDue;
}