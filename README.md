# systmatic
A Linux distribution, developed during a video series on his Youtube channel. The distribution is built using Linux from Scratch, initially with version 9.1. In this repository, you can find the entire source code that was written and developed during the video series.

Not only does sysmtaic represent a learning experience to understand how a distribution is being built, marcus-s also sees the distribution as a way to showcase his ideas of how a modern Linux distribution should look like, as it is comprised of many of his own creations - even including his own Desktop environment at a later stage.

As of now there is no script to compile everything at once - for now the components have their own build scripts.

### Components
**libSystm**
One of the core libraries, used by many components in the distribution. Developed by marcus-s himself - it contains quite a number of utilitarian classes, such as the database class for reading and writing systm databases, classes to read and write binary files, and many others.

**systmPack**
The package manager of the distribution. It uses the libSystm library to read and write information about installed packages on the local system, and into the server database. The code is separated into the client and server versions.
