Name:           ros-hydro-schunk-svh-driver
Version:        0.1.5
Release:        0%{?dist}
Summary:        ROS schunk_svh_driver package

Group:          Development/Libraries
License:        LGPL
URL:            http://www.ros.org/wiki/schunk_svh_driver
Source0:        %{name}-%{version}.tar.gz

Requires:       ros-hydro-dynamic-reconfigure
Requires:       ros-hydro-roscpp
Requires:       ros-hydro-rospy
Requires:       ros-hydro-rqt-gui
Requires:       ros-hydro-rqt-gui-py
Requires:       ros-hydro-sensor-msgs
Requires:       ros-hydro-std-msgs
Requires:       ros-hydro-urdf
Requires:       ros-hydro-xacro
BuildRequires:  ros-hydro-catkin
BuildRequires:  ros-hydro-dynamic-reconfigure
BuildRequires:  ros-hydro-roscpp
BuildRequires:  ros-hydro-rospy
BuildRequires:  ros-hydro-rqt-gui
BuildRequires:  ros-hydro-rqt-gui-py
BuildRequires:  ros-hydro-sensor-msgs
BuildRequires:  ros-hydro-std-msgs
BuildRequires:  ros-hydro-urdf
BuildRequires:  ros-hydro-xacro

%description
SVH Driver wrapper to enable control of the Schunk five finger hand

%prep
%setup -q

%build
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree that was dropped by catkin, and source it.  It will
# set things like CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/hydro/setup.sh" ]; then . "/opt/ros/hydro/setup.sh"; fi
mkdir -p build && cd build
%cmake .. \
        -UINCLUDE_INSTALL_DIR \
        -ULIB_INSTALL_DIR \
        -USYSCONF_INSTALL_DIR \
        -USHARE_INSTALL_PREFIX \
        -ULIB_SUFFIX \
        -DCMAKE_INSTALL_PREFIX="/opt/ros/hydro" \
        -DCMAKE_PREFIX_PATH="/opt/ros/hydro" \
        -DSETUPTOOLS_DEB_LAYOUT=OFF \
        -DCATKIN_BUILD_BINARY_PACKAGE="1" \

make %{?_smp_mflags}

%install
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree that was dropped by catkin, and source it.  It will
# set things like CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/hydro/setup.sh" ]; then . "/opt/ros/hydro/setup.sh"; fi
cd build
make %{?_smp_mflags} install DESTDIR=%{buildroot}

%files
/opt/ros/hydro

%changelog
* Mon Sep 29 2014 Georg Heppner <heppner@fzi.de> - 0.1.5-0
- Autogenerated by Bloom

