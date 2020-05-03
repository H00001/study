# WordCount

a flink word count demo

```
package com.flink.demo01

import org.apache.flink.api.java.utils.ParameterTool
import org.apache.flink.streaming.api.scala.StreamExecutionEnvironment
import org.apache.flink.streaming.api.windowing.time.Time

object WordCount {
  def main(args: Array[String]): Unit = {
    // 获取NetCat的port
    val port: Int = try {
      ParameterTool.fromArgs(args).getInt("port")
    } catch {
      case e: Exception => {
        System.err.println("No port set, Use default port 6666")
      }
    }


    val env = StreamExecutionEnvironment.getExecutionEnvironment


    val data = env.socketTextStream("hadoo", port)

    val words = data.flatMap(_.split("\\s+")) // 获取数据并进行切分，生成一个个单词
    val tups = words.map(w => WordWithCount(w, 1)) // 将一个个单词生成一个个对偶元组

    //   val res = window.sum("count") // 聚合
    val res = words.reduce((a, b) => WordWithCount(a.word, a.count + b.count))
    // 将结果打印
    res.print.setParallelism(1)
    // 开始执行
    env.execute("scala wordCount")


  }

  case class WordWithCount(word: String, count: Int)

}
```