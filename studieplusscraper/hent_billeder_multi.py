from multiprocessing import Pool, current_process
from getpass import getpass
import time, os

THREAD_COUNT = os.cpu_count() * 4
print(f"Using: {THREAD_COUNT} threads.")

def hent_billede_multi(username, password, r):
    from main import log_ind, hent_billede
    import time
    time.sleep(int(current_process().name.split('-')[-1]))

    page, browser, p = log_ind(username, password)
    hent_billede(page, browser, p, r)
    print(f"worker[{current_process().name}]: is done")

def start_multi(username, password, r):
    import info
    args = []

    work_size = (r[1] - r[0]) // THREAD_COUNT
    print(f"work_size = {work_size}")
    for i in range(THREAD_COUNT):
        start = work_size * i + r[0]
        stop = work_size * (i+1) + r[0] if i != THREAD_COUNT - 1 else r[1]
        args.append((username, password, (start, stop)))

    time_start = time.time()
    with Pool(THREAD_COUNT) as p:
        p.starmap(hent_billede_multi, args)

    print("\n" * 5)
    print(f"Gemt alle data i '{info.BILLED_STI}'")
    print(f"Finished in : {round(time.time() - time_start, 2)} seconds)")
    print(f"With a avg of time of {(time.time() - time_start) / (r[1] - r[0])} per picture")

if __name__ == "__main__":
    start_multi(input("UniLogin brugernavn: "), getpass("UniLogin password: "), (200000, 260000))
