#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>
#include <vector>
#include <unordered_map>

using namespace std;

class AdminData {
    public:
    char Adminname[50];
    char password[50];
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

int limitadmin=1;
int limituser=2;
const int movienum=6;//give choice to admin to add movie ...by default 3 is there but space for 6 is considered

const char* mainserverIP = "2409:408a:1c35:1a95:1605:8d49:c21f:f3d1";  // Replace with the desired IPv6 address
const int mainserverPort = 12345;
const int serverAdmin_client_login= 12346;
const int serverAdmin_client_other= 12347;

Movie movie[movienum];
int hall[9][9];
unordered_map<string,int>m;
sem_t* sem1 ;//admin-server
sem_t* sem2 ;
sem_t* sem3 ;//client_admin

int login_signup_handle(){
         int option;
        cout << "1. Login\n2. Signup" << endl;
        cin >> option;

        if (option == 1) {
            char Adminname[50];
            char password[50];

            cout << "Adminname: ";
            cin >> Adminname;
            cout << "Password: ";
            cin >> password;

            bool loggedIn = false;
            // for (int i = 0; i < limitadmin; i++) {
            //     if (strcmp(Adminname, Admin_data[i].Adminname) == 0 && strcmp(password, Admin_data[i].password) == 0) {
            //         cout << "Login successflimitadmin." << endl;
            //         loggedIn = true;
            //         break;
            //     }
            // }

            if (!loggedIn) {
                cout << "Login failed. Invalid credentials." << endl;
            }
        } 
        else if (option == 2) {

            int clientSocket = socket(AF_INET6, SOCK_STREAM, 0);

            // Connect to server using the public IPv6 address
            struct sockaddr_in6 serverAddr;
            serverAddr.sin6_family = AF_INET6;
            serverAddr.sin6_port = htons(mainserverPort); // Use the same port as the server

            if (inet_pton(AF_INET6, mainserverIP, &serverAddr.sin6_addr) != 1) {
                std::cerr << "Invalid IPv6 address." << std::endl;
                return EXIT_FAILURE;
            }

            if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
                perror("Connect error");
                return EXIT_FAILURE;
            }

            char newAdminname[50];
            char newPassword[50];

            cout << "Enter a new Adminname: ";
            cin >> newAdminname;
            
            // Check if the Adminname already exists
            bool AdminnameExists = false;
            // for (int i = 0; i <limitadmin; i++) {
            //     if (strcmp(newAdminname, Admin_data[i].Adminname) == 0) {
            //         AdminnameExists = true;
            //         break;
            //     }
            // }
            if (AdminnameExists) {
                cout << "Adminname already exists. Try a different one." << endl;
            } else {
                cout << "Enter a new password: ";
                cin >> newPassword;
            
                AdminData adminData;
                strcpy(adminData.Adminname,newAdminname);
                strcpy(adminData.password,newPassword);
                send(clientSocket, &adminData, sizeof(adminData), 0);

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



                //  sem_wait(sem1);
                //     for (int i = 0; i <limitadmin; i++) {
                //         if (strlen(Admin_data[i].Adminname) == 0) {
                //             strcpy(Admin_data[i].Adminname, newAdminname);
                //             strcpy(Admin_data[i].password, newPassword);
                //             cout << "Admin Signup successful." << endl;
                //             break;
                //         }
                //     }
                //  sem_post(sem1);
            }
        }
        return 1;

}
void moviedetails(int num){
    // 3 movies by default
    // to change movie , to change movie cost and all thing will be given to admin
    int rat[num]={9,8,7,0,0,0};
    string mname[num]={"barbie","openhimer","Baby","","",""};
    string lang[num]={"English","spanish","hindi","","",""};
    int cst[num]={90,70,50,0,0,0};
    // less than 7 rating all is Rs 30 
    // 7 => 50 , // 8 =>70 // 9=>90 \\ 10=>100;
    // dynamic pricing to be added in as future scope
    int i=0;
    // sem_wait(sem2);
    for(int j=0;j<movienum;j++){
        if(rat[i]>0&&(movie[j].rating==0||movie[j].rating==-1))
        {
            strcpy(movie[i].name,mname[i].c_str());
            strcpy(movie[i].lang,lang[i].c_str());
            movie[i].rating=rat[i];
            movie[i].cost=cst[i];
            i++;
        }
    }

    // sem_post(sem2);
}
void showmovie(int num){
    int i=0;
    // sem_wait(sem2);
    while(i<num){
        if(movie[i].rating>0)
        {   
            cout<<"Movie : "<<i+1<<"\n";
            cout<<"Name: "<<movie[i].name<<"\n";
            cout<<"In:"<<movie[i].lang<<"\n";
            cout<<"Rating: "<<movie[i].rating<<"\n";
            cout<<"Price: "<<movie[i].cost<<"\n";
            cout<<"\n";
        }
      i++;
    }
    // sem_post(sem2);
}
void addmovie(int num){
    int i=0;
    // sem_wait(sem2);
    while(movie[i].rating>0)i++;
    // sem_post(sem2);
 
    if(i==6){
        cout<<"Capacity FULL !!! Please remove older movies \n";
        return ;
        }
    cout<<"Please Provide Movie Details below :\n";
    cout<<"Movie Name :";cin>>movie[i].name;
    cout<<"Language :";cin>>movie[i].lang;
    cout<<"Rating :";cin>>movie[i].rating;
    cout<<"Ticket Price";cin>>movie[i].cost;

}
void removemovie(int num ,int whichmovie){
    // lsit all the movie to admin so that he can choose the one which is to be removed
    // "whichmovie" must be movie index which is to be removed
    // sem_wait(sem2);
    movie[whichmovie].rating=-1;
    strcpy(movie[whichmovie].name, "");
    strcpy(movie[whichmovie].lang, "");
    movie[whichmovie].cost=0;
    // sem_post(sem2);
}
void all(){
        int choice=0;
        int which;
        while(choice!=5){
            cout<<"\nEnter 1 to initialize default movie details \n";
            cout<<"Enter 2 to List all the movies in the hall\n";
            cout<<"Enter 3 to Add a movie \n";
            cout<<"Enter 4 to Remove a movie from the List \n";
            cout<<"Enter 5 to Exit\n";
            cout << "Enter your choice (1-5): ";
            cin >> choice;
            
            switch (choice) {
                case 1:
                    moviedetails( movienum);
                    break;
                case 2:
                    showmovie(movienum);
                    break;
                case 3:
                    addmovie(movienum);
                    break;
                case 4:
                    cout << "Enter index of movie to be removed : ";
                    cin >> which;
                    removemovie(movienum, which);
                    break;
                case 5:
                    cout<<"Have a Nice Day !!\nDo visit again!!!!\n";
                    break;
                default:
                    cout << "Invalid choice." << endl;
                    break;
                 }
            }
        cout<<"I am thread with name all who handles movie display\n";
}
void handleClient(int clientSocket) {
    char buffer[1024];
    ssize_t bytesReceived;
   
    while (true) {
        // Receive request type from the client
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived <= 0) {
            // Connection closed or error
            break;
        }
         ssize_t rec=0;
        buffer[bytesReceived] = '\0';
        // Process the request type and send the corresponding item
        int requestType = atoi(buffer);
        const char* itemToSend = nullptr;
        int k=0;
         int initial_amt=2000;
        string t;
        int counter;
        switch (requestType) {
            case 1:
                send(clientSocket, &movie,sizeof(movie), 0);
                break;
            case 2:
                send(clientSocket, &hall,sizeof(hall), 0);
                break;
            case 3:
                int seat[10];
                rec = recv(clientSocket,seat, 10* sizeof(int), 0);
                if (rec > 0) {
                    for(int i=0;i<10;i++){
                        if(seat[i]>=0){
                            hall[seat[i]/10][seat[i]%10]=-1;
                        }
                    }
                    // Now 'hall' on the server side is updated.
                } else if (rec == 0) {
                    std::cerr << "Client disconnected." << std::endl;
                } else {
                    perror("Error receiving data from the client");
                }
                break;
            case 4:

                counter=1;
                send(clientSocket,&counter,sizeof(counter),0);

                char name[1024];
                rec = recv(clientSocket,name,sizeof(name), 0);

                if(rec==0)std::cerr << "Client disconnected." << std::endl;
                else if(rec<0)perror("Error receiving data from the client");

                t=string(name);
                if(m.find(t)==m.end())
                initial_amt=2000;
                else
                initial_amt=m[t];

                send(clientSocket,&initial_amt,sizeof(initial_amt),0);
                
                int final_amt;
                recv(clientSocket,&final_amt,sizeof(final_amt),0);

                // cout<<"User"<<": "<<t<<"::: final amt is "<<final_amt<<"\n";
                 m[t]=final_amt;

                 break;
            case 5:
                int seat1[10];
                rec = recv(clientSocket,seat1, 10* sizeof(int), 0);
                if (rec > 0) {
                    
                    for(int i=0;i<10;i++){
                        if(seat1[i]>=0){
                            hall[seat1[i]/10][seat1[i]%10]=1;
                        }
                    }
                    // Now 'hall' on the server side is updated.
                } else if (rec == 0) {
                    std::cerr << "Client disconnected." << std::endl;
                } else {
                    perror("Error receiving data from the client");
                }
                break;
                
            default:
                itemToSend = "Invalid Request";
                k=1;
        }

        // Send the item to the client
        if(k==1)
        send(clientSocket, itemToSend,strlen(itemToSend), 0);
    }

    // Close the client socket
    close(clientSocket);
}
void act_server(){
    

    // Create socket for IPv6
    int serverSocket = socket(AF_INET6, SOCK_STREAM, 0);

    // Bind socket to port
    struct sockaddr_in6 serverAddr;
    serverAddr.sin6_family = AF_INET6;
    serverAddr.sin6_port = htons(serverAdmin_client_other);
    serverAddr.sin6_addr = in6addr_any;

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // Listen for incoming connections
    listen(serverSocket, 5);

    cout << "Server listening on port " << serverAdmin_client_other<< "..." << endl;
    int user_cnt=0;
    // vector<thread> threads;
    while (user_cnt<limituser) {
        // Accept connection
        int clientSocket = accept(serverSocket, nullptr, nullptr);

        // Handle the client in a separate thread or process
        // threads.emplace_back(handleClient, ClientSocket);
        handleClient(clientSocket);
        user_cnt++;
    }

    // The server will never reach here in this example, as it's intended to run indefinitely

    // Close the server socket (this line will not be reached in this example)
    close(serverSocket);

    // return 0;

}


