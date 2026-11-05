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

time.sleep(1)
os.system("cls")

print("Output terminal for messages.")
print(f"Messages from the id '{receiver_id}' will be received here.\n")

start_time = 0
while True:
    if time.time() - start_time >= 1:
        encoded = messenger.handler.get_image(receiver_id)
        result = messenger.received_message(encoded)
        if result is not None:
            message, message_time = result

            seconds_day = message_time % (3600 * 24)
            hours = seconds_day // 3600 + 1 # + 1 is to offset to UTC+1
            minutes = seconds_day % 3600 // 60
            seconds = seconds_day % 3600 % 60

            print(f"From {receiver_id} ({hours:02d}:{minutes:02d}:{seconds:02d}): {message}")
        
        start_time = time.time()