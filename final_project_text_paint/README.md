# Text Paint #
#### Video Demo:  <https://www.youtube.com/watch?v=0kStipoAIG0>
#### Description: I've recreated a text-based version of paint in which users can draw, resize, erase, save/load, and more from the terminal. The program was written entirely in C.

## Objective ##
The overall project goal, in my eyes, was to solidify the concepts I had learned in the first half of the course with memory allocation and potentially data structures. Having taken an introductory CS course already, I borrowed an project idea from there and beefed up its implementation for CS50x.

Having finished the project now, I can say with full confidence that I have solidified some of the lower-level concepts that confuse most CS students in the beginning. Though I wished for more application of data structures in the project, the time deadline for the course forces me to submit this early. A complete project, but yet to be fully worthy of showcasing.

## Design ##
The program is designed to operate all in the command terminal. Given my experience in more backend development--and hesitance to start learning how to create a UI hours before the final deadline--I opted to stick with this good-old fashioned interface for now.

As for the code, I decided to split the code into three main helper files and one main file, using a Makefile to ease the process of running and debugging code in the future. Each file dives deeper into the problem, Main being the most high-level, followed by Commands and then User Interaction & Canvas Manipulation.

The code relies heavily on an abstraction of the "canvas" as a struct containing a matrix of characters (each character representing a single pixel in the final file).

### User Interaction ###
Possibly my least favorite part of any project, I despise working on how to handle user input (especially when it's inaccurate). This proved to be the toughest part to program, even with all the memory allocation required in the other functions. Dealing with input buffers and file pointers, however, proved to be valuable as I was forced to delve deeper into their implementation in C and I finally understood what was happening under the hood of the input stream while I interacted with the user. That being said, being able to throw all the tedious work of user interaction into another file makes it easier to concentrate on the beefier memory allocation that happens in the next file.

### Canvas Manipulation ###
As stated before, the canvas was abstracted into a struct, making the code a lot more readible and linking dimensions (an integrable part of working with arrays) right into the `char**`. This file defined functions useful for any matrix manipulation (including all of the memory allocation).

### Commands ###
This file's goal is to combine together the User Interaction and Canvas Manipulation functions together in a nicely packaged function that handles all the lower level (and tedious) work of linking the two helper files.

### Main ###
This file's sole purpose is to call commands in the appropriate order and run the program for as long as the user demands it. It's purposely made to be simple, very abstracted away from the lower level workings that the helper files will deal with in an effort to organize the program.

## Looking Forward ##
I would've genuinely hoped to explore front-end projects for my final endeavour in CS50x, but given the time-constraints (and how much time I spent on weeks 8 & 9) I figured it would be better to stick to what I liked more. That being said, looking forward I would love to incorporate this program into a much more capable version in Python, perhaps even linking in some of that web application work we've practiced in the course. Or maybe even explore computer vision and machine learning in Python.

Overall though, this course has been great and I'm glad to say that the project I've created took me a long time to plan out and deploy. Thanks CS50!