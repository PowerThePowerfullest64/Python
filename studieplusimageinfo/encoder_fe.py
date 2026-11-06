import encoder
import sys

path = sys.argv[1]          # the file you right-clicked
file_to_encode = open(path, "rb")
data_to_encode = file_to_encode.read()
file_to_encode.close()

width = 500
while True:
    if width > 65000:
        break
    try:
        encoder.write_image(message=data_to_encode, output_path=path + ".jpg", width=width)
        break
    except encoder.TooLargeJpgError:
        width *= 2
