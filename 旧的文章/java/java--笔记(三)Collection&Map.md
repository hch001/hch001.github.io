---
title: "Java--笔记(三)Collection&Map"
date: 2020-05-13T15:49:39+08:00
draft: false
---


## Collection 
参考：疯狂Java讲义p282-283，我扩展了一下这个图  
![023.png](https://s1.ax1x.com/2020/05/15/Ys3m1P.png)


### Set -- TreeSet
- Sorted 判断两个元素是不是同一个对象的依据是 compareTo 的返回值，添加元素时调用新元素的 compareTo 方法和已有的元素对比
- TreeSet 如果想要正常运行的话，只能添加同一种类型的对象，先加入的对象，调用自身的 compareTo 方法与已加入的对象进行比较。  
- 因此是可以重写 compareTo ，TreeSet只依靠该函数的返回值进行判断是否是同一个对象，返回值为0代表相同，正数代表大于，负数小于，如果返回值是0，即使是同一个对象也看做不同。  
- 极端情况下仍可以改写 CompareTo 使得TreeSet的元素是不同类型。最好不要这样  

- 规范：应保证 equals 的结果和 compareTo 相一致，compareTo返回0，equals应该返回true  
- 修改元素中涉及compare相关的变量会导致，TreeSet 无法删除某些元素

```java
// 极端的情况下
public class Test{
    
    public static void main(String[] args) 
    {
        TreeSet ts = new TreeSet();

        ts.add(new String("hello"));
        ts.add(new A(10)); 
        // 顺序不能反，显示String对象加入，然后A加入调用A的 compareTo
        // 将 arg0 转化为String类型，比较
        System.out.println(ts); 
        // 调用 TreeSet 对象的 toString 方法将会调用所有元素的toString
        // 并以逗号进行分割，用方括号包围
        // 输出：[hello, length:10]
        // 但是如果再加入一个元素，需要考虑到已加入的所有元素的对比就会出错，
        // 比如再加入一个new A()。 已加入的A类型的对象无法转换成 String 报错
    }

}

class A implements Comparable{ // 对象要实现CompareTo 接口

    public String toString() // 改写toString
    {
        return "length:"+this.length;
    }

    int length;
    A(int l){this.length=l;}

    @Override
    public int compareTo(Object arg0) {
        return this.length-((String)arg0).length();
    }
}

```
- 使用定制排序
```java
// 定制排序
public class Test
{
    public static void main(String[] args)
    {
        TreeSet ts = new TreeSet(
            (o1,o2) - > ((A)o1).count-((A)o2).count 
        );
        // 原型 new TreeSet(Comparator comparator) 函数式接口可以用 lambda 表达式代替
    }
}   

class A
{
    int count;
    A(int c){this.count=c;}

    public String toString()
    {
        return "count:"+count;
    }
}
```
### Set -- HashSet 
- **根据 equals 和 hashCode 函数 缺一不可** 的返回值来判断两个元素是否是同一对象

### List 
- 根据 equals 判断元素是否相同  
- 不指定元素类型时，可以添加不同类型的对象

### 特殊的ArrayList，固定长度的List
- Arrays工具类中提供了asList(Object... a)方法，将对个对象转换成List集合，这个集合不是ArrayList也不是Vector，是与ArrayList同名的 Arrays的内部类 ArrayList。它是固定长度的，所以不支持add和remove。Arrays$ArrayList    

### 性能
- 数组以连续内存区来存储元素，随机访问性能好  
- 链表在增删的时候性能好，数组经常考虑到重新分配大小   
- 总体来说 ArrayList 比 LinkedList 性能好一点  




## Map
![025.png](https://s1.ax1x.com/2020/05/19/Y5vstx.png)
### 查重方式
- 和HashSet的判断重复的方式一样，**用 hashcode() 和 equals() 缺一不可** 的结果作为判断的依据




## Set 和 Map 
### 联系
- 先实现了HashMap TreeMap 等集合，之后通过包装了所有 values 为 null 的 Map 实现了 Set 集合  
- EnumMap
```java
enum Season
{
    SPRING,SUMMER,FALL,WINTER;
}

public class Test
{
    public static void main(String[] args)
    {
        EnumMap<Season,String> em = new EnumMap<>(Season.class);
        // EnumMap em = new EnumMap<>(Season.class);
        em.put(Season.SUMMER, "hot");
    }
}
```
### 性能 
    - HashMap 和 Hashtable 实现机制差不多(数组+链表)，但是 Hashtable 是线程安全的，HashMap 通常比 Hashtable 快
    - TreeMap 使用红黑树来管理键值对，每个节点都是一个键值对，速度比 HashMap 和 Hashtable 慢，尤其是插入删除的时候
    - IdentityHashMap 没什么特别之处，与 HashMap 的区别在于使用 == 判断相等
    - EnumMap 的性能最好，但只能使用同一个类的枚举值
    

### rehash
- HashMap 和 Hashtable 的 rehash 过程，参考：[rehash过程]

[rehash过程]: https://blog.csdn.net/yimi099/article/details/62043566