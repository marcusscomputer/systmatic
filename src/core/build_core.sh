#!/bin/bash

# Prepare environment
set +h
umask 022
LOCATION=/home/marcus-s/Developer/Projects/systmatic/depot/systmatic/src/core
TOOLS=$LOCATION/tools
SYSROOT=$LOCATION/build
LFS=/home/marcus-s/Developer/Projects/systmatic/depot/systmatic/src/core
LC_ALL=POSIX
LFS_TGT=$(uname -m)-systmatic-linux-gnu
MAKEFLAGS="-j3"
TARS=tarballs

# Enter tarball folder
cd $TARS

# ------------------------------------------------------------------------------

# BINUTILS
echo "## BINUTILS"
tar -xf binutils-2.34.tar.xz
cd binutils-2.34
mkdir build && cd build
../configure --prefix=$TOOLS --with-sysroot=$SYSROOT --with-lib-path=$TOOLS/lib --target=$LFS_TGT --disable-nls --disable-werror
make $MAKEFLAGS
mkdir $TOOLS/lib && ln -s lib $TOOLS/lib64
make install
cd ../..
rm -rf binutils-2.34
echo "## DONE BINUTILS"

# ------------------------------------------------------------------------------

# GCC PASS 1
echo "## GCC PASS 1"
tar -xf gcc-9.2.0.tar.xz
cd gcc-9.2.0
tar -xf ../mpfr-4.0.2.tar.xz
mv -v mpfr-4.0.2 mpfr
tar -xf ../gmp-6.2.0.tar.xz
mv -v gmp-6.2.0 gmp
tar -xf ../mpc-1.1.0.tar.gz
mv -v mpc-1.1.0 mpc
for file in gcc/config/{linux,i386/linux{,64}}.h
do
	cp -uv $file{,.orig}
	sed -e 's@/lib\(64\)\?\(32\)\?/ld@/tools&@g' \
		-e 's@/usr@/tools@g' $file.orig > $file
	echo '
#undef STANDARD_STARTFILE_PREFIX_1
#undef STANDARD_STARTFILE_PREFIX_2
#define STANDARD_STARTFILE_PREFIX_1 "/tools/lib/"
#define STANDARD_STARTFILE_PREFIX_2 ""' >> $file
	touch $file.orig
done
sed -e '/m64=/s/lib64/lib/' -i.orig gcc/config/i386/t-linux64
mkdir build
cd build
../configure \
	--target=$LFS_TGT \
	--prefix=$TOOLS \
	--with-glibc-version=2.11 \
	--with-sysroot=$SYSROOT \
	--with-newlib \
	--without-headers \
	--with-local-prefix=$TOOLS \
	--with-native-system-header-dir=$TOOLS/include \
	--disable-nls \
	--disable-shared \
	--disable-multilib \
	--disable-decimal-float \
	--disable-threads \
	--disable-libatomic \
	--disable-libgomp \
	--disable-libquadmath \
	--disable-libssp \
	--disable-libvtv \
	--disable-libstdcxx \
	--enable-languages=c,c++
make $MAKEFLAGS
make install
cd ../..
rm -rf gcc-9.2.0
echo "## DONE GCC PASS 1"

# ------------------------------------------------------------------------------

