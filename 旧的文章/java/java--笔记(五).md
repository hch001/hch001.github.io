---
title: "Java--笔记(五)"
date: 2020-05-21T17:25:21+08:00
draft: true
---
## 异常
![026.png](https://s1.ax1x.com/2020/05/21/Yqxm6I.png)
- 异常有两大类：Checked 和 Runtime，非 RuntimeException 类及其子类都称为 Checked 异常
    - Checked异常是 java 特有的，是可以被处理修复的
    - 必须显示处理 Checked 异常，否则无法通过编译
- catch 中声明的变量是局部变量
- 先处理子类异常，否则将不会被执行
- 使用 | 分割可以捕获多种异常
- 捕获多种异常时，异常变量有隐式的 final 修饰
```java
public static void main(String[] args)
{
    try
    {
        int a = 10;
        int b = 0;
        FileInputStream fis = new FileInputStream("a.txt");
    }
    catch(IOException | ArithmeticException e)
    {
        System.out.println("出现了异常");
    }
}
    getMessage()
    printStackTrace()
    printStackTrace(PrintStream s)
    getStackTrace()
```

- try 是必需的，catch 和 finally 至少要有一个
- 哪怕在catch 中已经 return，也会先执行 finally，但如果是使用 System.exit(1); 退出虚拟机，那么将不会执行 finally。
- 使用 finally 回收物理资源(数据库连接、网络连接和磁盘文件)，try 和 catch 不一定能执行到。
- 在 try 之后使用圆括号来代表自动关闭资源(Java 7 之后)
```java
public static void main(String[ args])
{
    try(
        BufferedReader br = new BufferedReader(new FileReader("a.txt"));
        PrintStream ps = new PrintStream(new FileOutputStream("b.txt"));
    ) // 自动关闭资源 br 和ps
    {
        System.out.println(br.readLine());
        
    }
    catch(IOException e)
    {
        
    }
}
```

- 使用 throws (运用在方法中，checked) 抛出异常的优缺点
    - 需要显示处理异常
    - 抛出异常的方法被子类重写时，子类的异常只能和父类的异常相同或是子类，数量也不能超过
    - 使用 checked 可以帮助在编译时找出问题
```java
// 编译错误的例子
class Person
{
    String[] names;
    public void printName() throws  NullPointerException
    {
        for(String name:names)
            System.out.println(name);
    }
}

class Student extends Person
{
    public void printName() throws Exception 
    // 编译不通过 Exception大于NullPointerException
    {
        ...
    }
}
```
- 使用 throw （运用在方法内部 Runtime）
    - 可以不用处理，更灵活
    - 容易被忽视
```java
// 使用RuntimeException
public class A {
    public static void main(String[] args) 
    {
        throw new RuntimeException(); // 不报错
    }
}

// 抛出Exception 报错
public class A {
    public static void main(String[] args) 
    {
        throw new Exception(); // 报错
    }
}
```