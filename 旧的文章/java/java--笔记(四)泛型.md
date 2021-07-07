---
title: "Java--笔记(四)泛型"
date: 2020-05-19T17:25:21+08:00
draft: false
---

### 不存在泛型的子类型
- 无论泛型的类型参数是什么，内存中都只对应一种类，就是说泛型本身不创造新的类。因此在**静态方法、静态初始化、静态变量等不能用类型形参**，使用 instacceof 时也要注意。
```java
// 示例
List<Integer> l1 = new ArrayList<>();
List<String> l2 = new ArrayList<>();

System.out.println(l1.getClass() == l2.getClass());
// 输出true
```
- **假设有一个具有泛型声明的类 Test，那么Test\<String>不是Test\<Object>的子类！**
```java
public static void test(List<Object> c)
{
    for(int i=0;i<c.size();i++)
    {
        System.out.println(c.get(i));
    }
}

public static void main(String[] args)
{
    test(new ArrayList<String>());
    // 报错 List<String> 并不能当作 List<Object>用，即前者不是后者的子类
}
```
- **但是数组中存在子类型**
```java
Apple[] apples=new Apple[10];
if(apples instanceof Fruit[]) 
// 只有在apples==null的时候才返回false，否则都是true，说明Apple[] 是 Fruit[]的子类型
{
    System.out.println("true");
}
```









