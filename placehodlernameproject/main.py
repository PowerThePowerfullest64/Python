import numpy as np

def is_png(data: np.ndarray[np.uint8]):
    signature = np.frombuffer(bytes.fromhex("89 50 4E 47"), dtype=np.uint8)
    return len(data) >= 4 and (data[:4] == signature).all()
