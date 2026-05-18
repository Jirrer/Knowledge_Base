# Knowledge Base

## Terminal program that offers a central location to store, read, and create ideas and concepts

The knowledge base is meant to act as a sort of [commonplace](https://en.wikipedia.org/wiki/Commonplace_book) for knowledge 
that has been developed over time. Often times people find themselves googling procedures that they at one point in time knew.
This program is meant to serve as your memory and avoids you from going down the same google search rabbit hole more than once.

The progarm offers an intentionaly simple console interface and stores all of its information in a SQLite database. The user can
add a new knowledge entry, give it a category tag, and use the custom built text editor to explain to their future self how they
came up with their solution. This entrys can then be searched using the Fuzzy search that implements a double metaphone search
algorithm.

## Installation
* The zip source code can be installed from the versions tab
* Once unpacked - the binary executable and schema.sql files will be available to use
* Before the program can connect to the database, you will need to create a .env file with a variable pointing towards it
