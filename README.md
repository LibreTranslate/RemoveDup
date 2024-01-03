# RemoveDup

A fast, memory efficient Python module to remove duplicates from parallel text corpora.

It's useful for cleaning up datasets that contain duplicate entries for training language models.

## Installation

```bash
pip install removedup
```

## Usage

```python
from removedup import rdup

src, tgt, removed = rdup("source.txt", "target.txt")
print(src, tgt, removed)
# source.txt.dedup
# target.txt.dedup
# <num lines removed>
```

## Notes

Source and target must have the same number of lines. No validation checks are made.

Duplication checks are only made on the source content. If you want to check for duplicates on the target, simply switch the order of the parameters.

## Build

```bash
git clone https://github.com/LibreTranslate/RemoveDup
cd RemoveDup
python setup.py build
```

## Standalone Binary

You can also use removedup as a standalone Windows, macOS or Linux application (but you currently need to build from source, we don't provide binaries).

```
mkdir build
cd build && cmake .. && make -j4
./rdup source.txt target.txt
```

## Contributing

We welcome pull requests!

## License

AGPLv3
