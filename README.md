# if1210-2026-tubes-template

Template Tugas Besar IF1210 Algoritma dan Pemrograman 1 2026

## Struktur Folder

```text
.
├── Makefile            # Instruksi kompilasi otomatis
├── header/             # Tempat menyimpan semua file header (.h)
│   └── tes.h
├── src/                # Tempat menyimpan semua file sumber (.c)
│   ├── main.c
│   └── tes.c
└── build/              # Folder output hasil kompilasi (otomatis dibuat)
    └── alprog1         # Executable file (hasil build)
```

## Persyaratan

Pastikan sistem Anda sudah terinstal:
- **gcc** (GNU Compiler Collection) - untuk kompilasi program C
- **make** - untuk automasi build process

Untuk mengecek apakah sudah terinstal, jalankan:
```bash
gcc --version
make --version
```

## How to Run

### Build Program
Untuk mengkompilasi program:
```bash
make build
```
atau cukup:
```bash
make
```

### Run Program
Untuk build dan menjalankan program:
```bash
make run
```

### Clean Build
Untuk menghapus hasil kompilasi:
```bash
make clean
```
