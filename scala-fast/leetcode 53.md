# leetcode 53

https://leetcode-cn.com/problems/permutations/

```scala
object Solution {
var ll:List[List[Int]] = Nil
 def permute(nums: Array[Int]): List[List[Int]] = {
    val l: List[Int] = Nil
    val r: List[List[Int]] = List()
    var map = Map[Int,Int]()
    process(l, nums, r, 0, map)
    ll
  }

  def process(l: List[Int], nums: Array[Int], r: List[List[Int]], i: Int, map: Map[Int,Int]) {
    if (i == nums.length) {
      ll = ll ::: l :: r
      return
    }
    for (e <- nums) {
      if (map.getOrElse(e, 0) == 1) {
      }
      else {
        val n = l :+ e
        val m = map
        m += (e -> 1)
        process(n, nums, r, i + 1, m)
      }
    }
  }

}
```

```
// scala 是一门函数试编程语言，那就不应该有太多副作用。
// 能用 val 坚决使用val
```