#include <string>
#include <iostream>
//#include <fcntl.h>
#include <libc.h>
using namespace std;
class dataBase{
private:
    //variable only accessed by fuctions inside class
    dataBase(){};//constructor, outside cannot construct object, only through public getinstance

    int connectionCount = 0;
    mutex dbmtx;

public:
    //static, getInstance() belong to this class, not a object
    //multi thread access getInstance(), only one thread will call constructor
    static dataBase& getInstance(){
        static dataBase instance_;
        return instance_;
    }
    //const :sql name will not be changed
    //string sql : the string will be copied once and pass
    //string &sql : no copy, read the original string, reference
    void query(const string & sql){
        cout<<sql<<endl;
    }

    //copy and assign are forbidden
    dataBase(const dataBase&) = delete;
    dataBase& operator = (const dataBase&) = delete;

    //data protection: if threads change a variable, all threads using same one mutex
    //lock_guard lock mtx when constructing, unlock automatically upon leaving function,no need to lock/unlock
    void connect() {
        lock_guard<mutex> lock(dbmtx);// threads in 1 process use mutex, processes use flock
        connectionCount++;
    }
};

int main(){
    //  dataBase db; X
    // &: reference, another name of the only object, & in declarition is reference, & in expression is address
    dataBase& db = dataBase::getInstance();
    db.query("SELECT");
    //db == db2 same object
    dataBase& db2 = dataBase::getInstance();


    //lock file in processes
    //create a file once process start. if file exsits, other process is using this file, wait or exist
    int fd = open("/tmp/text.txt", O_CREAT | O_EXCL, 0644);
    if (fd==-1){
        cout<<"fail to open file"<<endl;
        return -1;
    }
    // some oprations
    unlink("/tmp/text.txt");//delete it
    close(fd);
    return;
}