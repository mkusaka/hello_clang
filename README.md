# hello_clang
[![CircleCI](https://circleci.com/gh/mkusaka/hello_clang/tree/master.svg?style=svg)](https://circleci.com/gh/mkusaka/hello_clang/tree/master)

# quick start

install [Docker](https://www.docker.com/), clone this repository and...

wakeup docker container
```bash
bin/wakeup
```

into docker container
```bash
bin/console
```

build
```bash
make
```

build && test
```
make test
```

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

# progress
- [x] ステップ1
- [x] ステップ2
- [x] ステップ3
- [x] ステップ4
- [ ] ステップ4
- [ ] ステップ5
- [ ] ステップ6
- [ ] ステップ7
- [ ] ステップ8
- [ ] ステップ9
- [ ] ステップ10
- [ ] ステップ11
- [ ] ステップ12
- [ ] ステップ13
- [ ] ステップ14
- [ ] ステップ15
- [ ] ステップ16
- [ ] ステップ17
- [ ] ステップ18
- [ ] ステップ19
- [ ] ステップ20
- [ ] ステップ21
- [ ] ステップ22
- [ ] ステップ23
- [ ] ステップ24
- [ ] ステップ25
- [ ] ステップ26
- [ ] ステップ27
