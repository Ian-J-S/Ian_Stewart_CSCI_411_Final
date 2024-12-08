# CSCI-411 Final Project
## Cooley-Tukey algorithm for FFT

#### Dependencies
The only dependency is libsnfile. The following command will install
libsndfile via apt.
```
sudo apt-get install libsndfile1-dev
```

#### Source files
* fft.cpp contains my implementation of the FFT algorithm. 
* wavegen.cpp is used to create some pure sinewaves for testing. 

#### Tests
A bash script is provided to generate some notes and run tests.
It will also compile the .cpp files with make.
The notes directory also contains some audio files I found online
of a few instruments playing single notes. The tests will show the
expected output and the results from the note detection after the
FFT is run. 
```
./test.sh
```

#### Dockerfile
I included a Dockerfile with the necessary dependencies. It isn't needed but I thought it might
be more convenient. To build:
```
docker build -t 411-final .
```
and to automatically compile and run the tests:
```
docker run -v "$(pwd)":/usr/src -it --rm 411-final
```
