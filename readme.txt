OBS!

This project requires an input device with at least 10 pin outputs.
We have elected to use a raspberry pi to translate keyboard inputs.

How to run (with a rasberry pi 3 or similar):

1. install required python packages in requirements.txt
2. connect rpi GPIO 2, 3, 4, 14, 15, into DTEK pins 0-4, GPIO 17, 27, 22, 23, 24 into DTEK pins 5-9
3. run controller.py on the rpi board.
4. connect DTEK board into VGA.
5. run the makefile, upload binary to DTEK board
6. play the game with WASD + R, and IJKL + P for player 1 resp. player 2
