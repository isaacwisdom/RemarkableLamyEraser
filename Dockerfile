FROM --platform=linux/amd64 ubuntu:20.04

#for x11-forwarding of qtcreator
RUN DEBIAN_FRONTEND="noninteractive" apt-get update
RUN DEBIAN_FRONTEND="noninteractive" apt-get install -y mesa-utils libgl1-mesa-glx

#install qtcreator application
RUN DEBIAN_FRONTEND="noninteractive" apt install -y qtcreator



#download and setup sdk
RUN DEBIAN_FRONTEND="noninteractive" apt-get install -y curl git ssh rsync

RUN DEBIAN_FRONTEND="noninteractive" curl -s -L https://storage.googleapis.com/remarkable-codex-toolchain/codex-x86_64-cortexa7hf-neon-rm11x-toolchain-3.1.15.sh -o installer.sh; chmod +x installer.sh; ./installer.sh -y

RUN rm -rf /root/.config
COPY docker-qt-support/config /root/.config
COPY docker-qt-support/start-qt.sh /root/start-qt.sh
RUN chmod +x /root/start-qt.sh
WORKDIR /mnt/RemarkableLamyEraser/

CMD ["/root/start-qt.sh"]


