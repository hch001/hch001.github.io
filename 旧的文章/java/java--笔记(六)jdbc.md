---
title: "Java--笔记(六)jdbc"
date: 2020-05-23T07:53:19+08:00
draft: true
---


## jar包依赖
- 使用 mysql 数据库，导入 mysql_connector_java_5_1_44_bin.jar 包即可

---

## 代码实现
### Statement 和 PreparedStatement 使用
|参数|移动方向|支持更新|
|:-:|:-:|:-:|
|默认|只能向前|只读，不支持更新|
|TYPE_FORWARD_ONLY|只能向前移动|只读，不支持更新|
|TYPE_SCROLL_INSENSITIVE|支持双向滚动|支持更新且实时跟踪数据库中的数据变化|
|TYPE_SCROLL_SENSITIVE|支持双向滚动|支持更新但是不及时|

```java
// query
import java.sql.*;

public class A {    
    public static void main(String[] args)
    {
        try{
            String table = "actor";
            String username="";
            String password="";
            Class.forName("com.mysql.jdbc.Driver");
            // ！ 新版：Class.forName("com.mysql.cj.jdbc.Driver");
            // 加载驱动

            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2?useSSL=false&serverTimezone=UTC", username, password);
            // 记得加上密码
            
            PreparedStatement ppStatement = connection.prepareStatement("select *\n" +
                    "from "+table+" \n" +
                    "where  actor.foreignName like '%#%'\n" +
                    ";");
            // preparedStatement 和 StateMent 的区别

            ResultSet results =  ppStatement.executeQuery();
            ResultSetMetaData metaData = results.getMetaData();
            // 获取元数据 例如查询后的列名
            int columnNum=metaData.getColumnCount();
            while(results.next())
            {
                for(int i=1;i<=columnNum;i++) // 从1开始
                    System.out.print(results.getString(i)+"\t");
                System.out.println();                
            }
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
    }
}
```
### update (delete 直接写在sql里面然后执行 executeUpdate 就行)
```java
// update
    public static void main(String[] args)
    {
        try{
            Class.forName("com.mysql.jdbc.Driver");
            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2?useSSL=false", "username", "password");
            PreparedStatement pstmt = connection.prepareStatement(sql,ResultSet.TYPE_SCROLL_INSENSITIVE,ResultSet.CONCUR_UPDATABLE);
            // 指定了能够双向移动和数据库更新敏感

            ResultSet rs = pstmt.executeQuery();
            int columnCount = rs.getMetaData().getColumnCount();

            rs.last(); // 光标定位到最后一条
            int rowCount = rs.getRow(); // 获取行数
            for(int i=rowCount;i>0;i++)
            {
                rs.absolute(i); // 定位到这一行
                for(int j=1;j<=columnCount;j++)
                    System.out.print(rs.getString(j)+"\t");
                System.out.println();
                rs.updateString(1,"lalalala"); // 将第一个字段改为lalalala
                rs.updateRow(); // 提交更改
            }
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
    }
```

### CallableStatement 使用
```sql
--  假设已经创建了函数
delimiter //
create ratingProc(IN name varchar(50), IN ignored boolean, out rating_avg float, out nums)
begin
    -- Declare local variables for rating, nums
    declare rating_tmp float;
    declare nums_tmp int;

    -- ignore ?
    IF ignored THEN
        -- 和上面一样
        select avg(film.rating),count(*) 
        from actor,perform,film
        where (
            film.releaseDate > '2010-01-01' 
            and actor.name = name 
            and actor.actorID = perform.actorID 
            and perform.filmID = film.filmID 
        )
        into rating_tmp, nums_tmp;
    
    ELSE 
        select avg(film.rating),count(*) 
        from actor,perform,film
        where (
            actor.name = name 
            and actor.actorID = perform.actorID 
            and perform.filmID = film.filmID 
        )
        into rating_tmp, nums_tmp;
    END IF;

    select rating_tmp,nums_tmp into rating,nums;
end //
```
```java
// 已经得到上述的函数
public static void main(String[] args)
{
    try{
        Class.forName("com.mysql.jdbc.Driver");
        Connection connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2?useSSL=false", "root", password);

        CallableStatement statement = connection.prepareCall("call ratingProc(?,false,?,?);");

        // 输入参数
        statement.setString(1,"张涵予");
        // 设置输出
        statement.registerOutParameter(2,Types.FLOAT);
        statement.registerOutParameter(3,Types.INTEGER);
        statement.execute();
        // 输出结果
        System.out.println("average:"+String.format("%.2f",statement.getFloat(2))+"\nnums:"+statement.getInt(3));
        // String.format()用于控制精度
    }
    catch(Exception e)
    {
        e.printStackTrace();
    }
}
```
### RowSet
- RowSet 和 ResultSet  

|类名|方法|连接|默认状态|其他|
|:-:|:-:|:-:|:-:|:-:|
|ResultSet|-|需要与数据库保持连接|只能向前或者只读(可以修改)|-|
|RowSet|在Result的基础上进行扩展|连接或离线|前后滚动和可以更新|添加了对 JavaBeans 组件模型的 JDBC API 支持|

- 五种RowSet  

