#!/bin/bash
#
# Script to stop the service and uninstall TSDB

RED='\033[0;31m'
GREEN='\033[1;32m'
NC='\033[0m'

bin_link_dir="/usr/bin"
lib_link_dir="/usr/lib"
inc_link_dir="/usr/include"

data_link_dir="/usr/local/starkv/data"
log_link_dir="/usr/local/starkv/log"
cfg_link_dir="/usr/local/starkv/cfg"

service_config_dir="/etc/systemd/system"
stardb_service_name="alStarkvServer"

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
  pid=$(ps -ef | grep "alStarkvServer" | grep -v "grep" | awk '{print $2}')
  if [ -n "$pid" ]; then
    ${csudo} kill -9 $pid   || :
  fi
}

function clean_service_on_systemd() {
    stardbd_service_config="${service_config_dir}/${stardb_service_name}.service"

    if systemctl is-active --quiet ${stardb_service_name}; then
        echo "Starengine stardbd is running, stopping it..."
        ${csudo} systemctl stop ${stardb_service_name} &> /dev/null || echo &> /dev/null
    fi
    ${csudo} systemctl disable ${stardb_service_name} &> /dev/null || echo &> /dev/null

    ${csudo} rm -f ${stardbd_service_config}	
}

function clean_service_on_sysvinit() {
    #restart_config_str="stardb:2345:respawn:${service_config_dir}/stardbd start"
    #${csudo} sed -i "\|${restart_config_str}|d" /etc/inittab || :    
    
    if pidof alStarkvServer &> /dev/null; then
        echo "Starengine stardbd is running, stopping it..."
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

function clean_service() {
    if ((${service_mod}==0)); then
        clean_service_on_systemd
    elif ((${service_mod}==1)); then
        clean_service_on_sysvinit
    else
        # must manual stop stardbd
        kill_stardbd
    fi
}

# Stop service and disable booting start.
clean_service

# Remove all links
${csudo} rm -f ${bin_link_dir}/alStarkvServer       || :
${csudo} rm -f ${bin_link_dir}/alStarkvClient      || :
# ${csudo} rm -f ${bin_link_dir}/stardbdemo   || :
# ${csudo} rm -f ${bin_link_dir}/stardbdump   || :
# ${csudo} rm -f ${cfg_link_dir}/*          || :
# ${csudo} rm -f ${inc_link_dir}/stardb.h     || :
# ${csudo} rm -f ${inc_link_dir}/stardberror.h     || :
# ${csudo} rm -f ${lib_link_dir}/libstardb.*  || :

${csudo} rm -f ${log_link_dir}            || :
${csudo} rm -f ${data_link_dir}           || :

if ((${service_mod}==2)); then
    kill_stardbd
fi

echo -e "${GREEN}StarEngine is removed successfully!${NC}"
