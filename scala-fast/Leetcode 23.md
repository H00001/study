# Leetcode 23

## 思路

使用有限队列，堆排序时间复杂度是`nlogn` 需要排链表长度次，每次拍链表个数个 时间复杂度 `链表长度*log链表个数`

 

```scala
import java.util.PriorityQueue

class MergeList {

  object Solution {
    def mergeKLists(lists: Array[ListNode]): ListNode = {
      val pq: PriorityQueue[ListNode] = new PriorityQueue[ListNode]((a, b) => {
        if (a.x > b.x) {
          1
        } else {
          -1
        }
      })
      val l = new ListNode()
      var p = l
      do {
        for (i <- lists.indices) {
          if (lists(i) != null) {
            pq.add(lists(i))
            lists(i) = lists(i).next
          }
        }
        p.next = pq.poll()
        p = p.next

      } while (pq.size() != 0)
      l.next
    }

  }

}

class ListNode(var _x: Int = 0) {
  var next: ListNode = _
  var x: Int = _x
}
```

