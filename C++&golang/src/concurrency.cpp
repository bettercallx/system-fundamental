#include<iostream>
#include<thread>
using namespace std;

void threadfunc(int n){
    for(int i=0;i<n;i++){
        cout<<"Thread function executing "<<i<<endl;
    }
}
//almost no work, cout is too fast and it's a shared resource

int main(){
    thread t1(threadfunc,3);
    thread t2(threadfunc,5);
    t1.join();
    t2.join();
    return 0;
}


