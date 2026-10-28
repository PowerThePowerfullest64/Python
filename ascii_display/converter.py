from us import tuffness

import cv2
import numpy as np
from PIL import Image, ImageDraw, ImageFont
import time, os
from moviepy import VideoFileClip#, AudioFileClip

ASCII_CHARS = np.array(list(" .'`^,:;Il!i><~+_-?[}{)(|/tfjrxnvczXYUJCLQOZmwqpdbkhao*#MW&8%B@$"))

def load_monospace_font(size=12):
    for name in ["consola.ttf", "cour.ttf", "DejaVuSansMono.ttf", "Menlo.ttc"]:
        try:
            return ImageFont.truetype(name, size)
        except OSError:
            pass
    raise RuntimeError("No monospace font found.")

def build_atlas(font):
    ascent, descent = font.getmetrics()
    cw = int(np.ceil(font.getlength("@")))
    ch = ascent + descent
    atlas = np.zeros((len(ASCII_CHARS), ch, cw), np.float32)
    for i, c in enumerate(ASCII_CHARS):
        img = Image.new("L", (cw, ch), 0)
        ImageDraw.Draw(img).text((0, 0), c, font=font, fill=255)
        atlas[i] = np.asarray(img, np.float32) / 255.0
    return atlas, cw, ch
    

def add_audio(ascii_vid_path, src_path, output_path):
    src = VideoFileClip(src_path)
    vid = VideoFileClip(ascii_vid_path)
    if src.audio is not None:
        vid = vid.with_audio(src.audio)
    vid.write_videofile(output_path, codec="libx264", audio_codec="aac")
    src.close(); vid.close()

def convert(input_file_path: str, output_path="", cols=100):
    """mp4 -> mp4 but ascii"""
    if output_path == "":
        output_path = input_file_path + str(cols)
    output_path += ".mp4"
    input_file_path = "input_" + input_file_path + ".mp4"
    print(f"Making {output_path}...")

    cap = cv2.VideoCapture(input_file_path)
    if not cap.isOpened():
        raise FileNotFoundError(input_file_path)

    fps = cap.get(cv2.CAP_PROP_FPS) or 30.0
    orig_w = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    orig_h = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    total = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

    atlas, cw, ch = build_atlas(load_monospace_font(12))
    rows = max(1, int((orig_h / orig_w) * cols * (cw / ch)))

    out_w, out_h = cols * cw, rows * ch
    out_w -= out_w % 2
    out_h -= out_h % 2

    os.makedirs("temps", exist_ok=True)
    ascii_path = os.path.join("temps", "TEMP_" + output_path)
    out = cv2.VideoWriter(ascii_path, cv2.VideoWriter_fourcc(*"mp4v"), fps, (out_w, out_h))
    if not out.isOpened():
        raise RuntimeError("VideoWriter failed to open")

    n, start = 0, time.time()
    while True:
        ret, frame = cap.read()
        if not ret:
            break

        small = cv2.resize(frame, (cols, rows), interpolation=cv2.INTER_NEAREST)
        gray = cv2.cvtColor(small, cv2.COLOR_BGR2GRAY)
        
        idx = ((gray / 255.0) ** 0.85 * 255).astype(np.uint8) // 4

        hsv = cv2.cvtColor(small, cv2.COLOR_BGR2HSV).astype(np.float32)
        hsv[..., 1] = np.clip(hsv[..., 1] * 1.5, 0, 255)
        hsv[..., 2] = np.clip(hsv[..., 2] * 1.2 + 15, 0, 255)
        bright = cv2.cvtColor(hsv.astype(np.uint8), cv2.COLOR_HSV2BGR)

        mask = atlas[idx].transpose(0, 2, 1, 3).reshape(rows * ch, cols * cw)
        
        mask = np.clip(mask * 1.5, 0, 1)  # optional: bolder strokes
        color = cv2.resize(bright, (cols * cw, rows * ch), interpolation=cv2.INTER_NEAREST)

        img = (color * mask[:, :, None]).astype(np.uint8)[:out_h, :out_w]
        out.write(img)

        n += 1
        if n % 10 == 0:
            avg = (time.time() - start) / n
            print(f"{n}/{total} ~{avg * (total - n):.1f}s left")

    cap.release()
    out.release()
    add_audio(ascii_path, input_file_path, output_path)

    print("Done making", output_path)

if __name__ == "__main__":
    name = input("File name without input_: ")
    cols = 0
    while (True):
        try:
            cols = int(input("How many coloms should the output have? "))
        except:
            print("Columns are given as an integer. Try again.")
            continue
        break
    convert(name, cols=cols)
    input("\nPress enter to exit...")
    