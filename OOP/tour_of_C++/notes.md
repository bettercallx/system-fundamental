# basic
char 1 byte(8-bit) bool 1 int 4 doule8 unsigned4(non-negative integer) sizeof(int) == 4

```
x&y    // bitwise and
x|y    // bitwise or
x^y    // bitwise exclusive or
~x     // bitwise complement
“x&&y  // logical and
x||y   // logical or
!x     // logical not (negation)
```

auto : auto b = true; //the type can be deduced from the initializer

`scope and lifetime`
local scope

class scope

namespace scope

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
   int *p = new int (0);
   *p = 100;
   delete p;
   p = nullptr;

   //initialization + unique_ptr
   auto p = make_unique<int>(0);
   *p = 100;

```