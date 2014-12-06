libstandard
===========

C language library of variety for UNIX/Linux programming.

## Fueatures
Data science utility
String control
XML/HTML access using XPath
mathematics utility
DB control

## Requirement
glib
Apache portable rumtime
libxml2
libdpgmm of github user y-mitsui
GNU Scientific Library
libmysql
cmake
## Install

    $ cmake .
    $ make && sudo make install

## Usage

    $ gcc `pkg-config --cflags standard` yourprogram.c `pkg-config --libs standard`


	$ doxygen
	
Open browser html/index.html for reading documentation.
