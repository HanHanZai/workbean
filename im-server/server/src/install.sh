#!/bin/bash
# author: cloudtalk
# date: 03/24/2020

LOG4CXX=apache-log4cxx-0.10.0
LOG4CXX_PATH=http://cloud.b56.cn/res/apache/logging/log4cxx/0.10.0/$LOG4CXX.tar.gz
PROTOBUF=protobuf-3.6.1
HIREDIS=hiredis-master
CUR_DIR=

check_user() {
  if [ $(id -u) != "0" ]; then
    echo "Error: You must be root to run this script, please use root to install cloudtalk 3.0"
    exit 1
  fi
}

get_cur_dir() {
  # Get the fully qualified path to the script
  case $0 in
  /*)
    SCRIPT="$0"
    ;;
  *)
    PWD_DIR=$(pwd)
    SCRIPT="${PWD_DIR}/$0"
    ;;
  esac
  # Resolve the true real path without any sym links.
  CHANGED=true
  while [ "X$CHANGED" != "X" ]; do
    # Change spaces to ":" so the tokens can be parsed.
    SAFESCRIPT=$(echo $SCRIPT | sed -e 's; ;:;g')
    # Get the real path to this script, resolving any symbolic links
    TOKENS=$(echo $SAFESCRIPT | sed -e 's;/; ;g')
    REALPATH=
    for C in $TOKENS; do
      # Change any ":" in the token back to a space.
      C=$(echo $C | sed -e 's;:; ;g')
      REALPATH="$REALPATH/$C"
      # If REALPATH is a sym link, resolve it.  Loop for nested links.
      while [ -h "$REALPATH" ]; do
        LS="$(ls -ld "$REALPATH")"
        LINK="$(expr "$LS" : '.*-> \(.*\)$')"
        if expr "$LINK" : '/.*' >/dev/null; then
          # LINK is absolute.
          REALPATH="$LINK"
        else
          # LINK is relative.
          REALPATH="$(dirname "$REALPATH")""/$LINK"
        fi
      done
    done

    if [ "$REALPATH" = "$SCRIPT" ]; then
      CHANGED=""
    else
      SCRIPT="$REALPATH"
    fi
  done
  # Change the current directory to the location of the script
  CUR_DIR=$(dirname "${REALPATH}")
}

init_dev_lib() {
  echo "=========================================="
  echo "|------------开始安装软件运行环境-----------|"
  echo "=========================================="
  sleep 3s
  #安装autoconf
  yum -y install autoconf
  #安装libtool
  yum -y install libtool
  #安装openssl
  yum -y install openssl openssl-devel

  yum -y install apr-devel
  yum -y install apr-util-devel
  yum -y install mariadb-devel
  yum -y install cmake
  yum -y install libuuid-devel
  #yum -y install openssl-devel
  yum -y install curl-devel
  #安装maven
  yum -y install maven
  #安装java
  #yum -y install java
  #"*"表示将java-1.8.0的所有相关Java程序都安装上
  #yum -y install java-1.8.0-openjdk*
  echo "=========================================="
  echo "|------------软件运行环境安装完成-----------|"
  echo "=========================================="
  sleep 2s
}

check_sys_var() {
  echo "=========================================="
  echo "|------------欢迎来到CloudTalk 3.0---------|"
  echo "=========================================="
  sleep 3s
  CENTOS_VERSION=$(less /etc/redhat-release)
  echo "--------------开始检查运行环境--------------"
  sleep 1s
  # shellcheck disable=SC2076
  if [[ $CENTOS_VERSION =~ "7." ]] || [[ $CENTOS_VERSION =~ "8." ]]; then
    echo "---------------环境检验通过--------------"
    sleep 1s
    echo "-------------开始安装CloudTalk 3.0------"
    sleep 3s
    init_dev_lib
  else
    echo "------错误：Centos 版本需要为7.0及以上-----"
    exit 1
  fi
}

build_log4cxx() {
  echo "=========================================="
  echo "|------------开始安装 LOG4CXX-------------|"
  echo "=========================================="
  sleep 3s
  cd log4cxx
  #  download $LOG4CXX.tar.gz $LOG4CXX_PATH
  #   tar -xf $LOG4CXX.tar.gz
  cd $LOG4CXX
  # ./configure --prefix=${HOME}/libs && make && make install
  # ./configure --prefix=${HOME}/libs --with-apr=${HOME}/libs && make && make install
  # ./configure --with-charset=utf-8 --with-apr=${HOME}/libs --with-apr-util=${HOME}/libs && make && make install

  ./configure --prefix=$CUR_DIR/log4cxx --with-apr=/usr --with-apr-util=/usr --with-charset=utf-8 --with-logchar=utf-8

  # cp ../inputstreamreader.cpp ./src/main/cpp/
  # cp ../socketoutputstream.cpp ./src/main/cpp/
  # cp ../console.cpp ./src/examples/cpp/
  make
  make install
  cd ../../
  cp -rf log4cxx/include slog/
  mkdir -p slog/lib/
  cp -f log4cxx/lib/liblog4cxx.so* slog/lib/
  echo "=========================================="
  echo "|------------LOG4CXX 安装成功-------------|"
  echo "=========================================="
  sleep 2s
}

build_protobuf() {
  echo "=========================================="
  echo "|------------开始安装 protobuf-------------|"
  echo "=========================================="
  sleep 3s
  cd protobuf
  tar -xf $PROTOBUF.tar.gz
  cd $PROTOBUF
  ./configure --prefix=$CUR_DIR/protobuf
  make
  make install
  cd ..
  mkdir -p ../base/pb/lib/linux/
  cp lib/libprotobuf-lite.a ../base/pb/lib/linux/
  cp -r include/* ../base/pb/
  cd ..
  echo "=========================================="
  echo "|------------protobuf 安装成功-------------|"
  echo "=========================================="
  sleep 3s
}
build_hiredis() {
  echo "=========================================="
  echo "|------------开始安装 hiredis-------------|"
  echo "=========================================="
  sleep 3s
  cd hiredis
  unzip $HIREDIS.zip
  cd $HIREDIS
  make
  cp -a libhiredis.a ../../ct_db_proxy_server/
  cp -a hiredis.h async.h read.h sds.h adapters ../../ct_db_proxy_server
  cd ../../
  echo "=========================================="
  echo "|------------hiredis 安装成功-------------|"
  echo "=========================================="
  sleep 3s
}

check_user
get_cur_dir
check_sys_var
build_log4cxx
build_protobuf
build_hiredis

#ln -s libmysqlclient.so libmysqlclient_r.so
./c_build.sh version 3.0.1
