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

### FILE* open_output_memstream(void);

Open output stream.
The stream returned by `open_output_memstream`  must close with `mclose`.
Do not close with `fclose`.

### char* mclose(FILE* wf, int* plen);

Close output stream returned by `open_output_memstream`.
`plen` is a pointer to the variable that wrote the number of bytes.
Return value is a pointer to resulting memory block.
You must close the memory block with `free` when it becomes unnecessary.

Note: null termination don't be added automatically.

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
