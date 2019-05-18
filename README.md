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
[低レイヤを知りたい人のためのCコンパイラ作成入門](https://www.sigbus.info/compilerbook/)
