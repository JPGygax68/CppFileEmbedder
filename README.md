CppFileEmbedder
===============

Small C++ header file generator to embed multiple files into your code, indexed by filename/path.

How to use
----------

### Generating the header file

Invoke the tool like this:

``CppFileEmbedder <headerfile> <resource files>``

This will generate a C++ header file that defines a static object called `embeds`.

### Using the header file

The header file generated by CppFileEmbedder is intended to be included in a single translation unit (source file). It is not guarded against multiple inclusion.

In one of your source files, include it simply like this:

```C++
#include "my_embedded_files.hpp"
```

### Accessing the embedded files

The `embeds` object features an overloaded index operator (`[]`) and a `get()` method, both of which must be passed one of the exact filenames or paths that were specified as resource files when executing CppFileEmbedder.

The difference between `[]` and `get()` is that the former returns a `const char *`, while the latter return a `const unsigned char *`.

### Installing

The CppFileEmbedder project is managed by [CMake](http://www.cmake.org/), which is therefore the best way to install it.

To do so:

- Get the source from Github at https://github.com/JPGygax68/CppFileEmbedder
- Create a subdirectory called `build` and create or direct a shell there
- Generate the project configuration by executing `cmake ../src -DCMAKE_INSTALL_PREFIX=<your install directory>`

``CMAKE_INSTALL_PREFIX`` must be set to the root directory where you put software that you build from source yourself. Under Windows, for example, you might use ``D:/Libraries`` (forward slash! CMake cannot cope with backslashes), which would result in files being installed to ``D:\Libraries\bin\`` (the executable) and ``D:\Libraries\cmake\`` (the CMake export file - see below).

From there, as usual with CMake, you can build and install the utility by executing `cmake --build . --target install` or by using your C++ toolchain directly.

### Using from CMake

CppFileEmbedder is not just built with CMake, it is also intended to be integrated into CMake-based build environments.

To this end, the ``install`` target installs CMake export files to ``${CMAKE_INSTALL_PREFIX}/cmake/``, called ``CppFileEmbedder-targets.cmake`` and ``CppFileEmbedder-targets-debug.cmake``.

Using the tool from your own ``CMakeLists.txt`` is easy. 

-   Use `find_file()` to locate the export file, followed by `include()`, e.g.:
    
    ```cmake
    find_file(CppFileEmbedder_CMAKE cmake/CppFileEmbedder-targets.cmake)
    include(${CppFileEmbedder_CMAKE})
    ```

-   Use a custom command to execute CppFileEmbedder, e.g.:

    ```cmake
    set(MY_EMBEDDABLE_FILES embeddable/file1.xyz embeddable/file2.bin)
    add_custom_command(
      OUTPUT generated/my_embedded_files.hpp
      COMMAND CppFileEmbedder generated/my_embedded_files.hpp ${MY_EMBEDDABLE_FILES} 
      DEPENDS ${MY_EMBEDDABLE_FILES}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    ```
    
    (You will generally want your generated file to be in a subdirectory of your `build` directory; in this example, we use     `generated`).
    
-   Do not forget to inform the compiler about the `generated` directory, e.g.:

    ```CMake
    include_directories(${CMAKE_BINARY_DIR}/generated)
    ```
    
    This will allow you to include the header file without having to hardcode a relative path.
    
With this in place, your embedding header file will be generated automatically, provided you list it as a dependency for one of your targets.

### Final words

This utility is being used in its current state in an experimental project, but is otherwise to be considered in infant stage, if that. I would however love to get feedback on it, and will try to fix bugs and mistakes if reported.

Enjoy!