FROM ubuntu:latest
LABEL title="411-final"
ENV WORKDIR=/usr/src
WORKDIR /usr/src

ARG DEBIAN_FRONTEND=noninteractive

# Update the package lists
RUN apt-get update -y

# Install required packages
RUN apt-get install -y \
    libsndfile1-dev \
    build-essential \
    g++ \
    make

# Copy the project files to the working directory
COPY . ${WORKDIR}

# Default commands to run when the container starts
CMD ["./test.sh"]