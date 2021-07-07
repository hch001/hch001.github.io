---
title: "Java--笔记(二)继承、接口和内部类"
date: 2020-05-05T21:04:08+08:00
draft: false
---

### 属性的继承与方法的继承 (图示)
- 类方法可以被继承
- 继承属性和方法不太一样，在父类引用子类对象的时候(向上转型)，如果方法被重写，那么将调用子类重写过的方法，但是访问属性时是父类的属性，除非强制转化才能输出子类的属性。（具体如下图所示，黄色是父类部分，蓝色是子类部分，箭头代表沿用）
![027(1).png](https://s1.ax1x.com/2020/05/25/tpQ8E9.png)  
上图中的子类调用方法时，实际上是调用了父类的方法。
![027(2).png](https://s1.ax1x.com/2020/05/25/tpQJ41.png)  
上图中 Fruit f = new Apple(); f.name 仍是 fruit，使用 ((Apple)f).name，f.name 就变成 apple 了
![027(3).png](https://s1.ax1x.com/2020/05/25/tpQN36.png)  
上图说明了（在没重写的情况下）子类的属性是引用了父类的。方法看是调用的子类，只有一种特殊情况仍调用父类的方法，那就是在父类的函数中用到泛型，而子类的函数中没用泛型。下面的代码就是例子。
```java
public class f {
    public static void main(String[] args)
    {
        Fruit f = new Apple();
        f.func("");  // 输出 ffffffff
    }
}

class Fruit
{
    public <T> void func(T a){System.out.println("fffffffffff");}
}
class Apple extends Fruit
{
    public void func(String a){System.out.println("aaaaaaaaaaaaaaa");}
}
```
![027(4).png](https://s1.ax1x.com/2020/05/25/tpQajO.png)  
这里使用 ((Apple)f).name，输出的就变成 apple 了。方法调用的特殊情况如上所示。

### java接口变化
- java 8之前
    - 变量只能是 public static final，方法只能有声明，并且是public，省略默认是public  
    - 接口的内部类和内部接口都是public static  
    - 无构造器、初始化块，只能在定义的时候指定默认值  

-  java 8之后可以在接口内实现方法，包括：  
    - 类方法（无法被继承，实现类不能继承接口类的静态方法而普通类的静态方法可以被继承）
    - 显式声明default的方法（可以被实现类重写），通过接口的实现类的实例来调用（所以不能用static修饰）  
    - <u>这里的default是一个关键字，与权限访问的那个默认权限无关</u>  
- java 9之后可以使用private方法实现，但是不能与abstract共用  

总结：
- java 7 
    - 常量
    - 抽象方法 
- java 8
    - java 7
    - 静态方法 （static 无法被继承）
    - 默认方法 default （public default）
- java 9
    - java 8
    - 私有方法 （private）
    - 静态私有 （static private）  

**加一个public static的内部类**
```java
    interfacce Output // 定义一个接口
    {
        public static final int devNum; // 常量

        private void test() // java 9之后支持的私有方法 只供该接口使用
        {
            System.out.println("接口的私有方法");
        }

        public abstract void printData(String msg); // 标准的抽象方法

        public default void func() // 定义接口中的默认方法
        {
            System.out.println("接口的默认方法");
        }
        public static void staticMethod() // 静态方法
        {
            System.out.println("接口的静态方法");
        }
    }

    public class Dev implements Output
    {
        public void func() // 这里的权限要写public，因为实现类的权限不能小于所继承的类
        {
            System.out.println("重写默认方法");
        }

        public void printData(String msg) // 实现抽象方法
        {
            System.out.println(msg);
        }

        public static void main(String[] args)
        {
            Output.staticMethod(); // 只能通过接口类调用，实现类不继承接口类的类方法，但继承普通类的类方法
            Dev dev=new Dev();
            dev.printData("啦啦啦");
        }
    } 

```
- 接口的例子

```java

public class Computer { 
    
    private Output __output; // 私有变量output设备

    Computer(Output output) // 初始化
    {
        this.__output=output;
    }

    public void print() // 输出
    {
        __output.printData();
    }

    public static void main(String[] args)
    {
        Dev dev=new Dev();
        Computer computer=new Computer(dev);
        computer.print(); 
    }
}

class Dev implements Output // 某种输入设备
{
    public void printData()
    {
        System.out.print("fasdfsfds");
    }
}

interface Output // 输入设备的接口
{
    void printData();
}

Dev  实现  ->    output    <-  依赖   computer
成功的将具体设备和computer分隔开
```





### 内部类的访问权限
- private、protected、static 多了三种类修饰符，外部类只能是public或者默认  
- <u>非静态内部类不可用有静态成员</u>  
- 内部类用 **外部类名.this** 来访问外部类的成员  
- 非静态内部类查询顺序：内部类 局部变量->内部类变量->外部类变量  
- 接口能定义 **public static** 内部类  
- 当外部类被另一个类继承时，即使在子类中声明另一个同名的内部类，原本的内部类不会被重写，因为两个内部类所处的外部空间不同，两个内部类不同

```java

public class Outer 
{
    String outerName="oooo";
    private class Inner // 非静态内部类，内部不能有静态成员
    {
       String name="123";
       
    }
    
    static private class Inner2 // 静态内部类，内部可以有非静态成员
    {
        static String name2="abc"; // 静态变量
        String name3="qqqq";
        void func()
        {
            System.out.print(outerName); // 错误，静态内部类不能访问外部类的非静态成员
        }
    }

    private void func() // 外部类的一个方法
    {
        System.out.print(new Inner().name); // 通过实例访问非静态内部类的成员
        System.out.print(Inner2.name2); // 通过 内部类名.成员 访问静态内部类中的静态变量
        System.out.print(new Inner2().name3);
    }
    public static void main(String[] args)
    {
        new Outer().func(); // 成功输出"123"
        new Inner2(); // 不报错
        new Inner(); // 静态方法不可调用非静态成员，包括非静态内部类，报错！
    }
}
```

### 访问规则总结

|类别|权限|
|:-:|:-:|
|非静态内部类（只有非静态成员）| 可以直接访问外部类的所有成员| 
|静态内部类（静态、非静态两种成员）| 无论是静态还是非静态成员都不能访问外部类的非静态成员     都可以访问外部类的静态成员|  
|       | 可以通过new Outer().成员 的方法得到外部类的非静态成员，但是这是通过外部类的实例访问的，静态内部类并不具备访问外部类的非静态成员的权限|
|外部类 | 可以访通过new Inner().非静态成员 访问所有内部类的非静态类成员
|       | 通过 静态内部类名.静态成员 来访问静态内部类的静态成员
|       |静态方法不可访问非静态内部类以及非静态成员


- 局部内部类（处于方法中）无访问控制符和static修饰符  
- 一个类中可能存在多个同名局部内部类（不同方法中），java 用不同的数字加以区分 Class$NumLocalInnerClass.class

- 匿名类创建时将会创建对象所以不能是抽象类，同时没有构造器，但可以有初始化块
```java
// 匿名类实现
public class Computer { 
    
    private Output __output; // 私有变量output设备

    Computer(Output output) // 初始化
    {
        this.__output=output;
    }

    public void print() // 输出
    {
        __output.printData();
    }

    public static void main(String[] args)
    {
        Computer computer=new Computer(new Output() // 匿名类的实现
        {
            public void printData()
            {
                System.out.print("fasdfsfds");
            }
        }
        );
        computer.print(); 
    }
}

interface Output // 输入设备的接口
{
    void printData();
}

```


- lambda 不能调用函数式接口中定义的默认方法，但是匿名内部类可以
- 非抽象的枚举类无子类，默认用final修饰
```java
interface Weather
{
    void prediction();
}
public enum Season implements Weather
{
    SUMMER("夏天")
    {
        public void prediction()
        {
            System.out.println("Rainy day");
        }
    },
    WINTER("冬天")
    {
        public void prediction()
        {
            System.out.println("Snowy day");
        }
    }; 
    // 因为要实现抽象方法，所以系统用abstract修饰而不是final修饰，这两个枚举值相当于匿名内部类的实例。
    public enum Operation // 疯狂java讲义 P226
    {
        PLUS
        {
            public double eval(double x,double y)
            {
                return x+y;
            }
        },
        MINUS
        {
            public double eval(double x,double y)
            {
                return x+y;
            }
        };
        public abstract double eval(double x,double y);
    }
}
```



### 函数式接口与Lambda (摘自 疯狂Java讲义第3版)
- 函数式接口只有一个抽象方法，可以有多个static、default之类具有函数体的方法(使用@FunctionalInterface来检查)
- 形参，圆括号表示形参列表，只有一个时可以省略，无参数不能省略  
- 箭头  
- 代码块，用 {}，只有一条语句可以省略花括号，如果只有一条语句且是返回语句，可以省略 return  

```java
// 匿名类的实现方式
public class test 
{
    public static void main(String[] args)
    {
        ProcessArray pa=new ProcessArray();
        int[] target={3,-4,6,4};
        pa.process(target,new Command() // 匿名内部类 需要实现process函数
        {
            public void process(int[] target) //实现函数
            {
                int sum=0;
                for(int tmp :target)
                {
                    sum+=tmp;
                }
                System.out.println(sum);
            }
        });
    }
}

// lambda 实现方式
public class Test 
{
    public static void main(String[] args)
    {
        ProcessArray pa=new ProcessArray();
        int[] target={3,-4,6,4};
        pa.process(target,(int[] target) ->  // lambda 表达式代替匿名内部类
        {
            int sum=0;
            int sum=0;
            for(int tmp :target)
            {
                sum += tmp;
            }
            System.out.println(sum);
        });
    }
}

// 例子 1
@FunctionalInterfacce
interface Eatable // 函数式接口
{
    void taste();
}

public class Test
{
    public void eat(Eatable e) // 需要实现接口的实例作为参数
    {
        System.out.println(e);
        e.taste();
    }

    public static void main(String[] args)
    {
        Eatable e = () -> System.out.println("难吃"); // 通过lambda直接实现Eatable接口
        Test test = new Test();
        test.eat(e);

        // 另一种写法
        // Test test = new Test();
        // test.eat(() -> System.out.println("难吃");)

    }
}
```
- 方法引用（四种）
```java
@FunctionalInterface
interface Converter // 定义一个实现字符串到整型对象的接口
{
    Integer convert(String from);
}

// 例子 1 使用类方法
public class Test
{
    public static void main(String[] agrs)
    {
        Converter converter = from -> Integer.valueOf(from); // 调用类方法
        // 其他方法
        // Converter converter = Integer::valueOf; 参数将传给 Integer::valueOf 方法
    }
}

// 例子2 使用特定对象方法
public vlass Test
{
    public static void main(String[] args)
    {
        Converter converter = from -> "abcdef".indexOf(from); 
        // 返回from在字符串的下标，返回值是 int，隐式转换为 Integer
        // 可以进行如下替换
        // Converter converter = "abcdef"::indexOf;
    }
}

// 例子 3 使用某类对象的实例方法
@FunctionalInterface
interface MyTest
{
    String func(String a,int b,int c);
}

    MyTest mt = (a,b,c) -> a.substring(b,c);
    // 其他写法
    // MyTest mt = String::substring;
    // 调用String类型的实例对象的方法，第一个参数 a 作为实例对象的调用者
    // b 和 c 作为调用方法的剩余两个参数

// 例子 4 使用构造器
@FunctionalInterface
interface MyTest
{
    String func(byte[] bytes);
}
    byte[] bytes={2,32,1};
    MyTest mt = bytes -> new String(bytes); // 使用构造器
    // 其他写法
    // MyTest mt = String::new;

```