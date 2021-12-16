#!/bin/bash
#
# This file is used to install starengine rpm package on centos systems. The operating system 
# is required to use systemd to manage services at boot
#set -x
# -----------------------Variables definition---------------------
script_dir=$(dirname $(readlink -f "$0"))
# Dynamic directory
data_dir="/var/lib/starkv"
log_dir="/var/log/starkv"
data_link_dir="/usr/local/starkv/data"
log_link_dir="/usr/local/starkv/log"

# static directory
cfg_dir="/usr/local/starkv/cfg"
bin_dir="/usr/local/starkv/bin"
init_d_dir="/usr/local/starkv/init.d"

cfg_install_dir="/etc/starkv"
bin_link_dir="/usr/bin"
lib_link_dir="/usr/lib"
inc_link_dir="/usr/include"

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

function kill_stardbd() {
#  ${csudo} pkill -f stardbd || :
  pid=$(ps -ef | grep "alStarkvServer" | grep -v "grep" | awk '{print $2}')
  if [ -n "$pid" ]; then
    ${csudo} kill -9 $pid   || :
  fi
}

# function install_include() {
#     # ${csudo} rm -f ${inc_link_dir}/stardb.h ${inc_link_dir}/stardberror.h|| :
#     # ${csudo} rm -f ${inc_link_dir}/mid.h || :
#     # ${csudo} ln -s ${inc_dir}/stardb.h ${inc_link_dir}/stardb.h  
#     # ${csudo} ln -s ${inc_dir}/stardberror.h ${inc_link_dir}/stardberror.h  
#     # ${csudo} cp    ${inc_dir}/mid.h ${inc_link_dir}/mid.h  
# }

# function install_lib() {
#     # ${csudo} rm -f ${lib_link_dir}/libstardb* || :
#     # ${csudo} rm -f ${lib_link_dir}/libmpc* || :
    
#     # ${csudo} ln -s ${lib_dir}/libstardb.* ${lib_link_dir}/libstardb.so.1
#     # ${csudo} ln -s ${lib_link_dir}/libstardb.so.1 ${lib_link_dir}/libstardb.so
#     # ${csudo} cp    ${lib_dir}/libmpc.so ${lib_link_dir}/
#     # ${csudo} cp    ${lib_dir}/libmpc.a ${lib_link_dir}/
# }

