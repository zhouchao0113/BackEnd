# 设计模式

| 序号 | 模式 & 描述                                                  | 包括                                                         |
| :--- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| 1    | **创建型模式** 这些设计模式提供了一种在创建对象的同时隐藏创建逻辑的方式，而不是使用 new 运算符直接实例化对象。这使得程序在判断针对某个给定实例需要创建哪些对象时更加灵活。 | 工厂模式（Factory Pattern）<br />抽象工厂模式（Abstract Factory Pattern）<br />单例模式（Singleton Pattern）建造者模式（Builder Pattern）原型模式（Prototype Pattern） |
| 2    | **结构型模式** 这些设计模式关注类和对象的组合。继承的概念被用来组合接口和定义组合对象获得新功能的方式。 | 适配器模式（Adapter Pattern）<br />桥接模式（Bridge Pattern）<br />过滤器模式（Filter、Criteria Pattern）<br />组合模式（Composite Pattern）<br />装饰器模式（Decorator Pattern）<br />外观模式（Facade Pattern）<br />享元模式（Flyweight Pattern）<br />代理模式（Proxy Pattern） |
| 3    | **行为型模式** 这些设计模式特别关注对象之间的通信。          | 责任链模式（Chain of Responsibility Pattern） <br />命令模式（Command Pattern）<br />解释器模式（Interpreter Pattern）<br />迭代器模式（Iterator Pattern）<br />中介者模式（Mediator Pattern）<br />备忘录模式（Memento Pattern）<br />观察者模式（Observer Pattern）<br />状态模式（State Pattern）<br />空对象模式（Null Object Pattern）<br />策略模式（Strategy Pattern）<br />模板模式（Template Pattern）<br />访问者模式（Visitor Pattern） |
| 4    | **J2EE 模式** 这些设计模式特别关注表示层。这些模式是由 Sun Java Center 鉴定的。 | MVC 模式（MVC Pattern）<br />业务代表模式（Business Delegate Pattern）<br />组合实体模式（Composite Entity Pattern）<br />数据访问对象模式（Data Access Object Pattern）<br />前端控制器模式（Front Controller Pattern）<br />拦截过滤器模式（Intercepting Filter Pattern）<br />服务定位器模式（Service Locator Pattern）<br />传输对象模式（Transfer Object Pattern） |

## 常见的设计模式

**单例模式**：单例模式主要解决一个**全局使用的类频繁的创建和销毁的问题**。单例模式下可以**确保某一个类只有一个实例**，而且自行实例化并向整个系统提供这个实例。单例模式有三个要素：一是**某个类只能有一个实例**；二是它**必须自行创建这个实例**；三是它必须**自行向整个系统提供这个实例**。

**工厂模式**：工厂模式主要解决接口选择的问题。该模式下定义一个创建对象的接口，让其子类自己决定实例化哪一个工厂类，使其创建过程延迟到子类进行。

**观察者模式**：定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变时，所有依赖于它的对象都得到通知并被自动更新。

## [单例模式](https://www.runoob.com/design-pattern/singleton-pattern.html)

单例模式主要解决一个**全局使用的类频繁的创建和销毁的问题**。单例模式下可以**确保某一个类只有一个实例**，而且自行实例化并向整个系统提供这个实例。单例模式有三个要素：一是**某个类只能有一个实例**；二是它**必须自行创建这个实例**；三是它必须**自行向整个系统提供这个实例**。

- **意图：**保证一个类仅有一个实例，并提供一个访问它的全局访问点。

- **主要解决：**一个全局使用的类频繁地创建与销毁。

- **何时使用：**当您想控制实例数目，节省系统资源的时候。

- **如何解决：**判断系统是否已经有这个单例，如果有则返回，如果没有则创建。

- **关键代码：**构造函数是**私有**的。当我们在程序中声明一个对象时，编译器为调用构造函数，由于在class外部不允许访问私有成员，所以这将导致编译出错。

## 单例模式的多线程安全问题：

在单例模式的实现中，如果不采取任何措施，在多线程下是不安全的，可能会同时创建多个实例。因此，为了保证单例模式在多线程下的线程安全，一般采用下面几种方式实现单例模式：

1) 饿汉式：

- 优点：没有加锁，执行效率会提高。

- 缺点：类**加载时就初始化**，浪费内存。

```c++
public class Singleton {  
    private static Singleton instance = new Singleton();  
    private Singleton (){}  
    public static Singleton getInstance() {  
    return instance;  
    }  
}
```

2) 懒汉式：

- 线程不安全：

  **描述：**这种方式是最基本的实现方式，这种实现最大的问题就是不支持多线程。因为没有加锁 synchronized，所以严格意义上它并不算单例模式。

  ```c++
  public class Singleton {  
      private static Singleton instance;  
      private Singleton (){}  
    
      public static Singleton getInstance() {  
      if (instance == null) {  
          instance = new Singleton();  
      }  
      return instance;  
      }  
  }
  ```

- 线程安全

  - 优点：**第一次调用才初始化**，避免内存浪费。

  - 缺点：必须**加锁** synchronized 才能保证单例，但加锁会**影响效率**。

  ```c++
  public class Singleton {  
      private static Singleton instance;  
      private Singleton (){}  
      public static synchronized Singleton getInstance() {  
      if (instance == null) {  
          instance = new Singleton();  
      }  
      return instance;  
      }  
  }
  ```

  























































