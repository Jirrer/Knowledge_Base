# Knowledge Base

## Terminal program that offers a central location to store, read, and create ideas and concepts

The knowledge base is meant to act as a sort of [commonplace](https://en.wikipedia.org/wiki/Commonplace_book) for knowledge 
that has been developed over time. Often times people find themselves googling procedures that they at one point in time knew.
This program is meant to serve as your memory and avoids you from going down the same google search rabbit hole more than once.

The progarm offers an intentionaly simple console interface and stores all of its information in a SQLite database. The user can
add a new knowledge entry, give it a category tag, and use the custom built text editor to explain to their future self how they
came up with their solution. This entrys can then be searched using the Fuzzy search that implements a double metaphone search
algorithm.

## Architecture


[Full architecture](https://viewer.diagrams.net/?tags=%7B%7D&lightbox=1&highlight=0000ff&edit=_blank&layers=1&nav=1&title=Knowledge_Base.drawio&dark=auto#R%3Cmxfile%3E%3Cdiagram%20name%3D%22Page-1%22%20id%3D%22L9fV7j8l4KuJhznF8ZX1%22%3E3Vffb5swEP5reOzEj0DJY5ek26RMmoSmbHvz8BXogEPGBNhfPxObgIfaNV1IpL4g7rPvbH%2F3%2BTgMZ5U1Hxgp4s9IITVskzaGszZse3lriWcHtBJwLVsCEUuohKwBCJLfoEBToVVCodQmcsSUJ4UOhpjnEHINI4xhrU97wFRftSARTIAgJOkU3SWUxxL1XXPAP0ISxf3KlqlGMtJPVkAZE4r1CHI2hrNiiFy%2BZc0K0o67nhfpd%2F%2FE6HFjDHL%2BEod1%2FDPc7H7QONxvv7qLZmfdVDeWI8MAndAwxFVQiRUL4ZlgKrMlb3v2urCBMpHxGCPMSboZ0PcMq5xCt0VTWMOcLWIhQEuAj8B5q5RBKo4CinmWqlGxS9Z%2B6%2Fzfub35fTy2blRwabXKUloiLAL%2BzJncY6aEwgEzEDGEH4OU8GSvc0aU1qLjvCEd4kVl5ITs2E%2FnYuBY6Ko4UBNyZOJgdZxwCApySFUtLqXOl%2FTfk7RS%2FgoAxqEZrTE9czzSuadEXQ93YqGgpjeVrS6%2BszRnYqkvFWfRsHtdDY8VPBL0%2F2nYv6aGLevtZGeWCuNdMzvuSyrMPyoKKQv55X1Imo7naYlZQ%2FmLC7Zt864oXlVv%2FGm98fV64%2Fh6vbFvZ6s39hkV7b1BRS%2BvqWjvIoq%2B%2B%2FLpQjq2FnPp2D%2Blu6CEk1I0GPCKDmOLop8WUCD8u3b7TA2HpxPn2n8VAH8u4paXIm6VYkVnJ%2B7YmbV6I3cCb8IcfmQOY6O%2FQWfzBw%3D%3D%3C%2Fdiagram%3E%3C%2Fmxfile%3E)


## Installation
* The zip source code can be installed from the versions tab
* Once unpacked - the binary executable and schema.sql files will be available to use
* Before the program can connect to the database, you will need to create a .env file with a variable pointing towards it
