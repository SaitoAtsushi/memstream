# memstream library

This library provide stream interface for string.
You can read and make string via `FILE*`.

## Building & installation

```
make install
```

## Build requirements

* MinGW32
* Windows10
* GCC or Clang

## Privided functions

This library provide following three function.
Please refer to examples also.

### FILE* open_output_memstream(char** str, size_t* n);

Open output stream.
`str` is a pointer to store a pointer to the resulting memory block.
`n` is a pointer to the variable that wrote the number of bytes.
`*str` and `*n` will be written by `mclose`.
Using `*str` or `*n` before run mclose is undefined behavior.
The stream returned by `open_output_memstream`  must not close with `fclose`.

Note: null termination don't be added automatically.

### int mclose(FILE* wf);

Close output stream returned by `open_output_memstream`.

### FILE* open_input_memstream(const char* str, size_t len);

Open input stream.
`str` is a pointer to the memory block you want to read.
`len` is memory block size.

## License

Copyright (c) 2018 SAITO Atsushi

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3. Neither the name of the authors nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
