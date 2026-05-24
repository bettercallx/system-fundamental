# Chapter2 - undefined type
## Struct
```
// struct + 手动管理容易出错, struct is simply a class with members public by default
//define a vector
struct Vector {
    double* elem;
    int sz;
}
Vector v;
//elem do not point to anything, for it to be to useful, initialize it
vector_init(vector &v, int s){
    v.elem = new double[s]; // allocate a array of s doubles, dynamic area and heap
    v.sz = s;
}

```
## Class

```
Vector在栈上固定大小 12bytes
Vector object is a "handle" containing a pointer to elem and number of elements

class Vector {
    double*ele; //pointer 8 bytes
    int sz; //int 4
};
vector object can have a different number of elements
stack(fixed size 8bytes)      heap(可变大小 variable size) allocated by new
┌──────────┐
│ elem ─────────────→  [1.0][2.0][3.0][4.0][5.0]
│ sz = 5   │
└──────────┘

┌──────────┐
│ elem ─────────────→  [1.0][2.0][3.0]...[5000.0]
│ sz = 5000│
└──────────┘

struct Point{
    int x;
    int y;
};
vector<Point> points_;
points_.push_back({2,3});
**{}聚合初始化**，相当于
Point p; p.x = 2; p.y =3; push_back(p);

**operator 重载**
struct Point{
    int x;
    int y;
    Point operator+(const Point& other){
        return {x+other.x, y+other.y};
    }
    bool operator==(const Point& other){
        return x == other.x &&y == other.y;
    }
};
Point a = {1,2};
Point b = {3,4};
Point c = a+b;//Point operator+
bool res = (a==b);//false, bool operator==

class Vector {
    double* elem;
    int sz;
public:
    // 重载 []，让 Vector 能像数组一样用下标访问
    double& operator[](int i) {
        return elem[i];
    }
};

Vector v(5);
v[3] = 7.0;    // 实际上调用的是 v.operator[](3)

union -> variant
```

# Chapter3 - Modularity

