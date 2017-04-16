# phook

Runs a command after a parent process has finished.

```
Usage: phook [OPTION]...
Runs a command after a parent process has finished.

With no OPTION provided, will do nothing and exit

Mandatory arguments to long options are mandatory for short options too
  -a, --after=COMMAND        executes command after the parent process has ended
  -e, --execute=COMMAND      executes command on start
  -p, --process=PID          waits for PID to exit instead of parent process
  -h, --help                 display this help and exit
      --version              output version information and exit
```

## Installation

You need macOS for this to work.

```
wget https://github.com/drinchev/phook/archive/v0.0.6.tar.gz
tar -xzf v0.0.6.tar.gz
cd phook-0.0.6
make install
```

Alternative option would be to use [TheFox](https://github.com/TheFox)'s [homebrew tap](https://github.com/TheFox/homebrew-brewery) :

```
brew tap thefox/brewery
brew install phook
```

## Usage

See [examples](https://github.com/drinchev/phook/tree/master/examples).

## Contributors

- [@jayrhynas](https://github.com/jayrhynas)
- [@shishirchawla](https://github.com/shishirchawla)

---

Credits goes to http://serverfault.com/a/641004/152846

---

Copyright (C) 2017 Ivan Drinchev.

This software may be modified and distributed under the terms of the MIT license.


