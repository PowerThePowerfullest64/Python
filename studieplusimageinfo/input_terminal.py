import sys, time, os
from messenger import Messenger

package: str
if len(sys.argv) > 1:
    package = sys.argv[1]
else:
    print("Input terminal did not receive a receiver id as an argument.")
    time.sleep(10)
    raise ValueError("Input terminal did not receive a receiver id as an argument.")

args = package.split(",@31_-x")
receiver_id: int = int(args[0])
username: str = args[1]
password: str = args[2]

print("Logging in...")
messenger = Messenger(username, password)
print("Finished loggin in.")

time.sleep(1)
os.system("cls") # MAKE CLEAR A THING!
os.system("title " + "input")

print("Input terminal for messages.\n")

start_time = 0
while True:
    user_input = input("Input: ")

    messenger.send_message(user_input.encode())
    