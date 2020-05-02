# 简单实现action，非常简单

```
import java.util.concurrent.locks.LockSupport
import java.util.concurrent.{ConcurrentHashMap, LinkedBlockingQueue}


class GunAction[E <: GunMessage] {

  val data = new LinkedBlockingQueue[E]()
  val map = new ConcurrentHashMap[Int, E => Boolean]()

  def +!(message: E): Unit = {
    data.put(message)
  }

  def +(message: E): Unit = {
    data.put(message)
    message match {
      case m: BaseGunMessage =>
        m.thread = Thread.currentThread()
        LockSupport.park()
      case _ =>
    }
  }

  def regsite(f: E => Boolean, t: Int): Unit = {
    map.put(t, f)
  }

  def init(): Unit = {
    new Thread(() => {
      while (true) {
        val v = data.take()
        map.get(v.Type())(v)
        v {
          case v: BaseGunMessage =>
            if (v.thread != null) {
              LockSupport.unpark(v.thread)
            }
          case _ =>
        }
      }
    }
    ).start()
  }

}

trait GunMessage {
  def Type(): Int
}

abstract class BaseGunMessage extends GunMessage {
  var thread: Thread = _
}

//val cons = (m: message) => true
```