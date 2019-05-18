# hello_clang

# command list
## docker env
run container

```bash
# without build
$ docker-compose up -d
# with (force) build
$ docker-compose up -d --build
```

enter into container

```bash
$ docker-compose exec main bash
```

## compile
### form mac terminal
build
```bash
$ docker-compose exec main make
```

test
```bash
$ docker-compose exec main make test
```

clean up build assets
```bash
$ docker-compose exec main make clean
```

### from inside docker
build
```bash
$ make
```

test
```bash
$ make test
```

clean up build assets
```bash
$ make clean
```

# ref
- [低レイヤを知りたい人のためのCコンパイラ作成入門](https://www.sigbus.info/compilerbook/)
