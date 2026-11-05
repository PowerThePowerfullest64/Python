from messenger import Messenger
from getpass import getpass
import time

print("\nPlease input your Unilogin credentials (We will not steal it 😉)")
print("-----------------------------------------------------------------\n")
username = input("Username: ")
password = getpass()
print("\nLogging in...")
messenger: Messenger = Messenger(username, password)
print("Finished login.")

# Andreas: 195082
# Anders: 150480
names = {
    "andreas": 195082,
    "anders": 150480,
}
user_id_raw = input("Type receiver user-id: ").lower().strip()
try:
    user_id = int(user_id_raw)
except:
    user_id = names[user_id_raw]
    
running = True
while running:
    user_input = input("Input: ")

    if user_input == "help":
        print("'get' to collect mail from reciver\n'send' to send a message")  
    elif user_input == "get":
        read_message = messenger.handler.get_image(user_id)
        messenger.received_message(read_message)
    elif user_input == "send":
        message = input("What do you want to sent? (text only): ")
        messenger.send_message(message)
    elif user_input == "private":
        message = getpass("What do you want to sent? (text only): ")
        messenger.send_message(message)
    else:
        print("Invalid input.")

# messenger.handler.change_profile_pic(input("Name the file you would like to change pfp to: "))

messenger.handler.close()
print("Closed.")