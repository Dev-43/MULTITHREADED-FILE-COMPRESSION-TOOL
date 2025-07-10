# MULTITHREADED FILE COMPRESSION TOOL


**COMPANY**: CODTECH IT SOLUTIONS  
**NAME**: DEVESH LAXMAN DOLAS  
**INTERN ID**: CT04DG2484  
**DOMAIN**: C++ Programming  
**DURATION**: 4 WEEKS  
**MENTOR**: NEELA SANTOSH  

---
# 🗜️ Huffman Compression Tool

A full-featured C++ application for **compressing** and **decompressing** files using **Huffman Encoding**. This tool uses a custom format `.huff` for compressed data and supports both text and binary files. It features a user-friendly Windows console interface with file dialogs, progress bars, logging, and multithreaded processing.

---

## 🧠 What is Huffman Compression?

Huffman coding is a lossless data compression algorithm that assigns shorter binary codes to more frequent characters and longer codes to less frequent ones, optimizing the overall file size.

---

## 📂 Project Structure

```
📁 HuffmanCompressor/
│
├── main.cpp               # Main program: interface and control flow
├── compressor.h           # Handles compression logic using Huffman encoding
├── decompressor.h         # Handles decompression logic for .huff files
├── display.h              # Console interface, menus, and animations
├── File_picker.h          # Windows file open/save dialogs
├── input.h                # File validation, splitting, and info extraction
├── log.h                  # Logs compression/decompression statistics
```

---

## 🖥️ Features

✅ File selection using Windows file dialog  
✅ Supported formats: `.txt`, `.pdf`, `.csv`, `.jpg`, `.bin`, etc.  
✅ Adaptive chunking based on file size  
✅ Multithreaded compression using `std::async`  
✅ Bit-to-byte conversion for binary file writing  
✅ Huffman tree-based compression & code table  
✅ Decompression using header metadata  
✅ Console UI with interactive selection & progress bar  
✅ Detailed logging with compression ratio and timestamps  

---

## 🔁 How It Works

### 🗜️ Compression Process
1. File is selected via dialog.
2. File type is validated.
3. File is read and divided into **chunks**.
4. Huffman **frequency map** and **tree** are created.
5. Data is compressed in **parallel** per chunk.
6. Output `.huff` file is written:
   - File extension
   - Code map
   - Bit count
   - Compressed data

### 🔓 Decompression Process
1. `.huff` file is selected.
2. Metadata (extension, code map, bit count) is extracted.
3. Bitstream is reconstructed from byte data.
4. Data is decoded using the Huffman table.
5. Original file is saved with `_decompressed.<ext>`.

---

## 🧩 Module Breakdown

### `main.cpp`
- Handles control flow and user interaction.
- Displays menus, handles input and file selection.
- Manages mode (compress/decompress) and output saving.

### `compressor.h`
- Huffman tree generation and encoding.
- Parallel compression of file chunks.
- Bitstring-to-byte conversion.
- Writes final `.huff` file and logs stats.

### `decompressor.h`
- Reads and validates `.huff` metadata.
- Converts byte stream to bitstream.
- Decodes bitstream into original data.
- Saves file with appropriate extension.

### `display.h`
- Displays centered UI headers and menu.
- Uses arrow keys for selection.
- Shows animated progress bar during processing.

### `File_picker.h`
- Windows API file open/save dialogs.
- Autofills suggestions (e.g., `<name>_compressed.huff`).

### `input.h`
- Verifies file format against whitelist.
- Extracts filename, extension, and size.
- Splits file into chunks adaptively.

### `log.h`
- Logs compression/decompression stats to `log.log`.
- Includes time, size, ratio, and success/failure state.

---

## 🧪 Supported File Types

```
.txt, .json, .csv, .xml, .log, .html, .ini, .yaml, .pdf,
.xlsx, .xls, .docx, .exe, .bin, .png, .jpg
```

---

## 🛠️ Building and Running

### Requirements
- Windows OS (uses Windows API)
- C++17 or newer
- Visual Studio / MinGW / g++ compiler with Windows SDK

### 🧱 Build Command (g++)
```bash
g++ main.cpp -o HuffTool -lcomdlg32 -lole32 -luuid
```

### 💡 Run Instructions
1. Open terminal or double-click executable.
2. Use arrow keys and `Enter` to navigate.
3. Press `Space` to select a file.
4. Choose mode: `Compress` or `Decompress`.
5. Save compressed/decompressed output using dialog or fallback name.
6. Wait for completion and check output/logs.

---

## 📁 Output Files

| File | Description |
|------|-------------|
| `<name>_compressed.huff` | Compressed binary file |
| `<name>_decompressed.<ext>` | Reconstructed original file |
| `log.log` | Log file with compression/decompression stats |

---

## 📊 Sample Log Entry

```
[2025-07-09 01:32:55] Compression
Extention  : txt
Original   : 524288 bytes
Compressed : 169876 bits
Ratio      : 0.40
Reduction  : 60.00%
Status     : Success
------------------------------
[2025-07-9 01:33:08] Decompression
Extension   : txt
Raw Bytes   : 524288 bytes
Status      : Success
------------------------------
```

---

## 🖼️ Screenshots

```
 *******************Huff Compression Tool*******************

  > COMPRESS
    DECOMPRESS

  > Process File
    Exit

[#####===========           ] Compressing... 58%
```

---

## ⚠️ Limitations

- Only runs on **Windows** due to `windows.h` and `comdlg32` dependencies.
- Requires `.huff` format for decompression (not general-purpose).
- GUI is terminal-based, not graphical (e.g., Qt or WinForms).

---

## 📝 Future Improvements

- Add GUI support using Qt or Dear ImGui.
- Add encryption support for secure `.huff` files.
- Add decompression support for external Huffman files.
- Cross-platform support for Linux/macOS.



## 🙌 Credits

Built by Devesh Dolas for educational and practical demonstration of Huffman compression using C++.

---

## 💬 Contact

For suggestions, bugs, or collaboration:  
**Email:** deveshdolas9@gmail.com 
---
