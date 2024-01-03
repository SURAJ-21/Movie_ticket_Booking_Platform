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
#include "seatmatrix.h"
#include <arpa/inet.h>
#include <sys/socket.h>


using namespace std;
int limituser=2;

string Usrid;//global (will be updated in logn_signup function )
string which_platform="M";//m=>movie

sem_t* sem1 = sem_open("usr_signup", 0, 0666, 1);//client_admin
sem_t* sem2 = sem_open("movie",0, 0666, 1);


const char* AdminserverIP = "2409:408a:1c35:1a95:e1a0:12f0:3e0b:e5ac";  // Replace with the desired IPv6 address
const int serverAdmin_client_login= 12346;
const int serverAdmin_client_other= 12347;

struct Person {
    string id;
    int curr_bal;
    int total_spend;
};

class UserData{
    public:
    char username[50];
    char password[50];
};

class Movie{
    public:
    char name[10];
    char lang[10];
    int rating;
    int cost;
};

const int movienum=6;//give choice to admin to add movie ...by default 3 is there but space for 6 is considered
int hall[9][9];
Movie movie[movienum];

int  login_signup_user(){

    int option;
    cout << "1. Login\n2. Signup" << endl;
    cin >> option;
    cout<<option<<"\n";
    if (option == 1) {
        char username[50];
        char password[50];
        cout << "username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
        Usrid=username;
        bool loggedIn = false;

        // for (int i = 0; i < limituser; i++) {
        //     if (strcmp(username, user_data[i].username) == 0 && strcmp(password, user_data[i].password) == 0) {
        //         cout << "Login success." << endl;
        //         loggedIn = true;
        //         break;
        //     }
        // }

        // if (!loggedIn) {
        //     cout << "Invalid credentials.UserId or Passward is incorrect." << endl;
        //     return 0;
        //    }
        } 
    else if (option == 2) {
        int clientSocket = socket(AF_INET6, SOCK_STREAM, 0);

        // Connect to server using the public IPv6 address
        struct sockaddr_in6 serverAddr;
        serverAddr.sin6_family = AF_INET6;
        serverAddr.sin6_port = htons(serverAdmin_client_login); // Use the same port as the server

        if (inet_pton(AF_INET6, AdminserverIP, &serverAddr.sin6_addr) != 1) {
            std::cerr << "Invalid IPv6 address." << std::endl;
            return EXIT_FAILURE;
        }

        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
            perror("Connect error");
            return EXIT_FAILURE;
        }
        
        char newusername[50];
        char newPassword[50];
        
        bool usernameExists = false;

        cout << "Enter a new username: ";
       
        cin >> newusername;
        Usrid=newusername;
        // // Check if the username already exists
        // sem_wait(sem1);
        // for (int i = 0; i <limituser; i++) {
        //     if (strcmp(newusername, user_data[i].username) == 0) {
        //         usernameExists = true;
        //         break;
        //     }
        // }
        // sem_post(sem1);

        int r=1;
        if (usernameExists) {
            cout << "This username already exists. Try a different one." << endl;
            r=0;
        } 
        else {
            cout << "Enter a new password: ";
            cin >> newPassword;
        
            // sem_wait(sem1);
            // for (int i = 0; i <limituser; i++) {
            //     if (strlen(user_data[i].username) == 0) {
            //         strcpy(user_data[i].username, newusername);
            //         strcpy(user_data[i].password, newPassword);
            //         cout << "User Signup successful." << endl;
            //         r=1;
            //         break;
            //     }
            // }
            // sem_post(sem1);

            UserData userdata;
            strcpy(userdata.username,newusername);
            strcpy(userdata.password,newPassword);
            send(clientSocket, &userdata, sizeof(userdata), 0);

            // Receive data from the server
            char buffer[1024] = {0};
            ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

            if (bytesReceived <= 0) {
                // Connection closed or error
                close(clientSocket);
                // break; // Exit the loop and terminate the client
                return 0;
            }

            std::cout << "Message from server: " << buffer << std::endl;

            // Sleep for a while to simulate some processing
            //  usleep(1000000); // Sleep for 1 second (adjust as needed)
            close(clientSocket);
            }
            if(r==0)
            cout<<"Server is Overloaded , Try after sometime !!!";
        
            return r;
    }
    return 1;
}
void list_all_Movies(int num,int clientSocket){

    const char* requestType = "1"; // Change this to test different request types
    send(clientSocket, requestType, strlen(requestType), 0);

    // Receive and print the item from the server
    // char buffer[1024];
    // Movie movie[movienum];
    ssize_t bytesReceived = recv(clientSocket,&movie, sizeof(movie), 0);

    if (bytesReceived > 0) {
        // buffer[bytesReceived] = '\0';
        cout << "Received Item: " << endl;
    } else {
        cerr << "Error receiving item from the server." << endl;
    }

    int i=0;
    while(movie[i].rating!=0){
      cout<<"\t\t\tMovie "<<i+1<<":\n";
      cout<<"Name: "<<movie[i].name<<"\n";
      cout<<"In:"<<movie[i].lang<<"\n";
      cout<<"Rating: "<<movie[i].rating<<"\n";
      cout<<"Price: "<<movie[i].cost<<"\n";
      i++;
      cout<<"\n";
    }
}
void showseat(int clientSocket){

    const char* requestType = "2"; // Change this to test different request types
    send(clientSocket, requestType, strlen(requestType), 0);

    // Receive and print the item from the server
    // char buffer[1024];
        // int hall[9][9];


    char rechall[sizeof(hall)];
    ssize_t updatedhall = recv(clientSocket, rechall, sizeof(rechall), 0);
    if (updatedhall > 0) {
        std::memcpy(hall, rechall, sizeof(hall));
        // Now 'hall' on the server side is updated.
    } else {
        std::cerr << "Error receiving data from the client." << std::endl;
    }

    if(which_platform=="M"){
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++)
            {
                if(hall[i][j]==1){
                cout<<i<<j<<" ";
                }
                else{
                    cout<<" * ";
                }
                if(j==1||j==6)cout<<"   ";
            }
            cout<<"\n";
            if(i==2||i==6)cout<<"\n";
        }
    }
    // else if(which_platform=="S")
    // stadium();
}
int selectseat(int clientSocket,string &which_seats,vector<int>&seat){

    int cost_per_seat=50;// to be changed further // dynamic // G/S/T seats ...diff cost 
    int ts=seat.size();

    for(int i=0;i<ts;i++){
       cout<<"Seat "<<i+1<<" : ";
       cin>>seat[i];
        if(hall[seat[i]/10][seat[i]%10]==-1){
            cout<<"Seat :"<<seat[i]<<" is already booked !!! Choose other seats!!!\n";
            i--;
            seat[i]=-1;
            continue;
        }
        else{
            hall[seat[i]/10][seat[i]%10]=-1;
        }
        which_seats=which_seats+" "+to_string(seat[i]);
    }

    const char* requestType = "3"; // Change this to test different request types
    send(clientSocket, requestType, strlen(requestType), 0);
    int book[10];

    for(int i=0;i<10;i++)
    if(i<seat.size())
    book[i]=seat[i];
    else
    book[i]=-1;

    send(clientSocket, &book,sizeof(book), 0);
    
    return ts*cost_per_seat;
    
}
void update_transaction(int clientSocket,int spend){
   
   int counter=0;
   recv(clientSocket,&counter,sizeof(counter),0);

   if(counter==1)
   ssize_t bytesSent= send(clientSocket,Usrid.c_str(), sizeof(Usrid), 0);

    int intial_amt;
    
    ssize_t rec=recv(clientSocket,&intial_amt,sizeof(intial_amt),0);
    // if (rec > 0) {
    //     for(auto k:seat)
    // counter=2;
    // send(clientSocket,&counter,sizeof(counter),0);
    //     hall[k/10][k%10]=-1;
    // cout<<"Hi\n";
    // // Now 'hall' on the server side is updated.
    //     } else if (rec == 0) {
    //         std::cerr << "Client disconnected." << std::endl;
    //     } else {
    //         perror("Error receiving data from the client");
    //     }

    int final_amt=intial_amt-spend;
   

    // write intial_amt and spend in shared memory 


    if(final_amt<0)
    final_amt=0;

    // recv(clientSocket,&counter,sizeof(counter),0);
    // if(counter==2)

    send(clientSocket,&final_amt,sizeof(final_amt), 0);
     cout<<"i am in update transaction\n";
     cout<<"hello\n";
     cout<<"Final Amt is :"<<final_amt<<"\n";
    // vector<Person>people;
    // // sem_wait(sem2);
    // ifstream infile("Curr_tranctions.txt");
    // if (infile.is_open()) {
    //     while (infile) {
    //         Person person;
    //         infile >> person.id >> person.total_spend;
    //         if (infile) {
    //             if(person.id!=Usrid)
    //             people.push_back(person);
    //         }
    //     }
    //     infile.close();
    // }
    // else cerr << "Unable to open the file for reading in update_transaction fnc" << endl;
    // // sem_post(sem2);
    // Person person;
    // person.id=Usrid;
    // person.total_spend=final_amt;
    // people.push_back(person);
    // // sem_post(sem2);
    // ofstream outfile("Curr_tranctions.txt");
    // if (outfile.is_open()) {
    //     for (const Person& person : people) {
    //         outfile << person.id << " " << person.total_spend << endl;
    //     }
    //     outfile.close();
    // }
    // else cerr << "Unable to open the file for writing in update_transaction fnc\n";
    // cout<<"\nDone\n";
    // sem_wait(sem2);
}
int payment(int curr_spend){
    // sem_wait(sem2);
    system("x-terminal-emulator");

    // new terminal will be opened ... here run payment.cpp
    // not only this program will be pausee here to do further work unit the opened terminl is close using exit 

    int gen_ticket=0;
    ifstream infile("Curr_tranctions.txt");
    if (infile.is_open()) {
        while (infile) {
            Person person;
            infile >> person.id >> person.total_spend;
            if (infile) {
                if(person.id==Usrid){
                    if(person.total_spend>0){
                        gen_ticket=1;
                        break;
                    }
                }
            }
        }
        infile.close();
    } 
    else cerr << "Unable to open the file for reading." << endl;
    // sem_post(sem2);
    return gen_ticket;
}
void generate_ticket(int amt,string movie,string day,string tme, int no_of_seats,string which_seats,int hall_no,string screen){
    time_t now = time(0);
    tm* today = localtime(&now);
    time_t tomorrow = now + 24 * 60 * 60; // Add 24 hours in seconds
    tm* tmorrow = localtime(&tomorrow);
    time_t dayAfterTomorrow = tomorrow + 24 * 60 * 60; // Add another 24 hours
    tm* dayAfterTmrw = localtime(&dayAfterTomorrow);

    cout<<"================================Choose Date:=================================\n";
    cout<<"\t\t 1.Today\t\t 2.Tommorow \t\t 3.Day after Tommorow\n";
    int d;cin>>d;
    cout<<"==============================Select TimeSlot:================================\n";
    cout<<"A: 9:00 \tB: 11:00\tC: 13:00\n";
    cout<<"D: 15:00 \tE: 17:00\tF: 19:00\n";
    cout<<"G: 21:00 \tH: 23:00\tI: 23:30\n";
    char t;cin>>t;
    vector<string>v1={"9:00","11:00","13:00","15:00","17:00","19:00","21:00","23:00","23:30"};
    tme=v1[t-65];
    string today2,tommorow,dayaftrtom;
    if(d==1)today2=day=to_string(today->tm_mon + 1) + "/" + to_string(today->tm_mday) + "/" + to_string(today->tm_year + 1900);
    else if(d==2)tommorow=day=to_string(tmorrow->tm_mon + 1) + "/" + to_string(tmorrow->tm_mday) + "/" + to_string(tmorrow->tm_year + 1900);
    else dayaftrtom=day=to_string(dayAfterTmrw->tm_mon + 1) + "/" + to_string(dayAfterTmrw->tm_mday) + "/" + to_string(dayAfterTmrw->tm_year + 1900);
    
    cout<<"Congratulations !!! Your Ticket has been booked\n\n";
    cout<<"Movie : "<<movie<<"\n";
    cout<<"Date : "<<day<<"\n";
    cout<<"Time : "<<tme<<"\n";
    cout<<"Hall : "<<hall_no<<"  "<<"Screen : "<<screen<<"\n";
    cout<<"Seat : "<<no_of_seats<<"("<<which_seats<<")"<<"\n";
    cout<<"\n";

}
int terminator(UserData* user_data,Movie* movie){
    cout<<"Have a Nice Day !!\nDo visit again!!!!\n";
    shmdt(user_data);
    shmdt(movie);
    sem_close(sem2);
    return 0;
}
void release_seats(vector<int>&seat){
  unordered_map<int,int>m;
  ifstream infile("seat.txt");
    if (infile.is_open()) {
        while (infile) {
            char r,c;
            infile >>r>>c;
            if (infile) {
                m[(r-48)*10+(c-48)]++;
            }
        }
        infile.close();
    } 
    else cerr << "Unable to open the seat file for reading in release seat." << endl;
    // sem_post(sem2);
    for(int i=0;i<seat.size();i++){
        if(m[seat[i]]>0)m[seat[i]]=0;
    }
     
    //  sem_wait(sem2);
    ofstream outfile("seat.txt");
    if (outfile.is_open()) {
        for(auto k:m){
            if(k.second>0)
             outfile << to_string(k.first/10)<< " " <<  to_string(k.first%10) << endl;
        }
        outfile.close();
    }
    else cerr << "Unable to open the file for writing in release seat fnc\n";
    // sem_post(sem2);
}

