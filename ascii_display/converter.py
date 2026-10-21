from us import tuffness

import cv2
import numpy as np
from PIL import Image, ImageDraw, ImageFont

ASCII_CHARS = [" ", ".", ":", "-", "=", "+", "*", "#", "%", "@"]

def pixel_to_ascii(brightness):
    return ASCII_CHARS[brightness // 26]

def convert(input_file_path: str, output_path="output.mp4", cols=100):
    cap = cv2.VideoCapture(input_file_path)
    if not cap.isOpened():
        raise FileNotFoundError(f"Error: Could not find or open {input_file_path}")

    fps = cap.get(cv2.CAP_PROP_FPS) or 30.0
    orig_w = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    orig_h = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    
    char_width = 10
    char_height = 18

    # rows and cols of characters
    rows = int((orig_h / orig_w) * cols * (char_width / char_height))

    #pixels
    out_width = cols * char_width
    out_height = rows * char_height
    
    # Set up the MP4 video writer type shift
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    out = cv2.VideoWriter(output_path, fourcc, fps, (out_width, out_height))

    font = ImageFont.load_default()
    frame_count = 0

    print(f"Beginning to render {total_frames} frames.\n")

    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break

        # Shrink the frame down to our ASCII grid size (aura/nigger)
        small_frame = cv2.resize(frame, (cols, rows))
        gray_frame = cv2.cvtColor(small_frame, cv2.COLOR_BGR2GRAY)

        #create BLACK canvas
        pil_img = Image.new('RGB', (out_width, out_height), color=(0, 0, 0))
        draw = ImageDraw.Draw(pil_img)

        for y in range(rows):
            for x in range(cols):
                b, g, r = small_frame[y, x] #bgr :sob:
                brightness = gray_frame[y, x]
                char = ASCII_CHARS[brightness // 26]

                if char != " ":
                    draw.text(
                        (x * char_width, y * char_height),
                        char,
                        font=font,
                        fill=(int(r), int(g), int(b))
                    )
                    
        out_frame = cv2.cvtColor(np.array(pil_img), cv2.COLOR_RGB2BGR)
        out.write(out_frame)

        frame_count += 1
        if (frame_count % 30 == 0):
            print(f"Processed {frame_count} / {total_frames} frames!")



    cap.release()
    out.release()
    print(f"Done! Saved your new video to {output_path}")

if __name__ == "__main__":
    convert("input.mp4", "ascii_output.mp4", cols=200)
    
    