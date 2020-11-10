# Makefile的默认目标[PHONY all default]

参考文章

makefile文件中, 默认执行一条规则, 不管这条规则是什么, 只要是排在第一个的就默认执行.

```makefile
all: build


# build binary
build: generate fmt vet
	go build -o bin/build main.go

# Run against the configured Kubernetes cluster in ~/.kube/config
run: generate fmt vet manifests
	go run ./main.go

# Run tests
test: generate fmt vet manifests
	go test ./... -coverprofile cover.out
```

上面如果直接执行`make`, 就会执行`all`目标, 如果你想默认执行`run`操作, 只要把ta放在`all`前面即可...

`all` 与 `default` 这两个目标标签是人为的, 比较常用的目标名称, 但对 make 的行为没有任何影响.

`all`的语义应该是, 一步生成多个可执行文件. 所以其一般用于汇总多个中间文件, 生成最终目标.

`default`暂时还不清楚.

至于`.PHONY`, 这是一个伪目标. 比如上面你希望默认执行`run`, 但按照语义, 把`run`放在`build`前又不符合正常流程, 那就可以使用`.PHONY`.

只要在`all`前面添加一句

```
.PHONY: run
```

即可.
