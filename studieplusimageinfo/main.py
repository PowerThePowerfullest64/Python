import numpy as np
import cv2
from time import time

import math

class InputSizeError(Exception):
    pass

def str_to_bits(message: str) -> str:
    """Turns a string into a string of 0 and 1"""
    bytes = message.encode()
    
    return bytes_to_bits(bytes)

def bytes_to_bits(bytes: bytes) -> str:
    bits = ""
    for i in range(len(bytes)):
        bits += f"{bytes[i]:08b}"
    return bits

def bits_to_str(bits: str) -> str:
    """""Turns bits into string"""
    text = ""
    for i in range(len(bits)//8):
        ch = chr(int(bits[i*8:(i+1)*8], 2))
        text += ch
    return text
        

def write_image(message: bytes, output_path: str, width: int, height: int = None, chunk_size: int = 4): #🤫 dont tell studie+
    start = time()

    MAGIC_HEADER = "01010100111100011011100101101010"
    LENGTH_BIT_COUNT = 64

    bits = bytes_to_bits(message)
    bit_length = len(MAGIC_HEADER) + LENGTH_BIT_COUNT + len(bits)
    length_bits = f"{bit_length:0{LENGTH_BIT_COUNT}b}"
    bits = MAGIC_HEADER + length_bits + bits

    if height == None:
        height = int(math.ceil(len(bits) / width * chunk_size) * chunk_size)
    
    image = np.zeros((height, width, 3), dtype=np.uint8)
    image[:] = (0, 0, 0)

    bits_width = width // chunk_size
    bits_height = height // chunk_size
    bit_capacity = bits_width * bits_height

    if len(bits) > bit_capacity:
        raise InputSizeError(f"Input file too big for image. Image supports only {bit_capacity} bits but was given {len(bits)} bits")

    print(f"Image bit-capacity is {bit_capacity} bits.")
    print(f"Message size is {len(bits)} bits.")

    bit_index = 0
    for by in range(bits_height):
        if bit_index >= len(bits):
            break
        for bx in range(bits_width):
            if bit_index >= len(bits):
                break

            if bits[bit_index] == '1':
                px, py = bx * chunk_size, by * chunk_size
                rows = image[py:py+chunk_size]
                for row in rows:
                    row[px:px+chunk_size] = (255,255,255)

            bit_index += 1
    
    
    cv2.imwrite(output_path, image, [cv2.IMWRITE_JPEG_QUALITY, 50])

    duration = time() - start
    print(f"Image successfully encoded in {duration} s.")
        
if __name__ == "__main__":
    file = open("catclapping.mp4", "rb")
    message = file.read()
    file.close()
    write_image(message, width=500, output_path="encoded.jpg", chunk_size=2)