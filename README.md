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

This requires: pkg-config, libpng, libjpeg.

## Usage

Scan a given image file (possible format: png, jpg)

```sh
qrscan path/to/file

# Or read from stdin

cat path/to/file | qrscan -
```
