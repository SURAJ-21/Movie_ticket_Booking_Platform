#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <fcntl.h>
#include <ctime>
#include<semaphore.h>

using namespace std;

int is_new=1;

struct Person {
    char id[50];
    int curr_bal;
    int total_spend;
};

int show_current_balance(Person *person){
     cout<<"Into_show_current_bal fnc\n";
    int curr_bal=person[0].curr_bal;

    cout << "Current Balance : "<<curr_bal<<"\n";
    return curr_bal;
}
int extract_spend(Person *person){
    return person[0].total_spend;
}
int update_new_balance(int present_bal,int spend){
  return present_bal-spend;
}

int main() {

    key_t key = ftok("/tmp", 'P');//movie client-admin(creater)
    int shmid = shmget(key, sizeof(Person)*3, 0666);
    Person* person = (Person*)shmat(shmid, NULL, 0);
    
     char userid[50];
    cout<<"Enter Your userId :";
   
    cin>>userid;
    
    // cout<<person[0].id<<"\n";
    cout<<"hi\n";
    if(person != nullptr){
    if(strcmp(userid,person[0].id)==0||userid=="sr"){
         int balance=show_current_balance(person); //1
         int spend=extract_spend(person);//2
             if(spend>balance){
                person[0].total_spend=-1;
                cout<<"Insuffiecient Balance to Book ticket";
                cout<<"Type"<<" exit "<< "in current terminal to return "<<"\n";
                shmdt(person);
                return 0;}

        cout<<"Booking Cost :"<<spend<<"\n";
        int newBalance=update_new_balance(balance,spend);//3
        if(newBalance<0){
            cout<<" Transation Aborted : Insufficient Account Balance \n";
        }
        else{
            cout<<"Remaining Amt in Wallet :"<<newBalance
            <<"\n";
            
        }
    }
    else{
        cout<<"Invalid credentials!!!!\n";
        cout<<"Wrong Username!!\n";
        cout<<"Type"<<" exit "<< "in current terminal to return to generate Ticket"<<"\n";
        shmdt(person);
        return 0;
    }
    }
    else{
          cerr << "Error: Shared memory not attached." << endl;
    return 1; 
    }


     
    
    cout<<"Type"<<" exit "<< "in current terminal to return to generate Ticket"<<"\n";
    shmdt(person);
}
