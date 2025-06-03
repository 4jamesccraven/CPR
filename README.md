![image](./assets/CPR-logo.png)

---

CPR is a command-line compression tool that makes use of Huffman Coding, which
is a type of variable-length encoding

## Usage
```
A compression format based on huffman coding 


cpr [OPTIONS] files...


POSITIONALS:
  files FILES                 The file(s) to compress/decompress 

OPTIONS:
  -h,     --help              Print this help message and exit 
  -o,     --out FILE          Save the (de)compressed data to a file 
  -c, -e, --compress, --encode Excludes: --decompress 
                              Compress the file(s) 
  -d,     --decompress, --decode Excludes: --compress --print-frequency 
                              Decompress the file(s) 
  -t,     --print-frequency Excludes: --decompress 
                              Print the frequency table 
  -C,     --print-codebook    Print the character encoding 
  -p,     --print             Print the output of (de)compression 
  -O,     --no-extract        Do not reconstruct files, even when not using -p
```

### Common Operations
1. Compress file(s)

`cpr FILES -c`

2. Compress file with archive name

`cpr FILES -co my_archive.cprx`

3. Decompress a file

`cpr my_archive.cprx -d`

4. Inspect an archive's contents

`cpr my_archive.cprx -dp`

5. View how a file will be encoded

`cpr FILES -ctCp`

## Building
First clone the repo, and enter the project directory
```bash
git clone https://github.com/4jamesccraven/CPR.git
cd CPR
```
If you have just installed, you can simply run `just build`; otherwise do the following:
```bash
cmake -S . -B build
# parallel flag optional, speeds up build time.
cmake --build build --parallel $(nproc)
```
If you have nix with flakes enabled, you can play around with it in a shell by running
`nix shell github:4jamesccraven/CPR`

## Motivation & Limitations

This is mostly a toy example of Huffman Encoding, and it is not ideal for general use.
Notably, The encoding process (unintentionally) un-nests all sub directories. I may fix
it eventually, I may not. `¯\_(ツ)_/¯`
