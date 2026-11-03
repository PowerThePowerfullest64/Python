import numpy as np
import cv2

import math

def str_to_bits(message: str) -> str:
    """Turns a string into a string of 0 and 1"""
    bytes = message.encode()
    bits = ""
    for i in range(len(bytes)):
        bits += f"{bytes[i]:08b}"
    return bits

def write_image(message: str, output_path: str, width: int, height: int = None, chunk_size: int = 4): #🤫 dont tell studie+
    IMAGE_END_COLOR = (0, 255, 0)

    bits = str_to_bits(message)

    if height == None:
        height = int(math.ceil(len(bits) / width * chunk_size) * chunk_size)
    
    image = np.zeros((height, width, 3), dtype=np.uint8)
    image[:] = (0, 0, 0)

    bits_width = width // chunk_size
    bits_height = height // chunk_size
    bit_capacity = bits_width * bits_height

    if len(bits) > bit_capacity:
        raise ValueError(f"Too big message for image. Image supports only {bit_capacity} bits but was given {len(bits)} bits")

    print(f"Image bit-capacity is {bit_capacity} bits.")
    print(f"Message size is {len(bits)} bits.")

    bit_index = 0
    for by in range(bits_height):
        if bit_index >= len(bits):
            break
        for bx in range(bits_width):
            if bit_index >= len(bits):
                px, py = bx * chunk_size, by * chunk_size
                rows = image[py:py+chunk_size]
                for row in rows:
                    row[px:px+chunk_size] = IMAGE_END_COLOR
                break

            if bits[bit_index] == '1':
                px, py = bx * chunk_size, by * chunk_size
                rows = image[py:py+chunk_size]
                for row in rows:
                    row[px:px+chunk_size] = (255,255,255)

            bit_index += 1
    
    
    cv2.imwrite(output_path, image, [cv2.IMWRITE_JPEG_QUALITY, 50])

def read_image(input_path: str, output_path: str, chunk_size: int = 4) -> str:
    image = cv2.imread(input_path)
    if image is None:
        raise FileNotFoundError(f"Could not read image: {input_path}")

    height, width, _ = image.shape

    bits_width = width // chunk_size
    bits_height = height // chunk_size

    total_color = np.zeros(3)
    text = ""
    bits = ""
    stop = False
    for by in range(bits_height):
        if stop:
            break
        for bx in range(bits_width):
            total_color = np.zeros(3)

            px, py = bx * chunk_size, by * chunk_size
            rows = image[py:py+chunk_size]
            for row in rows:
                for i in row[px:px+chunk_size]:
                    total_color += i
            avg_color = total_color / chunk_size**2
            if avg_color[1] >= 200 and avg_color[0] <= 100 and avg_color[2] <= 100:
                stop = True
                break
            brightness = (avg_color[0] + avg_color[1] + avg_color[2]) / 3
            bits += '1' if brightness >= 128 else '0'

    with open(output_path, "w") as f:
        f.write(bits)
        
if __name__ == "__main__":
    write_image(input("Message: "), width=500, output_path="encoded.jpg")
    read_image("encoded.jpg", "decoded.txt")