function install_bin() {
    # Remove links
    ${csudo} rm -f ${bin_link_dir}/alStarkvServer     || :
    ${csudo} rm -f ${bin_link_dir}/alStarkvClient    || :
    # ${csudo} rm -f ${bin_link_dir}/stardbdump || :
    # ${csudo} rm -f ${bin_link_dir}/stardbdemo || :
    # ${csudo} rm -f ${bin_link_dir}/rmstardb   || :

    ${csudo} chmod 0555 ${bin_dir}/*

    #Make link
    [ -x ${bin_dir}/alStarkvServer ] && ${csudo} ln -s ${bin_dir}/alStarkvServer ${bin_link_dir}/alStarkvServer             || :
    [ -x ${bin_dir}/alStarkvClient ] && ${csudo} ln -s ${bin_dir}/alStarkvClient ${bin_link_dir}/alStarkvClient          || :
    # [ -x ${bin_dir}/stardbdump ] && ${csudo} ln -s ${bin_dir}/stardbdump ${bin_link_dir}/stardbdump || :
    # [ -x ${bin_dir}/stardbdemo ] && ${csudo} ln -s ${bin_dir}/stardbdemo ${bin_link_dir}/stardbdemo || :
#   [ -x ${bin_dir}/remove.sh ] && ${csudo} ln -s ${bin_dir}/remove.sh ${bin_link_dir}/rmstardb || :
}

function install_config() {
    if [ ! -f ${cfg_install_dir}/starkv.cfg ]; then
        ${csudo} ${csudo} mkdir -p ${cfg_install_dir}
        [ -f ${cfg_dir}/starkv.cfg ] && ${csudo} cp ${cfg_dir}/starkv.cfg ${cfg_install_dir}
        ${csudo} chmod 644 ${cfg_install_dir}/*
    fi

    ${csudo} mv ${cfg_dir}/starkv.cfg ${cfg_dir}/starkv.cfg.org
    ${csudo} ln -s ${cfg_install_dir}/starkv.cfg ${cfg_dir}
}

function clean_service_on_sysvinit() {
    #restart_config_str="stardb:2345:respawn:${service_config_dir}/stardbd start"
    #${csudo} sed -i "\|${restart_config_str}|d" /etc/inittab || :    
    
    if pidof alStarkvServer &> /dev/null; then
        ${csudo} service alStarkvServer stop || :
    fi
    
    if ((${initd_mod}==1)); then
        ${csudo} chkconfig --del alStarkvServer || :
    elif ((${initd_mod}==2)); then
        ${csudo} insserv -r alStarkvServer || :
    elif ((${initd_mod}==3)); then
        ${csudo} update-rc.d -f alStarkvServer remove || :
    fi
    
    ${csudo} rm -f ${service_config_dir}/alStarkvServer || :
    
    if $(which init &> /dev/null); then
        ${csudo} init q || :
    fi
}

function install_service_on_sysvinit() {
    clean_service_on_sysvinit

    sleep 1

    # Install stardbd service  
    ${csudo} cp %{init_d_dir}/alStarkvServer ${service_config_dir} && ${csudo} chmod a+x ${service_config_dir}/alStarkvServer

    #restart_config_str="stardb:2345:respawn:${service_config_dir}/stardbd start"
    #${csudo} grep -q -F "$restart_config_str" /etc/inittab || ${csudo} bash -c "echo '${restart_config_str}' >> /etc/inittab"
    
    if ((${initd_mod}==1)); then
        ${csudo} chkconfig --add alStarkvServer || :
        ${csudo} chkconfig --level 2345 alStarkvServer on || :
    elif ((${initd_mod}==2)); then
        ${csudo} insserv alStarkvServer || :
        ${csudo} insserv -d alStarkvServer || :
    elif ((${initd_mod}==3)); then
        ${csudo} update-rc.d alStarkvServer defaults || :
    fi
}

function clean_service_on_systemd() {
    stardbd_service_config="${service_config_dir}/alStarkvServer.service"

    # stardbd service already is stoped before install in preinst script
    #if systemctl is-active --quiet stardbd; then
    #    echo "Starengine is running, stopping it..."
    #    ${csudo} systemctl stop stardbd &> /dev/null || echo &> /dev/null
    #fi
    ${csudo} systemctl disable alStarkvServer &> /dev/null || echo &> /dev/null

    ${csudo} rm -f ${stardbd_service_config}
}

# stardb:2345:respawn:/etc/init.d/stardbd start

function install_service_on_systemd() {
    clean_service_on_systemd

    stardbd_service_config="${service_config_dir}/alStarkvServer.service"

    ${csudo} bash -c "echo '[Unit]'                             >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'Description=Starengine server service' >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'After=network-online.target'        >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'Wants=network-online.target'        >> ${stardbd_service_config}"
    ${csudo} bash -c "echo                                      >> ${stardbd_service_config}"
    ${csudo} bash -c "echo '[Service]'                          >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'Type=simple'                        >> ${stardbd_service_config}"
    ${csudo} bash -c "echo 'ExecStart=/usr/bin/alStarkvServer'  >> ${stardbd_service_config}"
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
    ${csudo} systemctl enable alStarkvServer
}

function install_service() {
    if ((${service_mod}==0)); then
        install_service_on_systemd
    elif ((${service_mod}==1)); then
        install_service_on_sysvinit
    else
        # manual start stardbd
        kill_stardbd
    fi
}

function install_Starengine() {
    echo -e "${GREEN}Start to install StarKVEngine...${NC}"

    #install log and data dir , then ln to /usr/local/stardb
    # ${csudo} mkdir -p ${log_dir} && ${csudo} chmod 777 ${log_dir}
    # ${csudo} mkdir -p ${data_dir} 
    
    # ${csudo} rm -rf ${log_link_dir}   || :
    # ${csudo} rm -rf ${data_link_dir}  || :
    
    # ${csudo} ln -s ${log_dir} ${log_link_dir}     || :
    # ${csudo} ln -s ${data_dir} ${data_link_dir}   || :
    
    # Install include, lib, binary and service
    # install_include
    # install_lib
    install_bin
    install_service
    install_config	

    # Ask if to start the service
    echo
    echo -e "\033[44;32;1mStarkvengine is installed successfully!${NC}"
    echo
    echo -e "${GREEN_DARK}To configure StarKVEngine ${NC}: edit /etc/starkv/starkv.cfg"
    if ((${service_mod}==0)); then
        echo -e "${GREEN_DARK}To start StarKVEngine     ${NC}: ${csudo} systemctl start alStarkvServer${NC}"
    elif ((${service_mod}==1)); then
        echo -e "${GREEN_DARK}To start StarKVEngine     ${NC}: ${csudo} update-rc.d alStarkvServer default  ${RED} for the first time${NC}"
        echo -e "                      : ${csudo} service alStarkvServer start ${RED} after${NC}"
    else 
        echo -e "${GREEN_DARK}To start StarKVEngine     ${NC}: ./alStarkvServer${NC}"
    fi

    echo -e "${GREEN_DARK}To access StarKVEngine    ${NC}: use ${GREEN_UNDERLINE}alStarkvClient${NC} in shell${NC}"

    echo
    echo -e "\033[44;32;1mStarKVEngine is installed successfully!${NC}"
}


## ==============================Main program starts from here============================
install_Starengine
