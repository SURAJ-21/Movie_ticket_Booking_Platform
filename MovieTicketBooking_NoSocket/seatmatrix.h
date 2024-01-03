#include<iostream>
#include<bits/stdc++.h>
using namespace std;

void stadium(){
    //stadium seat
    char seat[9][9];
    for(int i=0;i<9;i++){

        for(int j=0;j<9;j++){
            if((i>=3&&i<6)&&(j>=3&&j<6))
            cout<<"   ";
            else
            cout<<" "<<i<<j;
        }
        cout<<"\n";
    }
}
void moviehall(){
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
    else cerr << "Unable to open the seat file for reading." << endl;
    
    for(int i=0;i<9;i++){
            for(int j=0;j<9;j++)
            {
                if(m[i*10+j]==0){
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

