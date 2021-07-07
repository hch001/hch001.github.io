---
title: "知识图谱--D2RQ使用"
date: 2020-04-27T14:49:37+08:00
draft: false
---
 
## 一、简介
**D2RQ** 是一个可以通过虚拟RDF来访问关系型数据库的平台。它不需要将显式转换成 **RDF** 格式，就能像访问真正的 **RDF** 数据那样进行访问操作。它提供了下述的功能：  
    - 在非 **RDF** 格式的数据下使用 **SPARQL**  
    - 将数据库以自定义的方式转换存储为 **RDF** 格式  
    - 通过 **Web** 以链接数据(**Linked Data**)的方式访问数据库  
    - 使用 **apache-jena API** 对非 **RDF** 数据进行访问  
架构图如下图所示：  
![017.png](https://s1.ax1x.com/2020/04/28/J5G3wT.png)

 
 
## 二、虚拟 RDF 查询 (.ttl)
### 2.1 安装（略）
- 不多赘述，网上有很多攻略

 

### 2.2 生成映射文件
- 首先假设已经有了数据存在 **RDB(Rational Database)** 中，可以通过 **D2RQ** 来进行虚拟化RDF来访问 **RDB** 中的数据。（命名不规范之后再修改）  
    - **generate-mapping -u root -p password -o mapfile.ttl jdbc:mysql:///dbname**   
    **mapfile** 自定义，**dbname** 是数据库的名称，输出的文件是一个以 **ttl** 结尾的映射文件( **Turtle** 文件，一种 **RDF** 文件)，查询时 **D2RQ** 通过这个映射将输入的 **SPARQL** 转换成 **SQL** 语句进行查询。  
    ![008.png](https://s1.ax1x.com/2020/04/28/J5GETS.png)  
    - 如上图所示生成了 **ttl** 文件，为了简化写法，增加了前缀 **` @prefix : <http://www.db1.com#>. `**  db1 是数据库名  
    - ：代表处在 **\<http://www.db1.com#\>** 的命名空间(db1)下，第一条语句定义的。 **:Actors** 就表示再这个空间下的Actors。
    - 为了简化，属性表中的 **d2rq:property** 之后的内容也改为在 **：** 的命名空间下。  
    - 生成的表如下所示  
    ![生成表的映射关系009](https://s1.ax1x.com/2020/04/28/J5GZFg.png)  
    - 还有一种关联表的映射不太一样，如下所示  
    ![关联表的映射010](https://s1.ax1x.com/2020/04/28/J5GKln.png) 
    - 存在某种关系 **table1 relation table2** 。**table1** 就是 **relation** 的定义域，**table2** 就是值域。例如 导演(directors) 指导(direct) 电影(films)。  
    - 生成的 **` .ttl `** 文件中已经包含了对 **RDB** 的描述，在之后的查询中，会转换成 **SQL** 语句到 **RDB** 中查询  

 

### 2.3 利用网页查询  
- 输入 **d2r-server.bat mapfile.ttl**  
![输入命令012](https://s1.ax1x.com/2020/04/28/J5GeYQ.png)  
- 在浏览器中打开。**http://localhost:2020/** 效果如下图所示。  
![013](https://s1.ax1x.com/2020/04/28/J5GuSs.png) 
- 查询方式 **`SPARQL`** 。如下图所示。  
![查询结果014](https://s1.ax1x.com/2020/04/28/J5GMyq.png)  

 

### 2.4 Python 脚本连接
- 代码如下所示  
```python
if __name__ == '__main__':
    sparql = SPARQLWrapper("http://localhost:2020/sparql")
    sparql.setQuery("""
    PREFIX : <http://www.db1.com#>
    PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#>
    SELECT DISTINCT ?title  WHERE {
        ?m rdf:type :Films.
        ?m :type "movie".
        ?m :title ?title.
        ?m :rating ?r.
        }
        LIMIT 10
    """)
    sparql.setReturnFormat("json")
    result = sparql.query().convert()
    for i in result['results']['bindings']:
        print(i)
```
- 输出结果如下所示  
![输出结果016](https://s1.ax1x.com/2020/04/28/J5G1mV.png) 

参考: https://zhuanlan.zhihu.com/knowledgegraph

 
 
## 三、生成实体 RDF 文件 (.nt)

###  将RDB中的数据转换为RDF(Resource Description Framework)格式
- **.\dump-rdf.bat -o filename.nt .\mapfile.ttl**  根据 前面生成的 **mapfile.ttl** 中的映射规则将 **RDB** 中的数据转换为 **RDF** 格式。 **.nt (N-TRIPLE)** 是 **RDF** 导出格式的一种。  
![RDF格式的数据011](https://s1.ax1x.com/2020/04/28/J5GmWj.png)
