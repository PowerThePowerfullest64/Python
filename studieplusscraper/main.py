"""
Samlet script: logger ind på Studie+ via UniLogin, henter skema-siden for en
given uge, og parser lektierne ud til strukturerede data (JSON).

Dette er hele "scraper-modulet" fra arkitekturplanen samlet i ét script -
næste skridt er at oversætte denne logik til Kotlin/Android (WebView +
WorkManager + Room), men her kan vi nemt teste og finpudse logikken først.

Kør:
    pip install playwright beautifulsoup4
    python -m playwright install chromium
"""

import getpass
from os import path

from playwright.sync_api import sync_playwright
from pathlib import Path

# ============================================================
# OPSÆTNING - ret disse ved behov
# ============================================================

STUDIEPLUS_LOGIN_URL = "https://all.studieplus.dk/"
BILLED_URL = "https://all.studieplus.dk/shared/brugerfoto?indi_id="
BILLED_STI = "C:/GitHub/Python/studieplusscraper/billeder"

# instkey for "AARHUS TECH" (instnr 281075). Findes i login-sidens JSON-data,
# hvis du nogensinde skal skifte institution.
INSTKEY = "630064991"

# ============================================================
# DEL 1: LOGIN + HENT SKEMA-SIDENS HTML (Playwright)
# ============================================================

def log_ind(username, password):
    """
    Udfører hele login-flowet i en rigtig (headless) browser og navigerer
    til skema-siden. Returnerer den færdigrenderede HTML som en streng.
    """
    p = sync_playwright().start()
    browser = p.chromium.launch(headless=True)
    page = browser.new_page()

    print(f"[1] Går til {STUDIEPLUS_LOGIN_URL}")
    page.goto(STUDIEPLUS_LOGIN_URL)

    page.evaluate(f"selectInst('{INSTKEY}')")
    page.wait_for_timeout(500)

    print("[2] Klikker 'Unilogin og Lokalt login'")
    with page.expect_navigation(timeout=30000):
        page.click("#unilogin")

    print("[3] Vælger UniLogin som identitetsudbyder")
    with page.expect_navigation(timeout=30000):
        page.click('button[name="selectedIdp"][value="uni_idp"]')

    print("[4] Indtaster brugernavn")
    page.fill('input[type="text"]', username)
    with page.expect_navigation(timeout=30000):
        page.keyboard.press("Enter")

    print("[5] Indtaster password")
    page.fill('input[type="password"]', password)
    with page.expect_navigation(timeout=30000):
        page.keyboard.press("Enter")

    print("[6] Venter på automatisk redirect væk fra UniLogin...")
    try:
        page.wait_for_url(lambda url: "unilogin.dk" not in url, timeout=10000)
    except Exception as e:
        print(f"    Timeout/fejl mens vi ventede: {e}")
    page.wait_for_load_state("load", timeout=15000)
    page.wait_for_timeout(1000)
    print(f"    -> Endte på: {page.url}")

    if "all.studieplus.dk" not in page.url or "login" in page.url.lower():
        print("    Venter 5 sek. ekstra, ifald redirectet bare er langsomt...")
        page.wait_for_timeout(5000)
        if "all.studieplus.dk" not in page.url or "login" in page.url.lower():
            page.screenshot(path="login_fejl_screenshot.png", full_page=True)
            raise RuntimeError(
                f"Login lykkedes tilsyneladende ikke - endte på {page.url}. "
                f"Se 'login_fejl_screenshot.png' for detaljer."
            )
    print("✅ Login gennemført.")
    return page, browser, p


def hent_billede(page, browser, p, r: int = 1000):
    t = open("default_pic.jpg", "rb") # 134063
    test_body = t.read()
    t.close()
    
    for i in range(r[0], r[1]):
        print(f"Navigerer til billede side: {BILLED_URL}{i}")
        try:
            response = page.goto(f"{BILLED_URL}{i}")
            if response and response.body()[:100] != test_body[:100]:
                with open(path.join(BILLED_STI, (str(i) + ".jpg")), "wb") as f:
                    f.write(response.body())
            if response.status == 401:
                print(f"401 Error with {i}")
                page.screenshot(path=f"fejl_billede_side{i}.png", full_page=True)
        except Exception as e:
            print(f"    Navigation fejlede ({e}), venter 3 sek. og prøver igen...")
            page.wait_for_timeout(3000)
            page.goto(f"{BILLED_URL}{i}")

    page.wait_for_load_state("networkidle", timeout=30000)
    page.wait_for_timeout(3000)
    
    browser.close()
    p.stop()

# ============================================================
# HOVEDPROGRAM
# ============================================================

if __name__ == "__main__":
    import time
    username = input("UniLogin brugernavn/kode: ")
    password = getpass.getpass("UniLogin password: ")

    r = tuple
    while True:
        try:
            r = (input("Range for search int, int: ").strip().split(","))
            r[0] = int(r[0])
            r[1] = int(r[1])
            if r[0] <= 0 or r[1] <= 0 or r[0] >= r[1]: raise ValueError
            break
        except:
            print("\nPlease type 2 positive ints, first should be lower than the second!\n")
            time.sleep(1.5)

    time_start = time.time()
    
    page, browser, p = log_ind(username, password)
    hent_billede(page, browser, p, r)


    print(f"Gemt alle data i '{BILLED_STI}'")
    print(f"Finished in : {round(time.time() - time_start, 2)} seconds)")
    print(f"With a avg of time of {(time.time() - time_start) / (r[1] - r[0])} per picture")