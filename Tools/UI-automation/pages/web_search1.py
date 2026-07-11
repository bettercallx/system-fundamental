class searchPage:
    def __init__(self,page):
        self.page = page
    
    def goto(self):
        self.page.goto("https://duckduckgo.com/")
    
    def search(self, query):
        self.page.locator("input[type='text']").fill(query)
        self.page.keyboard.press("Enter")