---
title: "知识图谱--Apache-Jena的使用"
date: 2020-04-28T14:31:44+08:00
draft: false
---

## 1、简介
根据官网上的描述，jena是一个用于构建语义Web和链接数据应用程序的免费java框架。
构架图如下：  
![jena架构图018](https://s1.ax1x.com/2020/04/28/J5G8TU.png)




## 2、安装
需要安装 **apache-jena** 和 **apache-jena-fuseki** 这两个在官网上就能下，这里下载两个 **zip** 压缩包，之后解压即可。



## 3、使用  
### 通过命令在浏览器中查询
根据图中描述，**TDB** 是 **jena** 用于存储 **RDF** 格式的组件。先把已有的 **RDF** 转化成 **TDB** 。 
- **RDF -> TDB** ，在 **jena** 安装目录下有个 **bat** ，输入 **tdbloader.bat --loc="dirpath" "path"**，**dirpath** 是提前创建的用于存放 **TDB** 的空目录，**path** 是 **RDF** 文件地址。就是之前用 **D2RQ** 生成的 **.nt** 文件。  
- 这样就获得了 **TDB** 文件  
- 然后需要启动 **fuseki** ，这样可以在网页上进行查询  
- 输入 **fuseki-server --loc=TDBpath /dbname** ，**TDBpath** 就是存放 **TDB** 的目录，**dbname** 就是将要生成的库名，这个可以随便填。会生成 **run** 文件夹   
![命令019](https://s1.ax1x.com/2020/04/28/J5GJkF.png)  
- 在网页中的效果如下图，可以查询到有290w个三元组。    
![020](https://s1.ax1x.com/2020/04/28/J5GYY4.png) 
- 进行查询（SPARQL）  
![查询语句021](https://s1.ax1x.com/2020/04/28/J5GtfJ.png)
- 查询电影 **流浪地球** 的主演还参加过哪些电影。查询结果如下图：  
![查询结果022](https://s1.ax1x.com/2020/04/28/J5GUp9.png)  

### 通过配置文件指定推理机和TDB文件 --- 参考 [实践篇（四）：Apache jena SPARQL endpoint及推理]
- 进入 **`fuseki`** 的的文件夹下，输入命令 `fuseki-server.bat` 要记住要加 **`bat`**，加和不加是两种情况，因为我们上一步生成的TDB文件有很多个，批处理。执行成功后关闭命令界面，会生成文件夹 **`run`**，在 **`configuration`** 目录下新增一个 **`.ttl`** 的配置文件，命名不重要。内容如下  
```java
@prefix :      <http://base/#> .
@prefix tdb:   <http://jena.hpl.hp.com/2008/tdb#> .
@prefix rdf:   <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix ja:    <http://jena.hpl.hp.com/2005/11/Assembler#> .
@prefix rdfs:  <http://www.w3.org/2000/01/rdf-schema#> .
@prefix fuseki: <http://jena.apache.org/fuseki#> .


:service1        a                fuseki:Service ;
fuseki:dataset                    <#dataset> ;
fuseki:name                       "db" ;
fuseki:serviceQuery               "query" , "sparql" ;
fuseki:serviceReadGraphStore      "get" ;
fuseki:serviceReadWriteGraphStore "data" ;
fuseki:serviceUpdate              "update" ;
fuseki:serviceUpload              "upload" .


<#dataset> rdf:type ja:RDFDataset ;
    ja:defaultGraph <#model_inf> ;
    .

<#model_inf> a ja:InfModel ;
    ja:baseModel <#tdbGraph> ;

   #OWL推理机
    # ja:reasoner [ja:reasonerURL <http://jena.hpl.hp.com/2003/OWLFBRuleReasoner>];

    #开启规则推理机，并指定规则文件路径
    ja:reasoner [
      ja:reasonerURL <http://jena.hpl.hp.com/2003/GenericRuleReasoner> ;
	ja:rulesFrom <file:///D:/softwares/apache-jena-fuseki-3.14.0/apache-jena-fuseki-3.14.0/run/databases/myRules.ttl>;]
   .

<#tdbGraph> rdf:type tdb:GraphTDB ;
   tdb:dataset <#tdbDataset>;
    .

<#tdbDataset> rdf:type tdb:DatasetTDB ;
  tdb:location "D:/softwares/apache-jena-3.14.0/apache-jena-3.14.0/TDB"
 .
```

- 内容中有两个推理机，有一个被注释了（只能有一个推理机），需要自行更改的有  

```java
fuseki:name   库名 ;  # 自定义库名
在OWL推理机和规则推理机(自定义)中选择:
OWL -> ja:reasoner [ja:reasonerURL <http://jena.hpl.hp.com/2003/OWLFBRuleReasoner>];
规则推理机 ->ja:reasoner [
      ja:reasonerURL <http://jena.hpl.hp.com/2003/GenericRuleReasoner> ; #选择开启规则推理机
	ja:rulesFrom <file:///D:/softwares/apache-jena-fuseki-3.14.0/apache-jena-fuseki-3.14.0/run/databases/myRules.ttl>; #自定义规则路径  ] 
   .

tdb:location path; # path是存放tdb的文件夹

```
- 下面是自定义的 **`myRules.ttl`** 文件，存放在 **`run/databases`** 文件夹下。

```java
@prefix : <http://www.db1.com#> .
@prefix owl: <http://www.w3.org/2002/07/owl#> .
@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
@prefix xsd: <XML Schema> .
@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .

[ruleGoodFilm: (?m :rating ?r) equal(?r , 8.0) -> (?m rdf:type :GoodFilm)] // 规则
```

---

## 4、存在的问题

- 



[实践篇（四）：Apache jena SPARQL endpoint及推理]: https://zhuanlan.zhihu.com/p/33224431

# 5、参考：   
实践篇（四）：Apache jena SPARQL endpoint及推理: https://zhuanlan.zhihu.com/p/33224431  