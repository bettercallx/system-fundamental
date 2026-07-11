# cmd

## OS

```
macOS - homebrew
brew install git

ubuntu/debian - apt/apt-get
sudo apt update && sudo apt install 

windows - winget
winget install xxx
```

## Programming language

```
python - pip/pip3
pip3 install playwright/requests/numpy

Node.js/Javascript - npm/pnpm
npm install react/vue/lodash

Rust - cargo
cargo add serde

Go - go
go get/install

Java - maven/gradle 通过pom.xml/build.gradle自动下载配置以依赖
```

## containers

```
Docker - docker
docker pull/docker run
```

.venv 隔离python解释器和pip，是python虚拟环境

`source .../.venv/bin/activate` 激活虚拟环境后，终端的PATH变量会把 `.venv/bin` 排在最前面，系统运行pytest先找到.venv的而不是用本机的pytest


