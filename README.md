# MCAL project - Cellular automaton

## Run the program

Ensure you have correct rights to execute the shell script (`chmod 755 ./run.sh`).

```sh
./run.sh [-v] [-f] resourceFile
```

The shell script will compile and execute the code.

`-v` will make verbose while compiling / linking (useful when an error occurs).

`-f` will rebuild the project.

`resourceFile` is a text file (commonly in `resources/example/`).

For example :

```sh
./run.sh resources/example/vie.txt
```

## Prerequires

Require | Needed
------- | -------
**Platform** | Linux (tested on Debian 10.5 and Ubuntu 20.04)
**C++ version** | C++17
**Library** | SFML >= 2.5


* Use a recent version of Bash if you use `run.sh` (otherwise, just do `make` and `./bin/debug/project`).
* To install the [SFML library](https://www.sfml-dev.org/) : `sudo apt-get install libsfml-dev`.

If you run it on WSL, make sure you have a X server, like [VcXsrv](https://sourceforge.net/projects/vcxsrv/), and run it.

SFML will display a warning message to tell you OpenGL's version is lower than requested, but it does not matter.


## Also

There's a graphic part. To disable it, go on `include/_conf/project_settings.h` and set `USE_GRAPHICS` to `false`.


## About

<small><i>Not completed yet...</i></small>


## License

See the [MIT License](https://github.com/NoxFly/MCAL-project/blob/master/LICENSE).
