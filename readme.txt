
To build the pico]OS realtime operating system,
you need to execute the makefile in the root directory.

The makefile takes two parameters:
  PORT  = name of the port to translate (= the subdirectory name)
  BUILD = version to build, possible values are DEBUG and RELEASE

Example:
  make PORT=x86dos BUILD=DEBUG

  Builds the x86 DOS port and includes debug informations.
  The generated library can be found in the directory lib/x86dos/deb/

Make targets:
  The makefile knows the targets 'all', 'clean' and 'docu', and
  'all' is the default.  'all' compiles the operating system.
  'clean' removes all generated binaries. 'docu' generates the
  html help with use of the doxygen tool.

Hint:
  When the makefile is started, it searches for the configuration file
  'config.mak' in the pico]OS root directory.  You can put in there your
  command line. Example:

   #--- start of file ---
   PORT  = x86dos
   BUILD = DEBUG
   #---- end of file ----

