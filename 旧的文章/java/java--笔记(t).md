---
title: "Java--笔记(t)"
date: 2020-12-29T01:18:09+08:00
draft: false
---

强制转换
```java
    // int型被转换成byte型
    byte val = (byte)1000_0101; 
    System.out.println(val);
    // 截取部分为 1000_0101 采用的是补码 (截取部分的符号为是0就采用原码)
    // 先转换成反码(第一位是符号位) 1000_0100 再转换成原码 1111_1011
    // 输出为 -123
```

string类是不可变类，String对象创建出来就不可能被改变  
常量池在编译器就确定，保存在.class文件中（类、方法、接口中的常量和字符串）   
移位计算  
- byte、short和char 先转换成int类型，再进行计算
- int 类型的移位先对32求余数  
- long类型的移位先对64求余数  
  
父类的private方法子类无法访问，也无法重写。     
初始化块的优先级比构造函数高。实际上执行javac编译时，就是将初始化块嵌入到构造函数的最前面。  
静态初始化块和静态成员变量指定初始值的执行顺序与源代码中的顺序相同。   
```java
    public class Test{
        static{
            a=20;
        }
        static int a=10; // 结果是10
    }

    public class Test{
        static int a=10;
        static{
            a=20;
        }
        // 结果是20
    }
```  
final只有在定义变量时指定初始值才有宏替换的效果。  
```java
    public static void main(String[] args){
        final String s1;
        s1 = "abc";
        final String s2 = "def";
        String s3 = s1 + s2;
        String s4 = "abcdef";
        System.out.print(s3==s4); //输出false
    }

    public static void main(String[] args){
        final String s1 = "abc";
        final String s2 = "def";
        String s3 = s1 + s2;
        String s4 = "abcdef";
        print(s3==s4); // 输出true
    }
```
private修饰的方法已经带有final的含义了。  
final修饰的方法不能被重写，但可以被重载。  
final和abstract不能同时修饰方法，private和abstract也不能同时修饰。   
**static和abstract是不能同时修饰方法的（static说明是静态方法，可以通过类直接访问，但是不能是abstract），但是可以同时修饰内部类。**

## 接口   
- 类的修饰符只有public或者默认（包）
- 成员变量自动加上public、static、final(与类不同，没有私有类变量)
- 类方法和默认方法、私有方法必须有方法体
- 接口可以继承多个多个接口  
- 接口没有初始化块，抽象类有
- 内部类的类名是 OutterClass$InnerClass，与外部类相关联，***不能在继承外部类的子类中重写内部类。***

## 内部类

- 多了private、protected、static
  
- 非静态内部类  
  - ***非静态内部类不能有静态成员***
  - 非静态内部类保存了一个对于外部类对象的引用，所以可以访问外部类的私有变量
  - 当存在与外部类同名的成员变量时，可以用this和OuterClass.this来区别。默认的访问顺序是局部变量、内部类、外部类。
  - 外部类不允许 ***直接*** 访问非静态类内部的成员变量(此时非静态类对象可能不存在)，需要先new一个非静态内部类的实例。
  - 非外部类使用非静态内部类
  ```java
        class Out{
            class In{

            }
        }
        public class Test{
            public static void main(String[]args){
                Out.In in = new Out().new In();
            }
        }
  ```
  - 继承非静态内部类
  ```java
    package t;

    public class Outter{
        class Inner {
            public Inner(String msg){
                System.out.print(msg);
            }
        }
    }


    package t;

    public class Sub extends Outter.Inner{
        Sub(Outter outter,String msg){ // 需要将所依赖的外部类实例引用进来
            outter.super(msg);  // 
        }

        public static void main(String args[]){
            new Sub(new Outter(),"aaaa");
        }
    }


  ```
- 静态内部类(看作外部类的静态变量)
  - 不允许访问外部类的非静态成员(静态内部类的实例方法也不行)
  - 因为被看作是外部类的静态成员，所以可以被外部类的所有方法和初始化块来使用
  - 外部类使用静态内部类的静态变量（内部类名.静态变量名）
  - 在外部创建静态内部类
  ```java
    Inner inner = new Outter.Inner();
  ```
- 接口中的内部类只能是public static的
- 局部内部类
  - 上一级单元是方法，所以没有static、public、private、protected等修饰符
  - 类名 **OutterClass$NumInnerClass** 相对于其他内部类多了一个Num，因为一个类中只能有一个同名成员内部类，而可以有多个同名局部内部类（不同方法中）

- 匿名内部类
  - 只能实现一个接口或者抽象方法
  - ***被局部内部类和匿名内部类(lambda)所访问的局部变量默认是final的，无法修改。***

## 函数式接口与lambda
- 函数式接口可以有多个类方法、默认方法和私有方法，只能有一个抽象方法
- lambda返回的是函数式接口的对象，但返回的目标类型可能是变化的。
```java
    @FunctionalInterface
    Interface Interface1{
        void func();
    }

    @FunctionalInterface
    Interface Interface2{
        void func();
    }

    public class Test{
        public static void main(String[] args){
            Object obj = ()->{
                System.out.println("fffff");
            }; //报错，不兼容，Object不是函数接口
        }
    }

        public static void main(String[] args){
            Object obj = (Interface1)()->{  // 强制转换
                System.out.println("fffff");
            };
        }

```
- 调用其他方法
  - 引用类方法
    ```java
    @FunctionalInterface
    interface Converter{
        Integer Convert(String from);
    } 

    Converter converter = from -> Integer.valueOf(from);
    Converter converter = Integer::valueOf;
    ```  
  - 引用对象的方法
    ```java
    Converter converter = from -> "fjsdf".indexOf(from);
    Converter converter = String::indexOf;
    ```
  - 引用对象的方法
    ```java
    interface MyTest{
        String test(String a,int b,int c);
    }

    Mytest mytest = (a,b,c) ->a.substring(b,c);
    Mytest mytest = String::subString;
    ```
  - 引用构造器
    ```java
    InterfaceX interfacex = String::new;
    ```
