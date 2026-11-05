import numpy as np
import cv2
from time import time

import math

class InputSizeError(Exception):
    pass

class TooLargeJpgError(Exception):
    """Either the jpg is to tall or wide"""
    pass

def bytes_to_bits(bytes: bytes) -> str:
    return  "".join([f"{bytes[i]:08b}" for i in range(len(bytes))])

def write_image(message: bytes, output_path: str, width: int, height: int = None, chunk_size: int = 2, do_print: bool = False): #🤫 dont tell studie+
    start = time()

    if width % 2 != 0:
        raise InputSizeError("Width cannot be an odd number")

    MAGIC_HEADER = "01010100111100011011100101101010"
    LENGTH_BIT_COUNT = 64

    bits = bytes_to_bits(message)
    bit_length = len(MAGIC_HEADER) + LENGTH_BIT_COUNT + len(bits)
    length_bits = f"{bit_length:0{LENGTH_BIT_COUNT}b}"
    bits = MAGIC_HEADER + length_bits + bits

    if do_print:
        print(f"Calculated bit length of data to {bit_length} bits")

    if height == None:
        height = int(math.ceil(len(bits) / width * chunk_size) * chunk_size)

    if height > 2**16:
        raise TooLargeJpgError(f"Height of the JPG was over the limit of {2**16} pixels ({height} pixels)")
    if width > 2**16:
        raise TooLargeJpgError(f"Width of the JPG was over the limit of {2**16} pixels ({width} pixels)")

    if do_print:
        print(f"Image resolution is {width}x{height} pixels")
    
    image = np.zeros((height, width, 3), dtype=np.uint8)
    image[:] = (0, 0, 0)

    bits_width = width // chunk_size
    bits_height = height // chunk_size
    bit_capacity = bits_width * bits_height

    if do_print:
        print(f"Bit resolution is {bits_width}x{bits_height} bits")

    if len(bits) > bit_capacity:
        raise InputSizeError(f"Input file too big for image. Image supports only {bit_capacity} bits but was given {len(bits)} bits")

    if do_print:
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
            if do_print and bit_index % (bit_length // 100) == 0 and bit_length > 10000:
                print(f"Written {round(bit_index/bit_length * 100)}% bits")
    
    
    cv2.imwrite(output_path, image, [cv2.IMWRITE_JPEG_QUALITY, 5])

    duration = time() - start
    if do_print:
        print(f"Image successfully encoded {bit_length} bits in {duration} s.")

if __name__ == "__main__":
    input_path = input("File to encode: ")
    output_path = input("Encoded filename: ")
    if output_path.split('.')[-1] != "jpg":
        raise SyntaxError("Encoded filename must end in .jpg")
    
    file = open(input_path, "rb")
    data = file.read()
    write_image(message=data, output_path=output_path, width=2500, chunk_size=2)