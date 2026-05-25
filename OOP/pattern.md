**1. 工厂方法 (Factory Method)**

*意图*:把"创建哪个具体类"的决定延后到运行时或子类。
*场景*:根据配置/参数创建不同子类对象,调用方不想知道具体类型。

```cpp
class Shape {
public:
    virtual void draw() = 0;
    virtual ~Shape() = default;
};
class Circle : public Shape { void draw() override { /*...*/ } };
class Square : public Shape { void draw() override { /*...*/ } };

std::unique_ptr<Shape> createShape(const std::string& type) {
    if (type == "circle") return std::make_unique<Circle>();
    if (type == "square") return std::make_unique<Square>();
    return nullptr;
}
```

---

**2. 建造者 (Builder)**

*意图*:把复杂对象的构造过程和它的表示分离。
*场景*:对象有十几个可选参数,构造函数参数列表爆炸。

```cpp
class HttpRequest {
public:
    class Builder {
        HttpRequest req;
    public:
        Builder& url(std::string u) { req.url_ = std::move(u); return *this; }
        Builder& method(std::string m) { req.method_ = std::move(m); return *this; }
        Builder& header(std::string k, std::string v) {
            req.headers_[k] = v; return *this;
        }
        HttpRequest build() { return std::move(req); }
    };
private:
    std::string url_, method_;
    std::map<std::string, std::string> headers_;
};

// 用法: auto r = HttpRequest::Builder().url("...").method("GET").build();
```

---

**3. 适配器 (Adapter)**

*意图*:把一个接口转成客户端期望的另一个接口。
*场景*:接入第三方库,它的接口和你系统的接口对不上。

```cpp
class OldPrinter {                       // 旧接口
public:
    void printText(const char* s) { /*...*/ }
};

class Printer {                          // 新接口
public:
    virtual void print(const std::string& s) = 0;
};

class PrinterAdapter : public Printer {  // 适配
    OldPrinter old_;
public:
    void print(const std::string& s) override { old_.printText(s.c_str()); }
};
```

---

**4. 装饰器 (Decorator)**

*意图*:动态地给对象加功能,不用继承爆炸。
*场景*:I/O 流的缓冲、加密、压缩可任意叠加。

```cpp
class Stream {
public:
    virtual void write(const std::string& s) = 0;
    virtual ~Stream() = default;
};

class FileStream : public Stream {
    void write(const std::string& s) override { /* 写文件 */ }
};

class EncryptedStream : public Stream {
    std::unique_ptr<Stream> inner_;
public:
    EncryptedStream(std::unique_ptr<Stream> s) : inner_(std::move(s)) {}
    void write(const std::string& s) override {
        inner_->write(encrypt(s));
    }
    std::string encrypt(const std::string& s) { /*...*/ return s; }
};

// 用法: EncryptedStream(std::make_unique<FileStream>()).write("hi");
```

---

**5. 代理 (Proxy)**

*意图*:为对象提供替身,控制访问。
*场景*:延迟加载、权限检查、远程调用。

```cpp
class Image {
public:
    virtual void display() = 0;
    virtual ~Image() = default;
};

class RealImage : public Image {
    std::string filename_;
public:
    RealImage(std::string f) : filename_(std::move(f)) { /* 加载,慢 */ }
    void display() override { /*...*/ }
};

class ImageProxy : public Image {        // 延迟到真正用时才加载
    std::string filename_;
    std::unique_ptr<RealImage> real_;
public:
    ImageProxy(std::string f) : filename_(std::move(f)) {}
    void display() override {
        if (!real_) real_ = std::make_unique<RealImage>(filename_);
        real_->display();
    }
};
```

---

**6. 观察者 (Observer)**

*意图*:一对多依赖,被观察者变化时通知所有观察者。
*场景*:事件系统、GUI、发布订阅。

```cpp
class Observer {
public:
    virtual void onEvent(int value) = 0;
    virtual ~Observer() = default;
};

class Subject {
    std::vector<Observer*> observers_;
public:
    void attach(Observer* o) { observers_.push_back(o); }
    void notify(int v) { for (auto* o : observers_) o->onEvent(v); }
};
```

---

**7. 策略 (Strategy)**

*意图*:把一族算法封装起来,可以互换。
*场景*:同一个流程里,某一步有多种算法可选(排序比较器、压缩算法、定价策略)。

```cpp
class CompressStrategy {
public:
    virtual std::string compress(const std::string& data) = 0;
    virtual ~CompressStrategy() = default;
};

class ZipStrategy : public CompressStrategy { /*...*/ };
class GzipStrategy : public CompressStrategy { /*...*/ };

class Compressor {
    std::unique_ptr<CompressStrategy> s_;
public:
    Compressor(std::unique_ptr<CompressStrategy> s) : s_(std::move(s)) {}
    std::string run(const std::string& d) { return s_->compress(d); }
};
```

C++ 里也可以直接用 `std::function` 实现,不一定非要继承体系。

---

**8. 模板方法 (Template Method)**

*意图*:在基类里定义算法骨架,把可变步骤留给子类。
*场景*:流程固定但某几步实现不同(测试框架的 setUp/test/tearDown)。

```cpp
class DataProcessor {
public:
    void process() {           // 骨架,不让子类改
        read();
        transform();           // 子类定制
        write();
    }
protected:
    virtual void read() { /* 默认 */ }
    virtual void transform() = 0;
    virtual void write() { /* 默认 */ }
};

class JsonProcessor : public DataProcessor {
    void transform() override { /* 处理 JSON */ }
};
```
