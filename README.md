libstandard
===========

C language library of variety for UNIX/Linux programming.

## Features
* Data science utility
* String control
* XML/HTML access using XPath
* mathematics utility
* DB control

## Requirement
* glib-2.x
* Apache portable rumtime
* libxml2 with command xml2-config
* libdpgmm of github user y-mitsui
* GNU Scientific Library
* libmysql with command of mysql_config
* cmake
## Install

    $ cmake .
    $ make && sudo make install

## Usage
**[Compile your project]**

    $ gcc `pkg-config --cflags standard` yourprogram.c `pkg-config --libs standard`

**[Generate Document]**

	$ doxygen
	
Open html/index.html with web browser for reading documentation.
