# C++补充

> * [构造函数可以抛出异常吗，有什么问题？](#构造函数可以抛出异常吗有什么问题)
> * [初始化列表的异常怎么捕获？](#初始化列表的异常怎么捕获)
> * [析构函数可以抛出异常吗，有什么问题？](#析构函数可以抛出异常吗有什么问题)
> * [析构函数如何处理异常](#析构函数如何处理异常)
> * [智能指针](#智能指针)
> * [内存泄漏](#内存泄漏)
> * [野指针](#野指针)
> * [强制转换](#强制转换)
> * [RTTI](#RTTI)
> * [RAII](#RAII)
> * [CPP11新特性](#CPP11新特性)
> * [仿函数](#仿函数)

# [C++ 异常处理](https://www.runoob.com/cplusplus/cpp-exceptions-handling.html)

异常是程序在执行期间产生的问题。C++ 异常是指在程序运行时发生的特殊情况，比如尝试除以零的操作。

异常提供了一种转移程序控制权的方式。C++ 异常处理涉及到三个关键字：**try、catch、throw**。

- **throw:** 当问题出现时，程序会抛出一个异常。这是通过使用 **throw** 关键字来完成的。
- **catch:** 在您想要处理问题的地方，通过异常处理程序捕获异常。**catch** 关键字用于捕获异常。
- **try:** **try** 块中的代码标识将被激活的特定异常。它后面通常跟着一个或多个 catch 块。

## [构造函数可以抛出异常吗，有什么问题？](https://www.cnblogs.com/qinguoyi/p/10304882.html)

构造函数中应该避免抛出异常。

> * 构造函数中抛出异常后，对象的**析构函数将不会被执行**
> * 构造函数抛出异常时，本应该在析构函数中被delete的对象没有被delete，会导致**内存泄露**
> * 当对象发生部分构造时，已经构造完毕的子对象（非动态分配）将会逆序地被析构。

## 初始化列表的异常怎么捕获？

> * 初始化列表构造，**当初始化列表出现异常时，程序还未进入函数体，因此函数体中的try-catch不能执行，catch也无法处理异常。**可以通过函数try块解决该问题。
> * 函数try块中的try出现在表示构造函数初始值列表的冒号以及表示构造函数体的花括号之前，与这个try关联的catch既能处理构造函数体抛出的异常，也能处理成员初始化列表抛出的异常。


## 析构函数可以抛出异常吗，有什么问题？

析构函数不应该抛出异常

> * **其他正常，仅析构函数异常**。 如果析构函数抛出异常，则**异常点之后的程序不会执行**，如果析构函数在异常点之后执行了某些必要的动作比如释放某些资源，则这些动作不会执行，会造成诸如资源泄漏的问题。
> * **其他异常，且析构函数异常**。 通常异常发生时，c++的机制**会调用已经构造对象的析构函数来释放资源**，此时若析构函数本身也抛出异常，则**前一个异常尚未处理，又有新的异常**，会造成程序崩溃的问题。

## 析构函数如何处理异常？

> * 若析构函数抛出异常，**调用std::abort()来终止程序**
> * 在析构函数中catch捕获异常并作处理，吞下异常；
> * 如果客户需要对某个操作函数运次期间抛出的异常做出反应，class应该提供普通函数执行该操作，而非在析构函数中。

## [智能指针](https://www.cnblogs.com/TianFang/archive/2008/09/20/1294590.html)

C++里面的四个智能指针: auto_ptr, shared_ptr, weak_ptr, unique_ptr 其中后三个是c++11支持，并且第一个已经被11弃用。

为什么要使用智能指针：

智能指针的作用是管理一个指针，因为存在以下这种情况：**申请的空间在函数结束时忘记释放，造成内存泄漏**。使用智能指针可以很大程度上的避免这个问题，因为智能指针就是一个类，**当超出了类的作用域是，类会自动调用析构函数，析构函数会自动释放资源。**所以智能指针的作用原理就是**在函数结束时自动释放内存空间，不需要手动释放内存空间**。

- **auto_ptr**（c++98的方案，cpp11已经抛弃）

采用所有权模式。

```
auto_ptr< string> p1 (new string ("I reigned lonely as a cloud.”));
auto_ptr<string> p2;
p2 = p1; //auto_ptr不会报错.
```

此时不会报错，p2剥夺了p1的所有权，但是当程序运行时访问p1将会报错。所以auto_ptr的缺点是：存在潜在的内存崩溃问题！

- **unique_ptr**（替换auto_ptr）

unique_ptr实现独占式拥有或严格拥有概念，保证同一时间内只有一个智能指针可以指向该对象。它对于避免资源泄露(例如“以new创建对象后因为发生异常而忘记调用delete”)特别有用。

采用所有权模式，还是上面那个例子

```
unique_ptr p3 (``new` `string (``"auto"``));  ``//#4``unique_ptr p4；            ``//#5``p4 = p3;``//此时会报错！！
```

编译器认为p4=p3非法，避免了p3不再指向有效数据的问题。因此，unique_ptr比auto_ptr更安全。

另外unique_ptr还有更聪明的地方：当程序试图将一个 unique_ptr 赋值给另一个时，如果源 unique_ptr 是个临时右值，编译器允许这么做；如果源 unique_ptr 将存在一段时间，编译器将禁止这么做，比如：

```
unique_ptr<string> pu1(new string ("hello world"));
unique_ptr<string> pu2;
pu2 = pu1;                                      // #1 not allowed
unique_ptr<string> pu3;
pu3 = unique_ptr<string>(new string ("You"));   // #2 allowed
```

其中#1留下悬挂的unique_ptr(pu1)，这可能导致危害。而#2不会留下悬挂的unique_ptr，因为它调用 unique_ptr 的构造函数，该构造函数创建的临时对象在其所有权让给 pu3 后就会被销毁。这种随情况而已的行为表明，unique_ptr 优于允许两种赋值的auto_ptr 。

注：如果确实想执行类似与#1的操作，要安全的重用这种指针，可给它赋新值。C++有一个标准库函数std::move()，让你能够将一个unique_ptr赋给另一个。例如：

```
unique_ptr<string> ps1, ps2;
ps1 = demo("hello");
ps2 = move(ps1);
ps1 = demo("alexia");
cout << *ps2 << *ps1 << endl;
```

- **shared_ptr**

shared_ptr实现共享式拥有概念。**多个智能指针可以指向相同对象**，**该对象和其相关资源会在“最后一个引用被销毁”时候释放**。从名字share就可以看出了资源可以被多个指针共享，它使用计数机制来表明资源被几个指针共享。可以通过成员函数use_count()来查看资源的所有者个数。除了可以通过new来构造，还可以通过传入auto_ptr, unique_ptr,weak_ptr来构造。当我们调用release()时，当前指针会释放资源所有权，计数减一。当计数等于0时，资源会被释放。

shared_ptr 是为了解决 auto_ptr 在对象所有权上的局限性(auto_ptr 是独占的), 在使用引用计数的机制上提供了可以共享所有权的智能指针。

成员函数：

use_count 返回引用计数的个数

unique 返回是否是独占所有权( use_count 为 1)

swap 交换两个 shared_ptr 对象(即交换所拥有的对象)

reset 放弃内部对象的所有权或拥有对象的变更, 会引起原有对象的引用计数的减少

get 返回内部对象(指针), 由于已经重载了()方法, 因此和直接使用对象是一样的.如 shared_ptr<int> sp(new int(1)); sp 与 sp.get()是等价的

- **weak_ptr**

weak_ptr 是一种不控制对象生命周期的智能指针, 它指向一个 shared_ptr 管理的对象. 进行该对象的内存管理的是那个**强引用的 shared_ptr**. weak_ptr只是提供了对管理对象的一个访问手段。weak_ptr 设计的目的是为配合 shared_ptr 而引入的一种智能指针来协助 shared_ptr 工作, 它只可以从一个 shared_ptr 或另一个 weak_ptr 对象构造, 它的构造和析构不会引起引用记数的增加或减少。**weak_ptr是用来解决shared_ptr相互引用时的死锁问题,如果说两个shared_ptr相互引用,那么这两个指针的引用计数永远不可能下降为0,资源永远不会释放**。它是对对象的一种**弱引用**，**不会增加对象的引用计数**，和shared_ptr之间可以相互转化，shared_ptr可以直接赋值给它，它可以通过调用lock函数来获得shared_ptr。

```
class B;
class A
{
public:
shared_ptr<B> pb_;
~A()
{
cout<<"A delete\n";
}
};
class B
{
public:
shared_ptr<A> pa_;
~B()
{
cout<<"B delete\n";
}
};
void fun()
{
shared_ptr<B> pb(new B());
shared_ptr<A> pa(new A());
pb->pa_ = pa;
pa->pb_ = pb;
cout<<pb.use_count()<<endl;
cout<<pa.use_count()<<endl;
}
int main()
{
fun();
return 0;
}
```

可以看到fun函数中pa ，pb之间互相引用，两个资源的引用计数为2，当要跳出函数时，智能指针pa，pb析构时两个资源引用计数会减一，但是两者引用计数还是为1，导致跳出函数时资源没有被释放（A B的析构函数没有被调用），如果把其中一个改为weak_ptr就可以了，我们把类A里面的shared_ptr pb_; 改为weak_ptr pb_; 运行结果如下，这样的话，资源B的引用开始就只有1，当pb析构时，B的计数变为0，B得到释放，B释放的同时也会使A的计数减一，同时pa析构时使A的计数减一，那么A的计数为0，A得到释放。

注意的是我们不能通过weak_ptr直接访问对象的方法，比如B对象中有一个方法print(),我们不能这样访问，pa->pb_->print(); 英文pb_是一个weak_ptr，应该先把它转化为shared_ptr,如：shared_ptr p = pa->pb_.lock(); p->print();

## 内存泄漏

当一个对象已经不需要再使用本该被回收时，另外一个正在使用的对象持有它的引用从而导致它不能被回收，这导致本该被回收的对象不能被回收而停留在堆内存中，这就产生了内存泄漏。

## 野指针

野指针指向一个已删除的对象或 申请访问受限内存区域的指针。

* 原因
  * **指针变量未初始化**
  * **指针释放未置空**
  * **指针操作超出作用域**。返回**指向栈内存的指针或引用**，因为**栈内存在函数结束时会被释放**。

## [强制转换](https://www.cnblogs.com/larry-xia/p/10325643.html)

C++中强制转换为static_cast, dynamic_cast,const_cast, reinterpret_cast

* static_cast
  * static_cast用于将一种基础数据类型强制转换为另一种基础数据类型。static_cast不能转换掉expression的const、volitale或者__unaligned属性。
* const_cast
  * const_cast用于强制去掉不能被修改的常数特性，但需要特别注意的是const_cast不是用于去除变量的常量性，而是**去除指向常数对象的指针或引用的常量性**，其去除常量性的对象必须为指针或引用。
* reinterpret_cast
  * 在C++语言中，reinterpret_cast主要有三种强制转换用途：**改变指针或引用的类型、将指针或引用转换为一个足够长度的整形、将整型转换为指针或引用类型**。

- dynamic_cast
  - 其他三种都是编译时完成的，dynamic_cast是运行时处理的，运行时要进行类型检查。不能用于内置的基本数据类型的强制转换。

## [RTTI](https://www.cnblogs.com/xuelisheng/p/9479288.html)

Runtime Type Information，运行时类型检查，在C++层面主要体现在dynamic_cast和typeid

> * dynamic_cast
>   动态类型转换
> * typeid
>   typeid 运算符允许在运行时确定对象的类型，获取对象的实际类型

## [RAII](https://blog.csdn.net/quinta_2018_01_09/article/details/93638251)

RAII全称是“Resource Acquisition is Initialization”，直译过来是“资源获取即初始化”。是C++语言的一种管理资源、避免泄漏的惯用法。

- 利用的就是C++构造的对象最终会被销毁的原则。RAII的做法是使用一个对象，在其构造时获取对应的资源，在对象生命期内控制对资源的访问，使之始终保持有效，最后在对象析构的时候，释放构造时获取的资源。

* 在构造函数中申请分配资源，在析构函数中释放资源。因为C++的语言机制保证了，当一个对象创建的时候，自动调用构造函数，当对象超出作用域的时候会自动调用析构函数。所以，在RAII的指导下，我们应该使用类来管理资源，将资源和对象的生命周期绑定。
* RAII的核心思想是将资源或者状态与对象的生命周期绑定，通过C++的语言机制，实现资源和状态的安全管理,智能指针是RAII最好的例子

## CPP11新特性

* nullptr常量
  * C++中NULL仅仅是`define NULL 0`的一个宏定义，因此，有时候会产生歧义
    * 比如f（char*）和f（int），参数传NULL的话到底该调用哪个？
    * 事实上，在VS下测试这样的函数重载会优先调用f（int），但是f（char *）也是正确的，因此C++引入nullptr来避免这个问题
  * nullptr是一个空指针，可以被转换成其他任意指针的类型
* auto类型指示符
  * 让编译器替我们去分析表达式所属的类型，直接推导
  * 尤其是STL中map的迭代器这种很长的类型，适合用auto
* decltype类型指示符
  * 从表达式的类型推断出要定义的变量的类型，跟表达式的类型也就是参数类型紧密相关
  * `delctype (f()) sum = x;` 并不实际调用函数f()，只是使用f()的返回值当做sum的类型
  * `delctype (i) sum = x;`和`delctype ((i)) sum = x;` 其中i为int类型，前面的为int类型，后面的为int&引用
* 范围for语句
  * 多与auto配合使用

```C++
string str("somthing");
for(auto i:str) //对于str中的每个字符，i类型为char
    cout << c << endl;

for(auto &i:str) //对于若要改变每个字符的值，需要加引用
    cout << c << endl;
```

* 定义双层vector
  * `vector<vector<int>>(m, vector<int>(n, 0))` 创建m行n列的二维数组，全部初始化为0
* [lambda表达式](https://blog.csdn.net/qq_43265890/article/details/83218413)
  * 用于实现匿名函数，匿名函数只有函数体，没有函数名

```C++
[capture list] (params list) mutable exception-> return type {function body};  //1
[capture list] (params list) -> return type {function body};  //1 省略mutable，表示const不可修改
[capture list] (params list) {function body};		//2 省略返回类型，按照函数体返回值决定返回类型
[capture list] {function body};		//3 省略参数列表，无参函数
```

* 参数
  * capture list：捕获外部变量列表
  * params list：形参列表
  * mutable指示符：用来说用是否可以修改捕获的变量
  * exception：异常设定
  * return type：返回类型
  * function body：函数体

```C++
//示例
sort(vec.begin(), vec.end(), [](int a, int b)->bool{return a < b})
```

* 参数捕获方式
  * 值捕获(传参)、引用捕获（传引用）、隐式捕获（传=，函数体直接使用变量））。 

* 智能指针
  * shared_ptr
  * weak_ptr
  * unique_ptr
* 右值引用
  * 左值引用，必须引用左值 `int a = 0; int &b = a;`
  * 右值引用可以引用结果 `int && i = 0`

## 仿函数

* 定义
  * 仿函数(functor)又称之为函数对象（function object），其实就是重载了operator()操作符的struct或class，是一个能行使函数功能的类
  * 它使一个类的使用看上去像一个函数，这个类就有了类似函数的行为，就是一个仿函数类。 

## [lambda表达式](https://www.cnblogs.com/jimodetiantang/p/9016826.html)

lambda表达式的作用：

1. **省下定义函数的过程**。
2. 对于一些比较抽象并且整个程序执行下来只需要调用一两次的函数，使用lambda就**不需要考虑命名问题**。
3. **简化代码的可读性**，由于普通的函数阅读经常要跳到开头def定义部分，使用lambda函数可以省去这样的步骤。