int main() {
    
   
     for(int i=0;i<9;i++)
       for(int j=0;j<9;j++)
         hall[i][j]=1;
    // key_t key1 = ftok("/tmp", 'A');//admin-server(creater)
    // int shmid1 = shmget(key1, sizeof(AdminData) *limitadmin, 0666);
    // AdminData* Admin_data = (AdminData*)shmat(shmid1, NULL, 0);

    // key_t key2 = ftok("/tmp", 'C');//client-admin(creater)
    // int shmid2 = shmget(key2, sizeof(UserData) * limituser, IPC_CREAT | 0666);
    // UserData* user_data = (UserData*)shmat(shmid2, NULL, 0);

    // key_t key3 = ftok("/tmp", 'M');//movie client-admin(creater)
    // int shmid3 = shmget(key3, sizeof(Movie) * movienum, IPC_CREAT | 0666);
    // Movie* movie = (Movie*)shmat(shmid3, NULL, 0);
    
    // sem1 = sem_open("ad_signup", O_CREAT | O_EXCL, 0666, 1);//admin-server
    // if (sem1 == SEM_FAILED) {
    //     perror("sem_open");
    //     cout<<"sem1 error"<<"\n";
    //     exit(0);
    // }
    // sem2 = sem_open("movie", O_CREAT | O_EXCL, 0666, 1);
    // if (sem1 == SEM_FAILED) {
    //     perror("sem_open");
    //     cout<<"sem2 error"<<"\n";
    //     exit(0);
    // }
    // sem3 = sem_open("usr_signup", O_CREAT | O_EXCL, 0666, 1);//client_admin
    // if (sem1 == SEM_FAILED) {
    //     perror("sem_open");
    //     cout<<"sem3 error"<<"\n";
    //     exit(0);
    // }
    pid_t p =fork();
    if(p==0)
    {
        // this child process to handle acting as a server for client 
       int user_cnt=0;
       cout << "Admin Server started." << endl;
     // Create socket for IPv6
        int serverSocket = socket(AF_INET6, SOCK_STREAM, 0);

        // Bind socket to port
        struct sockaddr_in6 serverAddr;
        serverAddr.sin6_family = AF_INET6;
        serverAddr.sin6_port = htons(serverAdmin_client_login); // Choose a port
        serverAddr.sin6_addr = in6addr_any;

        bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

        // Listen for incoming connections
        listen(serverSocket, 5);

        std::cout << "Server listening on port 12346..." << std::endl;
        int admin_cnt = 0;

       while (user_cnt<limituser)
         {

            // sem_wait(sem3);
            // while(strlen(user_data[user_cnt].username) !=0){
            //     cout <<"\n-------------------"<<getpid()<<" : User " << user_data[user_cnt].username << " logged in------------------------" << endl;
            //     user_cnt = (user_cnt + 1);
            //     }
            // sem_post(sem3);
            // sleep(2);

            int clientSocket = accept(serverSocket, nullptr, nullptr); 
            UserData userdata;
            ssize_t bytesReceived = recv(clientSocket, &userdata, sizeof(userdata), 0);

            if (bytesReceived <= 0) {
                // Connection closed or error
                close(clientSocket);
                continue; // Exit inner loop and wait for the next client
            }

            // Process administrator's data (replace this with your logic)
            std::cout << "Received Client Data:" << std::endl;
            std::cout <<"New Client :: " "Username: " << userdata.username <<" ==> Signed in "<< std::endl;
            // std::cout << "Password: " << adminData.password << std::endl;

            // Send a response back to the client
            const char* responseMessage = "Data received by the server!";
            send(clientSocket, responseMessage, strlen(responseMessage), 0);
             
            user_cnt = (user_cnt + 1);
         }    

        cout<<"AdminServer cannot take more than "<<limituser<<" Clients\n";

        close(serverSocket);
        // shmdt(user_data);
        // shmctl(shmid2, IPC_RMID, NULL); // forceflly deletes the shared memory 
    }
    else
    {     
        // this is main admin process
        cout << "Admin started." << endl;
        login_signup_handle();
        //below calls must be in switch case ...interactive
        thread t1(all);
        thread t2(act_server);
        t1.join();
        t2.join();

       
        //  moviedetails(movie, movienum);
        //  showmovie(movie, movienum);
        // //   addmovie(movie, movienum);
    }
        int status;
        while (wait(&status) > 0);

        // sem_close(sem2);
        // sem_close(sem1);
        // sem_close(sem3);
        // shmdt(Admin_data);
        // shmdt(movie);
        // shmctl(shmid1, IPC_RMID, NULL); // forceflly deletes the shared memory 
    //  shmdt(user_data);
    //  shmdt(Admin_data);
    //  shmctl(shmid1, IPC_RMID, NULL); // forceflly deletes the shared memory 
    //  shmctl(shmid2, IPC_RMID, NULL); // forceflly deletes the shared memory 
     
    return 0;
}
