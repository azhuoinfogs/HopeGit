%define homepath         /usr/local/stardb
%define cfg_install_dir  /etc/stardb
%define __strip /bin/true

Name:		Starengine
Version:	%{_version}
Release:	3%{?dist}
Summary:	starengine from stardbdata
Group:	  Application/Database
License:	AGPL
URL:		  www.stardbdata.com

#BuildRoot:  %_topdir/BUILDROOT
BuildRoot:   %{_tmppath}/%{name}-%{version}-%{release}-root 

#Prefix: /usr/local/stardb

#BuildRequires:	
#Requires:	

%description
Big Data Platform Designed and Optimized for IoT

#"prep" Nothing needs to be done 
#%prep
#%setup -q
#%setup -T 

#"build" Nothing needs to be done 
#%build
#%configure
#make %{?_smp_mflags}

%install
#make install DESTDIR=%{buildroot}
rm -rf %{buildroot}

echo topdir: %{_topdir}
echo version: %{_version}
echo buildroot: %{buildroot}

libfile="libstardb.so.%{_version}"

# create install path, and cp file
mkdir -p %{buildroot}%{homepath}/bin
mkdir -p %{buildroot}%{homepath}/cfg
mkdir -p %{buildroot}%{homepath}/connector
mkdir -p %{buildroot}%{homepath}/driver
mkdir -p %{buildroot}%{homepath}/examples
mkdir -p %{buildroot}%{homepath}/include
mkdir -p %{buildroot}%{homepath}/init.d
mkdir -p %{buildroot}%{homepath}/script

cp %{_compiledir}/../packaging/cfg/stardb.cfg         %{buildroot}%{homepath}/cfg
cp %{_compiledir}/../packaging/rpm/stardbd            %{buildroot}%{homepath}/init.d
cp %{_compiledir}/../packaging/tools/post.sh        %{buildroot}%{homepath}/script
cp %{_compiledir}/../packaging/tools/preun.sh       %{buildroot}%{homepath}/script
cp %{_compiledir}/build/bin/stardb                    %{buildroot}%{homepath}/bin
cp %{_compiledir}/build/bin/stardbd                   %{buildroot}%{homepath}/bin
cp %{_compiledir}/build/bin/stardbdemo                %{buildroot}%{homepath}/bin
cp %{_compiledir}/build/bin/stardbdump                %{buildroot}%{homepath}/bin
cp %{_compiledir}/build/lib/${libfile}              %{buildroot}%{homepath}/driver
cp %{_compiledir}/../src/inc/stardb.h                 %{buildroot}%{homepath}/include
cp %{_compiledir}/../src/inc/stardberror.h            %{buildroot}%{homepath}/include
cp -r %{_compiledir}/../src/connector/grafana       %{buildroot}%{homepath}/connector
cp -r %{_compiledir}/../src/connector/python        %{buildroot}%{homepath}/connector
cp -r %{_compiledir}/../src/connector/go            %{buildroot}%{homepath}/connector
cp %{_compiledir}/build/lib/stardb-jdbcdriver*dist.*  %{buildroot}%{homepath}/connector
cp -r %{_compiledir}/../tests/examples/*            %{buildroot}%{homepath}/examples

#Scripts executed before installation
%pre
csudo=""
if command -v sudo > /dev/null; then
    csudo="sudo"
fi

# Stop the service if running
if pidof stardbd &> /dev/null; then
    if pidof systemd &> /dev/null; then
        ${csudo} systemctl stop stardbd || :
    elif $(which service  &> /dev/null); then
        ${csudo} service stardbd stop || :
    else
        pid=$(ps -ef | grep "stardbd" | grep -v "grep" | awk '{print $2}')
        if [ -n "$pid" ]; then
           ${csudo} kill -9 $pid   || :
        fi
    fi
    echo "Stop stardbd service success!"
    sleep 1
fi
# if stardb.cfg already softlink, remove it
if [ -f %{cfg_install_dir}/stardb.cfg ]; then
    ${csudo} rm -f %{homepath}/cfg/stardb.cfg   || :
fi 

# there can not libstardb.so*, otherwise ln -s  error
${csudo} rm -f %{homepath}/driver/libstardb*   || :

#Scripts executed after installation
%post
csudo=""
if command -v sudo > /dev/null; then
    csudo="sudo"
fi
cd %{homepath}/script
${csudo} ./post.sh
 
# Scripts executed before uninstall
%preun
csudo=""
if command -v sudo > /dev/null; then
    csudo="sudo"
fi
# only remove package to call preun.sh, not but update(2) 
if [ $1 -eq 0 ];then
  #cd %{homepath}/script
  #${csudo} ./preun.sh
  
  if [ -f %{homepath}/script/preun.sh ]; then
    cd %{homepath}/script
    ${csudo} ./preun.sh
  else
    bin_link_dir="/usr/bin"
    lib_link_dir="/usr/lib"
    inc_link_dir="/usr/include"
    
    data_link_dir="/usr/local/stardb/data"
    log_link_dir="/usr/local/stardb/log"
    cfg_link_dir="/usr/local/stardb/cfg"

    # Remove all links
    ${csudo} rm -f ${bin_link_dir}/stardb       || :
    ${csudo} rm -f ${bin_link_dir}/stardbd      || :
    ${csudo} rm -f ${bin_link_dir}/stardbdemo   || :
    ${csudo} rm -f ${bin_link_dir}/stardbdump   || :
    ${csudo} rm -f ${cfg_link_dir}/*          || :
    ${csudo} rm -f ${inc_link_dir}/stardb.h     || :
    ${csudo} rm -f ${inc_link_dir}/stardberror.h     || :
    ${csudo} rm -f ${lib_link_dir}/libstardb.*  || :
    
    ${csudo} rm -f ${log_link_dir}            || :
    ${csudo} rm -f ${data_link_dir}           || :
    
    pid=$(ps -ef | grep "stardbd" | grep -v "grep" | awk '{print $2}')
    if [ -n "$pid" ]; then
      ${csudo} kill -9 $pid   || :
    fi    
  fi  
fi
 
# Scripts executed after uninstall
%postun
 
# clean build dir
%clean
csudo=""
if command -v sudo > /dev/null; then
    csudo="sudo"
fi
${csudo} rm -rf %{buildroot}

#Specify the files to be packaged
%files
/*
#%doc

#Setting default permissions
%defattr  (-,root,root,0755)
#%{prefix}

#%changelog
