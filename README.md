# Compilers task submission
My (Prabhanjan Prabhu, 221CS234) submission for the compilers task for WEC systems/security recruitment.  
<br>

### Dependencies
- [flex](https://github.com/westes/flex)
- [GCC/G++](https://gcc.gnu.org/)  
  <br>

### Usage
_Note:_ I have only tested this on linux, not sure about other operating systems.
Run the following commands to build the binary:

```
flex lexer.lex
g++ parser.cpp -o compile
```

  <br>
Then to run the binary, do:

```
./compile <file>
```

Some example files have been provided in the examples folder.
The binary parses the contents of the file into sentences separated by punctuation and then semantically analyzes the sentences to check for validity. If a sentence is valid, then the semantic breakdown of each token is provided.  
<br>
![example 1](assets/example_1.png)
<p align="center"> <em> Screenshot of binary being run on provided example 1.</em> </p>
