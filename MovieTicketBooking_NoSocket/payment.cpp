#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// #include "payment.h"

using namespace std;

int is_new=1;

struct Person {
    string id;
    int balance;
};

int show_current_balance(string userid){

    int cur_bal=2000;
    ifstream infile("balances.txt");
    if (infile.is_open()) {
        while (infile) {
            Person person;
            infile >> person.id >> person.balance;
            if (infile) {
                if(person.id==userid){
                   is_new=0;
                   cur_bal=person.balance;
                }
            }
        }
        infile.close();
    } 
    else cerr << "Unable to open the file for reading." << endl;

    cout << "Current Balance : "<<cur_bal<<"\n";
    return cur_bal;
}
int extract_spend(string Usrid,int curr_bal){
     // use semaphore here in order to avoid race around
    int spent=0;
    vector<Person>people;
    ifstream infile("Curr_tranctions.txt");
    if (infile.is_open()) {
        while (infile) {
            Person person;
            infile >> person.id >> person.balance;
            if (infile) {
                if(person.id==Usrid)
                {
                    spent=person.balance;
                    if(spent>curr_bal)
                    person.balance=-1*person.balance;
                }
                people.push_back(person);
            }
        }
        infile.close();
    } 
    else cerr << "Unable to open the file for reading." << endl;

    ofstream outfile("Curr_tranctions.txt");
    if (outfile.is_open()) {
        for (const Person& person : people) {
            outfile << person.id << " " << person.balance << endl;
        }
        outfile.close();
        // cout << "Data has been updated." << endl;
    }
    else cerr << "Unable to open the file for writing\n";
    
    return spent;
}
void update_new_balance(string Usrid,int new_bal){
    vector<Person> people;

    // use semaphore here in order to avoid race around
    ifstream infile("balances.txt");
    if (infile.is_open()) {
        while (infile) {
            Person person;
            infile >> person.id >> person.balance;
            if (infile) {
                if(person.id!=Usrid)
                people.push_back(person);
            }
        }
        infile.close();
    } 
    else cerr << "Unable to open the file for reading." << endl;
    
    Person person;
    person.id=Usrid;
    person.balance=new_bal;
    people.push_back(person);

    ofstream outfile("balances.txt");
    if (outfile.is_open()) {
        for (const Person& person : people) {
            outfile << person.id << " " << person.balance << endl;
        }
        outfile.close();
        cout << "Data has been updated." << endl;
    } 
    else cerr << "Unable to open the file for writing\n";
}

int main() {
   
    vector<Person> people;

    cout<<"Enter Your UserId :";
    string userid;
    cin>>userid;
   
    int balance=show_current_balance(userid); //1
    int spend=extract_spend(userid,balance);//2

    if(spend>balance){cout<<"Insuffiecient Balance to Book ticket";return 0;}
    // writing the remaining balance back in file
    cout<<"Spend :"<<spend<<"\n";
    int newBalance=balance-spend;

    if(newBalance<0){
        cout<<" Transation Aborted : Insufficient Account Balance \n";
    }
    else{
        cout<<"Remaining Amt in Wallet :"<<newBalance<<"\n";
        update_new_balance(userid,newBalance);//3
    }
    
    cout<<"Type"<<" exit "<< "in current terminal to return to generate Ticket"<<"\n";
}
