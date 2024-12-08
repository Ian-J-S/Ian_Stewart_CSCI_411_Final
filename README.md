# CSCI-411 Final Project
## Cooley-Tukey algorithm for FFT

---

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
```
./test.sh
```