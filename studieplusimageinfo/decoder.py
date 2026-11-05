from time import time
import cv2
import numpy as np

class NoHeaderError(Exception):
    pass

class InvalidLengthError(Exception):
    pass

def strbits_to_bytes(bits: str) -> bytes:
    """Turns bits into bytes."""
    limit = (len(bits) // 8) * 8
    return bytes(int(bits[i:i+8], 2) for i in range(0, limit, 8))

def bits_to_bytes(bits: np.ndarray) -> bytes:
    n_bytes = len(bits) // 8
    return np.packbits(bits[:n_bytes * 8]).tobytes()

def read_image(input_path: str, output_path: str = "", chunk_size: int = 2, make_file: bool = True, do_print: bool = False) -> bytes | None:
    start = time()
    
    if make_file and output_path == "":
        raise NameError(f"read_image() in decoder.py cannot write to a file with no name. Please provide a name to the output_path")

    MAGIC_HEADER_STR = "01010100111100011011100101101010"
    MAGIC_HEADER = np.fromiter(MAGIC_HEADER_STR, dtype=np.uint8)
    LENGTH_BIT_COUNT = 64

    image = cv2.imread(input_path)
    if image is None:
        raise FileNotFoundError(f"Could not read image: {input_path}")

    height, width, _ = image.shape

    bits_width = width // chunk_size
    bits_height = height // chunk_size

    blocks = image.reshape(bits_height, chunk_size, bits_width, chunk_size, 3)
    brightness = blocks.mean(axis=(1, 3, 4))

    bits = (brightness >= 128).astype(np.uint8).ravel()

    if not np.array_equal(bits[:len(MAGIC_HEADER)], MAGIC_HEADER):
        raise NoHeaderError("Input image file has no header.")

    length_bits = bits[len(MAGIC_HEADER):len(MAGIC_HEADER) + LENGTH_BIT_COUNT]
    bit_length = int.from_bytes(np.packbits(length_bits).tobytes(), byteorder="big")

    if bit_length < len(MAGIC_HEADER) or bit_length > len(bits):
        raise InvalidLengthError("Image input file did not specify its length correctly.")

    bits = bits[len(MAGIC_HEADER) + LENGTH_BIT_COUNT:bit_length]

    byte_data = bits_to_bytes(bits)

    if make_file:
        if do_print:
            print(f"Beginning to write bytes to {output_path}")
        with open(output_path, "wb") as f:
            f.write(byte_data)

    duration = time() - start
    if do_print:
        print(f"Image successfully decoded with length of {len(bits)//8} bytes in {duration} s to '{output_path}'")

    if make_file:
        return
    else:
        return byte_data

if __name__ == "__main__":
    input_path = input("File to decode: ")
    if input_path.split('.')[-1] != "jpg":
        raise SyntaxError("Decoded file must end in .jpg")
    output_path = input("Decoded filename: ")

    read_image(input_path=input_path, output_path=output_path, chunk_size=2)
