# Seam Carving

This project implements seam carving in **C**, which is a technique that resizes images by removing seams (paths of pixels) with the least visual importance, preserving key image features better than traditional scaling or cropping.

- Computes dual-gradient energy for pixels
- Finds minimum energy seams using **dynamic programming**
- Removes seams **iteratively** to resize images
- Handles simple **binary image** input/output format

---

The input image is a binary file with this format:

- 2 bytes: height (big-endian)
- 2 bytes: width (big-endian)
- 3 * height * width bytes: RGB pixel data (row-major order)

---

Compile the project with the provided Makefile:

```bash
make
```
---
### Usage Example

To remove 5 vertical seams from an image:

```bash
./seamcarving input_image.bin 5
