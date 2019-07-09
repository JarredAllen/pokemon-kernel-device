# pokemon-kernel-device
A fake kernel software device which prints out the pokemon theme song when opened

### To Install
`git clone` this repository to a directory, and then run `make`. Make should produce a file named `pokemon.ko`.

Once you've done that, run `sudo insmod pokemon.ko` to insert this into the kernel, and it will be there. To access
this file, once you've inserted it into the kernel, check your `/var/log/kern.log` for a line which looks like
`...Pokemon module has been loaded: <major #>`. Once you've found that number, cd to `/dev` and run
`sudo mknod pokemon c <major #> 0`.

Once installed, you should be able to read `/dev/pokemon`.
