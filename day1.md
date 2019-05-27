[TOC]

## 1. Tips

### **No define :** replaced by  ***const*** and ***inline***

###  **C++ static : ** 3 ways

- 1.  ~~~c++
     //TO hide global variable and functions
     //a.cpp
     char a = 'A'; //if static char a = 'A'; 下面的main文件找不到char a
     //main.cpp
     int main(){extern char a; cout << a;} // result is 'A' 
     //extern -> to find (char)a in other file
     ~~~

  2.  **static in *class*: ** 

     - 类的静态成员函数/变量都是属于类而非对象的，没有this指针，但可以被对象调用
     - x..x 不要和 **java**里的 **static** 弄混了

  3. to **maintain the value **of variable

     ~~~c++
     int fun(){static int count = 10; reuturn count--;}
     //static作用下count在静态区仅仅是程序刚运行时初始化为10，后面不会反复初始化，而且整个源程序存在时fun里的count都存在，但退出该函数后，存在却不能调用它
     int count = 1;//global variable
     int main(){
         for(;count <= 3; count++) cout << count << " - "<<fun()<< endl; 
     }//result 1 10 2 9 3 8
     ~~~

### **const  ：** 

- 1. **限定域**

     ~~~c++
     template <typename T>
     T kkkk,zz;
     const T* const p = kkk;
     //左边的表示p所指向的对象不能改变，右边的const表示p不能改变指向
     *p = zz;//wrong 'const T*'
     p++;//wrong 'const p'
     ~~~

  2. **防止客户误用**

     ~~~c++
 class test{}
     const test operator*(const test& lhs,const test& rhs);
   //以下代码
     test a, b,c;
     if(a*b==c) //如果误写成a*b = c,编译就会报错,因为重载返回的是const,不能赋值
     ~~~
  
  3.  **const 成员函数 :** 神TM看书去，不知道怎么写

### **the vital *initialization* : ** 

#### 		**确保每个变量/对象都被初始化了** 

>   顺便一提，如果你的类可能会被继承，析构函数一定要是虚函数，否则可能会内存泄漏！因为如果你的析构函数不是虚函数，继承的子类也不会是虚函数，就有可能出现基类被析构而子类却没有被析构。

~~~c++
//对于构造函数，注意以下两种,第一种不是初始化，只是赋值
ABEntry(const string& name, const string& address){
 	theName = name; theAddress = address;num = 0;
}
ABEntry(const string& name, const string& address)
    :theName(name),theAddress(address),num(0) {} 
ABEntry():theNmae(),theAddress,num(0){}
//无参也不要忘记内置类型初始化
//第二种用了成员初使化列表（member initialization list）,很值得学习，效率也很高
~~~

#### 		**单例模式**的初始化

> ​      考虑以下情况，我们需要一个单例的类(如日志类)，为了避免其它类生成他们的实例化对象，他们（单例类）的构造函数都是private的，我们可以通过类里的静态函数/变量来来访问（并在访问时判断是否需要创建新的对象）,如下两种来避免线程死锁（互斥、初始化）
>
> 1. 第一种， 使用lock来锁定来保证互斥，但是可能会忘记解锁（有一种改进是放在构造和析构函数里）
>
> ~~~c++
> -test();
> -static test* test_; 
> +static test* visit(){
>   Lock();
>   if(test_ == NULL)
>        test_  = new test();
>  	unLock();
>   return test_;
> }
> ~~~
>
> ~~~c++
> +static test* visit(){
> 	Lock();
> 	static test obj;
>      unLock();
>      return &obj;
> }
> ~~~
>
> 或者也可以如上写visit，用内部静态变量，比较精简(注意返回值指针，是地址)
>
> 顺便回顾一下三种c++对象的创建方式：test test1; test test2 = test(); test* test3 = new test();
>
>  2. 第二种，直接在类的定义里就创建一个静态对象，调用的时候直接访问它
>
>     ~~~c++
>     -test();
>     -static test* visit_test = new test();//指针配地址
>     +static test* visit(){ return visit_test;}
>     ~~~
>
>     关于更多的懒汉/饿汉模式<https://www.cnblogs.com/qiaoconglovelife/p/5851163.html

## 2. 虚函数/虚基类/虚继承

### 2.1 虚基类和虚继承

- 作用是防止多继承中成员变量访问的二义性，即不会重复基类构造函数

- 直接贴代码

  ~~~c++
   class a{
          a(){cout << "A-constructor" << endl;}
     };
     class b : virtual public a{
          b(){cout << "B-constructor" << endl;}
     };
     class d :public a{ //the second time " : virtual public a "
          d(){cout << "D-constructor" << endl;}
     };
     class c : public b, public d{
       	c(){cout << "C-constructor" << endl;}
     };
  	c test;
  ~~~

  >  运行结果如下，可以看到我用了两次，第二次的时候a是d的虚基类，用了virtual关键字，注意看基类的构造函数
  >
  > ![](C:\Users\24403\Desktop\TIM图片20190527155012.png)

- 注意事项，当你创建派生类时，一定会调用基类，使用虚基类（虚基类值在继承处声明virtual表示他的基类是虚基类）的好处是如果有多个子类有一个基类，你不会重复的调用基类的构造函数,并且可以避免一些二义性；但是有一点要注意，因为基类的构造函数一定会被调用的，如果基类没有default构造函数你一定要在子类构造函数的参数初始化列表里加上基类的初始化！（平时你看不到是因为基类的default构造函数默认被调用了，所以你不需要对基类初始化）

  ~~~c++
  class a{
       	int n; 
          a(const int & n_):n(n_){cout << "A-constructor" << endl;}
     };
     class b : virtual public a{
          int n1;
          b(const int&n1_):a(n1_),n1(n1_){cout << "B-constructor" << endl;}
     };
     class d :public a{ 
          int n2;
          d(const int& n2_):a(n2_),n2(n2_){cout << "D-constructor" << endl;}
     };
     class c : public b, public d{
       	c(const int& x):a(x),b(x),d(x){cout << "C-constructor" << endl;}
     };
  	c test(1);
  ~~~

  

### 2.2 虚函数

- in