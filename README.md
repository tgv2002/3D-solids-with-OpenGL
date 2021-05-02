# COMPUTER GRAPHICS - ASSIGNMENT 0

## By - Gokul Vamsi Thota
## Roll number - 2019111009


### Brief Description

This is the zeroth assignment, which involves creation of three dodecahedrons, their display and applying certain animations on them and the camera. The three dodecahedrons chosen by me are:

* Decagonal Prism

* Hexagonal DiPyramid

* Undecagonal Pyramid



### Execution

It is assumed that all the required libraries (glfw, glad, glm) are pre-installed and present in a 'libraries' folder. The 'source' folder included here can be executed in the following way (with the help of CMakeLists.txt included).

* After extracting the folder into some location on your system, enter the directory 2019111009, with the command: `cd 2019111009`

* Move the 'libraries' folder (with installations of glfw, glad, glm) to this directory.

* In this directory create an empty folder 'build' with the command: `mkdir build`

* Navigate to this folder with the command: `cd build`

* Run the command: `cmake ..` , and after that run the command: `make` for finishing compiling and generating executable.

* Run the generated executable with the command: `./Hello-World`. You will be able to see the display now.



### Instructions

The following instructions are to be kept in mind for display and animation effects:


#### Display of objects

* Press '1' key to view Decagonal Prism on the screen.

* Press '2' key to view Hexagonal DiPyramid on the screen.

* Press '3' key to view Undecagonal Pyramid on the screen.


#### Moving camera

* Press 'A' key to move the camera to the left.

* Press 'D' key to move the camera to the right.

* Press 'W' key to move the camera upwards.

* Press 'S' key to move the camera downwards.

* Press 'SPACE' key to move the camera forward.

* Press 'BACKSPACE' key to move the camera backward.


#### Teleportation

* Press 'H' key to move the camera to fixed teleportation position 1 (south-west with respect to the origin).

* Press 'J' key to move the camera to fixed teleportation position 2 (south-east with respect to the origin).

* Press 'K' key to move the camera to fixed teleportation position 3 (west with respect to the origin).


#### Camera reset position

* Press 'L' to reset camera viewing to default (Focusing into the screen from a fixed point on positive z axis).



Note that the following instructions pertain to whatever object is currently being displayed on the screen.

#### Moving object

* Press 'UP' arrow key to move the object upwards.

* Press 'DOWN' arrow key to move the object downwards.

* Press 'LEFT' arrow key to move the object to the left.

* Press 'RIGHT' arrow key to move the object to the right.

* Press 'LEFT_SHIFT' key to move the object forward.

* Press 'RIGHT_SHIFT' key to move the object backward.


#### Spinning object about a fixed axis (X - AXIS)

* Press 'N' key to make the object spin about x - axis.

* Press 'M' key to make the object to stop spinning.

* Note that, when we switch to display of different object (by pressing 1 or 2 or 3), the state of the previous object is stored, and hence on switching back, the object is displayed at its previous state.



#### Spinning camera about object

* Press 'C' key to make the camera spin about the current object's position.

* Press 'L' key to make the camera to stop spinning and return to default position, as explained in camera reset instruction above.

* Note that, when we switch to display of different object (by pressing 1 or 2 or 3), the camera still spins about object, if it was spinning for previous object.

