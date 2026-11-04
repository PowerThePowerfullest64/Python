from time import time
import cv2
import numpy as np

class NoHeaderError(Exception):
    pass

class InvalidLengthError(Exception):
    pass

def bits_to_bytes(bits: str) -> bytes:
    """Turns bits into bytes."""
    limit = (len(bits) // 8) * 8
    return bytes(int(bits[i:i+8], 2) for i in range(0, limit, 8))

def read_image(input_path: str, output_path: str, chunk_size: int = 4):
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
    bits = ""
    for by in range(bits_height): #Make ts faster (todo/fix)
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
        if by % 500 == 0:
            print(f"Lines already read: {by}/{bits_height}")
    print(f"Lines already read: {bits_height}/{bits_height}")
    if bits[:len(MAGIC_HEADER)] != MAGIC_HEADER:
        raise NoHeaderError("Input image file has no header.")

    length_bits = bits[len(MAGIC_HEADER):len(MAGIC_HEADER) + LENGTH_BIT_COUNT]
    bit_length = int(length_bits, 2)
    if bit_length < len(MAGIC_HEADER) or bit_length > len(bits):
        raise InvalidLengthError("Image input file did not specify its length correctly.")

    bits = bits[len(MAGIC_HEADER) + LENGTH_BIT_COUNT:bit_length]

    byte_data = bits_to_bytes(bits)

    print(f"Beginning to write bytes to {output_path}")
    with open(output_path, "wb") as f:
        f.write(byte_data)

    duration = time() - start
    print(f"Image successfully decoded with length of {len(bits)//8} bytes in {duration} s to '{output_path}'")

if __name__ == "__main__":
    input_path = input("File to decode: ")
    if input_path.split('.')[-1] != "jpg":
        raise SyntaxError("Decoded file must end in .jpg")
    output_path = input("Decoded filename: ")

    read_image(input_path=input_path, output_path=output_path, chunk_size=2)
