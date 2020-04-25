# define variable

# Scala 数据类型

Scala 与 Java有着相同的数据类型，下表列出了 Scala 支持的数据类型：

| 数据类型 | 描述                                                         |
| :------- | :----------------------------------------------------------- |
| Byte     | 8位有符号补码整数。数值区间为 -128 到 127                    |
| Short    | 16位有符号补码整数。数值区间为 -32768 到 32767               |
| Int      | 32位有符号补码整数。数值区间为 -2147483648 到 2147483647     |
| Long     | 64位有符号补码整数。数值区间为 -9223372036854775808 到 9223372036854775807 |
| Float    | 32 位, IEEE 754 标准的单精度浮点数                           |
| Double   | 64 位 IEEE 754 标准的双精度浮点数                            |
| Char     | 16位无符号Unicode字符, 区间值为 U+0000 到 U+FFFF             |
| String   | 字符序列                                                     |
| Boolean  | true或false                                                  |
| Unit     | 表示无值，和其他语言中void等同。用作不返回任何结果的方法的结果类型。Unit只有一个实例值，写成()。 |
| Null     | null 或空引用                                                |
| Nothing  | Nothing类型在Scala的类层级的最底端；它是任何其他类型的子类型。 |
| Any      | Any是所有其他类的超类                                        |
| AnyRef   | AnyRef类是Scala里所有引用类(reference class)的基类           |





## 变量声明

> 类似于golang

在学习如何声明变量与常量之前，我们先来了解一些变量与常量。

- 一、变量： 在程序运行过程中其值可能发生改变的量叫做变量。如：时间，年龄。
- 二、常量 在程序运行过程中其值不会发生变化的量叫做常量。如：数值 3，字符'A'。

在 Scala 中，使用关键词 **"var"** 声明变量，使用关键词 **"val"** 声明常量。

声明变量实例如下：

```
var myVar : String = "Foo"
var myVar : String = "Too"
```

以上定义了变量 myVar，我们可以修改它。

声明常量实例如下：

```
val myVal : String = "Foo"
```

以上定义了常量 myVal，它是不能修改的。如果程序尝试修改常量 myVal 的值，程序将会在编译时报错。

------

## 变量类型声明

变量的类型在变量名之后等号之前声明。定义变量的类型的语法格式如下：

```
var VariableName : DataType [=  Initial Value]

或

val VariableName : DataType [=  Initial Value]
```

------

## 变量类型引用

在 Scala 中声明变量和常量不一定要指明数据类型，在没有指明数据类型的情况下，其数据类型是通过变量或常量的初始值推断出来的。

所以，如果在没有指明数据类型的情况下声明变量或常量必须要给出其初始值，否则将会报错。

```
var myVar = 10;
val myVal = "Hello, Scala!";
```

以上实例中，myVar 会被推断为 Int 类型，myVal 会被推断为 String 类型。

------

## Scala 多个变量声明

Scala 支持多个变量的声明：

```
val xmax, ymax = 100  // xmax, ymax都声明为100
```

如果方法返回值是元组，我们可以使用 val 来声明一个元组：

```
scala> val pa = (40,"Foo")
pa: (Int, String) = (40,Foo)
```





## 作用域保护

Scala中，访问修饰符可以通过使用限定词强调。格式为:

```
private[x] 
或 
protected[x]
```

这里的x指代某个所属的包、类或单例对象。如果写成private[x],读作"这个成员除了对[…]中的类或[…]中的包中的类及它们的伴生对像可见外，对其它所有类都是private。

这种技巧在横跨了若干包的大型项目中非常有用，它允许你定义一些在你项目的若干子包中可见但对于项目外部的客户却始终不可见的东西。

```
package bobsrockets{
    package navigation{
        private[bobsrockets] class Navigator{
         protected[navigation] def useStarChart(){}
         class LegOfJourney{
             private[Navigator] val distance = 100
             }
            private[this] var speed = 200
            }
        }
        package launch{
        import navigation._
        object Vehicle{
        private[launch] val guide = new Navigator
        }
    }
}
```