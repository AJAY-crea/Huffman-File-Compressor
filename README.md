# 📦 Huffman-File-Compressor

This is a C++ implementation of the **Huffman Encoding algorithm** for lossless file compression and decompression. It allows you to compress any binary file and later decompress it back to its original form with perfect accuracy.

---

## 🧠 Features

- Full **binary-safe** encoding and decoding  
- Custom frequency table storage for decoding  
- Handles any file format (text, binary, etc.)  
- Efficient memory and bitstream handling  
- CLI-based usage with informative error messages  

---

## 🛠️ Build Instructions

Use `g++` to compile the source files:

```bash
g++ compressor.cpp huffman_decoder.cpp -o compressor
g++ decompressor.cpp huffman_decoder.cpp -o decompressor
```

You can also combine both compressor and decompressor functionality in one binary if desired.

---

## 🚀 Usage

### Compress a file:

```bash
./compressor input.txt compressed.txt
```

### Decompress a file:

```bash
./decompressor compressed.txt decompressed.txt
```

> ⚠️ Ensure `input.txt` exists and that `compressed.txt` is writable.

---

## 📂 File Structure

| File                  | Description                              |
|-----------------------|------------------------------------------|
| `compressor.cpp`      | Main entry point for compression         |
| `decompressor.cpp`    | Main entry point for decompression       |
| `huffman_decoder.cpp` | Contains the Huffman encoding/decoding logic |

---

## 📊 Compression Format

The compressed file stores:
- A header with character frequency information
- Huffman-encoded binary content
- Padding metadata for accurate decompression

---

## ✅ Example

```bash
./compressor test.txt compressed.huff
./decompressor compressed.huff restored.txt
diff test.txt restored.txt   # Should show no output (identical)
```

---

## 📌 Future Work

- GUI interface
- Parallel encoding
- Support for multi-file archiving
- Bitstream optimization

---

## 👨‍💻 Author

Developed by **Ajay Veliginti**  
📧 [ajay@iitb.ac.in](mailto:ajay@iitb.ac.in)
