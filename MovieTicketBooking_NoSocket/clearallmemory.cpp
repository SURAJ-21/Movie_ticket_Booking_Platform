#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>

using namespace std;

class AdminData {
    public:
    char Adminname[50];
    char password[50];
};
class Movie{
    public:
    char name[10];
    char lang[10];
    int rating;
    int cost;

};
class UserData{
    public:
    char username[50];
    char password[50];
};

int limitadmin=1;
int limituser=2;
int movienum=6;

int main() {

    key_t key1 = ftok("/tmp", 'A');
    int shmid1 = shmget(key1, sizeof(AdminData) *limitadmin, 0666);

    key_t key2 = ftok("/tmp", 'C');
    int shmid2 = shmget(key2, sizeof(UserData) * limituser, 0666);
   
    key_t key3 = ftok("/tmp", 'M');//movie client-admin(creater)
    int shmid3 = shmget(key3, sizeof(Movie) * movienum, 0666);

    const char* sem1 = "ad_signup";
    sem_unlink(sem1);

    const char* sem2 = "usr_signup";
    sem_unlink(sem2);

    const char* sem3 = "movie";
    sem_unlink(sem3);

    shmctl(shmid3, IPC_RMID, NULL); // forceflly deletes the shared memory 
    shmctl(shmid1, IPC_RMID, NULL); // forceflly deletes the shared memory 
    shmctl(shmid2, IPC_RMID, NULL); // forceflly deletes the shared memory 
}