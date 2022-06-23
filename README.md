# How to build?
```
$ make
$ .\main.exe
```
Keep in mind that produces a file named main.exe, regardless of operating system.

# How to use
| command | behavior |
| --- | --- |
| quit | Exits repl |
| reset | Resets repl |
| lines starts with '#' | Puts them outside of main function, so you can define macros or include libraries. |
| anything else | Puts them inside of main function. |

# Requirements
tcc >= 0.9.27, gcc (mingw64) >= 11.2.0
