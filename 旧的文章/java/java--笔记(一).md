---
title: "Java--笔记(一)"
date: 2020-04-29T01:18:09+08:00
draft: false
---

- 基础类型（自动转换）
```java
            char(2)  \
                      int(4) -> long(8) -> float(4) -> double(8)
byte(1) -> short(2)  /
boolean(1bit)

```
- **private** 修饰构造函数一般用在 **singleton** 模式下。**[singleton实现方式]**  
- **java** 的抽象方法的函数声明不能省略具体的形参名  
- 非抽象方法一定要有函数体，不能把声明和实现分开

```java
public class Singleton
{
    private static Singleton singleton; // 私有的类变量用于缓存曾经创造的实例
    private Singleton() // 私有的构造函数 只能在类内部使用
    {

    }
    
    public static Singleton getInstance() // 提供外部调用的静态类方法
    {
        if(singleton==null)
            singleton=new Singleton();
        return singleton;
    }
}
```
- 某个对象的值为**null**，仍可以访问该对象的类的静态方法和静态变量。  




### Java继承相关 (参考:[图解Java多态内存分配以及多态中成员方法的特点])
- 转换类型的基本原则：简单->复杂，小数据->大数据，int -> long ，父类变量引用子类对象，这些都是自动转换，相反的情况就需要强制转换了。父类对象也可以强制转换成子类对象。  

```Java
Father f = new Son(); // 父类变量引用子类对象 自动转换
// 向上转型(upcasting)：子类引用的对象转换为父类类型称为向上转型。 
Son s = (Son)f; // 子类变量引用父类对象 强制转换
// 向下转型(downcasting)：父类引用的对象强制转换为子类类型称为向下转型。
```   

- **`Java `** 的子类构造器总会调用父类构造器，子类对象构造的三种情况如下  

    - 使用 **`super `** 对父类构造器进行调用  
    - 使用 **`this `**，将会调用本类中的其他构造器  
    - 不使用 **`super `** 也不使用 **`this `** 的情况下，系统将会隐式调用无参数的父类构造器  
    - 因此所有对象最后都是 **`java.lang.Object `** 类的扩展，所以每个对象最先执行的构造器是 **`java.lang.Object `** 的构造器 
- **`Java`** 的多态，引用变量有两个类型：编译时类型和运行时类型。编译根据声明的类型决定，而运行是根据实际对象决定，二者不一致就可能产生所谓的多态(**` Polymorphism `**)。常见的例子就像上面提到的将子类对象赋给父类变量。  
- 绑定--将方法调用和方法主体相关联。静态绑定和动态绑定，静态绑定的方法是在程序运行前就确定好的，不会改变的，**`final private static`** 构造函数等，动态则是根据运行时的状态决定。多态是因为动态绑定的机制存在的。
- 所以<u>多态的主体是方法</u>。在继承中将会初始化父类成员变量到子类实例中（父类和子类的这一个变量是同一个，子类中更改变量将会影响到父类，因为是同一个），如果子类中定义了与父类同名的变量，将会存在两个同名的变量（因为变量不存在重写，**声明**引用的类型是父类就调用父类的，**声明**的引用类型是子类的话，就调用子类的）。具体看笔记(二)
- 多态的好处
    - 向上转型可以在父类中统一调用不同子类的同名方法，提高扩展性  
    - 但是这样父类引用无法调用子类中新增的方法，需要用到向下转型


### java方法重写（易错）
```java
class Base
{
    Base()
    {
        test();
    }
    public void test(){
        System.out.println("父类的方法");
    }
}
class Sub
{
    String name;
    Sub()
    {
        name="123";
    }
    public void test(){
        System.out.println("子类的方法"+name.length());
    }

    public static void main(String[] args)
    {
        Sub s=new Sub(); // 报错 Exception in thread "main" java.lang.NullPointerException
        // 原因是先调用父类构造器，父类构造器调用test方法，这里的test方法被子类重写，调用的是子类的test方法。
        // 而此时还没有调用子类的构造函数，name是null，无法使用length()。
    }
}
```


### Java 创建对象的执行顺序
```java
Base
 |
Sub1
 |
Sub2
// 执行顺序
// 先加载类，执行静态初始化块（类相关联），执行初始化块（对象相关联），执行构造器（对象相关联）
// new Sub2(); 如果是第一次创建这类对象的话

Base的静态初始化块
Sub1的静态初始化块
Sub2的静态初始化块
（类的初始化完成）之后类就被加载到内存中，再次使用时不用重新加载
--------------
Base初始化块
Base构造器
Sub1初始化块
Sub1构造器
Sub2初始化块
Sub2构造器
（Sub2对象初始化完成）
---------------
```
静态初始化块和声明静态变量进行赋值的执行顺序相同；初始化块和声明成员变量进行初始化块的执行顺序相同。
```java
public class T
{
    static {
        a=10; // 与static int a=20;同属类的初始化代码，执行顺序从上到下
    }
    static int a=20; 
    {
        b=10; // 与int b=20;同属类的初始化代码，执行顺序从上到下
    }
    int b=20; 
}
```
- （静态方法、静态变量、静态初始化块、内部类枚举类无法访问实例变量），因为类成员的作用域更大，在类初始化时实例成员还未初始化。   


### instanceof 二元运算符
- 只能在具有继承关系和实现关系中使用
- 操作数A instanceof 操作数B  

|操作数A|操作数B|结果|
|:-:|:-:|:-:|
|子类对象|父类|true|
|父类对象|子类|false|
|null|任意类或接口|false|
|任意类或接口|null|error null不是类型|
|实现类的实例|接口|true|
|父类引用子类对象|父类|true|
|null强制转换成操作数B的对象|B类|false|
- 判断的依据是运行时的对象





### 执行命令java class 文件名时出现 java.lang.ClassNotFoundException
- 没有按照规范将生成的 .class 文件放在相应的目录下  
- 查看环境变量的 classpath 是否有问题
```java
// 标准的存放方式
// 首先有一个空文件夹project
// 想要有创建一个test类，这个类在包Lee中

// test.java
package Lee;
public class test
{
    public static void main(String[] args)
    {
        System.out.println("Hello world!");
    }
}

// 按照规范应该把这个文件放在源代码文件夹的Lee文件夹下。创建一个src文件夹和classes文件夹
project
    |------src
            |----Lee
                  |-----test.java
    |------classes

// 然后通过 javac -d path test.java 命令指定生成的test.class的地址
// 例如存放在classes下，执行 javac -d D:/project/src/classes test.java
// 将在classes文件夹下生成test.class文件 

project
    |------src
            |----Lee
                  |-----test.java
    |------classes
            |----Lee
                  |------test.class

// 在classes文件夹下执行 java Lee.test 或者 java Lee/test即可，根据文件的内容，将会在当前路径(project/classes)下搜索Lee文件夹中的test.class文件，如果在其他文件夹下执行命令将会失败并提示java.lang.ClassNotFoundException

```





[图解Java多态内存分配以及多态中成员方法的特点]: https://blog.csdn.net/javaoverflow/article/details/8888286  
[singleton实现方式]: https://www.cnblogs.com/rush/archive/2011/10/30/2229565.html

## 参考
图解Java多态内存分配以及多态中成员方法的特点: https://blog.csdn.net/javaoverflow/article/details/8888286  
singleton实现方式: https://www.cnblogs.com/rush/archive/2011/10/30/2229565.html