## 通配符(wildcards)和T图示以及extends和super的读写区别 -- 参考[java泛型 通配符]
![028.png](https://s1.ax1x.com/2020/05/26/tPSQaD.png)
- 首先看下面这个例子
```java
public static void main(String[] args)
{
    Apple[] apples = new Apple[10];
    Fruit[] fruits = apples;
    fruits[0] = new Strawberry();
    // java.lang.ArrayStoreException
    // 编译能过，运行出错
}
```
### extends（读取安全、写入不安全）
首先Java支持父类引用子类对象。  
如果使用 **extends+泛型** 添加数据，下面的例子因为无法确定类型，所以不能够写入（要保证List的类型安全，否则有可能同时存储 Apple 和 Strawberry）。  
但是读取的时候情况不同，因为使用了extends，那么无论如何从List<? extends Fruit>中取出的元素一定是 Fruit 的子类的对象，那么可以直接用Fruit来引用。
```java
// 试图用 extends+泛型 写入。失败
public static void main(String[] args)
{
    List<? extends Fruit> fruits = new ArrayList<>();
    fruits.add(new Apple()); // 编译不通过
}

// 使用 extends+泛型 读取。成功
public static void main(String[] args)
{
    // 假设已经得到一个List fruits 其中的元素都是Fruit的子类对象
    List<? extends Fruit> l = fruits;
    Fruit fruit = l.get(0); // 成功
}
```

### super（写入安全、读取不安全）
如果使用 **super+泛型** 读取数据，List<? super Apple> 你不知道具体取出的是什么类型的对象，就不能用某个具体的类的引用。  
但是写入时List<? super Apple>，至少是Apple的父类，那么Apple及其子类的对象都可以写入了，但是包括Apple的父类对象在内的其他类的对象都不能写入。
```java
// 假设Apple有一个子类BigApple
// 试图用 super+泛型 读取数据失败
public static void main(String[] args)
{
    // 假设已经有一个List apples 里面装的都是Apple的父类对象
    List<? super Apple> l = apples;
    Fruit fruit = l.get(0); // 错误
    Apple apple = l.get(0); // 错误
}


// 使用 super+泛型 写入数据，成功
public static void main(String[] args)
{
    List<? super Apple> apples = new ArrayList<>();
    apples.add(new Apple());
    apples.add(new BigApple()); //成功

    apple.add(new Fruit()); // 失败
}
```

### 总结
读取用extends，写入用super，读写都需要就不用通配符。




[java泛型 通配符]:https://blog.csdn.net/flfna/article/details/6576394


### 支持接口
```java
class Student <T extends Person & Comparable>
{

}
```

### 泛型和同名普通函数的优先级
- 同一类中普通函数要优先级高于泛型
- 父类中有泛型而子类中有同名的普通函数，和属性的继承一样，看声明的类型
```java
// 继承关系中的特殊情况
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


### 菱形语法
- 泛型的复杂使用情况（同时有类的泛型和构造函数的泛型声明E和T）
```java
public class A<E> // 类声明的泛型E
{
    public <T>A(T t) // 构造器中声明的泛型T
    {
        System.out.println(t);
        System.out.println(t.getClass());
    }

    public static void main(String[] args)
    {
        A<String> a1 = new A<>(5); // T->Integer
        A<String> a2 = new <Integer> A<String>(10);
        // E->String    T->Integer

        A<String> a3 = new <Integer>A<>(5);
        // 错误，已经使用了构造函数的泛型指定就能不再用菱形语法

        A<String> a4 = new A<String>(5);
        // E->String T->Integer

        A<String> a5 = new <Integer>A(10);
    }
}
```
- 总结 :一共有三处地方可以指定具体类型，第一处指定类的具体类型，第二处指定构造器的具体类型，第三处是类的具体类型，在省略的情况下根据参数来判断类型，在显示指定构造函数的时候（第二处）就不能使用菱形语法。

### 复杂的类型判断\<Java 8增强了泛型推理>
```java
class MyUtil<E>
{
    public static <T> MyUtil<T> nil() {return null;}
    // 返回一个MyUtil<T>类型的对象

    public static <T> MyUtil<T> cons(T head,MyUtil<T> tail) {return null;}
    // 返回一个MyUtil<T>类型的对象
}
public class Test
{
    public static void main(String[] args)
    {
        MyUtil<String> mu1 = MyUtil.nil();
        MyUtil<String> mu2 = MyUtil.<String>nil();
        // 上面两个均可以通过指定类型判断是String
        MyUtil.cons(42,MyUtil.nil());
        MyUtil.cons(42,MyUtil.<Integer>nil());
        // 上面两个可以通过42 判断是Integer类型的
    }
}
```


### 通配符与泛型
- 存在依赖关系的时候，尤其是被依赖的那一方，例如下面的例子应该用到泛型，没有被依赖的一方用通配符。
```java
// java源码
public interface Collection<E>
{
    boolean containsAll(Collection<?> c); // 没有依赖关系用通配符
    boolean addAll(Collection<? extends E> c); // 存在依赖关系，被依赖的一方用通配符E
}
```

### 类型擦除
- 当没有泛型没有指定类型又无法推导出来的时候，就被称为是**raw type(原始类型)**，默认是 **上限类型**，**使用通配符的时候输出就是 Object 类型的。**
```java
class Apple<T extends Number> // 泛型的上限是Number
{
    T value;
    Apple(T v)
    {
        this.value=v;
    }
    public T getValue()
    {
        return value;
    }
}

public class A
{
    public static void main(String[] args)
    {
        Apple a = new Apple(10); // 这里推导出的类型是 Apple<Integer>
        Apple b = a; // 省略了指定类型默认为 raw type（原始类型）是上限 Number
        Integer c = b.getValue(); // 报错 Number 类型的对象不能赋值给 Integer
    }
}
```

### 泛型与数组

- 不能创建包含**类型变量**或者**类型形参**的数组对象，但可以声明该数组
```java
List<String>[] l ; // 声明泛型数组正确
---
List<String>[] l = new ArrayList<String>[10]; // 错误，不能创建带有泛型的数组对象
---
List<String>[] l = new ArrayList[10]; // 正确，声明该数组不会报错，创建的是不带泛型的数组
```

- 原因



- 实现的两种方法 --- 参考[java泛型数组]
    - 像上面的例子中省略或者使用**通配符**就可以解决，但仍然存在风险
    - 使用 Array.newInstance(type,size) 创建

```java
// 使用通配符
public static void main(String[] args)
{
    List<?>[] l = new ArrayList<?>[10]; // 使用 ? 正确
    Object[] o = l;
    List<Integer> li = new ArrayList<Integer>();
    li.add(10);
    o[1] = li; // 直接往 泛型列表数组中 中添加了 List<Integer>元素
    String s = (String)l[1].get(0); 
    // 错误，Integer 不能转换为String

    //解决方法
    String s = null;
        if(l[1].get(0) instanceof String) // 先判断
    {
        s = (String)l[1].get(0);
    }
}


// T
// 失败范例
<T> T[] createArray(Collection<T> coll)
{
    return new T[coll.size()]; // 编译错误
}

//成功范例
import java.lang.reflect.Array;

public class Test {
    public static <T> T[] createArray(Class<T> type,int size)
    {
        T[] array = (T[])Array.newInstance(type,10);
        return array;
    }

    public static void main(String[] args)  {
        String[] ss = createArray(String.class,10);
        ss[0]="aaa";
        ss[1]="bbb";
        for(String s:ss)
        {
            System.out.println(s);
        }
    }
}

```
[java泛型数组]:https://www.cnblogs.com/lishuaiqi/p/10312016.html