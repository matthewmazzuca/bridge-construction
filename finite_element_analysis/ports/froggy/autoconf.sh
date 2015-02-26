#! /bin/bash -x

# Optionnaly set version and nbProc
: ${Version:="2.64"}
: ${gccVersion:="4.4.6"}
: ${nbProc:=$(echo `/usr/bin/getconf _NPROCESSORS_ONLN`)}
echo "Installation de autoconf-$Version"

#if [ -f $HOME/github/feelpp.config/etc/feelpprc.sh ]; then
#   . $HOME/github/feelpp.config/etc/feelpprc.sh
#fi

#export LD_PRELOAD=/applis/ciment/v2/x86_64/lib/libc.so.6

#Création d'un repertoire temporaire
  tar xzf ~/Downloads/autoconf-$Version.tar.gz
  cd autoconf-$Version
  mkdir -p build
  cd  build
  #installation en mode utilisateur
  # see http://www.linuxfromscratch.org/lfs/view/development/chapter06/gcc.html

  ../configure --prefix=/home/trophime/modules/tools/autoconf-${Version}_gcc_${gccVersion}
  make -j$nbProc
  make check
  make -j$nbProc install
