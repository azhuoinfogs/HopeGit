#!/bin/bash
#
# Script to stop the service and uninstall Starengine, but retain the config, data and log files.

RED='\033[0;31m'
GREEN='\033[1;32m'
NC='\033[0m'

#install main path
install_main_dir="/usr/local/stardb"
data_link_dir="/usr/local/stardb/data"
log_link_dir="/usr/local/stardb/log"
cfg_link_dir="/usr/local/stardb/cfg"
bin_link_dir="/usr/bin"
lib_link_dir="/usr/lib"
inc_link_dir="/usr/include"

service_config_dir="/etc/systemd/system"
stardb_service_name="stardbd"

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
  pid=$(ps -ef | grep "stardbd" | grep -v "grep" | awk '{print $2}')
  if [ -n "$pid" ]; then
    ${csudo} kill -9 $pid   || :
  fi
}

function clean_bin() {
    # Remove link
    ${csudo} rm -f ${bin_link_dir}/stardb      || :
    ${csudo} rm -f ${bin_link_dir}/stardbd     || :
    ${csudo} rm -f ${bin_link_dir}/stardbdemo  || :
    ${csudo} rm -f ${bin_link_dir}/stardbdump  || :
    ${csudo} rm -f ${bin_link_dir}/rmstardb    || :
}

function clean_lib() {
    # Remove link
    ${csudo} rm -f ${lib_link_dir}/libstardb.*      || :
}

function clean_header() {
    # Remove link
    ${csudo} rm -f ${inc_link_dir}/stardb.h       || :
    ${csudo} rm -f ${inc_link_dir}/stardberror.h       || :
}

function clean_config() {
    # Remove link
    ${csudo} rm -f ${cfg_link_dir}/*            || :    
}

function clean_log() {
    # Remove link
    ${csudo} rm -rf ${log_link_dir}    || :
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
    
    if pidof stardbd &> /dev/null; then
        echo "Starengine stardbd is running, stopping it..."
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
# Remove binary file and links
clean_bin
# Remove header file.
clean_header
# Remove lib file
clean_lib
# Remove link log directory
clean_log
# Remove link configuration file
clean_config
# Remove data link directory
${csudo} rm -rf ${data_link_dir}    || : 

${csudo} rm -rf ${install_main_dir}

osinfo=$(awk -F= '/^NAME/{print $2}' /etc/os-release)
if echo $osinfo | grep -qwi "ubuntu" ; then
#  echo "this is ubuntu system"
   ${csudo} rm -f /var/lib/dpkg/info/starengine* || :
elif  echo $osinfo | grep -qwi "centos" ; then
  echo "this is centos system"
  ${csudo} rpm -e --noscripts starengine || :
fi

echo -e "${GREEN}Starengine is removed successfully!${NC}"
