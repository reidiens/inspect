# Inspect
### Inspect the details of a file
<hr>
View details about any file on a Linux system including block & character devices, directories, and symbolic links.

In the future I plan to expand `inspect`'s functionality by adding filetype recognition and having it be able to output detailed information about the file's contents.
For example, being able to view song length, author, and album name for an mp3 or the magic number, section headers, and program headers of an ELF file.

### Building
To build, simply run `sudo make`.

The executable will automatically be placed in `/usr/bin`

### Usage
`i <filename>`

`inspect <filename>`
