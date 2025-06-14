# 📦 Huffman-File-Compressor

This project provides a **C++ implementation of the Huffman Encoding algorithm** for **lossless file compression and decompression**. It supports **any binary or text file**, efficiently reducing file sizes while preserving exact data.

---

## 🧠 Features

- ✅ Binary-safe compression and decompression
- ✅ Stores custom frequency metadata for accurate decoding
- ✅ Supports all file formats: `.txt`, `.png`, `.exe`, etc.
- ✅ Efficient memory usage and bitstream management
- ✅ Command-line interface with clear error handling

---

## 🛠️ Build Instructions

Use `g++` to compile:

```bash
# Compile the compressor
g++ compressor.cpp -o compressor

# Compile the decompressor
g++ decompressor.cpp -o decompressor.exe
```

---

## 🚀 Usage

### 🔐 Compress a file

```bash
./compressor input.txt compressed.txt
```

### 🔓 Decompress a file

```bash
./decompressor.exe compressed.txt decompressed.txt
```

> ⚠️ Ensure that:
> - `input.txt` exists
> - `compressed.txt` is writable
> - Executables (`compressor`, `decompressor.exe`) are compiled properly

---

## 📂 File Structure

| File                  | Purpose                                           |
|-----------------------|---------------------------------------------------|
| `compressor.cpp`      | Main file for compression                        |
| `decompressor.cpp`    | Main file for decompression                      |
| `huffman_decoder.cpp` | Core Huffman logic (shared by both executables)  |

---

## 📊 Compression Format Details

Each compressed file includes:
- Header with frequency data (used for reconstructing the Huffman Tree)
- Encoded binary stream
- Padding info (so bits align to byte boundaries)

---

## ✅ Example Run

```bash
./compressor test.txt compressed.huff
./decompressor compressed.huff restored.txt
diff test.txt restored.txt   # No output → files are identical
```

---





Feel free to ⭐ star the repository if you find this project helpful!
