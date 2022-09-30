## Follow [Instructions](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/jtag-debugging/building-openocd-macos.html#building-openocd-from-sources-for-macos)

for finding brew texinfo and capstone.h
```
export PATH=/opt/homebrew/opt/texinfo/bin:$PATH
export PATH=/opt/homebrew/opt/capstone/include/capstone:$PATH
```
Do NOT use binutils from homebrew. Uninstall or compile errors will occour!

```
  ./bootstrap (when building from the git repository)
  ./configure --prefix=/Users/<user>/OpenOCD
  make
  make install
```
