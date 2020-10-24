# Nunam-nRF52810-ProblemStatement
This repo conatins the problem statement provided by Numan.

Problem Statement:- 
We would like you to create a driver for the timer peripheral of the nRF52810 series chip. You can find information about this in page 403 and 404 here:  https://infocenter.nordicsemi.com/pdf/nRF52810_PS_v1.0.pdf

The aim of the driver is to generate an event periodically after a specified amount of microseconds. Here are the driver specifications:

- It should have a start function that starts the periodic event generation (initializes too) with two arguments: a integer specifying the time in us and a function pointer specifying the callback function.

- It should have a stop function stops the periodic event generation.

Use the driver in a simple application that prints a counter value that increments every 509 ms.
