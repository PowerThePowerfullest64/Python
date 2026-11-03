import numpy as np
import cv2
from time import time

import math

class NoHeaderError(Exception):
    pass

class InvalidLengthError(Exception):
    pass

class InputSizeError(Exception):
    pass

def str_to_bits(message: str) -> str:
    """Turns a string into a string of 0 and 1"""
    bytes = message.encode()
    bits = ""
    for i in range(len(bytes)):
        bits += f"{bytes[i]:08b}"
    return bits

def write_image(message: str, output_path: str, width: int, height: int = None, chunk_size: int = 4): #🤫 dont tell studie+
    start = time()

    MAGIC_HEADER = "01010100111100011011100101101010"
    LENGTH_BIT_COUNT = 64

    bits = str_to_bits(message)
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

def read_image(input_path: str, output_path: str, chunk_size: int = 4) -> str:
    start = time()

    MAGIC_HEADER = "01010100111100011011100101101010"
    LENGTH_BIT_COUNT = 64

    image = cv2.imread(input_path)
    if image is None:
        raise FileNotFoundError(f"Could not read image: {input_path}")

    height, width, _ = image.shape

    bits_width = width // chunk_size
    bits_height = height // chunk_size

    total_color = np.zeros(3)
    text = ""
    bits = ""
    for by in range(bits_height):
        for bx in range(bits_width):
            total_color = np.zeros(3)

            px, py = bx * chunk_size, by * chunk_size
            rows = image[py:py+chunk_size]
            for row in rows:
                for i in row[px:px+chunk_size]:
                    total_color += i
            avg_color = total_color / chunk_size**2
            brightness = (avg_color[0] + avg_color[1] + avg_color[2]) / 3
            bits += '1' if brightness >= 128 else '0'

    if bits[:len(MAGIC_HEADER)] != MAGIC_HEADER:
        raise NoHeaderError("Input image file has no header.")

    length_bits = bits[len(MAGIC_HEADER):len(MAGIC_HEADER) + LENGTH_BIT_COUNT]
    bit_length = int(length_bits, 2)
    if bit_length < len(MAGIC_HEADER) or bit_length > len(bits):
        raise InvalidLengthError("Image input file did not specify its length correctly.")

    bits = bits[len(MAGIC_HEADER) + LENGTH_BIT_COUNT:bit_length]

    with open(output_path, "w") as f:
        f.write(bits)

    duration = time() - start
    print(f"Image successfully decoded with length of {bit_length//8} bytes in {duration} s.")
        
if __name__ == "__main__":
    file = open("input.txt", "r", encoding="utf-8")
    message = file.read()
    file.close()
    write_image(message, width=8000, output_path="encoded2.jpg", chunk_size=2)
    read_image("encoded2.jpg", "decoded.txt", chunk_size=2)