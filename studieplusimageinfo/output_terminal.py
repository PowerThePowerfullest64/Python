import sys, time, os
from messenger import Messenger

package: str
if len(sys.argv) > 1:
    package = sys.argv[1]
else:
    print("Output terminal did not receive a receiver id as an argument.")
    time.sleep(10)
    raise ValueError("Output terminal did not receive a receiver id as an argument.")

args = package.split(',')
receiver_id: int = int(args[0])
username: str = args[1]
password: str = args[2]

print("Logging in...")
messenger = Messenger(username, password)
print("Finished loggin in.")

time.sleep(0.75)
os.system("clear") # MAKE CLEAR A THING!

print("Output terminal for messages.")
print(f"Messages from the id '{receiver_id}' will be received here.\n")

while True:
    # messenger.handler.get_image()
    # messenger.received_message()
    pass