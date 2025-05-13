from functools import partial
from itertools import repeat, batched
from math import ceil
from pathlib import Path
import argparse as ap


def main() -> None:
    P = ap.ArgumentParser(__name__, 'decode cpr file')
    P.add_argument('file', help='File to decode', type=Path)
    args = P.parse_args()

    int_from = partial(int.from_bytes, signed=False, byteorder='big')

    with open(args.file, 'rb') as f:
        entries = int_from(f.read(4))

        table: dict[str, int] = {}
        for _ in repeat(None, entries):
            char = chr(int_from(f.read(1)))
            freq = int_from(f.read(4))

            table[char] = freq

        files: dict[str, list[int]] = {}
        while (_ := f.read(1)):
            f.seek(-1, 1)
            name = ""
            while (char := chr(int_from(f.read(1)))) != '\0':
                name = name + char

            bits = int_from(f.read(4))
            bytes_ = [int_from(f.read(1))
                      for _ in repeat(None, ceil(bits / 8))]

            files[name] = bytes_

    print(f'# {args.file}')
    print('- Table')
    for c, code in table.items():
        print(f'    {repr(c):>4}: {int(code)}')
    print()
    for file, bytes_ in files.items():
        print(f'- {file}:')
        for group in batched(bytes_, 4):
            print('   ', end='')
            for b in group:
                print(f' {b:08b}', end='')
            print()


if __name__ == '__main__':
    main()
