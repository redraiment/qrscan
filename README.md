qrscan
====

Scan a QR code in the terminal using a given image.

## Install

```sh
git clone --recurse-submodules https://github.com/redraiment/qrscan.git
cd qrscan
make
sudo make install
```

This requires: libpng

## Usage

Scan a given image file

```sh
qrscan path/to/file

# Or read from stdin

cat path/to/file | qrscan -
```
