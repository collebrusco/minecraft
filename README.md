# minecraft
finally taking a stab at the mc clone   
maybe the [cs 384g final project](https://www.cs.utexas.edu/~graphics/s25/cs354h/final/overview/)?

# download build and run
clone with `--recurse-submodules` like so:    
```bash
git clone --recurse-submodules git@github.com:collebrusco/minecraft
```
if you already cloned it without recursing submodules you can always just run
```bash
git submodule update --init --recursive
```
that'll check them out. then (mac/linux) just run `make` in the repo dir.
