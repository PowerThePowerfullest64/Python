import numpy as np
import cv2
from time import time

import math

class InputSizeError(Exception):
    pass

class TooLargeJpgError(Exception):
    """Either the jpg is too tall or wide"""
    pass

MAGIC_HEADER = "01010100111100011011100101101010"
LENGTH_BIT_COUNT = 64

def write_image(message: bytes, output_path: str, width: int, height: int = None, chunk_size: int = 2, do_print: bool = False): #🤫 dont tell studie+
    start = time()

    if width % 2 != 0:
        raise InputSizeError("Width cannot be an odd number")

    header_bits = np.frombuffer(MAGIC_HEADER.encode(), dtype=np.uint8) - ord('0')

    msg_bits = np.unpackbits(np.frombuffer(message, dtype=np.uint8))

    bit_length = len(header_bits) + LENGTH_BIT_COUNT + len(msg_bits)

    length_bits = np.unpackbits(np.frombuffer(bit_length.to_bytes(8, byteorder="big"), dtype=np.uint8))

    bits = np.concatenate([header_bits, length_bits, msg_bits])

    if do_print:
        print(f"Calculated bit length of data to {bit_length} bits")

    if height is None:
        height = int(math.ceil(len(bits) / width * chunk_size) * chunk_size)

    if height > 2**16:
        raise TooLargeJpgError(f"Height of the JPG was over the limit of {2**16} pixels ({height} pixels)")
    if width > 2**16:
        raise TooLargeJpgError(f"Width of the JPG was over the limit of {2**16} pixels ({width} pixels)")

    bits_width = width // chunk_size
    bits_height = height // chunk_size
    bit_capacity = bits_width * bits_height

    if do_print:
        print(f"Image resolution is {width}x{height} pixels")
        print(f"Bit resolution is {bits_width}x{bits_height} bits")
        print(f"Image bit-capacity is {bit_capacity} bits.")
        print(f"Message size is {len(bits)} bits.")

    if len(bits) > bit_capacity:
        raise InputSizeError(f"Input file too big for image. Image supports only {bit_capacity} bits but was given {len(bits)} bits")

    grid = np.zeros(bit_capacity, dtype=np.uint8)
    grid[:len(bits)] = bits
    grid = grid.reshape(bits_height, bits_width) * 255

    gray = np.kron(grid, np.ones((chunk_size, chunk_size), dtype=np.uint8))

    if gray.shape != (height, width):
        full = np.zeros((height, width), dtype=np.uint8)
        full[:gray.shape[0], :gray.shape[1]] = gray
        gray = full

    image = cv2.merge([gray, gray, gray])

    cv2.imwrite(output_path, image, [cv2.IMWRITE_JPEG_QUALITY, 5])

    if do_print:
        print(f"Image successfully encoded {bit_length} bits in {time() - start} s.")

if __name__ == "__main__":
    input_path = input("File to encode: ")
    output_path = input("Encoded filename: ")
    if output_path.split('.')[-1] != "jpg":
        raise SyntaxError("Encoded filename must end in .jpg")
    
    file = open(input_path, "rb")
    data = file.read()
    write_image(message=data, output_path=output_path, width=2500, chunk_size=2, do_print=True)