|类名|描述|
|:-:|:-:|
|CachedRowSet|离线操作，数据在内存中进行增删改查最后再同步，可序列化，可作为JavaBeans在网络间进行传播，通过ResultSet或者表格类文件进行装填|
|WebRowSet|继承CachedRowSet，支持写入xml文件，或者从xml文件中装载|
|FilteredRowSet|通过设置Predicate进行过滤|
|JoinRowSet|支持在不同的RowSet之间进行JOIN操作|
|JdbcRowSet|对ResultSet的封装，需要保持连接，需要JDBC的驱动程序来保持连接|

- 离线 RowSet 使用
```java

public class A {
    public static void main(String[] args)
    {
        try{
            String table = "actor";
            String username="root";
            String password="";
            Class.forName("com.mysql.jdbc.Driver");
            // 加载驱动

            Connection connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2?useSSL=false", username, password);
            // 记得加上密码
            PreparedStatement ppStatement = connection.prepareStatement("select *\n" +
                    "from "+table+" \n" +
                    "limit 10;");

            ResultSet results =  ppStatement.executeQuery();

            RowSetFactory factory = RowSetProvider.newFactory();
            CachedRowSet cachedrs = factory.createCachedRowSet(); // 离线的RowSet
            cachedrs.populate(results); // 装载

            // 关闭结果集
            results.close();
            ppStatement.close();

            ResultSetMetaData metaData = cachedrs.getMetaData();

            int columnNum=metaData.getColumnCount();
            while(cachedrs.next())
            {
                for(int i=1;i<=columnNum;i++) // 从1开始
                    System.out.print(cachedrs.getString(i)+"\t");
                System.out.println();
            }
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
    }
}

// 另一种写法



```
- 分页离线 RowSet
```java
public class A
{
    public static void main(String[] args)
    {
        Connection connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2?useSSL=false", username, password);
        // 记得加上密码
        PreparedStatement ppStatement = connection.prepareStatement(sql);

        ResultSet results =  ppStatement.executeQuery();
        RowSetFactory factory = RowSetProvider.newFactory();
        CachedRowSet cachedrs = factory.createCachedRowSet(); // 离线的RowSet
        cachedrs.setPageSize(pageSize); // 设置每一页大小
        cachedrs.populate(results,(pageNum-1)*pageSize);
        // 从result的第(pageNum-1)*pageSize条记录开始，装载一页

        // 如果只需要这一页，下面两项可以关闭
        // 如果还想输出后续的页数就别关
        results.close();
        ppStatement.close();

        ResultSetMetaData metaData = cachedrs.getMetaData();
        int columnNum=metaData.getColumnCount();
        ...
    }
}
```

### PreparedStatement 和 Statement 的区别
- PreparedStatement 是继承 Statement 的子接口
- PreparedStatement 经过预编译(只需一次编译，避免了多条sql语句需要重复编译的问题)，可以防止 sql 注入，采用非法字符转义的方法
- PreparedStatement 支持使用 ? 占位符，然后使用 setXXX(index,value) 进行补充，面对大量语句时可以提高效率
- PreparedStatement 执行单条语句的速度不如 Statement，但批处理速度更高

### 事务
```java
public static void main(String[] args)
{
    Statement stmt conn.createStatement();
    conn.setAtuoCommit(false); // 取消自动提交 
    stmt.executeUpdate(...);
    stmt.executeUpdate(...);
    stmt.executeUpdate(...);
    conn.commit(); // 提交
    conn.rollbask(); // 回滚


    stmt.addBatch(sql);
    stmt.addBaych(sql);
    ...
    stmt.executeBatch(); // 批量提交SQL语句
    stmt.commit(); // 提交

}
```

### 连接池
频繁地打开、关闭连接 **->** 在应用启动时进行主动建立并存在连接池中
```java
public class Test {
    public static void main(String[] args)  {
        try
        {
            BasicDataSource ds = new BasicDataSource(); 
            // 创建数据源对象(产生数据源地工厂，只需要一个)
            ds.setDriverClassName("com.mysql.jdbc.Driver");
            // 设置驱动
            ds.setUrl("jdbc:mysql://localhost:3306/db2?useSSL=false");
            ds.setUsername("root");
            ds.setPassword("");
            ds.setInitialSize(5); // 设置初始连接数
            ds.setMaxTotal(20); // 最多20个
            ds.setMinIdle(2); // 最少两个空闲

            Connection conn = ds.getConnection();
            Statement stmt = conn.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE,ResultSet.CONCUR_UPDATABLE);
            stmt.execute("select * from actor limit 20;");
            ResultSet rs = stmt.getResultSet();
            while(rs.next())
            {
                System.out.println(rs.getString(1)+"\t"+rs.getString(2));
            }
            conn.close();
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
    }
}

```

---

## 一些注意的点
- index out of range
```java
// 返回的数据从1开始计数，而不是0，使用占位符的计数也是从1开始
```
- 使用 ? 占位符时遇到 syntax error
```java
// 使用 ? 占位符的时候，如果设置字符串将会自动加上两个单引号，那么原本的表名、列名、数据库名、关键字(insert update delete)等就会变成常量.
// 解决的方法就是使用拼接字符串而不是占位符
// 在python中用repr(value) 可以在 'value' 和 value 之间转化
```
- 使用 jdbc 时去除警告的方法
```java
Connection connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/db2?useSSL=false", "root", "");
// 加入?useSSL=false
```