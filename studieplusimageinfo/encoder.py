import numpy as np
import cv2
from time import time

import math

class InputSizeError(Exception):
    pass

def bytes_to_bits(bytes: bytes) -> str:
    bits = ""
    for i in range(len(bytes)):
        bits += f"{bytes[i]:08b}"
    return bits      

def write_image(message: bytes, output_path: str, width: int, height: int = None, chunk_size: int = 4): #🤫 dont tell studie+
    start = time()

    MAGIC_HEADER = "01010100111100011011100101101010"
    LENGTH_BIT_COUNT = 64

    bits = bytes_to_bits(message)
    bit_length = len(MAGIC_HEADER) + LENGTH_BIT_COUNT + len(bits)
    length_bits = f"{bit_length:0{LENGTH_BIT_COUNT}b}"
    bits = MAGIC_HEADER + length_bits + bits

    print(f"Calculated bit length of {bit_length} bits")

    if height == None:
        height = int(math.ceil(len(bits) / width * chunk_size) * chunk_size)

    print(f"Image resolution is {width}x{height} pixels")
    
    image = np.zeros((height, width, 3), dtype=np.uint8)
    image[:] = (0, 0, 0)

    bits_width = width // chunk_size
    bits_height = height // chunk_size
    bit_capacity = bits_width * bits_height

    print(f"Bit resolution is {bits_width}x{bits_height} bits")

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

            if bit_index % 10000 == 0:
                print(f"Written {bit_index+1}/{bit_length} bits")
    
    
    cv2.imwrite(output_path, image, [cv2.IMWRITE_JPEG_QUALITY, 50])

    duration = time() - start
    print(f"Image successfully encoded {bit_length} bits in {duration} s.")

if __name__ == "__main__":
    file = open("billeder.zip", "rb")
    data = file.read()
    write_image(message=data, output_path="encoded.jpg", width=16000, chunk_size=2)