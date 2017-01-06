cmake .. -DCMAKE_TOOLCHAIN_FILE=../CMakeCrossMips.txt -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/out
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/out
export PATH=/home/xuss/work/m150-tool/mipsel-gcc472-glibc216-mips32/bin:$PATH