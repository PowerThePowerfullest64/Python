from decoder import read_image
from encoder import write_image
import studieplushandler as sph
import os
import random

class Messenger:
    def __init__(self, username: str, password: str):
        self.handler: sph.StudiePlusHandler = sph.StudiePlusHandler(username, password)

    def send_message(self, message: str):
        tmp_filename = f"tmp_message{random.randint(0, 2147483647)}.jpg"
        # encode to a temporary file before sending it
        write_image(message=message.encode(), output_path=tmp_filename, width=500)
        
        self.handler.change_profile_pic(tmp_filename)

        os.remove(tmp_filename)

    def received_message(self, encoded_image: bytes):
        tmp_filename = f"tmp_encoded{random.randint(0, 2147483647)}.jpg"
        with open(tmp_filename, "wb") as f:
            f.write(encoded_image)
        decoded = read_image(tmp_filename, make_file=False)

        print(f"You received message:\n{decoded.decode()}")
        
        os.remove(tmp_filename)
    
    def detect_file_send(self):
        if self.message[:5] == 'file"':
            try:
                self.send_file(self.message[5:-1])
            except FileNotFoundError:
                # display(from="Error", message=f"{self.message[5:-1]} is not found")
                # raise NotImplementedError
                print("Name wrong")
    
    def send_file(self):
        pass
