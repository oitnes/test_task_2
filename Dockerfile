FROM ubuntu:latest

RUN apt-get update && \
    apt-get install apt-utils -y

RUN apt-get install -y gcc g++ make cmake

ENV PROJECT_DIR /project
ENV PROJECT_SOURCE_DIR ${PROJECT_DIR}/source
ENV PROJECT_BUILD_DIR ${PROJECT_DIR}/build

ADD . ${PROJECT_SOURCE_DIR}

RUN cmake -DCMAKE_BUILD_TYPE=Release -S ${PROJECT_SOURCE_DIR} -B ${PROJECT_BUILD_DIR}
WORKDIR ${PROJECT_BUILD_DIR}
RUN make

CMD ["./test_executable", "10"]
