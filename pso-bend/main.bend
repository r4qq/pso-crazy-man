object customVector {x, y}


def factorial(n: u24) -> u24:
  if n == 0:
    return 1
  else:
    return n * factorial(n - 1)

def func(v: customVector) -> f24:
  open customVector: a
  open customVector: b
  return a * b


def main() -> u24:
  return func(customVector {x: 2, y: 2})