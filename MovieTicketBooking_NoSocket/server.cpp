#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <unistd.h>

// admin x logged in ...should not be printed in the terminal... rather should be written in the log file ..so that no data loss 
// no data loss is there ...and ther terminal remains interactive and does'nt get intervenned by new incoming admin login's

using namespace std;
//2409:408a:88c:cdb6:34a9:e196:93af:f803
class AdminData {
    public:
    char Adminname[50];
    char password[50];
};

int limitadmin=1;

int main() {
    key_t key = ftok("/tmp", 'A');
    int shmid = shmget(key, sizeof(AdminData) * limitadmin, IPC_CREAT | 0666);
    AdminData* Admin_data = (AdminData*)shmat(shmid, NULL, 0);

    cout << "Server started." << endl;
    int admin_cnt = 0;

    if (shmid == -1) {
        // Print an error message and check errno for details
        cerr << "Failed to create shared memory: " << strerror(errno) << endl;
        return 1;  // Exit the program with an error code
    }

    while (admin_cnt<limitadmin) {
        //  sem_wait(sem1);
        if (strlen(Admin_data[admin_cnt].Adminname) > 0) {
            cout << "Admin " << Admin_data[admin_cnt].Adminname << " logged in." << endl;
            admin_cnt = admin_cnt + 1;
        }
        //   sem_post(sem1);
    }
    cout<<"Server cannot take more than "<<limitadmin<<" Admins\n";
    cout<<"Server is shutting down \n";

    shmdt(Admin_data);
    // shmctl(shmid, IPC_RMID, NULL); // forceflly deletes the shared memory 
    return 0;
}
