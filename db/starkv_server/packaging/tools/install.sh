#!/bin/bash
#
# This file is used to install database on linux systems. The operating system
# is required to use systemd to manage services at boot

set -e
#set -x

# -----------------------Variables definition---------------------
script_dir=$(dirname $(readlink -f "$0"))
# Dynamic directory
data_dir="/var/lib/stardb"
log_dir="/var/log/stardb"

data_link_dir="/usr/local/stardb/data"
log_link_dir="/usr/local/stardb/log"

cfg_install_dir="/etc/stardb"

bin_link_dir="/usr/bin"
lib_link_dir="/usr/lib"
inc_link_dir="/usr/include"

#install main path
install_main_dir="/usr/local/stardb"

# old bin dir
bin_dir="/usr/local/stardb/bin"

service_config_dir="/etc/systemd/system"

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

initd_mod=0
service_mod=2
if pidof systemd &> /dev/null; then
    service_mod=0
elif $(which service &> /dev/null); then    
    service_mod=1
    service_config_dir="/etc/init.d" 
    if $(which chkconfig &> /dev/null); then
         initd_mod=1 
    elif $(which insserv &> /dev/null); then
        initd_mod=2
    elif $(which update-rc.d &> /dev/null); then
        initd_mod=3
    else
        service_mod=2
    fi
else 
    service_mod=2
fi


# get the operating system type for using the corresponding init file
# ubuntu/debian(deb), centos/fedora(rpm), others: opensuse, redhat, ..., no verification
#osinfo=$(awk -F= '/^NAME/{print $2}' /etc/os-release)
osinfo=$(cat /etc/os-release | grep "NAME" | cut -d '"' -f2)
#echo "osinfo: ${osinfo}"
os_type=0
if echo $osinfo | grep -qwi "ubuntu" ; then
  echo "this is ubuntu system"
  os_type=1
elif echo $osinfo | grep -qwi "debian" ; then
  echo "this is debian system"
  os_type=1
elif echo $osinfo | grep -qwi "Kylin" ; then
  echo "this is Kylin system"
  os_type=1
elif  echo $osinfo | grep -qwi "centos" ; then
  echo "this is centos system"
  os_type=2
elif echo $osinfo | grep -qwi "fedora" ; then
  echo "this is fedora system"
  os_type=2
else
  echo "this is other linux system"
  os_type=0
fi

function kill_stardbd() {
  pid=$(ps -ef | grep "stardbd" | grep -v "grep" | awk '{print $2}')
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
    ${csudo} mkdir -p ${install_main_dir}/init.d
}

