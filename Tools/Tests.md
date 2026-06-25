# break down requirements & implement

---

## scripts

数据处理 — pandas读CSV/JSON、清洗数据、做统计。Text-to-SQL项目的evaluation结果分析会用到。
自动化测试 — Playwright&pytest、Selenium(浏览器自动化)
API调用 — requests库调REST API、或者用SDK（比如Anthropic的Claude SDK）。批量调模型、收集结果
脚本工具 — os、subprocess、shutil做文件操作，argparse做命令行参数。部署脚本、批量重命名、日志处理这类

### 1. 网络爬虫与数据采集脚本 (Web Scraping)

* **做什么：** 自动打开网页，抓取商品价格、新闻、电影评分等数据并保存到数据库或 Excel 中
* **特点：** 偏向于“只读”和大量并发

### 2. UI 自动化测试与 RPA 脚本 (UI Automation / Robotic Process Automation)

* **做什么：** 模拟真人去“点点点”。比如：自动登录、自动填写复杂的表单、点击按钮、截图留存、验证页面跳转是否正确。
* **特点：** Playwright 不仅能做测试，还能做 RPA（比如帮你每天自动登录某个网站签到、抢票）。

### 3. API 自动化测试与接口脚本 (API Testing)

* **做什么：** 不通过浏览器界面，直接向服务器的后端接口（比如 `/api/v1/login`）发送请求，验证返回的 JSON 数据是否正确。
* **特点：** 速度快（没有浏览器渲染的开销），是后端开发和测试最常用的脚本。

### 4. 运维与自动化脚本 (DevOps & Scripting)

* **做什么：** 批量修改本地文件名、定时备份数据库、监控服务器 CPU 占用、自动发送告警邮件等。
* **特点：** 替代日常枯燥的系统管理手工劳动。

一个成熟的自动化项目通常需要以下几个核心库的配合：

### 1. Pytest —— 测试框架

Playwright 负责点击、输入，而 Pytest 负责**组织和管理**，决定哪些脚本先跑，哪些后跑。 `assert`（断言）功能，比如 `assert title == "首页"`，用来判断测试是成功还是失败。失败时自动截图、重试。

### 2. Requests —— 接口测试

UI 自动化之外，实际工作中，很多前置步骤（比如造测试数据、或者一键清理账户状态）如果全靠浏览器点点点会慢死。这时候需要直接发 API 请求。发送 GET、POST 请求，处理 Cookie 和 Session。

### 3. Loguru —— 日志管理

自动化脚本经常在半夜无人值守时运行，如果报错了，你需要知道它死在哪里。替代 Python 自带的死板日志库，一行代码就能输出带有时间、颜色、报错行数的日志，并自动保存到文件。

### 4. Pydantic / Interrogate —— 配置与数据校验

当测试脚本需要读取外部的配置文件（比如 `config.yaml` 或 `config.json`，里面写着不同环境的 URL 和账号）时，用它来确保配置数据没有传错。

工作流是这样的：

1. **项目启动：** Python 脚本跑起来，首先通过 **内置的 `os` / `json` 库** 读取你的配置文件。
2. **初始化：** **Pytest** 接管控制权，拉起 **Playwright** 打开浏览器。
3. **前置准备：** 脚本用 **Requests** 发送了一个 API 请求，在后台快速创建了一个测试账号（省去了在界面上注册的时间）。
4. **执行测试：** **Playwright** 模拟真人在浏览器里疯狂点点点，期间 **Loguru** 在控制台不断打印出：`[INFO] 11:20:05 - 正在点击购买按钮...`
5. **断言与收尾：** 检查页面是否弹出了“购买成功”。如果没弹出，**Pytest** 触发报错，**Playwright** 立刻截一张报错图存到本地，最后关闭浏览器。

---

## 页面对象模式POM Page Object Pattern

自动化测试里经典设计模式，将“页面元素”和“测试逻辑”剥离。

* **页面对象层（Pages）：** 每一个网页对应 Python 里的一个类（Class）。
* 这个类里只存放两样东西：
1. 这个页面上的**元素在哪里**（比如：登录按钮的 ID、输入框的 Class）。
2. 这个页面能干什么**动作**（比如：`login()` 动作、`search_product()` 动作）。

* **测试用例层（Tests）：** 真正的测试逻辑，调用 Page 类提供的方法。

---

### 使用 POM 分层管理的示例

假设要测试“登录功能”：

第一步：在 `pages/login_page.py` 里封装页面：

```python
class LoginPage:
    def __init__(self, page):
        self.page = page
        # 把元素定位统一管起来
        self.username_input = "#input_user"
        self.password_input = "#input_password"
        self.submit_button = ".btn-submit-login-form"

    def navigate(self):
        self.page.goto("https://example.com/login")

    def login(self, username, password):
        self.page.fill(self.username_input, username)
        self.page.fill(self.password_input, password)
        self.page.click(self.submit_button)

```

第二步：在 `tests/test_login.py` 里写测试用例：

```python
from pages.login_page import LoginPage

def test_login_success(page):
    login_page = LoginPage(page)
    login_page.navigate()
    
    # 测试用例不关心底层怎么定位按钮
    login_page.login("my_username", "my_password") 
    assert page.locator(".welcome-msg").is_visible()

```

---

## 拆解需求

1. 用户是谁？用来干什么？
2. 用户从头到尾会做什么？按时间顺序列出使用流程
3. 流程中对应的技术是什么？读数据/调用api/存结果/展示界面

**case**: 设计一个自动化工具去测试agent的对话、评估reasoning和acting

1. 谁用、干嘛用？团队的工程师用，目的是批量验证agent改了代码之后有没有变差。
2. 使用流程是什么？准备一批测试问题 → 自动逐条发给agent → 收集agent的回答 → 跟预期答案对比 → 输出一个报告说哪些过了哪些挂了。
3. 每步背后需要什么？测试问题需要一个存case的文件（JSON或CSV）；发给agent需要调API或者用Playwright操作网页；对比需要定义评估标准（精确匹配、LLM-as-judge）；报告可以先是个终端输出或CSV。
4. 先做什么？先让一条case跑通整个流程，再批量，再加评估维度，再做报告

**case:** Text-to-SQL 

