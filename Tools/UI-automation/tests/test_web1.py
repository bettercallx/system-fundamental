import re
import json
from pathlib import Path
from playwright.sync_api import Page, expect
from pages.web_search1 import searchPage

def test_search(page: Page):
    page.goto("https://google.com/")
    #page.screenshot(path="debug.png")
    #option+cmd+c dev mode查看

    
    page.locator("textarea[name='q']").fill("github")
    #page.get_by_role("textbox").fill("github")
    page.keyboard.press("Enter")
    # re.compile(xxx) 就是模糊的正则匹配
    expect(page).to_have_title(re.compile(r".*github"))

# 读取json文件，逐行搜索，关于找不到json文件，需要用Path __file__代表这个文件在的目录
json_path = Path(__file__).parent / 'test_web1.json'
with open(json_path,'r') as f:
    test_data = json.load(f)

# json文件中[]在python是list 里面每个{...}是一个dict json.load 拿到list of dicts
def test_search_multi(page: Page):
    search_multi = searchPage(page)
    results = []
    for item in test_data:
        search_multi.goto()
        search_multi.search(item["query"])
        try:

            #1.f表示非字面文字 {}插入变量 2.避免引号冲突" '' "
            expect(page).to_have_title(re.compile(f".*{item['expected_title']}", re.IGNORECASE))
            results.append({"query": item["query"],"status":"PASS"})
        except AssertionError as e:
            results.append({"query": item["query"],"status":"FAIL","error":str(e)})
        for r in results:
            print(f"{r['status']}: {r['query']}")
        assert all(r["status"] == "PASS" for r in results) 
        

    