function install_bin() {
    # Remove links
    ${csudo} rm -f ${bin_link_dir}/stardb     || :
    ${csudo} rm -f ${bin_link_dir}/stardbd    || :
    ${csudo} rm -f ${bin_link_dir}/stardbdemo || :
    ${csudo} rm -f ${bin_link_dir}/stardbdump || :
    ${csudo} rm -f ${bin_link_dir}/rmstardb   || :

    ${csudo} cp -r ${script_dir}/bin/* ${install_main_dir}/bin && ${csudo} chmod 0555 ${install_main_dir}/bin/*

    #Make link
    [ -x ${install_main_dir}/bin/stardb ] && ${csudo} ln -s ${install_main_dir}/bin/stardb ${bin_link_dir}/stardb             || :
    [ -x ${install_main_dir}/bin/stardbd ] && ${csudo} ln -s ${install_main_dir}/bin/stardbd ${bin_link_dir}/stardbd          || :
    [ -x ${install_main_dir}/bin/stardbdump ] && ${csudo} ln -s ${install_main_dir}/bin/stardbdump ${bin_link_dir}/stardbdump || :
    [ -x ${install_main_dir}/bin/stardbdemo ] && ${csudo} ln -s ${install_main_dir}/bin/stardbdemo ${bin_link_dir}/stardbdemo || :
    [ -x ${install_main_dir}/bin/remove.sh ] && ${csudo} ln -s ${install_main_dir}/bin/remove.sh ${bin_link_dir}/rmstardb || :
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

function install_data() {
    ${csudo} mkdir -p ${data_dir}
    
    ${csudo} ln -s ${data_dir} ${install_main_dir}/data    
}

function install_connector() {
    ${csudo} cp -rf ${script_dir}/connector/* ${install_main_dir}/connector
}

function install_examples() {
    ${csudo} cp -rf ${script_dir}/examples/* ${install_main_dir}/examples
}

function clean_service_on_sysvinit() {
    #restart_config_str="stardb:2345:respawn:${service_config_dir}/stardbd start"
    #${csudo} sed -i "\|${restart_config_str}|d" /etc/inittab || :    
    
    if pidof stardbd &> /dev/null; then
        ${csudo} service stardbd stop || :
    fi

    if ((${initd_mod}==1)); then
        ${csudo} chkconfig --del stardbd || :
    elif ((${initd_mod}==2)); then
        ${csudo} insserv -r stardbd || :
    elif ((${initd_mod}==3)); then
        ${csudo} update-rc.d -f stardbd remove || :
    fi
    
    ${csudo} rm -f ${service_config_dir}/stardbd || :
    
    if $(which init &> /dev/null); then
        ${csudo} init q || :
    fi
}

function install_service_on_sysvinit() {
    clean_service_on_sysvinit

    sleep 1

    # Install stardbd service

    if ((${os_type}==1)); then
        ${csudo} cp -f ${script_dir}/init.d/stardbd.deb ${install_main_dir}/init.d/stardbd
        ${csudo} cp    ${script_dir}/init.d/stardbd.deb ${service_config_dir}/stardbd && ${csudo} chmod a+x ${service_config_dir}/stardbd
    elif ((${os_type}==2)); then
        ${csudo} cp -f ${script_dir}/init.d/stardbd.rpm ${install_main_dir}/init.d/stardbd
        ${csudo} cp    ${script_dir}/init.d/stardbd.rpm ${service_config_dir}/stardbd && ${csudo} chmod a+x ${service_config_dir}/stardbd
    fi
    
    #restart_config_str="stardb:2345:respawn:${service_config_dir}/stardbd start"
    #${csudo} grep -q -F "$restart_config_str" /etc/inittab || ${csudo} bash -c "echo '${restart_config_str}' >> /etc/inittab"
    
    if ((${initd_mod}==1)); then
        ${csudo} chkconfig --add stardbd || :
        ${csudo} chkconfig --level 2345 stardbd on || :
    elif ((${initd_mod}==2)); then
        ${csudo} insserv stardbd || :
        ${csudo} insserv -d stardbd || :
    elif ((${initd_mod}==3)); then
        ${csudo} update-rc.d stardbd defaults || :
    fi
}

function clean_service_on_systemd() {
    stardbd_service_config="${service_config_dir}/stardbd.service"

    if systemctl is-active --quiet stardbd; then
        echo "Starengine is running, stopping it..."
        ${csudo} systemctl stop stardbd &> /dev/null || echo &> /dev/null
    fi
    ${csudo} systemctl disable stardbd &> /dev/null || echo &> /dev/null

    ${csudo} rm -f ${stardbd_service_config}
}   

# stardb:2345:respawn:/etc/init.d/stardbd start

function install_service_on_systemd() {
    clean_service_on_systemd

    stardbd_service_config="${service_config_dir}/stardbd.service"

    ${csudo} bash -c "echo '[Unit]'                             >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'Description=Starengine server service' >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'After=network-online.target'        >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'Wants=network-online.target'        >> ${stardbd_service_config}"
    ${csudo} bash -c "echo                                      >> ${stardbd_service_config}"
    ${csudo} bash -c "echo '[Service]'                          >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'Type=simple'                        >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'ExecStart=/usr/bin/stardbd'           >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'LimitNOFILE=infinity'               >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'LimitNPROC=infinity'                >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'LimitCORE=infinity'                 >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'TimeoutStartSec=0'                  >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'StandardOutput=null'                >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'Restart=always'                     >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'StartLimitBurst=3'                  >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'StartLimitInterval=60s'             >> ${stardbd_service_config}"
    ${csudo} bash -c "echo                                      >> ${stardbd_service_config}"
    ${csudo} bash -c "echo '[Install]'                          >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'WantedBy=multi-user.target'         >> ${stardbd_service_config}"
    ${csudo} systemctl enable stardbd
}

function install_service() {
    if ((${service_mod}==0)); then
        install_service_on_systemd
    elif ((${service_mod}==1)); then
        install_service_on_sysvinit
    else
        # must manual stop stardbd
        kill_stardbd
    fi
}

vercomp () {
    if [[ $1 == $2 ]]; then
        return 0
    fi
    local IFS=.
    local i ver1=($1) ver2=($2)
    # fill empty fields in ver1 with zeros
    for ((i=${#ver1[@]}; i<${#ver2[@]}; i++)); do
        ver1[i]=0
    done

    for ((i=0; i<${#ver1[@]}; i++)); do
        if [[ -z ${ver2[i]} ]]
        then
            # fill empty fields in ver2 with zeros
            ver2[i]=0
        fi
        if ((10#${ver1[i]} > 10#${ver2[i]}))
        then
            return 1
        fi
        if ((10#${ver1[i]} < 10#${ver2[i]}))
        then
            return 2
        fi
    done
    return 0
}

function is_version_compatible() {

    curr_version=$(${bin_dir}/stardbd -V | head -1 | cut -d ' ' -f 3)

    min_compatible_version=$(${script_dir}/bin/stardbd -V | head -1 | cut -d ' ' -f 5)

    vercomp $curr_version $min_compatible_version
    case $? in
        0) return 0;;
        1) return 0;;
        2) return 1;;
    esac
}

function update_Starengine() {
    # Start to update
    if [ ! -e stardb.tar.gz ]; then
        echo "File stardb.tar.gz does not exist"
        exit 1
    fi
    tar -zxf stardb.tar.gz

    # Check if version compatible
    if ! is_version_compatible; then
        echo -e "${RED}Version incompatible${NC}"
        return 1
    fi

    echo -e "${GREEN}Start to update Starengine...${NC}"
    # Stop the service if running
    if pidof stardbd &> /dev/null; then
        if ((${service_mod}==0)); then
            ${csudo} systemctl stop stardbd || :
        elif ((${service_mod}==1)); then
            ${csudo} service stardbd stop || :
        else
            kill_stardbd
        fi
        sleep 1
    fi
    
    install_main_path

    install_log
    install_header
    install_lib
    install_connector
    install_examples
    if [ -z $1 ]; then
        install_bin
        install_service
        install_config

        echo
        echo -e "\033[44;32;1mStarengine is updated successfully!${NC}"
        echo
        echo -e "${GREEN_DARK}To configure Starengine ${NC}: edit /etc/stardb/stardb.cfg"
        if ((${service_mod}==0)); then
            echo -e "${GREEN_DARK}To start Starengine     ${NC}: ${csudo} systemctl start stardbd${NC}"
        elif ((${service_mod}==1)); then
            echo -e "${GREEN_DARK}To start Starengine     ${NC}: ${csudo} service stardbd start${NC}"
        else
            echo -e "${GREEN_DARK}To start Starengine     ${NC}: ./stardbd${NC}"
        fi

        echo -e "${GREEN_DARK}To access Starengine    ${NC}: use ${GREEN_UNDERLINE}stardb${NC} in shell${NC}"
        echo
        echo -e "\033[44;32;1mStarengine is updated successfully!${NC}"
    else
        install_bin
        install_config

        echo
        echo -e "\033[44;32;1mStarengine client is updated successfully!${NC}"
    fi

    rm -rf $(tar -tf stardb.tar.gz)
}

function install_Starengine() {
    # Start to install
    if [ ! -e stardb.tar.gz ]; then
        echo "File stardb.tar.gz does not exist"
        exit 1
    fi
    tar -zxf stardb.tar.gz

    echo -e "${GREEN}Start to install Starengine...${NC}"
    
	  install_main_path
	   
    if [ -z $1 ]; then
        install_data
    fi 
    
    install_log 
    install_header
    install_lib
    install_connector
    install_examples

    if [ -z $1 ]; then # install service and client
        # For installing new
        install_bin
        install_service
        install_config	

        # Ask if to start the service
        echo
        echo -e "\033[44;32;1mStarengine is installed successfully!${NC}"
        echo
        echo -e "${GREEN_DARK}To configure Starengine ${NC}: edit /etc/stardb/stardb.cfg"
        if ((${service_mod}==0)); then
            echo -e "${GREEN_DARK}To start Starengine     ${NC}: ${csudo} systemctl start stardbd${NC}"
        elif ((${service_mod}==1)); then
            echo -e "${GREEN_DARK}To start Starengine     ${NC}: ${csudo} service stardbd start${NC}"
        else
            echo -e "${GREEN_DARK}To start Starengine     ${NC}: stardbd${NC}"
        fi

        echo -e "${GREEN_DARK}To access Starengine    ${NC}: use ${GREEN_UNDERLINE}stardb${NC} in shell${NC}"
        echo
        echo -e "\033[44;32;1mStarengine is installed successfully!${NC}"
    else # Only install client
        install_bin
        install_config

        echo
        echo -e "\033[44;32;1mStarengine client is installed successfully!${NC}"
    fi

    rm -rf $(tar -tf stardb.tar.gz)
}


## ==============================Main program starts from here============================
if [ -z $1 ]; then
    # Install server and client
    if [ -x ${bin_dir}/stardbd ]; then
        update_Starengine
    else
        install_Starengine
    fi
else
    # Only install client
    if [ -x ${bin_dir}/stardb ]; then
        update_Starengine client
    else
        install_Starengine client
    fi
fi
