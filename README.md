# Contact Hash Table

<img src="assets/Contact-Commands.png" width="600">

### Welcome!

This is a contact log system where you can add people's information to a hash table with linked lists for each bucket. 

### What I Learned
- Using hash tables for large data sets
- Using a linked list
- Valgrind for dynamic memory management with malloc() and free()
- Reading and writing files in both text and binary formats
- Testing edge cases

Using hash table functions made it very useful in handling large data sets of employees as well as their numbers. We even tested this with edge cases to prove it would be correct. This case below had over 1500 lines of contacts in the fictional ‘University’ as well as 500 lines of testing the lookup/add functions.

The 11 functions include: 

- Create a log of contacts
- Look up names
- Clear the log
- Print contact names
- Plus read and write from .txt and .bin files. 
- As well as printing error messages for anything a user might input incorrectly for edge cases.

<img src="assets/Valgrind.png" width="800">

I also used Valgrind to make sure everything I allocated memory using malloc () for I also freed at the end. Ensuring 100% there were no memory leaks.

Each time a new item is added to a bucket in the hash table, it is appended to the end of the bucket's linked list.

Here is an example of what the contact log functions could look like!

<img src="assets/Contacts-Used.png" width="800">

## <a href="https://youtu.be/NNMdnVFqG_Q"> Demo Video on YouTube </a>

## Setup

If you want to play around with Contact Hash Table, feel free to clone my repo. To start, please enter the following commands on your terminal:

```
git clone https://github.com/CamNagle24/Contact_Hash_Table
make
```

At this point, you are free to type in whatever sequence of commands you like once you are cd'd into the proj1-code folder.

To get into the Contact Log System type

```
./contacts_main
```

Or if you want to run all 42 tests including reading/writing to binary files:

```
make test
```
<img src="assets/Passed-Tests.png" width="800">