echo "## LINUX API HEADERS"
tar -xf linux-5.5.3.tar.xz
cd linux-5.5.3
make mrproper
make headers
mkdir $TOOLS/include
cp -r usr/include/* $TOOLS/include
cd ..
rm -rf linux-5.5.3
echo "## DONE LINUX API HEADERS"

# ------------------------------------------------------------------------------

echo "## GLIBC"
tar -xf glibc-2.31.tar.xz
cd glibc-2.31
mkdir build
cd build
../configure \
	--prefix=$TOOLS \
	--host=$LFS_TGT \
	--build=$(../scripts/config.guess) \
	--enable-kernel=3.2 \
	--with-headers=$TOOLS/include
make $MAKEFLAGS
make install
cd ../..
rm -rf glibc-2.31
echo "## DONE GLIBC"

# ------------------------------------------------------------------------------

echo "## LIBSTDC++"
tar -xf gcc-9.2.0.tar.xz
cd gcc-9.2.0
mkdir build && cd build
../libstdc++-v3/configure \
--host=$LFS_TGT \
--prefix=$TOOLS \
--disable-multilib \
--disable-nls \
--disable-libstdcxx-threads \
--disable-libstdcxx-pch \
--with-gxx-include-dir=$TOOLS/$LFS_TGT/include/c++/9.2.0
make $MAKEFLAGS
make install
cd ../..
rm -rf gcc-9.2.0
echo "## DONE LIBSTDC++"

# ------------------------------------------------------------------------------

echo "## BINUTILS PASS 2"
tar -xf binutils-2.34.tar.xz
cd binutils-2.34
mkdir build && cd build
CC=$LFS_TGT-gcc AR=$LFS_TGT-ar RANLIB=$LFS_TGT-ranlib ../configure \
	--prefix=$TOOLS \
	--disable-nls \
	--disable-werror \
	--with-lib-path=$TOOLS/lib \
	--with-sysroot
make $MAKEFLAGS
make install
make -C ld clean
make -C ld LIB_PATH=/System/Linux/lib:/System/Linux/usr/lib
cp ld/ld-new $TOOLS/bin
cd ../..
rm -rf binutils-2.34
echo "## DONE BINUTILS PASS 2"

# ------------------------------------------------------------------------------

echo "## GCC PASS 2"
tar -xf gcc-9.2.0.tar.xz
cd gcc-9.2.0
cat gcc/limitx.h gcc/glimits.h gcc/limity.h > \
`dirname $($LFS_TGT-gcc -print-libgcc-file-name)`/include-fixed/limits.h
for file in gcc/config/{linux,i386/linux{,64}}.h
do
	cp -uv $file{,.orig}
	sed -e 's@/lib\(64\)\?\(32\)\?/ld@/tools&@g' \
	-e 's@/usr@/tools@g' $file.orig > $file
	echo '
#undef STANDARD_STARTFILE_PREFIX_1
#undef STANDARD_STARTFILE_PREFIX_2
#define STANDARD_STARTFILE_PREFIX_1 "/tools/lib/"
#define STANDARD_STARTFILE_PREFIX_2 ""' >> $file
	touch $file.orig
done
sed -e '/m64=/s/lib64/lib/' -i.orig gcc/config/i386/t-linux64
tar -xf ../mpfr-4.0.2.tar.xz
mv -v mpfr-4.0.2 mpfr
tar -xf ../gmp-6.2.0.tar.xz
mv -v gmp-6.2.0 gmp
tar -xf ../mpc-1.1.0.tar.gz
mv -v mpc-1.1.0 mpc
sed -e '1161 s|^|//|' -i libsanitizer/sanitizer_common/sanitizer_platform_limits_posix.cc
CC=$LFS_TGT-gcc CXX=$LFS_TGT-g++ AR=$LFS_TGT-ar RANLIB=$LFS_TGT-ranlib \
../configure \
	--prefix=$TOOLS \
	--with-local-prefix=$TOOLS \
	--with-native-system-header-dir=$TOOLS/include \
	--enable-languages=c,c++ \
	--disable-libstdcxx-pch \
	--disable-multilib \
	--disable-bootstrap \
	--disable-libgomp
make $MAKEFLAGS
make install
ln -s gcc $TOOLS/bin/cc
cd ../..
rm -rf gcc-9.2.0
echo "## DONE GCC PASS 2"

# ------------------------------------------------------------------------------

echo "## TCL"
tar -xf tcl8.6.10-src.tar.gz
cd tcl8.6.10-src
cd unix
./configure --prefix=$TOOLS
make $MAKEFLAGS
make install
chmod u+w $TOOLS/lib/libtcl8.6.so
make install private-headers
ln -s tclsh8.6 $TOOLS/bin/tclsh
cd ../..
rm -rf tcl8.6.10-src
echo "## DONE TCL"

# ------------------------------------------------------------------------------

echo "## EXPECT"
tar -xf expect5.45.4.tar.gz
cd expect5.45.4
cp configure{,.orig}
sed 's:/usr/local/bin:/bin:' configure.orig > configure
./configure --prefix=$TOOLS \
	--with-tcl=$TOOLS/lib \
	--with-tclinclude=$TOOLS/include
make $MAKEFLAGS
make SCRIPTS="" install
cd ..
rm -rf expect5.45.4
echo "## DONE EXPECT"

# ------------------------------------------------------------------------------

echo "## DEJAGNU"
tar -xf dejagnu-1.6.2.tar.gz
cd dejagnu-1.6.2
./configure --prefix=$TOOLS
make install
cd ..
rm -rf dejagnu-1.6.2
echo "## DONE DEJAGNU"

# ------------------------------------------------------------------------------

echo "## M4"
tar -xf m4-1.4.18.tar.xz
cd m4-1.4.18
sed -i 's/IO_ftrylockfile/IO_EOF_SEEN/' lib/*.c
echo "#define _IO_IN_BACKUP 0x100" >> lib/stdio-impl.h
./configure --prefix=$TOOLS
make $MAKEFLAGS
make install
cd ..
rm -rf m4-1.4.18
echo "## DONE M4"

# ------------------------------------------------------------------------------

echo "## NCURSES"
tar -xf ncurses-6.2.tar.gz
cd ncurses-6.2
sed -i s/mawk// configure
./configure --prefix=$TOOLS \
	--with-shared \
	--without-debug \
	--without-ada \
	--enable-widec \
	--enable-overwrite
make $MAKEFLAGS
make install
ln -s libncursesw.so $TOOLS/lib/libncurses.so
cd ..
rm -rf ncurses-6.2
echo "## DONE NCURSES"

# ------------------------------------------------------------------------------

echo "## BASH"
tar -xf bash-5.0.tar.gz
cd bash-5.0
./configure --prefix=$TOOLS --without-bash-malloc
make $MAKEFLAGS
make install
ln -s bash $TOOLS/bin/sh
cd ..
rm -rf bash-5.0
echo "## DONE BASH"

# ------------------------------------------------------------------------------

echo "## BISON"
tar -xf bison-3.5.2.tar.xz
cd bison-3.5.2
./configure --prefix=$TOOLS
make $MAKEFLAGS
make install
cd ..
rm -rf bison-3.5.2
echo "## DONE BISON"

# ------------------------------------------------------------------------------

echo "## BZIP2"
tar -xf bzip2-1.0.8.tar.gz
cd bzip2-1.0.8
make -f Makefile-libbz2_so
make clean
make $MAKEFLAGS
make PREFIX=$TOOLS install
cp -v bzip2-shared $TOOLS/bin/bzip2
cp -a libbz2.so* $TOOLS/lib
ln -s libbz2.so.1.0 $TOOLS/lib/libbz2.so
cd ..
rm -rf bzip2-1.0.8
echo "## DONE BZIP2"

# ------------------------------------------------------------------------------

echo "## COREUTILS"
tar -xf coreutils-8.31.tar.xz
cd coreutils-8.31
./configure --prefix=$TOOLS --enable-install-program=hostname
make $MAKEFLAGS
make install
cd ..
rm -rf coreutils-8.31
echo "## DONE COREUTILS"

# ------------------------------------------------------------------------------

echo "## DIFFUTILS"
tar -xf diffutils-3.7.tar.xz
cd diffutils-3.7
./configure --prefix=$TOOLS
make $MAKEFLAGS
make install
cd ..
rm -rf diffutils-3.7
echo "## DONE DIFFUTILS"

# ------------------------------------------------------------------------------

echo "## FILE"
tar -xf file-5.38.tar.gz
cd file-5.38
./configure --prefix=$TOOLS
make $MAKEFLAGS
make install
cd ..
rm -rf file-5.38
echo "## DONE FILE"

# ------------------------------------------------------------------------------

echo "## FINDUTILS"
tar -xf findutils-4.7.0.tar.xz
cd findutils-4.7.0
./configure --prefix=$TOOLS
make $MAKEFLAGS
make install
cd ..
rm -rf findutils-4.7.0
echo "## DONE FINDUTILS"

# ------------------------------------------------------------------------------

echo "## GAWK"
tar -xf gawk-5.0.1.tar.xz
cd gawk-5.0.1
./configure --prefix=$TOOLS
make $MAKEFLAGS
make install
cd ..
rm -rf gawk-5.0.1
echo "## DONE GAWK"

# ------------------------------------------------------------------------------

echo "## GETTEXT"
tar -xf gettext-0.20.1.tar.xz
cd gettext-0.20.1
./configure --disable-shared
make $MAKEFLAGS
cp -v gettext-tools/src/{msgfmt,msgmerge,xgettext} $TOOLS/bin
cd ..
rm -rf gettext-0.20.1
echo "## DONE GETTEXT"

# ------------------------------------------------------------------------------

echo "## GREP"
tar -xf grep-3.4.tar.xz
cd grep-3.4
./configure --prefix=$TOOLS
make $MAKEFLAGS
make install
cd ..
rm -rf grep-3.4
echo "## DONE GREP"

# ------------------------------------------------------------------------------

echo "## GZIP"
tar -xf gzip-1.10.tar.xz
cd gzip-1.10
./configure --prefix=$TOOLS
make $MAKEFLAGS
make install
cd ..
rm -rf gzip-1.10
echo "## DONE GZIP"

# ------------------------------------------------------------------------------

echo "## MAKE"
tar -xf make-4.3.tar.gz
cd make-4.3
./configure --prefix=$TOOLS --without-guile
make $MAKEFLAGS
make install
cd ..
rm -rf make-4.3
echo "## DONE MAKE"

# ------------------------------------------------------------------------------

echo "## PATCH"
tar -xf patch-2.7.6.tar.xz
cd patch-2.7.6
./configure --prefix=$TOOLS
make $MAKEFLAGS
make install
cd ..
rm -rf patch-2.7.6
echo "## DONE PATCH"

# ------------------------------------------------------------------------------

echo "## PERL"
tar -xf perl-5.30.1.tar.xz
cd perl-5.30.1
sh Configure -des -Dprefix=$TOOLS -Dlibs=-lm -Uloclibpth -Ulocincpth
make $MAKEFLAGS
cp -v perl cpan/podlators/scripts/pod2man $TOOLS/bin
mkdir -p $TOOLS/lib/perl5/5.30.1
cp -Rv lib/* $TOOLS/lib/perl5/5.30.1
cd ..
rm -rf perl-5.30.1
echo "## DONE PERL"

# ------------------------------------------------------------------------------

echo "## PYTHON"
tar -xf Python-3.8.1.tar.xz
cd Python-3.8.1
sed -i '/def add_multiarch_paths/a \
return' setup.py
./configure --prefix=$TOOLS --without-ensurepip
make $MAKEFLAGS
make install
cd ..
rm -rf Python-3.8.1
echo "## DONE PYTHON"

# ------------------------------------------------------------------------------

echo "## SED"
tar -xf sed-4.8.tar.xz
cd sed-4.8
./configure --prefix=$TOOLS
make $MAKEFLAGS
make install
cd ..
rm -rf sed-4.8
echo "## DONE SED"

# ------------------------------------------------------------------------------

echo "## TAR"
tar -xf tar-1.32.tar.xz
cd tar-1.32
./configure --prefix=$TOOLS
make $MAKEFLAGS
make install
cd ..
rm -rf tar-1.32
echo "## DONE TAR"

# ------------------------------------------------------------------------------

echo "## TEXINFO"
tar -xf texinfo-6.7.tar.xz
cd texinfo-6.7
./configure --prefix=$TOOLS
make $MAKEFLAGS
make install
cd ..
rm -rf texinfo-6.7
echo "## DONE TEXINFO"

# ------------------------------------------------------------------------------

echo "## XZ"
tar -xf xz-5.2.4.tar.xz
cd xz-5.2.4
./configure --prefix=$TOOLS
make $MAKEFLAGS
make install
cd ..
rm -rf xz-5.2.4
echo "## DONE XZ"

# ------------------------------------------------------------------------------

echo "## STRIPPING"
strip --strip-debug $TOOLS/lib/*
/usr/bin/strip --strip-unneeded $TOOLS/{,s}bin/*
rm -rf $TOOLS/{,share}/{info,man,doc}
find $TOOLS/{lib,libexec} -name \*.la -delete
chown -R root:root $TOOLS
cd ..
echo "## DONE STRIPPING"

# ------------------------------------------------------------------------------

echo " "
echo " SYSTMATIC: TOOLCHAIN BUILD FOR SYSTEM COMPILATION COMPLETED!"
echo " "

# ------------------------------------------------------------------------------

