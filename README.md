# GhostTag-OOP-Project
GhostTag is a C++-based social media application developed as an OOP theory project. It features post creation, user interactions, and profile management, built using double pointers, manual dynamic memory allocation, and core OOP principles.

Overview
--------
GhostTag is a console-based social media simulation written in C++.
It was developed as part of an Object-Oriented Programming (OOP) theory course assignment.
The project demonstrates advanced concepts such as:
- Classes, inheritance, polymorphism, and encapsulation
- Double pointers for managing dynamic arrays
- Manual dynamic memory allocation using new/delete
- Operator overloading
- Static members for unique ID management
- Menu-driven interface

The application allows multiple users to create profiles, post content, interact with posts, send messages, manage followers, and more — all in a text-based environment.

Features
--------
1. User Management
   - Create and delete user accounts
   - View user profiles
   - Block other users

2. Post Management
   - Create text or image posts
   - Remove posts
   - View posts with privacy restrictions
   - Save posts to favorites
   - Show trending post

3. Social Features
   - Follow/unfollow users
   - Like posts (operator overloading)
   - Comment on posts and remove comments
   - Share posts between users
   - Add and view stories
   - Send and receive messages

Technical Details
-----------------
- Language: C++
- Core Concepts: OOP, double pointers, manual memory allocation
- Memory is managed manually (no STL containers like vector were used)
- Uses static variables to auto-generate unique user and post IDs
- Modular structure with base and derived classes for posts

How to Compile and Run
----------------------
1. Make sure you have a C++ compiler installed (g++, MinGW, or MSVC).
2. Open a terminal in the project directory.
3. Compile:
   g++ GhostTag.cpp -o GhostTag
4. Run:
   ./GhostTag   (Linux/Mac)
   GhostTag.exe (Windows)

Usage Instructions
------------------
- On launch, you will see the main menu:
  1. User Management
  2. Post Management
  3. Social Features
  4. Exit
- Navigate by entering the number for your desired action.
- Follow prompts to create users, manage posts, or interact socially.

Credits
-------
Developer: Muhammad Abbas

Course: Object-Oriented Programming (OOP) Theory

Instructor: Ammar Haidar

AI tools were used for debugging assistance and concept clarification.

