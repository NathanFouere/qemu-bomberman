# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.234.0/containers/cpp/.devcontainer/base.Dockerfile

FROM --platform=linux/amd64 alpine:latest

RUN apk add bash git g++ gdb make qemu qemu-img qemu-system-x86_64 qemu-system-i386 qemu-ui-curses cdrkit && \
    adduser -D -s /bin/bash vscode && \
    touch /home/vscode/.bashrc && \
    chown vscode:vscode /home/vscode/.bashrc