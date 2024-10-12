#export PATH=$PATH:/opt/pub-gcc/gcc-arm-8.3-2019.03-x86_64-aarch64-linux-gnu/bin

OUTPUT_DEFAULT=config9211.elf
DIR_DEFAULT=/tmp/lt9211/

OUTPUT=""
DIR=""

if [ -z "$1" ];then
    OUTPUT=${OUTPUT_DEFAULT}
else
    OUTPUT=$1
fi

if [ ! -d "$2" ];then
    DIR=${DIR_DEFAULT}
else
    DIR=$2
fi

make clean;
make clean PROGRAM=${OUTPUT};
make PROGRAM=${OUTPUT} -j16 || exit 1
mkdir  ${DIR} -p
cp -v ${OUTPUT} ${DIR}
cp -v ${OUTPUT} ${DIR}/${OUTPUT}
