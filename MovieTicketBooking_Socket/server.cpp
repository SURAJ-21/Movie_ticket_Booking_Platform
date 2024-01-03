#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

// admin x logged in ...should not be printed in the terminal... rather should be written in the log file ..so that no data loss 
// no data loss is there ...and ther terminal remains interactive and does'nt get intervenned by new incoming admin login's

using namespace std;

class AdminData {
    public:
    char Adminname[50];
    char password[50];
};

int limitadmin=2;
const int mainserverPort = 12345;

int main() {
    // key_t key = ftok("/tmp", 'A');
    // int shmid = shmget(key, sizeof(AdminData) * limitadmin, IPC_CREAT | 0666);
    // AdminData* Admin_data = (AdminData*)shmat(shmid, NULL, 0);

    cout << "Server started." << endl;
     // Create socket for IPv6
    int serverSocket = socket(AF_INET6, SOCK_STREAM, 0);

    // Bind socket to port
    struct sockaddr_in6 serverAddr;
    serverAddr.sin6_family = AF_INET6;
    serverAddr.sin6_port = htons(mainserverPort); // Choose a port
    serverAddr.sin6_addr = in6addr_any;

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // Listen for incoming connections
    listen(serverSocket, 5);

    std::cout << "Server listening on port 12345..." << std::endl;
    int admin_cnt = 0;

    while (admin_cnt<limitadmin) {

         // Accept connection
        int clientSocket = accept(serverSocket, nullptr, nullptr); 

        AdminData adminData;
        ssize_t bytesReceived = recv(clientSocket, &adminData, sizeof(adminData), 0);

        if (bytesReceived <= 0) {
            // Connection closed or error
            close(clientSocket);
            continue; // Exit inner loop and wait for the next client
        }
        // Process administrator's data (replace this with your logic)
        std::cout << "\n Received Client Data:" << std::endl;
        std::cout << "New User :: ""Username: " << adminData.Adminname << "  Signed in "<<endl;
        
        // Send a response back to the client
        const char* responseMessage = "Data received by the server!";
        send(clientSocket, responseMessage, strlen(responseMessage), 0);

        admin_cnt++;
        close(clientSocket);
    }
    cout<<"Server cannot take more than "<<limitadmin<<" Admins\n";
    cout<<"Server is shutting down \n";
    close(serverSocket);
    // shmctl(shmid, IPC_RMID, NULL); // forceflly deletes the shared memory 
    return 0;
}
