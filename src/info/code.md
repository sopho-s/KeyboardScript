# Basic operations

Supported operators:
- +
- -
- *
- /

For addition, subtraction and multiplication, any integer by integer operation will keep the value an integer type, for division the number will be converted to a float

Any time an operation is done between an integer and float, both will be converted to a float

```
func main() {
	print(1 + 1);
}
```

Will give

```
2
```

```
func main() {
	print(1 + 1.0);
}
```

Will give

```
2.000000
```

```
func main() {
	print(1 / 1);
}
```

Will give

```
1.000000
```

# Assignment

Supported assignment:
- =
- +=
- -=
- *=
- /=

This follows the same rules as the operators

# Selectors and iterators

Supported selectors and iterators:
- if
- for
- while
- try


