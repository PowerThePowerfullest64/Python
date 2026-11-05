from playwright.sync_api import sync_playwright, Browser, Page, Playwright

class StudiePlusHandler:
    def __init__(self, username: str, password: str):
        self.STUDIEPLUS_LOGIN_URL = "https://all.studieplus.dk/"
        self.USER_URL = "https://all.studieplus.dk/brugerprofil/"
        self.BILLED_URL = "https://all.studieplus.dk/shared/brugerfoto?indi_id="
        self.INSTKEY = "630064991" #For aarhus tech

        self.do_print = False

        self.p: Playwright = sync_playwright().start()
        self.browser: Browser = self.p.chromium.launch(headless=True)
        self.user_page: Page = self.log_ind(username, password)
        self.receiver_page: Page = self.log_ind(username, password)

        

    def log_ind(self, username, password) -> Page:
        """
        Udfører hele login-flowet i en rigtig (headless) browser og navigerer
        til skema-siden. Returnerer den færdigrenderede HTML som en streng.
        """
        user_page = self.browser.new_page()

        if self.do_print:
            print(f"[1] Går til {self.STUDIEPLUS_LOGIN_URL}")
        user_page.goto(self.STUDIEPLUS_LOGIN_URL)

        user_page.evaluate(f"selectInst('{self.INSTKEY}')")
        user_page.wait_for_timeout(500)

        if self.do_print:
            print("[2] Klikker 'Unilogin og Lokalt login'")
        with user_page.expect_navigation(timeout=30000):
            user_page.click("#unilogin")

        if self.do_print:
            print("[3] Vælger UniLogin som identitetsudbyder")
        with user_page.expect_navigation(timeout=30000):
            user_page.click('button[name="selectedIdp"][value="uni_idp"]')

        if self.do_print:
            print("[4] Indtaster brugernavn")
        user_page.fill('input[type="text"]', username)
        with user_page.expect_navigation(timeout=30000):
            user_page.keyboard.press("Enter")

        if self.do_print:
            print("[5] Indtaster password")
        user_page.fill('input[type="password"]', password)
        with user_page.expect_navigation(timeout=30000):
            user_page.keyboard.press("Enter")
            

        if self.do_print:
            print("[6] Venter på automatisk redirect væk fra UniLogin...")
        try:
            user_page.wait_for_url(lambda url: "unilogin.dk" not in url, timeout=10000)
        except Exception as e:
            if self.do_print:
                print(f"    Timeout/fejl mens vi ventede: {e}")
        user_page.wait_for_load_state("load", timeout=15000)
        user_page.wait_for_timeout(1000)
        if self.do_print:
            print(f"    -> Endte på: {user_page.url}")

        if "all.studieplus.dk" not in user_page.url or "login" in user_page.url.lower():
            print("    Venter 5 sek. ekstra, ifald redirectet bare er langsomt...")
            user_page.wait_for_timeout(5000)
            if "all.studieplus.dk" not in self.receiver_page.url or "login" in user_page.url.lower():
                user_page.screenshot(path="login_fejl_screenshot.png", full_page=True)
                raise RuntimeError(
                    f"Login lykkedes tilsyneladende ikke - endte på {user_page.url}. "
                    f"Se 'login_fejl_screenshot.png' for detaljer."
                )
        if self.do_print:
            print("✅ Login gennemført.")

        user_page.goto(self.USER_URL)
        return user_page
    
    def get_image(self, receiver_id: int):
        read_bytes: bytes = bytes()
        if self.do_print:
            print(f"Navigerer til billede side: {self.BILLED_URL}{receiver_id}")
        try:
            response = self.receiver_page.goto(f"{self.BILLED_URL}{receiver_id}")
            read_bytes = response.body()
            
            if response.status == 401:
                if self.do_print:
                    print(f"401 Error with {receiver_id}")
                    
                self.receiver_page.screenshot(path=f"fejl_billede_side{receiver_id}.png", full_page=True)

        except Exception as e:
            print(f"    Navigation fejlede ({e}), venter 3 sek. og prøver igen...")
            self.receiver_page.wait_for_timeout(3000)
            self.receiver_page.goto(f"{self.BILLED_URL}{receiver_id}")
        
        return read_bytes

    def change_profile_pic(self, path: str):
        with self.user_page.expect_file_chooser() as fc_info:
            self.user_page.get_by_role("button", name="Skift billede").click()

        file_chooser = fc_info.value
        file_chooser.set_files(path)
        #self.user_page.screenshot(path="GGS.png", full_page=True)
    
    def close(self):
        self.browser.close()
        self.p.stop()
