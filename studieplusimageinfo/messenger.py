from decoder import read_image
from encoder import write_image
import studieplushandler as sph
import os
import random
import time

class Messenger:
    def __init__(self, username: str, password: str):

        self.handler: sph.StudiePlusHandler = sph.StudiePlusHandler(username, password)

        self.last_encoded = ""

    def send_message(self, message: bytes):
        current_time = int(time.time())
        encoded_time = current_time.to_bytes(byteorder="little", length=4)
        
        message = encoded_time + message

        tmp_filename = f"tmp_message{random.randint(0, 2147483647)}.jpg"
        # encode to a temporary file before sending it
        write_image(message=message, output_path=tmp_filename, width=500)
        
        self.handler.change_profile_pic(tmp_filename)

        os.remove(tmp_filename)

    def received_message(self, encoded: bytes) -> tuple[str, int] | None:
        if encoded == self.last_encoded or encoded is None:
            return
        self.last_encoded = encoded
        
        tmp_filename = f"tmp_encoded{random.randint(0, 2147483647)}.jpg"
        with open(tmp_filename, "wb") as f:
            f.write(encoded)
        
        decoded = read_image(tmp_filename, make_file=False)
        if decoded is None:
            os.remove(tmp_filename)
            return
        decoded_time = int.from_bytes(decoded[:4], byteorder="little")
        decoded = decoded[4:]

        os.remove(tmp_filename)

        return decoded.decode(), decoded_time
    
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
