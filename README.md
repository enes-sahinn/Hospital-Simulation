## Table of Contents

* [About the Project](#about-the-project)
  * [Built With](#built-with)
* [Getting Started](#getting-started)
  * [POSIX Threads](#posix-threads)
  * [Mutex and Semaphore](#mutex-and-semaphore)
* [Contact](#contact)

## About The Project

This project aims to create a simulation of a hospital by using POSIX Threads, Mutexes and Semaphores. In the project, our posix threads act as patients, and our global variables such as REGISTRATION_SIZE, CAFE_NUMBER, NURSE_NUMBER act as the resource used. Additionally, we have golabal variables that determine how long resource usages will take. In the project, the effect of the change in the processing time, the number of patients or the number of resources on the simulation can be easily observed. In addition, it can be observed that an unbalanced distribution causes a bottleneck on resources.

### Built With

* C

## Getting Started

Before examining the project, we need to know about POSIX Threads, Mutexes and Semaphores.

### POSIX Threads

POSIX Threads, commonly known as pthreads, is an execution model that exists independently from a language, as well as a parallel execution model. It allows a program to control multiple different flows of work that overlap in time. Each flow of work is referred to as a thread, and creation and control over these flows is achieved by making calls to the POSIX Threads API. POSIX Threads is an API defined by the standard POSIX.

### Mutex and Semaphore

A mutex object allows multiple process threads to access a single shared resource but only one at a time. On the other hand, semaphore allows multiple process threads to access the finite instance of the resource until available.
In mutex, the lock can be acquired and released by the same process at a time. But the value of the semaphore variable can be modified by any process that needs some resource but only one process can change the value at a time.

## Contact
 
Mail: enessah200@gmail.com  
LinkedIn: [enes-sahinn](https://www.linkedin.com/in/enes-sahinn/)
Project Link: [Hospital-Simulation](https://github.com/enes-sahinn/Hospital-Simulation)

