---
title: 'DKRZero: Open Source Embedded Accelerometer for Sports and Health Science Research'
tags:
  - Python
  - sports
  - Arduino
  - accelerometer
  - accelerometry
authors:
  - name: Christopher Steer
    affiliation: 1
affiliations:
 - name: St Mary's University, Waldegrave Road, Twickenham, Middlesex TW1 4SX. 
   index: 1
date: 4 February 2019
bibliography: paper.bib
---

# Summary

Accelerometry is a fundamental technique in sports research that 

There are many commercially-available accelerometers that are used within sports research, in which the 
closed and proprietary nature can be problematic. Potential issues include: the commercial accelerometers be expensive; 
they can be limited in access to the unprocessed sensor data; and conclusions and comparisons between accelerometers 
can be unresolved due to on-board proprietary sensor data processing algorithms. 

``DKRZero`` is the first embedded sports research accelerometer released by the DanceKickRun open source and hardware 
project. The accelerometer is based on the ESP32 Thing and Motion Shield open hardware produced by SparkFun [cite]. 
The Motion Shield has a LSM9DS1 sensor that enables linear, angular accelerations and the local magnetic field to be 
measured. 

The ``DKRZero`` embedded software is an Arduino library, written in C++, which is used with the main Arduino integrated 
development environment to setup the ESP32 Thing. Once programmed the software can be flexibly configured using a 
text file stored on a SD card. The configuration file is a JSON formatted file that contains three main types of objects: 
Sensors, that configure the LSM9DS1 nine-axis sensor, changing parameters such as filter bandwidth; Processors, which 
are used to perform calculations on the raw data (such as Cadence or sensor orientation); Connections, that are used 
to direct the output of all processors' calculations to their final destination, such as SD card or serial terminal. 

The  ``DKRZero`` library contains base classes for Processors, Sensors, and Connections, and has factory classes, enabling
easier extensibility for developers. 

# Acknowledgements

We acknowledge support from UKRI M2D network, and contributions from Eoin O'Sullivan, the St Mary's University 
Applied Physics Society, the Royal Ballet School and Giuseppe Cimardoro. 

# References
