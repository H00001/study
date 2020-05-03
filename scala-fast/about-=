# About =>

```
object EqualAndPoint {
  // Int,Int input and return List[Int]
  val fn0: (Int, Int) => Int = {
    // the block need to return a <function>
    // rather than create a noname <function>
    println("fn0:")
    _ + _
  }
  val fn1: (Int, Int) => Int = (a: Int, b: Int) => {
    // this block create a noname function
    println("fn1:")
    0
  }

  // def:execute every time
  // val:execute once
  def fn3: (Int, Int) => Int = {
    // equal with 0
    println("fn3:")
    _ + _
  }

  def fn4: (Int, Int) => Int = (a: Int, b: Int) => {
    // equal with 1
    println("fn4:")
    0
  }

  def fn5(x: Int, y: Int): Int = {
    // equal with 1
    println("fn5:")
    0
  }


  def main(args: Array[String]): Unit = {
    test(fn0)
    test(fn1)
    test(fn4)
    test(fn3)
    test(fn5)

    test(fn0)
    test(fn1)
    test(fn4)
    test(fn3)
    test(fn5)
  }
  fn0
  fn3
  fn3

  def test(fn: (Int, Int) => Int) {

  }
}
```