int main() {

    // const char* serverIP = "409:4064:2593:8c15:e731:d117:3a0a:b988";  // Replace with the desired IPv6 address
    // const int serverPort = 12347;

    // Create socket for IPv6
    int clientSocket = socket(AF_INET6, SOCK_STREAM, 0);

    // Connect to the server
    struct sockaddr_in6 serverAddr;
    serverAddr.sin6_family = AF_INET6;
    serverAddr.sin6_port = htons(serverAdmin_client_other);

    if (inet_pton(AF_INET6,AdminserverIP, &serverAddr.sin6_addr) != 1) {
        std::cerr << "Invalid IPv6 address." << std::endl;
        return EXIT_FAILURE;
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connect error");
        return EXIT_FAILURE;
    }

    
    // key_t key = ftok("/tmp", 'C');
    // int shmid = shmget(key, sizeof(UserData) * limituser, 0666);
    // UserData* user_data = (UserData*)shmat(shmid, NULL, 0);

    int movienum=6;//give choice to admin to add movie ...by default 3 is there but space for 6 is considered
    
    // key_t key3 = ftok("/tmp", 'M');//movie client-admin(creater)
    // int shmid3 = shmget(key3, sizeof(Movie) * movienum, 0666);
    // Movie* movie = (Movie*)shmat(shmid3, NULL, 0);


    // key_t key = ftok("/tmp", 'P');//movie client-admin(creater)
    // int shmid = shmget(key, sizeof(Person)*3, IPC_CREAT | 0666);
    // Person* person = (Person*)shmat(shmid, NULL, 0);
     
    int status=1,final_status=1;

    status=login_signup_user();  //1
     cout<<"i am here1\n";
    if(status==0){
         cout<<"i am here2\n";
         final_status=login_signup_user();  //1
    }
    // else if(status==-1)return terminator(user_data,movie);
         
    // if(final_status==0||final_status==-1){
    //     cout<<"Too many unsuccessful attempts \n";
    //      return terminator(user_data,movie);
    // }
    
    int choice=0;
    int which;
    int final_amt=0;///update this as per dynamic pricing  

    // sem_wait(sem2);
    list_all_Movies(movienum,clientSocket); //2
    // // sleep(30);
    // sem_post(sem2);

    int index=0,num_seats=0,hall_no=3;string name,date,time,screen="A2",which_seats="";
    cout<<"Enter the index of the movie to be selected :";
    cin>>index;

    // sem_wait(sem2);
    final_amt+=movie[index-1].cost;
    name=movie[index-1].name;
    name=name+"( "+movie[index-1].lang+" )";
    // sem_post(sem2);

    showseat(clientSocket); //3
    

    // calculat ethe ammount according to movie selected and seat quality 
    // now write this data into file with userid(unique) in form userid: cost(currecnt transaction /last tranction );
    // apply all the dynamic pricing and all policies here to manipulate the price
     
    cout<<"Total seats to be booked :";
    cin>>num_seats; 
    vector<int>seat(num_seats,0);
    final_amt+=selectseat(clientSocket,which_seats,seat);  //4
    int gen_ticket=-1;
      const char* requestType = "4"; // Change this to test different request types
    send(clientSocket, requestType, strlen(requestType), 0);
    update_transaction(clientSocket,final_amt); //5
}