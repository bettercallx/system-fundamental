#include <string>
#include <iostream>
using namespace std;
class dataBase{
private:
    dataBase(){};//constructor, cannot new by outside field
public:
    static dataBase& getInstance(){
        static dataBase instance_;
        return instance_;
    }
    void query(const string & sql){
        cout<<sql<<endl;
    }
};

int main(){
    //  dataBase db; X
    // only:
    dataBase& db = dataBase::getInstance();
    db.query("SELECT");
    //db == db2 same object
    dataBase& db2 = dataBase::getInstance();
}