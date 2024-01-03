#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

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
int movienum=6;//give choice to admin to add movie ...by default 3 is there but space for 6 is considered

sem_t* sem1 ;//admin-server
sem_t* sem2 ;
sem_t* sem3 ;//client_admin

void login_signup_handle(AdminData* Admin_data){
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
            for (int i = 0; i < limitadmin; i++) {
                if (strcmp(Adminname, Admin_data[i].Adminname) == 0 && strcmp(password, Admin_data[i].password) == 0) {
                    cout << "Login successflimitadmin." << endl;
                    loggedIn = true;
                    break;
                }
            }

            if (!loggedIn) {
                cout << "Login failed. Invalid credentials." << endl;
            }
        } 
        else if (option == 2) {
            char newAdminname[50];
            char newPassword[50];

            cout << "Enter a new Adminname: ";
            cin >> newAdminname;

            // Check if the Adminname already exists
            bool AdminnameExists = false;
            for (int i = 0; i <limitadmin; i++) {
                if (strcmp(newAdminname, Admin_data[i].Adminname) == 0) {
                    AdminnameExists = true;
                    break;
                }
            }
            if (AdminnameExists) {
                cout << "Adminname already exists. Try a different one." << endl;
            } else {
                cout << "Enter a new password: ";
                cin >> newPassword;


             sem_wait(sem1);
                for (int i = 0; i <limitadmin; i++) {
                    if (strlen(Admin_data[i].Adminname) == 0) {
                        strcpy(Admin_data[i].Adminname, newAdminname);
                        strcpy(Admin_data[i].password, newPassword);
                        cout << "Admin Signup successful." << endl;
                        break;
                    }
                }
             sem_post(sem1);
            }
        }
}
void moviedetails(Movie* movie,int num){
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
    sem_wait(sem2);
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
    sem_post(sem2);
}
void showmovie(Movie* movie,int num){
    int i=0;
    sem_wait(sem2);
    while(i<num){
        if(movie[i].rating>0)
        {
            cout<<"Name: "<<movie[i].name<<"\n";
            cout<<"In:"<<movie[i].lang<<"\n";
            cout<<"Rating: "<<movie[i].rating<<"\n";
            cout<<"Price: "<<movie[i].cost<<"\n";
        }
      i++;
    }
    sem_post(sem2);
}
void addmovie(Movie* movie,int num){
    int i=0;
    sem_wait(sem2);
    while(movie[i].rating>0)i++;
    sem_post(sem2);
 
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
void removemovie(Movie* movie, int num ,int whichmovie){
    // lsit all the movie to admin so that he can choose the one which is to be removed
    // "whichmovie" must be movie index which is to be removed
    sem_wait(sem2);
    movie[whichmovie].rating=-1;
    strcpy(movie[whichmovie].name, "");
    strcpy(movie[whichmovie].lang, "");
    movie[whichmovie].cost=0;
    sem_post(sem2);
}

int main() {

    key_t key1 = ftok("/tmp", 'A');//admin-server(creater)
    int shmid1 = shmget(key1, sizeof(AdminData) *limitadmin, 0666);
    AdminData* Admin_data = (AdminData*)shmat(shmid1, NULL, 0);

    key_t key2 = ftok("/tmp", 'C');//client-admin(creater)
    int shmid2 = shmget(key2, sizeof(UserData) * limituser, IPC_CREAT | 0666);
    UserData* user_data = (UserData*)shmat(shmid2, NULL, 0);

    key_t key3 = ftok("/tmp", 'M');//movie client-admin(creater)
    int shmid3 = shmget(key3, sizeof(Movie) * movienum, IPC_CREAT | 0666);
    Movie* movie = (Movie*)shmat(shmid3, NULL, 0);
    
    sem1 = sem_open("ad_signup", O_CREAT | O_EXCL, 0666, 1);//admin-server
    if (sem1 == SEM_FAILED) {
        perror("sem_open");
        cout<<"sem1 error"<<"\n";
        exit(0);
    }
    sem2 = sem_open("movie", O_CREAT | O_EXCL, 0666, 1);
    if (sem1 == SEM_FAILED) {
        perror("sem_open");
        cout<<"sem2 error"<<"\n";
        exit(0);
    }
    sem3 = sem_open("usr_signup", O_CREAT | O_EXCL, 0666, 1);//client_admin
    if (sem1 == SEM_FAILED) {
        perror("sem_open");
        cout<<"sem3 error"<<"\n";
        exit(0);
    }
    pid_t p =fork();
    if(p==0)
    {
        // this child process to handle acting as a server for client 
       int user_cnt=0;
       while (user_cnt<limituser)
         {
            sem_wait(sem3);
            while(strlen(user_data[user_cnt].username) !=0){
                cout <<"\n-------------------"<<getpid()<<" : User " << user_data[user_cnt].username << " logged in------------------------" << endl;
                user_cnt = (user_cnt + 1);
                }
            sem_post(sem3);
            // sleep(2);
         }    
        cout<<"AdminServer cannot take more than "<<limituser<<" Clients\n";
        // shmdt(user_data);
        // shmctl(shmid2, IPC_RMID, NULL); // forceflly deletes the shared memory 
    }
    else
    {     
        // this is main admin process
        cout << "Admin started." << endl;
        login_signup_handle(Admin_data);
        //below calls must be in switch case ...interactive

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
                    moviedetails(movie, movienum);
                    break;
                case 2:
                    showmovie(movie, movienum);
                    break;
                case 3:
                    addmovie(movie, movienum);
                    break;
                case 4:
                    cout << "Enter index of movie to be removed : ";
                    cin >> which;
                    removemovie(movie, movienum, which);
                    break;
                case 5:
                    cout<<"Have a Nice Day !!\nDo visit again!!!!\n";
                    break;
                default:
                    cout << "Invalid choice." << endl;
                    break;
                 }
            }
        //  moviedetails(movie, movienum);
        //  showmovie(movie, movienum);
        // //   addmovie(movie, movienum);
    }
        int status;
        while (wait(&status) > 0);

        // sem_close(sem2);
        // sem_close(sem1);
        // sem_close(sem3);
        shmdt(Admin_data);
        shmdt(movie);
        // shmctl(shmid1, IPC_RMID, NULL); // forceflly deletes the shared memory 
    //  shmdt(user_data);
    //  shmdt(Admin_data);
    //  shmctl(shmid1, IPC_RMID, NULL); // forceflly deletes the shared memory 
    //  shmctl(shmid2, IPC_RMID, NULL); // forceflly deletes the shared memory 
     
    return 0;
}
