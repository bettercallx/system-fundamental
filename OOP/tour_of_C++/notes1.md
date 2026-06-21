# Chapter1 - basic
char 1 byte(8-bit) bool 1 int 4 doule8 unsigned4(non-negative integer) sizeof(int) == 4

```
x&y    // bitwise and
x|y    // bitwise or
x^y    // bitwise exclusive or 按位异或 ^= 异或和
~x     // bitwise complement 按位取反
x&&y   //  logical and
x||y   // logical or
!x     // logical not (negation)
x<<3     // 左移 x*(2^k). x*(2^3) 011->011000
>>     // 右移
if (n & 1) //奇偶判断
```

auto : auto b = true; //the type can be deduced from the initializer

`scope, variable and shared_ptr`
scope: local scope, class scope, namespace scope

---
variable: 全局变量、局部变量、成员变量、静态局部变量、堆上变量、常量

**栈** - 局部变量

**堆** - new出来,手动delete

**静态区** - 全局变量、static变量(程序结束才释放)

---

全局变量: 静态区，写在class之外

```cpp
int g = 100;   // 静态区
```

局部变量: 递归时计算的局部变量在栈上，每次函数调用开一个新地址的leftH，每层各一份，指向分别的内存地址

```cpp
函数内声明,函数返回就销毁
void foo() {
    int x = 5;          // 局部变量，在栈上
    vector<int> v;       // 也是局部变量
}  // 函数结束，x 和 v 都没了

class Solution {
    int leftH;           // 成员变量，跟 Solution 对象绑定，分配地址0x100
public:
    void foo() {
        int x = 5;      // 局部变量
        leftH = 10;     // 访问的是对象的成员变量，地址0x100
    }
};
```

成员变量: class里public/private定义的变量，内存&生命周期跟随对象object

```cpp
// 情况1：对象在栈上 → 成员变量跟着在栈上
void foo() {
    Solution s;        // s 在栈上，s.leftH 也在栈上
}

// 情况2：对象在堆上 → 成员变量跟着在堆上
void foo() {
    Solution* s = new Solution();  // s 指向堆，s->leftH 在堆上
}

// 情况3：对象是全局的 → 成员变量跟着在静态区
Solution s;            // 全局对象，s.leftH 在静态区
```

---

静态局部变量: 函数里加static 只初始化一次 函数结束不销毁

```cpp
void foo(){
    static int count = 0; //第一次调用 静态区 初始化，之后一直在，函数结束不销毁
    count++;
}
```

堆上变量: new手动分配，不会自动销毁，需要delete

---
shared_ptr & lifetime : ref counts  ==0 -> end of lifetime

循环引用 两个类内部有持有对方的shared_ptr
```
Class A{
    shared_ptr<B> b_; //声明 等于nullptr
};
Class B{
    shared_ptr<A> a_;
};
auto a = make_shared<A>(); //A ref=1, A对象创建,计数+1
auto b = make_shared<B>(); //B ref=1
a->b_ = b;  //A ref=2, A中b_赋值指向A, A计数+1
b->a_ = a;  //B ref=2
//end of func, destruct A, A ref2-1=1, B have 1A; destruct B, B ref2-1=1, A have 1B,


//成员初始化列表
Class Server{
    shared_ptr<Connection> conn_;
    string id_;

    public:
    Server(shared_ptr<Connection> c) :conn_(c) {}
}

Server(shared_ptr<Connection> c, string n)   // server constructor, c 是形参
: conn_(c),id(n)                             // c 的值初始化成员变量 conn_, 直接赋值
{}                                           // 函数体，空的

//默认值+无参构造
Server() = default;
```


`dangling pointer & wild pointer`
```
    //dangling pointer: use pointer after it was deleted
    int *p = new int(1);
    delete p;
    //p keeping the address, but memory return to os
    *p = 100; //[X] 
    //p = nullptr; [Y] 
    
    
    //wild pointer: use not pointer not initialized
    int *p; // not initialized, p stores random address
    //or int *p = nullptr; [X],write para to a NULL address
    *p = 100; //[X], write para to a random address
   
   //initialization
   int *p = new int (0); // new : allocate memory + return memory address 分配内存+返回地址
   *p = 100;
   delete p;
   p = nullptr;

    //in expression, * == "contents of", & == "address of"
    char v[6]; // array of 6 char, index 0-5
    char *p = &v[3];// index 3, p point to the 4th element of v;
    char x = *p; //*p is the object p point to

    for(auto x: v) //copy
    for(auto &x:v) //refer to

    double cal(const vector<double> &) // cosnt &: not modify argument, no cost of copy

    //传数组，pointer as argument
    void process(int* arr, int size) //C风格 大小加指针
    //更好 避免裸指针传数组 传引用&
    void process(vector<int>& v)


   //initialization + unique_ptr
   auto p = make_unique<int>(0);
   *p = 100;
```

`const constexpr`
```
constexpr 在编译时计算
constexpr int BUFFER_SIZE = 1024 * 64;

常量和字符串用const就够了
const int ERR = 404;
const string PREFIX = "/api/v1";
```

argument passing and function value return are that of initialization

Initialization : 构造函数分配内存

Assignment : release资源, 分配新内存

