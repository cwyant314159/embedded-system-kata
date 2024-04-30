# Startup Code

The goal of this exercise is to provide a stable base from which subsequent 
exercises can build upon. Upon completion of this exercise, you should have the
following:

- the processor toolchain installed
- a build system for the exercise applications
- linker script configured and startup code running
- successful entrance to the `main` function

## Exercise

Write (or copy) the linker script and start up code for you processor and confirm
the following:

1. the BSS segment has been initialized to zero
2. the DATA segment has been initialized to expected values
3. all C++ global constructors have been called
4. static members of global C++ objects have expected values
5. static constants of global C++ objects have expected values

If all of the above checks pass, flash you board's LED. The flashing should be
visible to the human eye.

> NOTE: If you don't not have (or want) to use C++, skip items 3 through 5.

