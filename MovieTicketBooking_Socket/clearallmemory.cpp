#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>

using namespace std;

struct Person {
   char id[50];
    int curr_bal;
    int total_spend;
};

int main() {

    key_t key1 = ftok("/tmp", 'P');
    int shmid1 = shmget(key1, sizeof(Person) *3, 0666);

  
    shmctl(shmid1, IPC_RMID, NULL); // forceflly deletes the shared memory 
    // shmctl(shmid2, IPC_RMID, NULL); // forceflly deletes the shared memory 
}