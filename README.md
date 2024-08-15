qrscan
====

![Continuous Integration](https://github.com/redraiment/qrscan/actions/workflows/ci.yml/badge.svg) ![Continuous Deployment](https://github.com/redraiment/qrscan/actions/workflows/cd.yml/badge.svg)

Scan a QR code in the terminal using a given (PNG or JPEG) image.

## Install

### Pre-built binaries (Recommends)

Download the appropriate binary from the [latest release](https://github.com/redraiment/qrscan/releases/latest).

### Build on Debian/Ubuntu Linux

This requires pkg-config, libpng, and libjpeg.

```sh
sudo apt install --no-install-recommends -y pkg-config libpng-dev libjpeg-dev
git clone --recurse-submodules https://github.com/redraiment/qrscan.git
cd qrscan
make
sudo make install
```

### Build on MacOS

This requires pkg-config, libpng, and libjpeg.

```sh
brew install pkg-config libpng libjpeg
git clone --recurse-submodules https://github.com/redraiment/qrscan.git
cd qrscan
make
sudo make install
```

## Usage

`qrscan [OPTIONS] [PNG|JPG|-]`

### Image Options

* `--png`, `--jpg`: the format of the input file, inferred from the extension by default.
* `-c`, `--count`: suppress normal output, instead print a count of QR-Code.
* `-n NUM`, `--index NUM`: Output the NUM QR-Code value only.

### Other Options

* `-v`, `--version`: show the version and exit.
* `-h`, `--help`: show help message and exit.

## Examples

1) Read from image file:

```sh
qrscan image.png
```

2) Read from stdin:

```sh
cat image.jpg | qrscan --jpg -
```

3) Count QR-Code in image:

```sh
qrscan -c image.png
```

4) Read the second QR-Code in image:

```sh
qrscan -2 image.png
# Or
qrscan -n 2 image.png
```

# License

`qrscan` is open-source software, released under the MIT License.

Feel free to explore, learn, and extend to suit your needs.
