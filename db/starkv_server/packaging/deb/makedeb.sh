#!/bin/bash
#
# Generate deb package for ubuntu
set -e
#set -x

#curr_dir=$(pwd)
compile_dir=$1
output_dir=$2
starengine_ver=$3
cpuType=$4
osType=$5
verMode=$6
verType=$7

script_dir="$(dirname $(readlink -f $0))"
top_dir="$(readlink -f ${script_dir}/../..)"
pkg_dir="${top_dir}/debworkroom"

# echo "curr_dir: ${curr_dir}"
# echo "top_dir: ${top_dir}"
# echo "script_dir: ${script_dir}"
# echo "compile_dir: ${compile_dir}"
# echo "pkg_dir: ${pkg_dir}"
# echo "starkvengine_ver: ${starengine_ver}"

if [ -d ${pkg_dir} ]; then
	 rm -rf ${pkg_dir}
fi
mkdir -p ${pkg_dir}
cd ${pkg_dir}

# libfile="libstardb.so.${starengine_ver}"

# create install dir 
install_home_path="/usr/local/starkv"
mkdir -p ${pkg_dir}${install_home_path}
mkdir -p ${pkg_dir}${install_home_path}/bin
mkdir -p ${pkg_dir}${install_home_path}/cfg
mkdir -p ${pkg_dir}${install_home_path}/include
mkdir -p ${pkg_dir}${install_home_path}/init.d
mkdir -p ${pkg_dir}${install_home_path}/script
 
cp ${compile_dir}/../packaging/cfg/starkv.cfg         ${pkg_dir}${install_home_path}/cfg
cp ${compile_dir}/../packaging/deb/alStarkvServer            ${pkg_dir}${install_home_path}/init.d
cp ${compile_dir}/../packaging/tools/post.sh        ${pkg_dir}${install_home_path}/script
cp ${compile_dir}/../packaging/tools/preun.sh       ${pkg_dir}${install_home_path}/script
cp ${compile_dir}/bin/alStarkvServer                   ${pkg_dir}${install_home_path}/bin
cp ${compile_dir}/bin/alStarkvClient                    ${pkg_dir}${install_home_path}/bin

cp -r ${compile_dir}/../packaging/deb/DEBIAN        ${pkg_dir}/
chmod 755 ${pkg_dir}/DEBIAN/*

# modify version of control
debver="Version: "$starengine_ver
sed -i "2c$debver" ${pkg_dir}/DEBIAN/control
 
#get stardb version, then set deb name


if [ "$verMode" == "cluster" ]; then
  debname="Starkv-server-"${starengine_ver}-${osType}-${cpuType}
elif [ "$verMode" == "lite" ]; then
  debname="Starkv-server"-${starengine_ver}-${osType}-${cpuType}
else
  echo "unknow verMode, nor cluster or lite"
  exit 1
fi

if [ "$verType" == "beta" ]; then
  debname=${debname}-${verType}".deb"
elif [ "$verType" == "stable" ]; then 
  debname=${debname}".deb"
else
  echo "unknow verType, nor stabel or beta"
  exit 1
fi

# make deb package
dpkg -b ${pkg_dir} $debname
echo "make deb package success!"

cp ${pkg_dir}/*.deb ${output_dir} 

# clean tmep dir
rm -rf ${pkg_dir}

