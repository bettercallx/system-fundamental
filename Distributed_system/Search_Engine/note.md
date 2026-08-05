# Search Engine

## Keyword Matching 传统搜索 & Sematic search 语义搜索

**lexical search:**

词频统计: query 和文档有多少词重叠. BM25/TF-IDF

keyword matching: inverted index倒排索引,依赖词频算法,有一词多义和同义词未匹配问题

**sematic search:**

- 1.query 编码成vector embedding(向量化), 计算相似度cosine similarity
- 2.文档库达到数百万级别逐个计算太慢,用向量索引和近似最近邻索引 ANN approaxiamte nearest neighbor,HNSW (Hierarchical Navigable Small World)、IVF-PQ 等索引算法，实现毫秒级的海量向量检索; 常用向量数据库 / 引擎：Faiss、Milvus、Qdrant、Elasticsearch (Dense Vector 插件)

**pipeline:**

offline: doc/切分后的chunk 过一个encoder模型(bert)生成向量,存进向量数据库(FAISS/Milvus/Pinecone),并且建索引(HNSW/IVF)

online: query -> 同一个encoder模型生成向量 -> 去vector db里做approximate nearest neighbor检索 -> return topK results

**bi-encoder 召回 + cross-encoder rerank 的两阶段架构**: Bi-encoder 把 query 和 document 分别独立编码，速度快，适合召回阶段; Cross-encoder 把 query-document 拼在一起过模型，交互更充分所以精度更高，但太慢，只能用在 rerank 阶段对 top-k 候选重排

**hybrid search:**

双路召回 multiroute retrival:

query -> 倒排索引 -> 关键词retrieve result -----|

query -> 向量索引 -> semantic retrive result --|---> 100 重排序reranking(RRF (Reciprocal Rank Fusion)/ 交叉编码器) ->10

**混合检索** : 精准名字 (SKII)文件倒排索引 +场景化长尾词 (换季皮肤过敏) transformer 的dence retrival

**多路召回** : rerank阶段看query和doc的语义相关度

## 分布式系统

**数据分片和并行计算 sharding & partitioning:**
    索引分片 doc的倒排索引或向量索引太大, 单机保存装不下, 切分成多个shard分散到数百台机器上
    分布式检索 scatter gather : query -> 系统请求scatter给各个分片节点并行查找,最后聚合节点gather各节点top结果

**高可用与数据副本 Replication & Consistency：**
    每个shard都有主从副本 primary-replica; 副本同步; 一致性协议raft; 故障转移failover

**Distributed Storage & Compute：**
    离线建立索引,依赖分布式计算框架mapReduce Spark Flink

**负载均衡与微服务架构 Load Balancing & Microservices:**
    query 理解,召回,粗排,精排,rerank; 需要高并发RPC调用 限流升级

索引怎么切分？Shard 增加了怎么扩容？如何保证 99% 的请求在 50ms 内返回？多个 Shard 归并结果时怎么处理长尾延迟？

搜索引擎的底层（如倒排索引构建、向量检索、算子计算、内存数据结构）对延迟（Latency）和吞吐量（Throughput）要求极其苛刻，必须做到毫秒级响应

C++ 手动内存管理 零成本抽象 可以使用 SIMD 指令集（如 AVX-512）对向量计算和倒排链压缩/解压做硬件级加速

Rust 性能好 同时提供内存安全保障和优秀的并发模型

Go 原生并发支持好（Goroutine）开发效率高  适合写分布式的协调节点 网关和微服务

python deeplearning model的训练, offline embedding features抽取, semantic understanding + C++推理加速

## index sharding

**文档分片 doc partitioning:**
本地索引 local index

纵向切分, 将文档集合按 Doc ID 切割, 每个 Shard 负责一部分文档的完整倒排索引 doc1-100 doc101-200...

查询方式（Scatter-Gather）： 协调节点把用户 Query 广播发给所有 Shard, 每个 Shard 在本地检索出 Top-K, 最后由协调节点汇总归并

**词分片 term partitioning:**
全局索引 global index

横向切分, 所有文档作为一个整体, 但将词典按字母表或 Hash 切割, 每个 Shard 只负责特定词词条的完整倒排链Posting List shard0存a-c的词 shard1存e开头的词

