import decoder
import os
import sys

path = sys.argv[1]          # the file you right-clicked

output_path = os.path.splitext(path)[0]
decoder.read_image(input_path=path, output_path=output_path)
