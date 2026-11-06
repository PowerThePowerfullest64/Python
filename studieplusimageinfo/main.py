from messenger import Messenger
from getpass import getpass
import subprocess, sys
import time

print("\nPlease input your Unilogin credentials (We will not steal it 😉)")
print("-----------------------------------------------------------------\n")
username = input("Username: ")
password = getpass()

names = {
    "andreas": 195082,
    "anders": 150480,
    "christian": 95222
}

while True: 
    user_id_raw = input("Type receiver user-id: ").lower().strip()
    try:
        user_id = int(user_id_raw)
    except:
        user_id = names[user_id_raw]
    if user_id > 0:
        break
seprator = ",@31_-x"
package = f"{user_id}{seprator}{username}{seprator}{password}"

# create a new window, for output
subprocess.Popen([sys.executable, "input_terminal.py", package], creationflags=subprocess.CREATE_NEW_CONSOLE)
time.sleep(2) # open this one a little later

