#!/bin/bash
#
# This file is used to install Starengine client on linux systems. The operating system
# is required to use systemd to manage services at boot

set -e
#set -x

# -----------------------Variables definition---------------------
script_dir=$(dirname $(readlink -f "$0"))
# Dynamic directory
data_dir="/var/lib/stardb"
log_dir="/var/log/stardb"

log_link_dir="/usr/local/stardb/log"

cfg_install_dir="/etc/stardb"

bin_link_dir="/usr/bin"
lib_link_dir="/usr/lib"
inc_link_dir="/usr/include"

#install main path
install_main_dir="/usr/local/stardb"

# old bin dir
bin_dir="/usr/local/stardb/bin"


# Color setting
RED='\033[0;31m'
GREEN='\033[1;32m'
GREEN_DARK='\033[0;32m'
GREEN_UNDERLINE='\033[4;32m'
NC='\033[0m'

csudo=""
if command -v sudo > /dev/null; then
    csudo="sudo"
fi

update_flag=0

function kill_client() {
  pid=$(ps -ef | grep "stardb" | grep -v "grep" | awk '{print $2}')
  if [ -n "$pid" ]; then
    ${csudo} kill -9 $pid   || :
  fi
}

function install_main_path() {
    #create install main dir and all sub dir
    ${csudo} rm -rf ${install_main_dir}    || :
    ${csudo} mkdir -p ${install_main_dir}  
    ${csudo} mkdir -p ${install_main_dir}/cfg
    ${csudo} mkdir -p ${install_main_dir}/bin    
    ${csudo} mkdir -p ${install_main_dir}/connector
    ${csudo} mkdir -p ${install_main_dir}/driver
    ${csudo} mkdir -p ${install_main_dir}/examples
    ${csudo} mkdir -p ${install_main_dir}/include
}

function install_bin() {
    # Remove links
    ${csudo} rm -f ${bin_link_dir}/stardb     || :
    ${csudo} rm -f ${bin_link_dir}/stardbdump || :
    ${csudo} rm -f ${bin_link_dir}/rmstardb   || :

    ${csudo} cp -r ${script_dir}/bin/* ${install_main_dir}/bin && ${csudo} chmod 0555 ${install_main_dir}/bin/*

    #Make link
    [ -x ${install_main_dir}/bin/stardb ] && ${csudo} ln -s ${install_main_dir}/bin/stardb ${bin_link_dir}/stardb             || :
    [ -x ${install_main_dir}/bin/stardbdump ] && ${csudo} ln -s ${install_main_dir}/bin/stardbdump ${bin_link_dir}/stardbdump || :
    [ -x ${install_main_dir}/bin/remove_client.sh ] && ${csudo} ln -s ${install_main_dir}/bin/remove_client.sh ${bin_link_dir}/rmstardb || :
}

function clean_lib() {
    sudo rm -f /usr/lib/libstardb.so || :
    sudo rm -rf ${lib_dir} || :
}

function install_lib() {
    # Remove links
    ${csudo} rm -f ${lib_link_dir}/libstardb.*         || :

    ${csudo} cp -rf ${script_dir}/driver/* ${install_main_dir}/driver && ${csudo} chmod 777 ${install_main_dir}/driver/*  
    
    ${csudo} ln -s ${install_main_dir}/driver/libstardb.* ${lib_link_dir}/libstardb.so.1
    ${csudo} ln -s ${lib_link_dir}/libstardb.so.1 ${lib_link_dir}/libstardb.so
    
}

function install_header() {
    ${csudo} rm -f ${inc_link_dir}/stardb.h ${inc_link_dir}/stardberror.h    || :
    ${csudo} cp -f ${script_dir}/inc/* ${install_main_dir}/include && ${csudo} chmod 644 ${install_main_dir}/include/*    
    ${csudo} ln -s ${install_main_dir}/include/stardb.h ${inc_link_dir}/stardb.h
    ${csudo} ln -s ${install_main_dir}/include/stardberror.h ${inc_link_dir}/stardberror.h
}

function install_config() {
    #${csudo} rm -f ${install_main_dir}/cfg/stardb.cfg     || :
    
    if [ ! -f ${cfg_install_dir}/stardb.cfg ]; then
        ${csudo} mkdir -p ${cfg_install_dir}
        [ -f ${script_dir}/cfg/stardb.cfg ] && ${csudo} cp ${script_dir}/cfg/stardb.cfg ${cfg_install_dir}
        ${csudo} chmod 644 ${cfg_install_dir}/*
    fi 
    
    ${csudo} cp -f ${script_dir}/cfg/stardb.cfg ${install_main_dir}/cfg/stardb.cfg.org
    ${csudo} ln -s ${cfg_install_dir}/stardb.cfg ${install_main_dir}/cfg
}


function install_log() {
    ${csudo} rm -rf ${log_dir}  || :
    ${csudo} mkdir -p ${log_dir} && ${csudo} chmod 777 ${log_dir}
    
    ${csudo} ln -s ${log_dir} ${install_main_dir}/log
}

function install_connector() {
    ${csudo} cp -rf ${script_dir}/connector/* ${install_main_dir}/connector
}

function install_examples() {
    if [ -d ${script_dir}/examples ]; then
        ${csudo} cp -rf ${script_dir}/examples/* ${install_main_dir}/examples
    fi
}

function update_Starengine() {
    # Start to update
    if [ ! -e stardb.tar.gz ]; then
        echo "File stardb.tar.gz does not exist"
        exit 1
    fi
    tar -zxf stardb.tar.gz

    echo -e "${GREEN}Start to update Starengine client...${NC}"
    # Stop the client shell if running
    if pidof stardb &> /dev/null; then
        kill_client
        sleep 1
    fi
    
    install_main_path

    install_log
    install_header
    install_lib
    install_connector
    install_examples
    install_bin
    install_config
    
    echo
    echo -e "\033[44;32;1mStarengine client is updated successfully!${NC}"

    rm -rf $(tar -tf stardb.tar.gz)
}

function install_Starengine() {
    # Start to install
    if [ ! -e stardb.tar.gz ]; then
        echo "File stardb.tar.gz does not exist"
        exit 1
    fi
    tar -zxf stardb.tar.gz

    echo -e "${GREEN}Start to install Starengine client...${NC}"
    
	  install_main_path
    install_log 
    install_header
    install_lib
    install_connector
    install_examples
    install_bin
    install_config
 
    echo
    echo -e "\033[44;32;1mStarengine client is installed successfully!${NC}"

    rm -rf $(tar -tf stardb.tar.gz)
}


## ==============================Main program starts from here============================
# Install or updata client and client
# if server is already install, don't install client
  if [ -e ${bin_dir}/stardbd ]; then
      echo -e "\033[44;32;1mThere are already installed Starengine server, so don't need install client!${NC}"
      exit 0
  fi  
  
  if [ -x ${bin_dir}/stardb ]; then
      update_flag=1
      update_Starengine
  else
      install_Starengine
  fi
