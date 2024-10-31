cmake_minimum_required(VERSION 3.28)
project(OS_A2 C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

# Add include directories
include_directories(
        /usr/include
        /usr/local/include
        # Add other include directories if needed
)

add_executable(OS_A2
        Q0/add2PB.c
        Q0/findPhone.c
        Atom-Storage/Molecule.cpp
        Atom-Storage/AtomStorageServer.cpp
        Atom-Storage/AtomClient.cpp
        Atom-Storage/MoleculeFactory.cpp
)