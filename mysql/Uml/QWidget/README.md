&emsp;&emsp;@author 巷北  
&emsp;&emsp;@time 2026-01-08 10:13:16  

- [基本方法](#基本方法)
- [其它方法](#其它方法)
- [控件](#控件)
    - [QWidget](#QWidget)
    - [QLabel](#QLabel)
    - [QPixmap](#QPixmap)
- [高级玩法](#高级玩法)
    - [frame思想](#frame思想)

# 基本方法
- `setWindowTitle()` 这个不多说了, 设置`title`, 很简单.
- `setWindowIcon()` 这个也不多说了, 设置`icon`, 也很简单. 
- `resize()` 这个也不多说了, 设置大小, 很简单. 
- `show()` 这个也不多说了, 展示`Widget`, 直接调用即可.
- `setStyleSheet()` 这个是设置颜色等的方法. 在我这里呢, 就是简单的设置一下空间颜色, 方便查看. 但是实际中, 并不会使用.`imageLabel->setStyleSheet("background-color: red;" "border: 2px solid blue;");`这个就是设置红色颜色, 以及边缘边框设置两像素. 

# 其它方法
- `setSizePolicy(QSizePolicy::?, QSizePolicy::?)` 这个怎么说呢, 提供了一种方案吧. 如果, 我们使用`setFixedSize()`的话, 那么窗口扩大, 对应控件并不会自动拓展的. 如果使用了这个, 就会自动拓展. 当然, 跟你选择的`QSizePolicy` 类型有一定关系.

# 控件
## QWidget
- 这个不多说了. 本来整体上是只专门说这一个的. 不过写的时候, 发现几乎所有的控件都是其子类, 方法几乎是通用的, 所以专门来这里说上一嘴. 
- 关键函数, 摆放函数, `setLayout()`. 这个函数呢, 就是决定其摆放层级的, 非常关键. 确定了这个, 我们才能很好地确定其它东西. 
## QLabel
- `setAlignment(Qt::AlignCenter)` 这个方法不是`QWidget`的通用方法, 而是少数几个的成员方法, 但是我用的是`Lable`, 所以就放到这里来了. 
- 一开始看到这个, 我以为是指的它在父类(`QWidget`)中摆放的位置. 后来发现并不是, 而是控件在其内部摆放的方式. 也就是居中摆放. 所以, 如果想让`QLabel`相对父类居中摆放, 这个方法行不通. 
- `setPixmap()`这个是用于在`Label`中显示图片的. 知道这个方法就行, 具体如何展示的, 需要分析`QPixmap`类. 
- 哎, 找到了一种控制大小的方案了. 原理还每明白, 但就目前来看, 这么用着吧. 基本的摆放没问题就可以了. 
## QPixmap
- 这个是什么呢? 显示图片的? 应该是吧, 也应该属于`QWidget`吧, 我觉得应该是, 所以选择放在了这里. 
- 传入参数就是图片路径, 这个没什么好说的. 
- 然后就是缩放了. 有一个方法, 是`.scaled()`. 第一个参数呢, 一般传入的是`label`的`size()`, `size()`是一个对象, 同时包括`height`与`width`, 这样统一操作比较方便. 
- 然后呢, 图片就会根据这个来进行缩放. 之后, 还有几个参数, `Qt::KeepAspectRatio,Qt::SmoothTransformation)` 第一个是保持图像长宽比, 比较固定的, 第二个则是平滑转动. 
- 现在存在的问题是, 如何精确控制呢? 因为长宽比固定, 这样图片大小无法完美匹配. 
- 知道问题了. 对于其`.scaled()`的方法呢, 是返回一个新的对象, 所以下面的写法, 必然不正确. 

~~~cpp
  QString imagePath = utils::get_file_path("assets/login/2.jpg");
  QPixmap image(imagePath);
  image.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
  label->setPixmap(image);
~~~

- 对于`image`而言, 其中的`scaled`, 并不是对当前对象`scaled`, 而是会返回一个新的对象, 但是不会改变原来的对象. 所以, image其实并没有被`scaled`. 
- 不过呢, 这样也让我找到了一种能够统一比例的方案, 也算是因祸得福吧. 

# 高级玩法
## frame思想
- 在`python`的`tkinter`中, 有个实例概念类, 叫做`frame`. 在`QWidget`中, 也有同样的思想, 非常非常有意思. 
- 从底层来看, 属于嵌套, 归结为数学原理的话, 集合论的思想也蕴含在这里. 
- 怎么理解呢? 正常情况下, 我们只会实例化一个`QWidget`, 然后通过`.show()`展示. 我们摆放控件的话, 这能操控者一个`QWidget`. 
- 上面的做法是没有任何问题的. 但是`Qt`提供了一种机制, 让我们可以实例化多个`QWidget`, 其中父类, 是主要的`QWidget`, 跟上面说的`QWidget`作用几乎一样. 子`QWidget`呢, 则是内置于父类之中. 这样, 我们可以单独操控这个子`QWidget`, 灵活地摆放控件. 
- 同理, 我们还可以用子子`QWidget`, 等等等等. 
- 当然, 上面是我自己的理解, 具体还没有去实现, 但感觉必然没问题的. 思想摆在这里, 目的就是期望我们能够灵活地操控它们. 
- 刚才去测试了一下, 确实是我上面理解的那样.
- 但是呢, 存在问题是, 如果子控件使用`resize()`, 然后再布局(`layout`), 并不会显示. 但是如果设定`setFixedSize()` 就没有任何问题了. 
- 当然, 这里目前没有考虑布局等的问题, 所以整体上来看, 可能还是会有些小奇怪, 哈哈. 













