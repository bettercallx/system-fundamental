# Chapter 1

单机 -> 分布式 -> 加缓存 -> 加负载驱动

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

- web app: server(business logic, storage..) <----> client (HTML&javascript for presentation) 

- mobile app: web server. <--HTTP protocol--> mobile app
  API responce format - JSON

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
## 