- 与匿名和局部内部类一样访问的局部变量必须是effective final的。
- lambda只能为函数式接口创造实例，多个抽象方法的接口和抽象类都不行。
- **lambda不允许调用函数式接口的默认方法和私有方法，但是可以通过接口名.静态方法名()，来使用静态方法。匿名内部类可以使用抽象类或者接口的方法(除了私有方法不能用)。**



## 枚举类
- 不带有抽象方法的枚举类是final的（不能显式声明，但是是final的）
- 带抽象方法的枚举类要在枚举值中实现
- 构造函数是 **private**
- ***编译后会形成EnumClass$Num.class，匿名内部类，当使用花括号语法的时候***
  ```JAVA
    enum SeasonEnum {
        SPRING,SUMMER,FALL,WINTER;
    } // 简单枚举类

    enum SeasonEnum { //上述枚举类的class文件
        SPRING,
        SUMMER,
        FALL,
        WINTER;

        private SeasonEnum() { //默认的构造函数
        }
    }


    enum SeasonEnum {
        STRING{
            public void func(){
            }
        },SUMMER{
            public void func(){
            }
        },FALL{
            public void func(){
            }
        },WINTER{
            public void func(){
            }
        };
        abstract public void func();
    }     // 编译生成5个class，包括SeasonEnum.class和SeasonEnum$1-4.class

    enum SeasonEnum$1 { // javac 之后的一个类
    private SeasonEnum$1() {
    }

    public void func() {
    }
    }

  ```



## Collection

### Set


- hashSet（底层使用哈希散列表、链表和红黑树）
  - 无序

  - HashSet不保证同步，多线程同时修改hashmap，需要通过代码保证同步
  - 元素可以是null
  - hashset是基于hashmap的（map的value是无意义的常量）
  - 当equals()和hashCode()相同时判定相同，通过hashCode()计算存储的位置(流程如下)
    - 指定hashset的初始化大小(initialCapacity，默认是16)和loadfactor(负载因子，默认是0.75)。

    - 根据initialCapacity建立一个Entry<k,v>数组(Entry是一个接口，)，数组的每个位置称作Bucket（桶）,每个位置只能存储一个Entry。
    - 先计算hashCode，hashCode进行散列运算(取余操作)，得到数组下标。
    - 如果表中该下标没有元素，就添加。
    - 如果有就执行equals，相等就抛弃，不等就加入链表或者红黑树。
    - 这里的链表的实现是通过hashMap的静态内部类Node<k,v>实现的（实现了Entry<k,v>接口），即链表。
    - 当
- TreeSet实现了sortedSet接口
  - 每个加入的元素都需要实现Comparable接口（加入前会检查）
  - ***可以在构造函数中传入COmparator对象（Comparator是函数式接口），这时不需要传入对象实现Comparable。***
  - 判断元素相等的唯一标准是compareTo(),返回0表示相等
  - 需要额外的红黑树来维持次序
- EnumSet
  - 不允许加入null元素
  - 没有可以被外部调用的构造器，通过类方法创建对象
  - 只允许添加同一 ***枚举*** 类的枚举值
- EnumSet、TreeSet和HashSet都是线程不安全的。


### List

- 相比Collection（Iterator）多了ListIterator，增加了向前迭代的功能
- Arrays，操作数组的工具类
  - 内部也有一个ArrayList内部类，使用asList方法将根据参数生成固定长度的List集合。
- LinkedList不仅是List的实现类，也是Dequeue的实现类
  - 当成双端队列使用，可以是栈，也能当队列

### Queue

- PriorityQueue类和TreeSet类似，可以定制排序（此时不需要元素实现Comparable接口）和默认排序
- Dequeue是Queue的子接口
  - 双端队列，还可以当成栈来使用，以队列的头部为栈顶
  - ArrayDequeue实现了Dequeue
    - 基于数组，默认长度为16
    - 使用栈时，尽量用ArrayList而不是Stack，后者太旧且性能差


## 范型
- 范型类物理上不存在，只有一个类
  - 不能在静态方法、静态变量、静态初始化块中使用 ***类型形参***
  ```java
  public class R<T>{
      static T info;  // 错误
      T info; // 正确
      public static void bar(T t){} //错误
      public void foo(T t){} // 正确 
      public  static <T> void func(T t){} // 正确，这里的T和类的T不一样，不是类型形参

  }
  ```
- 用类型通配符表示所有范型类的父类
```java
class Super{

}
class Sub extends Super{

}
public class Test{
    public void func(List<Super> s){
        
    }
    public static void main(Sting[] args){
        // 报错，List<Sub> 不是 List<Super>的子类型
        new Test().func(new ArrayList<Sub>());
    }
}

//正确的func函数
public <T extends Super> void func(List<T> s){

}
//或者
public void func(List<? extends Super> s){

}
```
  

![alt 性能比较](https://img-blog.csdn.net/20171228171908904?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHloa21t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
***注：快速排序的辅助空间复杂度更改为为logn***