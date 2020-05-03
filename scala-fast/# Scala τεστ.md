# Scala τεστ

```scala
import java.util.concurrent.{ArrayBlockingQueue, Callable, CountDownLatch, ExecutorService, Executors, ThreadPoolExecutor}

class MapReduce(v: Int) {


  val th: ExecutorService = Executors.newFixedThreadPool(8)
  var c: CountDownLatch = new CountDownLatch(10)
  private var m: String => List[String] = _
  private var p: List[String] => List[String] = _
  private var r: Stream[String] => Object = _
  var d: List[String] = _

  def >|(): Unit = {

    for (a <- 0 to 10) {
      th.execute(() => {
        d = m("hello")
        c.countDown()
      })
    }

    c.await()
    for (a <- 0 to 10) {
      th.execute(() => {
        p(d)
        c.countDown()
      })
    }

    c.await()
    for (a <- 0 to 10) {
      th.execute(() => {
        p(d)
        c.countDown()
      })
    }
  }

  def Reduce(fn: Stream[String] => Object): Unit = {
    r = fn
  }

  def Map(fn: String => List[String]): Unit = {
    m = fn
  }

  def Partition(fn: List[String] => List[String]): Unit = {
    p = fn
  }


}

object test {

  val v = (x: String) => {
    println(x)
    null
  }


  val vo: List[String] = null


  def vp: String => List[String] = {
      null  
  }

  def main(args: Array[String]): Unit = {
    val m = new MapReduce(3)
    m.Map(v)
    m.Map(vp)
    m >| ()
  }
}
```

这是一个模拟map-reduce的demo，用于模仿mapreduce过程