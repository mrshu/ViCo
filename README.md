ViCo
====

Simple library for remote function calling and data retrieval. It can be also
simply used as a shell for embeded platforms (such as Arduino).


Function assigning
------------------

If you want some function to be called on some command, you have to include
`vico.h` in your source and then call `vico_fn_add` function. For more
examples see the `example.c` file


Building example file
---------------------

To build the example source file, run

	$ make example

You can run it then by executing

	$./example

