# Chapter 1

单机 -> 加database -> 加缓存 -> 加CDN ->加负载驱动 -> database scaling

low traffic -> 1*(web server + database)

high traffic -> load balancer(把traffic分发到多server上) + vertical/horizontal scaling(增加单机性能/增加更多server) + caching(高频访问放进内存)

read-heavy: 刷xhs,看商品, 绝大多数操作是read(cache/read replicas读写分离)

write-heavy: 下单,写入database,绝大多数是write(message queue,如kafka)

## single server setup

local host 访问域名 google.com 需要该IP地址 -> **DNS解析**

1.查找browser和OS的本地缓存域名IP

2.recursive resolver递归解析器 问internet service provider(verison..) 或者配置的DNS服务器

3.root name server 问根服务器.com,去TLD top level domain服务器

4.TLD server google.com? 5. Google authorirative name server 给出IP addr

有vpn节点时,DNS解析发生在vpn服务器

**TCP连接** 跟IP建立三次握手

**TLS加密** 在TCP连接上协议加密

**HTTP请求** 加密通道传输数据

最后web server返回HTML pages / JSON response

- 传统web app: server(business logic, storage..) <----> client (HTML展示,包含大量标签< div >< span >) 

- 现代前后端分离mobile app: web server <--HTTP protocol--> mobile app
  web server返回format JSON

```
GET /users/1
{
    "id":1,
    "address":{
        "street":"11 2nd st",
        "city": "NY",
    },
    "phoneNumbers":[
        "111 111 111",
        "222 222 222"
    ]
}

```  
## Database

分为 web server和 database server可以独立拓展,请求量增大可以加web server,数据量大可以单独升级database

Web Server（Web Tier） — 处理所有client请求,返回 HTML/JSON;它负责接收请求 & 执行业务逻辑 & 返回响应

Database Server（Data Tier） — 只负责存储和查询数据,Web Server 需要数据的时候,向 Database Server 发查询请求

database -- relational database & non relational database

- relational database : MySQL, PostgreSQL

支持JOIN & 强一致性。JOIN多表查询,一个字段把分散在不同表的data合并成一个完整结果

- non relational database : MongoDB(不支持JOIN,支持高性能和可拓展性, low latency, data are unstructured, store massive data)

1. only need serialize/deserialize(JSON XML YAML): 存取一个用户profile,读的时候解析,不需要JOIN跨表关联SQL查询

serialize 序列化- 数据结构(dict)转换成可以存储/传输的JSON字符串格式

deserialize 反序列化 - 将JSON解析回程序里的对象

2. massive data : SQL 保证事务的ACID特性, 横向scaling很复杂; NOSQL没有JOIN,加机器就能加容量,线性拓展

分类:
Key-Value Store — 一个大字典,比如 Redis。适合缓存、session 存储。

Document Store — 存 JSON/BSON 文档，每条数据可以有不同的结构。比如 MongoDB。适合内容管理、用户 profile

Column Store — 数据按列存而不是按行存，适合大规模分析查询。比如 Cassandra、HBase

Graph Store — 存节点和关系，适合社交网络、推荐系统。比如 Neo4j


## vertical scaling & horizontal scaling

vertical scale up - 单机加CPU RAM... - 简单, low traffic时可用, 单机goes down没有failover保护

horizontal scale out - 加servers - 支持更大traffic, 多用户连接web server需要 load balancer

## load balancer - web tier

distributes incoming traffic among  web servers

If server 1 goes offline, all the traffic will be routed to server 2. We will also add a new web server to the server pool to balance the load

## database replication - data tier

origin(master) and the copies(slaves)
