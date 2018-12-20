#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage: $0 <tag>"
	exit 2
fi

if [ ! -d imsmeta ]; then
	git clone https://github.com/UQ-RCC/imsmeta
fi

pushd imsmeta
	git checkout $1
popd

export LDFLAGS="-static"

export CC=/opt/x-tools/x86_64-pc-linux-musl/bin/x86_64-pc-linux-musl-gcc
export CXX=/opt/x-tools/x86_64-pc-linux-musl/bin/x86_64-pc-linux-musl-g++
export STRIP=/opt/x-tools/x86_64-pc-linux-musl/bin/x86_64-pc-linux-musl-strip

mkdir -p build-linux-amd64
pushd build-linux-amd64
	cmake -DCMAKE_BUILD_TYPE=MinSizeRel ../imsmeta
	make -j imsmeta
	$STRIP -s imsmeta
	mv imsmeta ../imsmeta-linux-amd64-$1
popd

export CC=/opt/x-tools/x86_64-w64-mingw32/bin/x86_64-w64-mingw32-gcc
export CXX=/opt/x-tools/x86_64-w64-mingw32/bin/x86_64-w64-mingw32-g++
export STRIP=/opt/x-tools/x86_64-w64-mingw32/bin/x86_64-w64-mingw32-strip

mkdir -p build-windows-amd64
pushd build-windows-amd64
	cmake -DCMAKE_BUILD_TYPE=MinSizeRel ../imsmeta
	make -j imsmeta
	$STRIP -s imsmeta
	mv imsmeta ../imsmeta-windows-amd64-$1.exe
popd

