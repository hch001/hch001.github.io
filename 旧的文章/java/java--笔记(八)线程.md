---
title: "Java--笔记(七八)线程"
date: 2020-06-11T07:53:19+08:00
draft: false
---

### 线程的三种实现形式
- Callable  
- Runnable
- extends Thread
```java
public static void main(Stringp[ args])
{
    try
    {
        // 使用FutureTask 可以获取返回值 FutureTask 实现了Runnable接口
        FutureTask<Integer> task = new FutureTask<>((Callable<Integer>) ()->  // 强制转换
        {
            int sum=0;
            for(int i=0;i<10;i++)
            {
                System.out.println(Thread.currentThread().getName()+"\t"+i);
                sum+=i;
            }
            return sum;
        });

        new Thread(task).start();
        System.out.println("sum="+task.get()); // 使用get获取返回值
        // 还可以加上timeout 和 unit 来规定超时
        
        // 使用Runnable来进行
        new Thread(()->{
            for(int i=0;i<10;i++) System.out.println(Thread.currentThread().getName()+"\t"+i);
        }).start(); 
        
        // 继承Thread类略

    }
    catch (Exception e)
    {
        System.out.println("^_^");
        e.printStackTrace();
    }
}

```


### 同步监视器

释放同步监视器:
- 代码块或方法执行结束  
- 出现**未处理的Error**或者**Exception**  
- 调用**wait**

不释放同步监视器:
- 调用**Thrrad.sleep()**或者**Thread.yield()**
- 调用了该线程的**suspend**




### 线程池种类
|名称|返回对象|特性|用法|
|:-:|:-:|:-:|:-:|
|newCachedThreadPool()|ExecutorService|具有缓冲的线程池|ExecutorService pool = Executors.newCachedThreadPool();|
|newFixedThreadPool(int nThreads)|ExecutorService|可重用的，固定数量的线程池|-|
|newSingleThreadExecutor()|ExecutorService|单线程的线程池|-|
|newScheduledThreadPool(int corePoolSize)|ScheduledExecutorService|在指定延迟后执行的线程组|ScheduledExecutorService pool = Executors.newScheduledThreadPool(10)|
|newWorkStealingPool(int parallelism)|ExecutorService|创建支持给定的并行级别的线程池，默认以CPU的数目设置|-|


### 线程池提交任务
- 线程池的方法主要有
    - submit(Runnable | Callable\<T> task) 
    - submit(Runnable task,T result) 执行结束后返回参数 result
    - shutdown() 不再接受新的任务，执行完所有已提交任务
    - shutdownNow()停止所有进程，返回等待执行的列表
    - schedule(Callable\<T> | Runnable task,long delay,TimeUnit unit) 任务在delay之后执行  
    - scheduleAtFixedRate(Callable\<T> | Runnable task,long delay,long period,TimeUnit unit) 在延迟执行之后，每隔period时间就再执行一次，TimeUnit是枚举类，TimeUnit.HOUR等
    - scheduleWithFixedDelay(Callable\<T> | Runnable task,long initialDelay,long period,TimeUnit unit) 第一次延迟执行之后，每次执行结束和下一次开始之间的间隔

- 提交Callable  
Callable 是一个函数式接口，包含一个 v call() throw Exception 。所以可用 lambda。
```java
public static void main(String[] args)
{
    ExecutorService pool = Executors.newSingleThreadExecutor();
    Callable<Integer> task=()-> // 创建一个实现Callable接口的实例
    {
        int sum=0;
        for(int i=0;i<10;i++)
        {
            System.out.println(i);
            sum+=i;
        }
        return sum;
    };
    pool.submit(task); // 提交
    pool.shutdown(); // 关闭线程池
}
```
- 提交Runnable
```java
public static void main(String[] args)
{
    ExecutorService pool = Executors.newSingleThreadExecutor();
            pool.submit(()->
            {
                System.out.println("fasdf");
            }); // 无返回值，使用lambda代替Runnable接口的实现对象
}
```




### 同步方法
- lock [Java中的锁分类与使用]
- synchronized 配合 wait() notify() notifyAll() 可以使用 监视器名.wait() 指定
- condition
- BlockingQueue [Java多线程-工具篇-BlockingQueue]  
    - ArrayBlockingQueue 
    - LinkedBlockingQueue  
    - DelayQueue
    - PriorityBlockingQueue  
    - SynchronousQueue

[Java多线程-工具篇-BlockingQueue]: https://cnblogs.com/jackyuj/archive/2010/11/24/1886553.html
[Java中的锁分类与使用]: https://www.cnblogs.com/hustzzl/p/9343797.html





### ThreadLocal
保存不同线程的局部变量
```java
class Account
{
    // 使用ThreadLocal
    private ThreadLocal<String> name = new ThreadLocal<>(); 
    public void setName(String name)
    {
        this.name.set(name);
    }
    public String getName()
    {
        return this.name.get();
    }
}

class TestThread extends Thread
{
    private Account account;
    TestThread(Account account)
    {
        this.account=account;
    }
    public void run()
    {
        for(int i=0;i<10;i++)
        {
            System.out.println(account.getName()+"\t"+i);
            if(i==4) account.setName(getName()); // 更改ThradLocal变量
            // 每个线程的该变量是独立的，虽然下面的例子中用的是同一个account
        }
    }
}

public class Test
{
    public static void main(String[] args) {
        try
        {
            Account account = new Account(); 
            new TestThread(account).start(); // 使用同一个account
            new TestThread(account).start(); // 使用同一个account
        }
        catch (Exception e)
        {
            System.out.println("^_^");
            e.printStackTrace();
        }
    }
}
```

### 包装线程不安全的集合类
Collections.synchronizedCollection() synchronizedList() synchronizedMap等，将线程不安全的类装换成线程安全的类。 
- Collection  
- List  
- Map  
- Set  
- SortedSet  
```java
Map<String,Integer> hashMap = Collections.synchronizedMap(new HashMap<String,Integer>());
```

### 线程安全的集合类
java.util.concurrent 包
- Concurrent 开头的集合类
    - ConcurrentHashMap
    - ConcurrentSkipListMap
    - ConcurrentSkipListSet
    - ConcurrentLinkedQueue
    - ConcurrentLinkedDeque  
    - 读操作不锁定，写操作锁定，但采用更高级的算法不会锁定整个集合
- CopyOnWrite 开头的集合类
    - CopyOnWriteArraySet 
    - CopyOnWriteArrayList 
    - 读操作不加锁，写操作创建集合的副本，在副本上进行写操作，适合读远大于写的场合，例如缓存