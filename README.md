# QSystemInfo

QSystemInfo is a lightweight system information tool written in c++ with the qt framework

## How to build?

You need to have [qt](https://www.qt.io/download-qt-installer) installed  to build the app.

```bash
# Compile in debug mode
git clone https://github.com/Ola-jed/QSystemInfo.git
cd QSystemInfo
cmake --build cmake-build-debug --target QSystemInfo -- -j 3
cmake-build-debug/QSystemInfo
```

Import the project if you use an ide.